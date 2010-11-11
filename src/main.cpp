#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <sys/reboot.h>
#include <deque>

#include "myTypes.h"
#include "buffer/ssBuffer.h"
#include "udp/udp_port.h"
#include "arg_parser/carg_parser.h"
#include "console_out.h"
#include "ICAppLayer/cmd.h"
#include "ICAppLayer/FunctionNode/param_desc.h"
#include "ICAppLayer/FunctionNode/FunctionNode.h"

#include "ICAppLayer/ICAppLayer.h"
#include "Functions/CommonFuncs.h"
#include "Functions/SpecFuncs.h"
#include "Functions/functions.h"
#include "SIG_handler.h"

#include "global.h"


errType process_cmdLine(int argc, char *argv[])
{
  errType result=err_result_ok;

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

  if( !ap_init( &parser, argc, argv, options, 0 ) )
    { show_error( "Необходимо задать аргументы запуска!", 0, 0 ); return err_result_error; }
  if( ap_error( &parser ) )                             /* bad option */
    { show_error( ap_error( &parser ), 0, 1 ); return err_result_error; }

 for( argind = 0; argind < ap_arguments( &parser ); ++argind )
 {
    const int code = ap_code( &parser, argind );
    if( !code ) break;                                  // no more options
    switch( code )
      {
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

  for( argind = 0; argind < ap_arguments( &parser ); ++argind )
  {
    const int code = ap_code( &parser, argind );
    const char * arg = ap_argument( &parser, argind );

    switch(code){
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

errType fileRead (char* fname, BYTE** buffer, size_t *sz){
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

void dbg_hex_print(BYTE* buffer, size_t len){
    printf("[");
    for (int i=0; i<len; i++) printf("%.2X ",buffer[i]);
    printf("]\n");
}

errType appInit(void)
{
    errType result=err_not_init;
    if (verbose_level) printf("Инициализация подсистем приложения... \n");

    
    result= app->StartListening();
    
    if (result==err_result_ok) srvInit();

    
    else printf("Ошибка запуска системы сокетной коммуникации\n");
    printf("Результат инициализации сервиса: %s\n", strErrTypes[(int)result]);
    return result;
}

errType appDeinit(void)
{
    printf("Деинициализация подсистем программного средства ... \n");
    srvDeinit();
    app->StopListening();
    return err_result_ok;
}

int main(int argc, char *argv[]) {
	errType result=err_result_ok;
	
	memset(&equipAddr,0,sizeof(in_addr));
	
	errType ret=process_cmdLine(argc, argv);
	
        if (ret!=err_result_ok) return ret;
        if ((eq_udp_sending_port==0) || (eq_udp_listen_port==0) || (equipAddr.s_addr==0)) {
	    printf("Пропущен один из обязательных параметров:\n");
	    printf( "  -u -  сокет для сервисного общения с верхней иерархией системы\n" );            
	    printf( "  -s -  сокет для посылки сообщений на нижнюю иерархию системы\n");               
	    printf( "  -r -  сокет для приёма сообщений от нижней иерархии системы\n");                
	    printf( "  -i -  адрес подсистемы обслуживания нижней иерархии\n\n");
	    return err_not_init;
        }
        
        if ((eq_udp_sending_port==eq_udp_listen_port) || (eq_udp_sending_port==wUdp) || (eq_udp_listen_port==wUdp)) 
        {
    	    printf("Ошибка параметров введены совпадающие номера сокетов\n");
    	    return err_not_init;
    	}
    	
        if ((eq_udp_sending_port==wUdp+1) || (eq_udp_listen_port==wUdp+1)) 
        {
    	    printf("Ошибка параметров: номер сокета оборудования взят из уже занятого пространства\n");
    	    return err_not_init;
        }
        
	CommonFuncs *cf;
	SpecFuncs *sf;
	
	installSIGhandlers(appDeinit);
	
	app=new ICAppLayer(wUdp);
	
	if (appInit()!=err_result_ok) AppTerminated=true;
	else {
	    
	    cf=new CommonFuncs(app);
	    sf=new SpecFuncs(app);
	
	    cf->StartCommonFuncs();
	    sf->StartSpecFuncs();
	
	    while(!app->terminated()) {
		app->ProcessMessages();
	    }
	
	    appDeinit();
	
	    if (app->terminated()==2) reboot(RB_AUTOBOOT);
	    delete app;
	}
	printf("Работа программного средства завершена\n\n");
	return (EXIT_FAILURE);
}
