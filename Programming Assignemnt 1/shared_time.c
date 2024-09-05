#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

/* shared memory headers */
#include <sys/mman.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error, too few arguments");
        return -1;
    }

    /* define constants for shared memory */
    const char *memoryName = "timeMemory";
    const int memorySize = 4096;

    /* fork child and check for error */
    pid_t pid = fork();

    if(pid < 0) {
        perror("Error calling fork()");
        return -1;

    } else if(pid == 0) {
        /* child process path */

        /* set up the file descriptor, size and map the memory */
        int memoryFd = shm_open(memoryName,  O_CREAT | O_RDWR, 0666);
        ftruncate(memoryFd, memorySize);
        struct timeval *startTime = (struct timeval *) mmap(0, memorySize, PROT_READ | PROT_WRITE, MAP_SHARED, memoryFd, 0);

        /* record the starting time */
        gettimeofday(startTime, NULL);

        /* load command into child process. */
        exit(execvp(argv[1], &argv[1]));

    } else {
        /* parent process path */

        wait(&pid); // wait for child process to terminate.

        /* access and read the start time from shared memory */
        int memoryFd = shm_open(memoryName, O_RDONLY, 0666);
        struct timeval *startTime = (struct timeval *) mmap(0, memorySize, PROT_READ, MAP_SHARED, memoryFd, 0);

        /* record current time */
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);

        /* calculate time delta */
        double elapsedTime = (double)(currentTime.tv_sec - startTime->tv_sec) + ((double)(currentTime.tv_usec - startTime->tv_usec) / 1000000);

        printf("Elapsed time: %.5f\n", elapsedTime);

        /* close and unlink the shared memory */
        close(memoryFd);
        shm_unlink(memoryName);

    }

    return 0;
}