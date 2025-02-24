#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getHostname() {
    FILE *file = fopen("/proc/sys/kernel/hostname", "r");
    if (!file) {
        fprintf(stderr, "Error opening hostname file\n");
        return;
    }
    char host[50];
    if (fgets(host, sizeof(host), file)) {
        printf("Current Host Name:           %s", host);
    }
    fclose(file);
}

void getCPU() {
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (!file) {
        fprintf(stderr, "Error opening CPU Info file\n");
        return;
    }
    char readInput[100];
    char modelName[100];
    char cpuType[50];
    int cores = 0;
    while (fgets(readInput, sizeof(readInput), file)) {
        if (strncmp(readInput, "model name", 10) == 0) {
            sscanf(readInput, "model name : %[^\n]", modelName);
        }
        if (strncmp(readInput, "vendor_id", 9) == 0) {
            sscanf(readInput, "vendor_id : %[^\n]", cpuType);
        }
        if (strncmp(readInput, "cpu cores", 9) == 0) {
            sscanf(readInput, "cpu cores : %d", &cores);
        }
    }
    printf("CPU Type:                    %s\n", cpuType);
    printf("CPU Model:                   %s\n", modelName);
    printf("CPU Cores:                   %d\n", cores);
    fclose(file);
}

void getKernel() {
    FILE *file = fopen("/proc/version", "r");
    if (!file) {
        fprintf(stderr, "Error opening kernel version file\n");
        return;
    }

    char version[100];
    if (fgets(version, sizeof(version), file)) {
        char kernel[100]; 
        char bufferCatch[100]; 
        sscanf(version, "Linux version %99s %99s", kernel, bufferCatch);
        printf("Kernel Version:              Linux version %s\n", kernel);
    }

    fclose(file);
}

void getMemory() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        fprintf(stderr, "Error opening memory information file\n");
        return;
    }
    char readInput[100];
    long totalMemory = 0;
    long availableMemory = 0;
    while (fgets(readInput, sizeof(readInput), file)) {
        if (strncmp(readInput, "MemTotal:", 9) == 0) {
            sscanf(readInput, "MemTotal: %ld kB", &totalMemory);
        }
        if (strncmp(readInput, "MemAvailable:", 12) == 0) {
            sscanf(readInput, "MemAvailable: %ld kB", &availableMemory);
        }
    }
    printf("Memory Configured:           %ld kB\n", totalMemory);
    printf("Memory Available:            %ld kB\n", availableMemory);
    fclose(file);
}

void getUptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        fprintf(stderr, "Error opening uptime file\n");
        return;
    }
    double uptime;
    if (fscanf(file, "%lf", &uptime) != 1) {
        fprintf(stderr, "Error reading uptime file\n");
        fclose(file);
        return;
    }
    fclose(file);
    int days = uptime / (24 * 3600);
    int hours = (uptime - (days * 24 * 3600)) / 3600;
    int minutes = (uptime - (days * 24 * 3600) - (hours * 3600)) / 60;
    int seconds = uptime - (days * 24 * 3600) - (hours * 3600) - (minutes * 60);
    printf("Uptime:                      %d:%02d:%02d:%02d\n", days, hours, minutes, seconds);
}

void getStat() {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        fprintf(stderr, "Error opening stat file\n");
        return;
    }
    char line[100];
    long userTime;
    long systemTime;
    long idleTime;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu ", 4) == 0) {
            long executionTime;
            long waitingTime;
            long interruptTime;
            sscanf(line, "cpu %ld %ld %ld %ld %ld %ld", &userTime, &executionTime, &systemTime, &idleTime, &waitingTime, &interruptTime);
            userTime += executionTime;
            break;
        }
    }
    printf("Time Spent in User Mode:     %ld ms\n", userTime);
    printf("Time Spent in System Mode:   %ld ms\n", systemTime);
    printf("Time Spent in Idle Mode:     %ld ms\n", idleTime);
    fclose(file);
}

int main() {
    printf("----------------------------------------------------------------------------\n");
    printf("                             System Information\n");
    printf("----------------------------------------------------------------------------\n");
    getHostname();
    getCPU();
    getKernel();
    getMemory();
    getUptime();
    getStat();
    printf("----------------------------------------------------------------------------\n");
    return 0;
}
