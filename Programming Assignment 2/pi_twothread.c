#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int pointsInCircle = 0;

void *runner(void *param);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Error, too few arguments");
        return -1;
    }

    const int numOfThreads = 2;

    /* create thread identifiers */
    pthread_t tids[numOfThreads];

    /* set default thread attributes */
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /* create threads */
    pthread_create(&tids[0], &attr, runner, argv[1]);
    pthread_create(&tids[1], &attr, runner, argv[1]);

    /* join threads */
    for(int i = 0; i < numOfThreads; i++) {
        pthread_join(tids[i], NULL);
    }

    /* estimate and print the value for pi */
    printf("%lf", (4 * pointsInCircle / strtod(argv[1], NULL)));

    return 0;
}

void *runner(void *param) {
    long maxPoints = strtol(param, NULL, 10);

    /* Generate pseudorandom x and y values to create points */
    for(int i = 0; i < maxPoints; i++) {
        double x = (double)rand() / (double)RAND_MAX;
        double y = (double)rand() / (double)RAND_MAX;

        /* check if the point is inside the unit circle*/
        if(pow(x, 2) + pow(y, 2) <= 1) {
            pointsInCircle++;
        }
    }

    pthread_exit(0);
}