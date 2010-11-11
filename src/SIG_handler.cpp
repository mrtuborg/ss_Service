#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "myTypes.h"
#include "global.h"

funcVoid SIGTERM_handler;

void term_handler(int i)
{ 
    if (verbose_level) printf ("Инициировано прерывание работы программного средства...\n");
    SIGTERM_handler();
    exit(EXIT_SUCCESS);
}

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
    printf(">>> Идентификатор процесса №%i\n\n", getpid());
}
