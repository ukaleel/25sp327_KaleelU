#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

volatile sig_atomic_t sumReceived = 0;
volatile sig_atomic_t childTermFlag = 0;
pid_t childPID;

void handle_siguser1(int sig, siginfo_t *info, void *context) {
    if (info->si_pid == childPID) {
        sumReceived = info->si_value.sival_int;
        printf("Parent: Received SIGUSR1 from Child (PID: %d). Sum = %d\n", childPID, sumReceived);
        kill(childPID, SIGUSR2);
    }
}

void handle_sigchld(int sig) {
    int status;
    waitpid(childPID, &status, 0);
    printf("Parent: Child (PID: %d) has terminated with status %d. Exiting.\n", childPID, WEXITSTATUS(status));
    childTermFlag = 1;
}

int main() {
    struct sigaction sigactionUser1;
    struct sigaction sigactionChild;

    childPID = fork();
    if (childPID == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (childPID == 0) {
        execl("./child", "child", NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    sigactionUser1.sa_flags = SA_SIGINFO;
    sigactionUser1.sa_sigaction = handle_siguser1;
    sigaction(SIGUSR1, &sigactionUser1, NULL);

    sigactionChild.sa_flags = SA_NOCLDSTOP;
    sigactionChild.sa_handler = handle_sigchld;
    sigaction(SIGCHLD, &sigactionChild, NULL);

    printf("Parent: fork-exec successful. Child pid (%d)\n", childPID);
    while (!childTermFlag) {
        printf("Parent: Working...\n");
        sleep(3);
    }
    return 0;
}