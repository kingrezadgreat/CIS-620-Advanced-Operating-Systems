#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>


float etime(void);
void *Calculate(int *data);

#define dim 160
int a [dim][dim];	
int b [dim][dim];
int c [dim][dim];
int dx = 0;

int main(int argc, char **argv ) {

    int i;  
    int j;
    for (i=0;i<dim;i++){
        for (j=0;j<dim;j++){
            a [i][j] = 1;
            b [i][j] = 1;
            c [i][j] = 0;
        }
    }
    
    int arg1 = atoi(argv[1]);
    pthread_t thread_id[arg1];
    
    dx = dim/arg1;
    int start = 0;
    int array [arg1]; 
    
    for(int i=0; i < arg1; i++)
    {
        array [i] = start;
        start = start + dim/arg1;
    }
    etime();
    for(int i=0; i < arg1; i++)
    {
    
        if ((pthread_create( &thread_id[i], NULL, (void *)&Calculate, (void *)&array[i] )) !=0){
            printf("error. no thread made");
        }
        //start = start + dim/arg1;
    }
    
    for(int j=0; j < arg1; j++)
    {
        pthread_join( thread_id[j], NULL);
    }
    
    float result = etime();
    printf("lapsed time: %f\n",result);
    
    return(0);
}



void *Calculate(int *data)
{
    int x;
    x =  *data;


    int temp = 0;
    int i;
    int j;
    int k;
    for (i=x;i<(x+dx);i++){
        //printf("%d\n",i);
        for (j=0;j<dim;j++){
            temp = 0;
            for (k=0;k<dim;k++){
                temp = temp + a[i][k]*b[k][j];
            }
            c[i][j] = temp;
        }
    }
}

















