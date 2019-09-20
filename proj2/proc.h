/*    proc.h    */
#include <stdio.h>
#include <stdlib.h>

typedef int WORD;

#define PNREGS 5

#define NPROC 10

#define SP     0

/* state */
#define XFREE    0
#define XREADY   1
#define XRUN     2
#define XBLOCK   3


struct xentry  {
       int  xid;
       WORD xregs[PNREGS];    /* save SP */
       WORD xbase;
       WORD xlimit;
       int  xstate;
};

#define STKSIZE 8192 

extern struct xentry xtab[];

extern int currxid;
/*
struct linked_list
{
    int number;
    struct linked_list *next;
};
*/
//typedef struct linked_list node;
//extern struct linked_list node;


//extern struct linked_list node;

