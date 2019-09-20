/* resched.c  -  resched */
#include <stdio.h>
#include <proc.h>

#include <signal.h>
#include <unistd.h>

/*------------------------------------------------------------------------
 * resched  --  find a live thread to run
 *
 *------------------------------------------------------------------------
 */
void resched()
{
    ualarm(0,0);
    register struct  xentry  *cptr;  /* pointer to old thread entry */
    register struct  xentry  *xptr;  /* pointer to new thread entry */
    int i,next;

    cptr = &xtab[currxid];

    next = currxid ;
    for(i=0; i<NPROC; i++) { 
        if( (++next) >= NPROC)
             next = 0;
        /*if the state of one of the elements of stack is XREADY, change it
        to XRUN and then start running*/
        if(xtab[next].xstate == XREADY) {
            xtab[next].xstate = XRUN;
            xptr = &xtab[next];
            currxid = next;
            ualarm(10000,0);
            ctxsw(cptr->xregs,xptr->xregs);
            return;
        }
    }
    printf("XT: no threads to run!\n");
    exit(0);
}

/*
void handler()
{
    sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet,SIGALRM);
    sigprocmask(SIG_UNBLOCK,&sigSet,NULL);
    
    xtab[currxid].xstate = XREADY;
    resched();
}
*/


