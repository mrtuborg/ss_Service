/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  aided functions to process_cmdLine
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
//#include "auto_config.h"

#include "./arg_parser/carg_parser.h"

/// programm revision number
#ifndef REVISION
 #define REVISION 0     ///< - revision by default is 0
#endif

char PROGVERSION[255] 	  = "0.1";                     ///< version of programm
char Program_name[255]    = "Сервис служебной системы";///< name of programm
char program_name[255]    = "ss_Service";              ///< filename of programm
char program_year[255]    = "2010";                    ///< copyright year


/******************************************************************//**
 * @brief       Show cmdline help information (--help)
 **********************************************************************/
void show_help( const char verbose )
{
  printf( "%s - Сервис служебной подсистемы НОЛС ТИ-3.12 (14Ш39). \n", Program_name );
  printf( "\nИспользование параметров: %s [options]\n",program_name);
  printf( "где options:\n" );
  printf( "  -h, --help		отобразить данную справку и выйти\n" );
  printf( "  -V, --version		отобразить информацию о версии и выйти\n" );
  printf( "  -u<arg>,		указать сокет для сервисного общения с верхней иерархией системы\n" );
  printf( "  -s<arg>,		указать сокет для посылки сообщений на нижнюю иерархию системы\n");
  printf( "  -r<arg>,		указать сокет для приёма сообщений от нижней иерархии системы\n");
  printf( "  -i<arg>,		указать адрес подсистемы обслуживания нижней иерархии\n");
  printf( "  -v, --verbose		вывод отладочной информации на экран\n" );
  if( verbose )
    {
    printf( "  -H, --hidden              example of hidden option (shown with -v -h)\n" );
    }
  printf( "\nОбо всех обнаруженных ошибках программы просьба сообщать по адресу: nosenko@ieee.org\n");
}

/******************************************************************//**
 * @brief       Show programm version information (--version)
 **********************************************************************/
void show_version()
{
  
  printf( "%s версия %s (ревизия №%s)\n", Program_name, PROGVERSION, REVISION);
  /*printf( "Copyright (C) %s Vladimir A. Nosenko.\n", program_year );
  printf( "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n" );
  printf( "This is free software: you are free to change and redistribute it.\n" );
  printf( "There is NO WARRANTY, to the extent permitted by law.\n" );
  */
}

/******************************************************************//**
 * @brief       Show cmdLine parser error
 **********************************************************************/
void show_error( const char * msg, const int errcode, const char help )
{
  if( msg && msg[0] != 0 )
    {
    fprintf( stderr, "%s: %s", program_name, msg );
    if( errcode > 0 ) fprintf( stderr, ": %s", strerror( errcode ) );
    fprintf( stderr, "\n" );
    }
    fprintf( stderr, "Введите `%s --help' для получения справки по использованию параметров командной строки.\n",program_name);
}

/******************************************************************//**
 * @brief       Show cmdLine internal error
 **********************************************************************/
void internal_error( const char * msg )
{
  char buf[80];
  snprintf( buf, sizeof( buf ), "внутренняя ошибка: %s.\n", msg );
  show_error( buf, 0, 0 );
  exit( 3 );
}

/**************************************************************************//**
 * @brief       Convert code with option from cmdLine argument to char buffer
 ******************************************************************************/
const char * optname( const int code, const ap_Option options[] )
{
  static char buf[2] = "?";
  int i;

  if( code != 0 )
    for( i = 0; options[i].code; ++i )
      if( code == options[i].code )
        { if( options[i].name ) return options[i].name; else break; }
  if( code > 0 && code < 256 ) buf[0] = code; else buf[0] = '?';
  return buf;
}
