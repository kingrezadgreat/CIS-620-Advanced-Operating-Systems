#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>  
#include <pthread.h>

float etime(void);
void *update(int *data);

int n;

void et(int arg) {
  n = arg*1000/4;
  pthread_t  cpid;
  int i;
  int *a;
  a = calloc(n, sizeof(int));
  
  float result;
  etime();
  if ((pthread_create(&cpid, NULL, (void *)&update, (void*)&a[0] )) != 0){
    printf("error. no thread made");
  } 
  pthread_join(cpid, NULL);
  free(a);
  result = etime();
  printf("lapsed time: %f\n",result);

}



void *update(int *data)
{ 
    int i;
    for(i=0 ; i < n ; i++ ) {
      data[i] = 3;  
    }
}

