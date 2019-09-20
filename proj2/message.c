#include <stdio.h>
#include<stdlib.h>
#include <proc.h>
#include <message.h>

#include <signal.h>
#include <unistd.h>



int xthread_init_mbox(xthread_mbox_t *mptr){
    mptr-> head = NULL;
    mptr-> tail = NULL;
    mptr-> message = -1;
    //printf("\n      mailbox initialized\n");
    return 0;
}

int xthread_send(xthread_mbox_t *mptr, int msg){
    int usec = ualarm(0,0);
    printf("      SEND start (id:%d)\n", currxid);

    if (mptr-> message > 0){
        // IF THE MAILBOX IS ALREADY FULL SEND AN ERROR -1 AND FINISH SEND
        printf("      SEND: ERROR mailbox full (id:%d)\n", currxid);
        ualarm(usec,0);
        return -1;
    
    } else if (mptr->head == NULL){
        // IF THERE IS NO WAITING THERAD IN THE MAILBOX JUST PUT THE MESSAGE IN MAILBOX
        printf("      SEND: message empty. NO linked list (id:%d)\n", currxid);
        
        mptr-> message = msg;

        ualarm(usec,0);
        printf("      SEND: ended (id:%d)\n", currxid);
        
        return 0;

    } else {
        // IF THERE IS WAITING THERAD IN THE MAILBOX, SEND THE MESSAGE DIRECTLY TO THE POINTER OF THREAD
        printf("      SEND: message empty, linkedlist AVAILABLE (id:%d)\n", currxid);
        
        // GET THE WAITING THREAD XID AND PUT THE MESSAGE TO THE ADDRESS OF THE THREAD
        // AND UPDATE HEAD AND TAIL
        int xid_wait = mptr-> head -> xid;
        *mptr-> head -> dest = msg;
        printf("      SEND: (id:%d) sends to (id:%d)\n", currxid, xid_wait);
        
        // NOW REMOVE HEAD AND UPDATE THE LINKED LIST
        node *myNode = mptr -> head; 
        mptr -> head = myNode->next;
        
        // IF HEAD IS EMPTY THEN UPDATE TAIL TO EMPTY TOO
        if (mptr -> head == NULL){
            mptr -> tail = NULL;
        }
        
        free(myNode);
        
        // SET THE WAITING THREAD TO READY AND LET IT DIE IN ITS RECEIVE THREAD
        xtab[xid_wait].xstate = XREADY;
        ualarm(usec,0);

        printf("      SEND ended (id:%d)\n", currxid);
        return 0;
    }
}


int xthread_broadcast(xthread_mbox_t *mptr, int msg){
    int usec = ualarm(0,0);
    printf("      BROADCAST start (id:%d)\n", currxid);
    if (mptr-> message > 0){
        // IF THE MAILBOX IS ALREADY FULL SEND AN ERROR -1 AND FINISH SEND
        printf("      BROADCAST: ERROR mailbox full (id:%d)\n", currxid);
        ualarm(usec,0);
        printf("      BROADCAST ended (id:%d)\n", currxid);
        return -1;
    
    } else if (mptr->head == NULL){
        // IF THERE IS NO WAITING THERAD IN THE MAILBOX THEN YOU CAN'T BROADCAST
        // DO NOTHING AND JUST RETURN -2 AS ERROR HANDLING ID
        printf("      BROADCAST WARNING: mailbox is empty --> no message to broadcast (id:%d)\n", currxid);
        ualarm(usec,0);
        printf("      BROADCAST ended (id:%d)\n", currxid);
        return -2;
        
    } else {
        // IF THERE IS A WAITING THERAD IN THE MAILBOX, SEND THE MESSAGE DIRECTLY TO THE POINTER OF THE THREAD
        // AND THEN UPDATE HEAD AND TAIL AND THEN SEND TO THE NEXT ONE UNTILL MAILBOX LINKEDLIST IS EMPTY 
        
        while(1){
            // IF THE THREAD LINKEDLIST IS EMPTY THEN BREAK THE WHILE LOOP
            if (mptr -> head == NULL){
                break;
            }
            printf("      BROADCAST: linkedlist AVAILABLE (id:%d)\n", currxid);
            
            // GET THE WAITING THREAD ID AND SET THE WAITING THREAD TO READY AND LET IT DIE IN THE RECEIVE WHEN SYSTEM RESCHEDULES
            int xid_wait = mptr-> head -> xid;
            xtab[xid_wait].xstate = XREADY;
            printf("      BROADCAST: (id:%d) sends to (id:%d)\n", currxid, xid_wait);
            
            // PUT THE MESSAGE TO THE ADDRESS PRIVIDED OF THE THREAD
            *mptr-> head -> dest = msg;
        
            // NOW REMOVE CURRENT HEAD AND UPDATE THE MESSAGE BOX HEAD 
            node *myNode = mptr -> head; 
            mptr -> head = myNode->next;

            // IF THE UPDATED HEAD IS EMPTY THEN UPDATE TAIL TO EMPTY TOO
            if (mptr -> head == NULL){
                mptr -> tail = NULL;
            }

            free(myNode);
        }
        ualarm(usec,0);
        printf("      BROADCAST ended (id:%d)\n", currxid);
        return 0;

    }
}

int xthread_recv(xthread_mbox_t *mptr, int *msgprt){
    printf("      RECEIVE: start (id:%d)\n", currxid);

    if ( mptr-> message == -1){
        // IF THE MESSAGE IS NULL THEN ADD THE NODE TO LINKEDLIST AND SET THE THREAD TO BLOCKING
        
        int usec = ualarm(0,0);
        printf("      RECEIVE: message empty -> BLOCKING (id:%d)\n", currxid);

        // SET THE THREAD TO BLOCKING
        xtab[currxid].xstate == XBLOCK;
        
        // GENERATE A NODE FOR LINKEDLIST AND UPDATE    
        node *temp_node;
        temp_node = (node *) malloc(sizeof(node));

        temp_node -> xid = currxid;
        temp_node -> next = NULL;
        
        // UPDATE ADDRESS POINTER
        temp_node -> dest = msgprt; 
        
        // UPDATE MESSAGEBOX
        if(mptr -> head == NULL){
            // IF RECEIVE THREAD IS THE FIRST ELEMENT IN LINKEDLIST
            mptr -> head = temp_node;
            mptr -> tail = temp_node;
        } else {
            // IF RECEIVE THREAD IS NOT THE FIRST ELEMENT IN LINKEDLIST
            mptr-> tail -> next = temp_node;
            mptr-> tail =temp_node;
        }
        
        //printf("----%d\n");
    
        ualarm(usec,0);
        
        // AS THE THREAD IS SET TO XBLOCK, YOU NEEDTO RESCHEDULE 
        resched();
    } 
    
    else {
        // IF THERE IS A MESSAGE IN THE MESSAGE BOX, REMOVE THE MESSAGE AND then SET IT TO -1

        int usec = ualarm(0,0);
        printf("      RECEIVE: message NOT empty (id:%d)\n", currxid);

        // SAVE CURRENT MESSAGE FROM MESSAGEBOX AND SET TO -1
        int curMsg = mptr-> message;
        mptr-> message = -1;

        // SAVE THE VALUE OF MESSAGE TO THE PROVIDED ADDRESS BY *msgprt
        *msgprt = curMsg;
        ualarm(usec,0);
    }
    
    printf("      RECEIVE ended (id:%d)\n", currxid);
    return 0;
}










