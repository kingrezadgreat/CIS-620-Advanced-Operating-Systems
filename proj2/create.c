#include <stdio.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * userret  --  entered when a thread exits by return
 *------------------------------------------------------------------------
 */
 // the retun of stack comes here
void userret()
{
   /*set the current state to free. Why??
   xtab is imported as proc.h is included on top*/
   xtab[currxid].xstate = XFREE;
   printf("XT: Old threads never die; they just fade away. (id:%d)\n",currxid);
   /* find the next runnable thread to trun */
   /*why it does not return. The return is saved into *(--saddr)  = (int)userret; */
   resched();
}

static int newxid()
{
    int i, xid;
    static int nextproc =0;
    /* find a free process entry */
  
    for(i=0; i<NPROC; i++) { 
        xid = nextproc;
        if((++nextproc) >= NPROC)
             nextproc = 0;
        if(xtab[xid].xstate == XFREE)
             return(xid);
    }
    printf("Error: run out of process table ! \n"); 
    exit(1);
}

/*------------------------------------------------------------------------
 *  xthread_create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
int xthread_create(int *procaddr,int nargs, int args)
{
    
    WORD *saddr; /* stack address */
    WORD *ap; /* input argument addres*/
    struct xentry *xptr;
    int xid;
    
    /* assign the next free space in the stack table. this is the add rest 
    of the new stack assigned to the new process */
    xid = newxid(); 
    
    /* Now lets save the address of the new process in the stack. The return 
    value of the newxid() is just the location of free stack in the stack table,
    but not the actual spae. Lets save the actual space in xprt and then change 
    the status of that location to ready. It is set to XREADY so next time the 
    rescheduler sees that while looping in the stack table, it sees that as 
    ready and run it.*/
    xptr = &xtab[xid]; // take this stack part and use
    xptr->xstate = XREADY; // free the level

    saddr = (WORD *) xptr->xbase; /* save the base of the stack table into saddr*/
    
    //  from here -->fill the stack with info
    ap = (&args) + nargs;
    for(; nargs > 0; nargs--)
         /* copy args onto new process' stack. this is to svae all local variables, 
         which are at the bottom of the stack*/
        *(--saddr) = *(--ap); 
    
    /* sooner or later you will be there. this is the function to recycle and free 
    space. This is the return address to which the return value of the therad will be 
    saved. In case of function foo(), the return of foo therad has to be saved here. 
    Once you arrive here you are already in foo therad and waiting for the return 
    result. And thus once userret is invoked, the space is freed and left for other
    threads to be used.*/     
    *(--saddr)  = (int)userret;   
    
    /*this stores the process address. like in (foo,1,7) . This one saves the address 
    of the function that will be executed */
    *(--saddr)  = (int)procaddr;
    
    --saddr;           /* for frame ebp; it's not important !? */
    saddr -= 2;        /* 2 words for si and di */
    xptr->xregs[SP] = (int) saddr ; // save stack pointer to this location
    //  to here <-- now we reached the top of stack pointer
    return(xid);
}
