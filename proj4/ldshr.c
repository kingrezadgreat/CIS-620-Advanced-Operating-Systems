#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include <pthread.h>
#include "ldshr.h"


struct packet{
	CLIENT * cl;
	double load;
	values input;
	double value;
	struct node * head;
};


void *getcpu(void * para){
	struct packet * msg = (struct packet *)para;
	msg->value = *sumqroot_lst_1(msg->head,msg->cl);
}


void *getgpu(void * para){
	struct packet * msg = (struct packet *)para;
	msg->value = *sumqroot_gpu_1(&msg->input,msg->cl);
}

void *getloads(void * para){
	struct packet * msg = (struct packet *)para;
	int * v;
	msg->load = *getload_1((void *)v, msg->cl);
}


void main (int argc, char **argv)
{

	char *ws[5] = {"chopin","bach","renoir","mozart","brahms"};
	struct packet packbuff[5];
	pthread_t threads[5];
	for(int i = 0; i < 5; i++)
	{
		packbuff[i].load=5;
		if(!(packbuff[i].cl = clnt_create(ws[i],RDBPROG,RDBVERS,"tcp"))){
		clnt_pcreateerror(argv[1]);
   		exit(1);
		}
	}
	for(int i =0; i<5; i++)
	{
		pthread_create(&threads[i],NULL,getloads,&packbuff[i]);			
	}
	
	for(int i =0; i<5; i++)
	{
		pthread_join(threads[i],NULL);
	}
	
	for(int i =0; i<5; i++)
	{
		printf("%s: %f  ",ws[i],packbuff[i].load);
	}
	printf("\n");
	
	double min1 = 1.0;
	double min2 = 1.0;
	
	int count1 = 0;
	int count2 = 0;
	for(int i =0; i<5; i++){
		if (min1> packbuff[i].load){
			min1 = packbuff[i].load;
			count1 = i;
		}
	}

	for(int i =0; i<5; i++){
		if (min2> packbuff[i].load && i!= count1){
			min2 = packbuff[i].load;
			count2 = i;

		}
	}
	
	//printf("%d %d \n", count1, count2);
	printf("(executed on %s and %s) \n", ws[count1], ws[count2] );


	if (strcmp(argv[1], "-lst") == 0){	
		FILE *fp;
		double input_data [1000];
		double temp;
		
		char str[80];
		
		strcpy(str, argv[2]);
		strcat(str, ".dat");
		
		fp = fopen(str, "r");
		if (fp == NULL) 
		{
			printf("I couldn't open results.dat for reading.\n");
			exit(0);
		}
	
		struct node *prev1,*head1,*prev2,*head2,*pp;
		head1=NULL;
	      	head2=NULL;
	 	
	 	int j = 0;
		while (fscanf(fp, "%lf\n", &temp) == 1)
		{
		        pp=malloc(sizeof(struct node));
	               	pp->value = temp;
	        	pp->next=NULL;

			if (j%2 == 0){
		        	if(head1==NULL)
		        		head1=pp;
	        		else
	        	    		prev1->next=pp;
	        		prev1=pp;
	
			}
			if (j%2 == 1){
		        	if(head2==NULL)
		        		head2=pp;
	        		else
	        	    		prev2->next=pp;
	        		prev2=pp;
	
			}
			j++;
		}
		
		packbuff[count1].head = head1;
		packbuff[count2].head = head2;

	
		pthread_create(&threads[count1],NULL,getcpu,&packbuff[count1]);
		//printf("hello\n");	
		pthread_create(&threads[count2],NULL,getcpu,&packbuff[count2]);
		pthread_join(threads[count1],NULL);
		pthread_join(threads[count2],NULL);
		double answer = packbuff[count1].value + packbuff[count2].value;
		printf("[Results from CPU Compute:  %f]\n",answer);
		}

	if (strcmp(argv[1], "-gpu") == 0)
	{	
	
		

		int n;
		int m;
		int s1;
		int s2;
		values input1,input2;
		input1.n = atoi(argv[2]);
		input1.m = atoi(argv[3]);
		input1.s = atoi(argv[4]);
		input2.n = atoi(argv[2]);
		input2.m = atoi(argv[3]);
		input2.s = atoi(argv[5]);
	
		packbuff[count1].input = input1;
		packbuff[count2].input = input2;
		pthread_create(&threads[count1],NULL,getgpu,&packbuff[count1]);
		pthread_create(&threads[count2],NULL,getgpu,&packbuff[count2]);
		pthread_join(threads[count1],NULL);
		pthread_join(threads[count2],NULL);
		double answer = packbuff[count1].value + packbuff[count2].value;
		printf("[Results from GPU Compute:  %f]\n",answer);
	}
	
}
