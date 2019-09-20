/*    messege.h    */
#include <stdio.h>
#include <stdlib.h>


struct linked_list
{
    int xid;
    int *dest;
    struct linked_list *next;
};

typedef struct linked_list node;

struct msg_box
{
    int message;
    node *head;
    node *tail;
    
};

typedef struct msg_box xthread_mbox_t;



