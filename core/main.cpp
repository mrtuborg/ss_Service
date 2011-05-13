/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Programm entry point.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <sys/reboot.h>
#include <deque>
#include <errno.h>

#include <extra/ortsTypes/ortsTypes.h>
#include <storage/ssBuffer/ssBuffer.h>
#include <peripheral/udp_port/udp_port.h>
#include <parsers/arg_parser/carg_parser.h>

#include "console_out.h"
#include <rcsLib/rcsCmd/rcsCmd.h>
#include <param_desc.h>
#include <functionNode.h>

#include <SrvAppLayer.h>
#include <commonFuncsMgr.h>
#include <specFuncsMgr.h>
#include <ServicesSpecifier.h>
#include <functions.h>
#include <SIG_handler.h>

#include <global.h>

/******************************************************************//**
 * @brief       Parsing commandline arguments.
 *
 * @param[in]   argc    - count of arguments strings
 * @param[in]   argv[]  - array of arguments strings
 *
 * @retval      err_result_ok - if execution was successful
 * @retval      err_not_found - if no arguments found
 * @retval      err_result_error - if parsing was unsuccessful
 *********************************************************************/
errType process_cmdLine(int argc, char *argv[])
{
    /**
   * @todo reorganize process to external library
   */

    errType result=err_result_ok;
    /// 1. Define arguments type: with (\a ap_yes) or without (\a ap_no) parameters
    const ap_Option options[] =
    {
        { 'V', "version",             ap_no },
        { 'h', "help",                ap_no },
        { 'v', "verbose",             ap_no },
        { 'u', "srv_udp_port",        ap_yes},
        { 's', "eqp_udp_send_port",   ap_yes},
        { 'r', "eqp_udp_recv_port",   ap_yes},
        { 'i', "eqp_ip_addr",         ap_yes},
        {   0, 0,                     ap_no } };

    Arg_parser parser;
    int argind;
    ///2. Initialize arguments parser ::ap_init
    if( !ap_init( &parser, argc, argv, options, 0 ) )
    { show_error( "Необходимо задать аргументы запуска!", 0, 0 ); return err_not_found; }
    ///3. Check for parsing errors ::ap_error
    if( ap_error( &parser ) )                             /* bad option */
    { show_error( ap_error( &parser ), 0, 1 ); return err_result_error; }

    ///4. Execute all arguments after it parsing
    for( argind = 0; argind < ap_arguments( &parser ); ++argind )
    {
        /// - get code of argument ::ap_code
        const int code = ap_code( &parser, argind );
        if( !code ) break;                                  // no more options
        /// - switch with argument code value

        switch( code ){
            /// - execute
        case 'V': show_version(); return err_extra;
        case 'h': show_help( verbose_level ); return err_extra;
        case 'v': verbose_level = 1; break;
        case 'u': break;
        case 's': break;
        case 'r': break;
        case 'i': break;
        default : internal_error( "неподдерживаемая опция" ); return err_extra;
    }
    } // end process options

    ///4. Execute only arguments with parameters after it parsing
    for( argind = 0; argind < ap_arguments( &parser ); ++argind )
    {
        /// - get code of argument ::ap_code
        const int code = ap_code( &parser, argind );
        /// - get argument parameter ::ap_argument
        const char * arg = ap_argument( &parser, argind );
        /// - switch with argument code value
        switch(code){
            /// - execute
        case 'u':
	    wUdp=atol(arg);
            break;

        case 's':
            eq_udp_sending_port=atol(arg);
            break;

        case 'r':
            eq_udp_listen_port=atol(arg);
            break;

        case 'i':
            strcpy(eq_ip_addr, arg);
            inet_aton(eq_ip_addr,&equipAddr);
            break;

        default:
            break;
        }

        if( !code ) { // option
            // non-option
            printf( "аргумент `%s' не является опцией", arg );
            printf( "\n" );

            result=err_result_error;
        }
    }
    if (!ap_arguments( &parser )) result=err_result_ok;
    return result;
}

/**
 * @brief useless function in this programm.
 * @details stays from good old times
 */
errType fileRead (char* fname, BYTE** buffer, size_t *sz)
{
    /// @todo reorganize function to reading xml-files for future purposes
    errType result=err_not_init;
    FILE *pFile;
    BYTE *buf;
    int ret;
    //size_t lSize=0;
    
    printf("Open file %s for reading\n",fname);
    pFile=fopen(fname,"rb");
    if (pFile==NULL) {
	printf ("File %s access error. Exiting.\n",fname);
	result=err_result_error;
	return err_result_error;
    }

    fseek (pFile , 0 , SEEK_END);
    *sz = ftell (pFile);
    rewind (pFile);

    buf=new BYTE[*sz];
    ret=fread(buf,*sz,1,pFile);
    
    //    printf("File size %lu readed %lu:\n=",ret, *sz);
    //    for (int i=0; i<*sz; i++) printf(" %.2X ", buf[i]);
    //    printf("=\n");

    fclose(pFile);
    
    *buffer=buf;
    
    if (*sz>0) result=err_result_ok;
    else result=err_result_error;
    
    return result;
}

/**
 * @brief prints hex bytes from \a buffer with size \a len.
 */
void dbg_hex_print(BYTE* buffer, size_t len)
{
    ///  @todo use this function in new debug print system
    printf("[");
    for (size_t i=0; i<len; i++) printf("%.2X ",buffer[i]);
    printf("]\n");
}

/******************************************************************//**
 * @brief       Initialize srvAppLayer subsystem.
 *
 * @details    result copied from srvAppLayer::StartListening
 * @retval      err_result_ok - execution was successful
 * @retval      err_sock_error - problems with communications subsystem
 **********************************************************************/
errType appInit(void)
{
    errType result=err_not_init;
    if (verbose_level) printf("Инициализация подсистем приложения... \n");

    /** Starting main programm threads srvAppLayer::StartListening():
     *  -# Prepare queues for sending and listening to/from clients
     *  -# Send & Listen threads for clients communication
     *  -# Listen thread for equipment communication
     */
    result= app->StartListening();
    
    /**
     * If threads started successfully - starts service specific function initialize ::srvInit()
     */
    if (result==err_result_ok) srvInit();

    else {
        printf("Ошибка запуска системы сокетной коммуникации\n");
        printf("Код ошибки: %d\n", errno);
        perror("\rОписание ошибки: ");
        printf("\r");
    }
    printf("Результат инициализации сервиса: %s\n", strErrTypes[(int)result]);
    return result;
}

/******************************************************************//**
 * @brief       Deinitialize srvAppLayer subsystem.
 *
 * @retval      always return err_result_ok, why not?
 **********************************************************************/
errType appDeinit(void)
{
    printf("Деинициализация подсистем программного средства ... \n");
    srvDeinit();
    app->StopListening();
    return err_result_ok;
}

/******************************************************************//**
 * @brief       Programm entrypoint
 *
 * @retval      EXIT_FAILURE
 * @retval      err_not_init
 **********************************************************************/
int main(int argc, char *argv[], char *environ[]) {

    memset(&equipAddr,0,sizeof(in_addr));

    /// 1. Process command line arguments \b argc and \b argv[] in ::process_cmdLine
    errType ret=process_cmdLine(argc, argv);
    ///     - if arguments parsing is unsuccessfull exiting from programm

    services_specifier specs;
    specs.load_specs("test.xml");

    if (ret!=err_result_ok) return ret;
    /// 2. Check arguments:
    /// - check for missing one of exact argument
    if ((eq_udp_sending_port==0) || (eq_udp_listen_port==0) || (equipAddr.s_addr==0)) {
        printf("Пропущен один из обязательных параметров:\n");
        printf( "  -u -  сокет для сервисного общения с верхней иерархией системы\n" );
        printf( "  -s -  сокет для посылки сообщений на нижнюю иерархию системы\n");
        printf( "  -r -  сокет для приёма сообщений от нижней иерархии системы\n");
        printf( "  -i -  адрес подсистемы обслуживания нижней иерархии\n\n");
        return err_not_init;
    }

    /// - check for equipment communication settings:
    ///     - sending port need to be not equal to listen port values
    ///     - sending or listen port neet to be not equal to client listen port
    if ((eq_udp_sending_port==eq_udp_listen_port) || (eq_udp_sending_port==wUdp) || (eq_udp_listen_port==wUdp))
    {
        printf("Ошибка параметров введены совпадающие номера сокетов\n");
        return err_not_init;
    }

    ///     - check for sending port number or listening port number was far from client port number on one port number
    ///     that reserved for client sending port.
    if ((eq_udp_sending_port==wUdp+1) || (eq_udp_listen_port==wUdp+1))
    {
        printf("Ошибка параметров: номер сокета оборудования взят из уже занятого пространства\n");
        return err_not_init;
    }


    commonFuncsMgr *cf;
    specFuncsMgr *sf;

    /// 3. Install system signals handlers ::installSIGhandlers()
    installSIGhandlers(appDeinit);

    app=new SrvAppLayer(wUdp);

    /// 4. Initialize application ::appInit()
    if (appInit()!=err_result_ok) AppTerminated=true;
    else {

        cf=new commonFuncsMgr(app);
        sf=new specFuncsMgr(app);
	
        ///4. Start functions generate from declarations
        ///- for common functions commonFuncsMgr::startCommonFuncs()
        cf->startCommonFuncs();
        ///- for special functions specFuncsMgr::startSpecFuncs()
        sf->startSpecFuncs();
	
        /// 5. Main programm loop srvAppLayer::ProcessMessages() while not terminated by signal srvAppLayer::terminated()
        while(!app->terminated()) {
            app->processMessages();
            app->srv_yield();
        }

        /// 6. Deinitialize application ::appDeinit()
        appDeinit();
	
        //if (app->terminated()==2) reboot(RB_AUTOBOOT);
        delete app;
    }
    printf("Работа программного средства завершена\n\n");
    return (EXIT_FAILURE);
}
