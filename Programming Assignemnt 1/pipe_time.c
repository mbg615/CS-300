#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error, too few arguments");
        return -1;
    }

    int pipeFd[2];
    if(pipe(pipeFd) == -1) {
        perror("Error making pipe");
        return -1;
    }

    pid_t pid = fork();

    if(pid < 0) {
        perror("Error calling fork()");
        return -1;
    } else if(pid == 0) {
        /* Child Process Path */
        close(pipeFd[0]);

        struct timeval startTime;
        gettimeofday(&startTime, NULL);

        write(pipeFd[1], &startTime, sizeof(startTime));
        close(pipeFd[1]);

        exit(execvp(argv[1], &argv[1]));

    } else {
        /* Parent Process Path */

        close(pipeFd[1]);

        struct timeval startTime;
        struct timeval currentTime;

        wait(&pid);

        read(pipeFd[0], &startTime, sizeof(startTime));
        gettimeofday(&currentTime, NULL);

        /* Calculate time delta */
        double elapsedTime = (double)(currentTime.tv_sec - startTime.tv_sec) + ((double)(currentTime.tv_usec - startTime.tv_usec) / 1000000);

        printf("Elapsed time: %.5f\n", elapsedTime);
        close(pipeFd[0]);

    }

    return 0;
}