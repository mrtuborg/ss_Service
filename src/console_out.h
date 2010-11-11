#ifndef CONSOLE_OUT_H
#define CONSOLE_OUT_H

extern char PROGVERSION[];
extern char Program_name[];
extern char program_name[];
extern char program_year[];

extern void show_help( const char verbose );
extern void show_version();
extern void show_error( const char * msg, const int errcode, const char help );
extern void internal_error( const char * msg );
extern const char * optname( const int code, const ap_Option options[] );

#endif // CONSOLE_OUT_H
