#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

volatile sig_atomic_t running = 1;
volatile sig_atomic_t sum = 0;
pid_t parentPID;

void handle_sigtstp(int sig) {
    printf("Child: SIGTSTP received. Sending sum to parent.\n");
    if (kill(parentPID, SIGUSR1) == -1) {
        perror("kill failed");
        exit(EXIT_FAILURE);
    }
}

void handle_siguser2(int sig) {
    printf("Child: Received SIGUSR2 from Parent. Current sum = %d\n", sum);
}

void handle_sigterm(int sig) {
    printf("Child: Received SIGTERM, exiting...\n");
    running = 0;
}

int main() {
    struct sigaction sigactionPause;
    struct sigaction sigactionUser2;
    struct sigaction sigactionTerminate;

    parentPID = getppid();

    sigemptyset(&sigactionPause.sa_mask);
    sigactionPause.sa_flags = 0;
    sigactionPause.sa_handler = handle_sigtstp;
    sigaction(SIGTSTP, &sigactionPause, NULL);

    sigemptyset(&sigactionUser2.sa_mask);
    sigactionUser2.sa_flags = 0;
    sigactionUser2.sa_handler = handle_siguser2;
    sigaction(SIGUSR2, &sigactionUser2, NULL);

    sigemptyset(&sigactionTerminate.sa_mask);
    sigactionTerminate.sa_flags = 0;
    sigactionTerminate.sa_handler = handle_sigterm;
    sigaction(SIGTERM, &sigactionTerminate, NULL);

    printf("Child: Running... Use 'kill -SIGTSTP %d' to interrupt me.\n", getpid());
    while (running) {
        sum += 10;
        sleep(1);
    }
    return 0;
}