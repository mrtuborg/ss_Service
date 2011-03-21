/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  System signals handlers manager.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "extra/ortsTypes/ortsTypes.h"
#include "global.h"

funcVoid SIGTERM_handler; ///< pointer to handling function for signal \b TERMINATE

/******************************************************************//**
 * @brief       signal \b TERMINATE handling function
 **********************************************************************/
void term_handler(int i)
{ 
    if (verbose_level) printf ("Инициировано прерывание работы программного средства...\n");
    SIGTERM_handler();
    exit(EXIT_SUCCESS);
}

/******************************************************************//**
 * @brief       signals handlers installer
 **********************************************************************/
void installSIGhandlers(funcVoid func)
{
    SIGTERM_handler=func;
    struct sigaction sa;
    sigset_t newset;
    sigemptyset(&newset);
    sigaddset(&newset, SIGHUP);
    sigprocmask(SIG_BLOCK, &newset, 0);
    sa.sa_handler = term_handler;
    sigaction(SIGTERM, &sa, 0);
    printf(">>> Идентификатор процесса #%i\n\n", getpid());
}
