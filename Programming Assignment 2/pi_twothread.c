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

    pthread_t tid_1; // thread 1 identifier
    pthread_t tid_2; // thread 2 identifier

    /* set default thread attributes */
    pthread_attr_t attr_1;
    pthread_attr_t attr_2;
    pthread_attr_init(&attr_1);
    pthread_attr_init(&attr_2);


    /* create and join threads */
    pthread_create(&tid_1, &attr_1, runner, argv[1]);
    pthread_create(&tid_2, &attr_2, runner, argv[1]);
    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);

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