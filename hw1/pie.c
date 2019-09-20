#include <stdlib.h>
const int M = 70000;
double cherrypie(int seed)
{   
    double x,y,z; 
    int i, count=0;
    srand48(seed);
    for(i=0; i<M; i++ ) {
         x= drand48();
         y= drand48();
         if(x*x+y*y < 1.0) count++;
    }
    z = (float)count / M * 4.0 ;
    return( z );
}
