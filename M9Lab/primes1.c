#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * isprime(void * data);

int main(int argc, char *argv[]) {
    long long number1;
    long long number2;

    

    if (argc != 3) {
        fprintf(stderr, "Enter two integer values.\n");
        exit(EXIT_FAILURE);
    }
    number1 = atoll(argv[1]);
    number2 = atoll(argv[2]);

    int rval;
    pthread_attr_t th_attr;

    if ((rval=pthread_attr_init(&th_attr))!=0){
        handle_error(rval,"pthread_attr_init");
    }

    if ((rval=pthread_attr_setdetachstate(&th_attr,PTHREAD_CREATE_JOINABLE))!=0){
        handle_error(rval,"pthread_attr_setdetachstate");
    }
 
    pthread_t tid_prime1;
    pthread_t tid_prime2;
    pthread_t tid_progress;

    if ((rval=pthread_create(&tid_prime1, &th_attr, isprime, &number1))!=0){
        handle_error(rval,"pthread_create isprime1");
    }

    if ((rval=pthread_create(&tid_prime2, &th_attr, isprime, &number2))!=0){
        handle_error(rval,"pthread_create isprime2");
    }
    if ((rval=pthread_create(&tid_progress, &th_attr, progress, NULL))!=0){
        handle_error(rval,"pthread_create progress");
    }

    pthread_detach(tid_progress);
    pthread_join(tid_prime1, NULL);
    pthread_join(tid_prime2, NULL);


    if ((rval=pthread_cancel(tid_progress))!=0){
        handle_error(rval,"pthread_cancel");
    }

    pthread_attr_destroy(&th_attr);
    printf("Done!\n");

    return 0;
}


void *isprime(void *data){
    long long num = *((long long *)data);
    int prime = 1;
    for (long long j = 2; j < num; j++) {
        if (num % j == 0) {
            prime = 0;
        }
    }

    printf("\n%lld is %s\n", num, prime ? "prime" : "composite");
    pthread_exit(NULL);
}

void * progress(void *data){
    (void)data;
    while(1){
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    pthread_exit(NULL);
}

