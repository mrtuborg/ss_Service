#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT(src/main.cpp,0.1,nosenko@rec-etu.com)

AM_CONFIG_HEADER(src/auto_config.h)
AM_INIT_AUTOMAKE(r168,0.1)

#AC_CONFIG_SRCDIR([config.h.in])
#AC_CONFIG_HEADERS([config.h])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stddef.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_LIB([pthread], [pthread_create],
[PTHREAD_LIBS=-lpthread],
[AC_MSG_ERROR([No pthread library is available])])

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([socket])


AC_PROG_INSTALL
AC_CONFIG_FILES([Makefile
                 src/Makefile])

AC_OUTPUT
