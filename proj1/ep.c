#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>  
#include <unistd.h>


float etime(void);
 
void ep(int arg) {

  int n = arg*1000/4;
  pid_t  cpid;
  int    i;
  int *a;
  a = calloc(n, sizeof(int));
  
  etime();
  if ((cpid = fork()) == 0){
    for (i=0;i<n;i++){
        a[i] = a[i]+i;
    }
    free( a );
    //printf ("End of Child\n");
    exit(0);
      
  } else {
    int status;
    waitpid(cpid, &status, 0);
    free( a );
    

    float result;
    result = etime();
    printf("lapsed time: %f\n",result);
    //printf ("End of Parent\n");
  }
}
