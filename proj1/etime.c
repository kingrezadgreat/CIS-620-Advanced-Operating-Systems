#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdlib.h> 
#include <sys/time.h>


float etime(void) {

    static struct timeval start_time,stop_time;
    
    float elapsed_time;

    gettimeofday(&stop_time,(struct timezone*)NULL);

    elapsed_time = (stop_time.tv_sec - start_time.tv_sec) + (float)(stop_time.tv_usec - start_time.tv_usec)/1000000.0;

    gettimeofday(&start_time,(struct timezone*)NULL);

    return elapsed_time;
}
