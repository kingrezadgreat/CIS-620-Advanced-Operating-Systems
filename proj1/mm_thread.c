#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>


float etime(void);
//void *Calculate(void *data);

#define dim 160
int a [dim][dim];	
int b [dim][dim];
int c [dim][dim];

int main(int argc, char **argv ) {

    char** new_argv = malloc((argc+1) * sizeof *new_argv);
    for(int i = 0; i < argc; ++i)
    {
        size_t length = strlen(argv[i])+1;
        new_argv[i] = malloc(length);
        memcpy(new_argv[i], argv[i], length);
    }
    new_argv[argc] = NULL;

    // do operations on new_argv
    for(int i = 0; i < argc; ++i)
    {
        //printf("%s\n", new_argv[i]);
    }
    
    printf("Running program: %s\n", new_argv[0]);
  
    int arg1 = atoi(argv[1]);

    /*
    int dim = 160;
    int a [dim][dim];	
    int b [dim][dim];
    int c [dim][dim];
    */
    
    
    for (int i=0;i<dim;i++){
        for (int j=0;j<dim;j++){
            a [i][j] = 1;
            b [i][j] = 1;
            c [i][j] = 0;
        }
    }
    
    
    //printf("%d\n",a[5][10]);
    
    /*
    
    pthread_t thread_id[arg1];
    
    int *ptr = 2;
    printf("   %d\n",ptr);
    
    for(int i=0; i < arg1; i++)
    {
        pthread_create( &thread_id[i], NULL, Calculate, (void*)ptr );
    }
    for(int j=0; j < arg1; j++)
    {
        pthread_join( thread_id[j], NULL);
    }
    */




    // free memory
    for(int i = 0; i < argc; ++i)
    {
        free(new_argv[i]);
    }
    free(new_argv);

  return(0);
}


/*
void *Calculate(void *data)
{
    printf("hello %d\n",data);
}

*/
