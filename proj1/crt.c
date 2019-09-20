#include <stdio.h>
#include <stdlib.h> 
#include <string.h>


void ep(int arg);
void et(int arg);
void *update(void *data);


int main(int argc, char **argv ) {

    char** new_argv = malloc((argc+1) * sizeof *new_argv);
    for(int i = 0; i < argc; ++i)
    {
        size_t length = strlen(argv[i])+1;
        new_argv[i] = malloc(length);
        memcpy(new_argv[i], argv[i], length);
    }
    new_argv[argc] = NULL;

    //printf("Running program: %s\n", new_argv[0]);
  
    int arg1 = atoi(argv[1]);

    if (new_argv[0][2] == 'p'){
        ep(arg1);
    } 
    if (new_argv[0][2] == 't'){
        et(arg1);
    }
    
    // free memory
    int i;
    for(i = 0; i < argc; ++i){
        free(new_argv[i]);
    }
    free(new_argv);



  return(0);
}


