#include <stdio.h>
#include <proc.h>
#include <message.h>

xthread_mbox_t mb;

int xidfoo1, xidfoo2, xidfoo3, xidfoo4, xidfoo5, xidfoo6; 
int xidbar1, xidbar2, xidbar3, xidbar4, xidbar5, xidbar6;
int xidoof1, xidoof2, xidoof3, xidoof, xidoof5, xidoof6; 


//  TEST FUNCTION SET 2

void foo(int f)
{
   int error = xthread_send(&mb, f);
   if (error==-1){
      printf("  --> ERROR in SEND: message box is full (id:%d)\n", currxid);
   }
}

void bar()
{
   int result = -1;
   xthread_recv(&mb, &result);
   printf("  --> %d (id:%d)\n", result, currxid);
}

void oof(int f)//, xthread_mbox_t *mptr)
{
   int error = xthread_broadcast(&mb, f);
   if (error==-1){
      printf("  --> ERROR IN BROADCAST: message box is full (id:%d)\n", currxid);
   } else if (error==-2){
      printf("  --> WARNING IN BROADCAST: receive threads might be in zombie state (id:%d)\n", currxid);   
   }
}


//  TEST FUNCTION SET 2
/*
void test1(int f)//, xthread_mbox_t *mptr)
{
   int error = xthread_send(&mb, f);
   if (error==-1){
      printf("  --> ERROR in SEND: message box is full (id:%d)\n", currxid);
   }
}

void test2()
{
   int result = -1;
   xthread_recv(&mb, &result);
   printf("  --> %d (id:%d)\n", result, currxid);
}
*/



xmain(int argc, char* argv[])
{
   int init_result = xthread_init_mbox(&mb);
   
   // EACH OF THESE SEGMENTS IS ONE SIGNLE TEST WHICH IS MENTIONED IN THE LAB REPORT

   ///*
   printf("\nTEST 1: RECEIVE --> SEND\n\n");
   xidbar1 = xthread_create(bar);
   xidfoo1 = xthread_create(foo, 1, 111);
   //*/

   
   /*
   printf("\nTEST 2: SEND --> RECEIVE\n\n");
   xidfoo2 = xthread_create(foo, 1, 222);
   xidbar2 = xthread_create(bar);
   */


   /*
   printf("\nTEST 3: RECEIVE --> RECEIVE --> BROADCAST\n\n");
   xidbar3 = xthread_create(bar);
   xidbar4 = xthread_create(bar);
   xidoof1 = xthread_create(oof, 1, 333);
   */

   
   /*
   printf("\nERROR TEST 1: SEND --> SEND --> RECEIVE (DIFFERENT ID)\n\n");
   xidfoo1 = xthread_create(foo, 1, 444);
   xidfoo2 = xthread_create(foo, 1, 555);
   xidbar1 = xthread_create(bar);
   */

   
   /*
   printf("\nERROR TEST 2: SEND --> SEND --> RECEIVE (SAME ID)\n\n");
   xidfoo1 = xthread_create(foo, 1, 666);
   xidfoo1 = xthread_create(foo, 1, 777);
   xidbar2 = xthread_create(bar);
   */
   
   
   /*
   printf("\nERROR TEST 3: BROADCAST --> RECEIVE\n\n");
   xidoof2 = xthread_create(oof, 1, 888);
   xidbar5 = xthread_create(bar);
   */
   
   
}


