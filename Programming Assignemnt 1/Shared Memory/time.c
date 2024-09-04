#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

/* Shared Memory Headers */
#include <sys/mman.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[]) {
    const char *memoryName = "timeMemory";
    const int memorySize = 4096;

    pid_t pid = fork();

    if(pid < 0) {
        perror("Error calling fork()");
        return -1;

    } else if(pid == 0) {
        /* Child Process Path */

        /* Set up the shared memory */
        int memoryFd = shm_open(memoryName,  O_CREAT | O_RDWR, 0666);
        ftruncate(memoryFd, memorySize);
        struct timeval *startTime = (struct timeval *) mmap(0, memorySize, PROT_READ | PROT_WRITE, MAP_SHARED, memoryFd, 0);

        gettimeofday(startTime, NULL);

        /* Load in the new process */
        exit(execvp(argv[1], &argv[1]));

    } else {
        /* Parent Process Path */
        wait(&pid);

        /* Access the shared memory */
        int memoryFd = shm_open(memoryName, O_RDONLY, 0666);
        struct timeval *startTime = (struct timeval *) mmap(0, memorySize, PROT_READ, MAP_SHARED, memoryFd, 0);

        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);

        /* Calculate time delta */

        double elapsedTime = (double)(currentTime.tv_sec - startTime->tv_sec) + ((double)(currentTime.tv_usec - startTime->tv_usec) / 1000000);

        printf("Elapsed time: %.5f\n", elapsedTime);

        close(memoryFd);
        shm_unlink(memoryName);

    }

    return 0;
}