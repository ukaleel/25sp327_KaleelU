#include <stdio.h>
#include <stdlib.h>

int main(){

    void *ptr = malloc(1024);
    if (!ptr){
        fprintf(stderr, "Could not allocate memory, malloc failed.\n");
    }

    printf("Success!\n");
    free(ptr);
    ptr = NULL;
}