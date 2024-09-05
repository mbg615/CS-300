#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error, too few arguments");
        return -1;
    }

    /* create pipe file descriptor, open pipe and check for error */
    int pipeFd[2];
    if(pipe(pipeFd) == -1) {
        perror("Error making pipe");
        return -1;
    }

    /* fork child and check for error */
    pid_t pid = fork();

    if(pid < 0) {
        perror("Error calling fork()");
        return -1;

    } else if(pid == 0) {
        /* child process path */

        close(pipeFd[0]); // close unused (READ) end of pipe.

        /* record the starting time */
        struct timeval startTime;
        gettimeofday(&startTime, NULL);

        /* write the starting time to the pipe */
        write(pipeFd[1], &startTime, sizeof(startTime));
        close(pipeFd[1]); // close write end of pipe.

        /* load command into child process. */
        exit(execvp(argv[1], &argv[1]));

    } else {
        /* parent process path */

        close(pipeFd[1]); // close unused (WRITE) end of pipe.

        struct timeval startTime;
        struct timeval currentTime;

        wait(&pid); // wait for child process to terminate.

        /* read the starting time from the pipe and record current time */
        read(pipeFd[0], &startTime, sizeof(startTime));
        gettimeofday(&currentTime, NULL);

        /* calculate time delta */
        double elapsedTime = (double)(currentTime.tv_sec - startTime.tv_sec) + ((double)(currentTime.tv_usec - startTime.tv_usec) / 1000000);

        printf("Elapsed time: %.5f\n", elapsedTime);
        close(pipeFd[0]); // close read end of pipe.

    }

    return 0;
}