#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

volatile long long counter_mutex = 0; // Counter for Mutex Worker
volatile long long counter_semaphore = 0; // Counter for Binary Semaphore Worker
volatile long long counter_cas = 0; // Counter for Compare-and-Swap Worker
volatile long long counter_tas = 0; // Counter for Test-and-Set Worker
volatile long long counter_dum = 0; // Counter without sync

volatile int tas_lock = 0; // 0: unlocked, 1: locked

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;


// 1. Mutex worker, finish the code to use pthread mutex to synchronize threads
void* mutexWorker(void* arg) {
    long maxcount = *(long*)arg;
    for (long i = 0; i < maxcount; i++) {
        pthread_mutex_lock(&mutex);
        counter_mutex++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// 2. Semaphore Worker, finish the code to use semaphore to synchronize threads
void* semaphoreWorker(void* arg) {
    long maxcount = *(long*)arg;
    for (long i = 0; i < maxcount; i++) {
        sem_wait(&semaphore);
        counter_semaphore++;
        sem_post(&semaphore);
    }
    return NULL;
}

// 3. CAS worker, finish the code to use CAS for atomic_increment
void atomic_increment(long long* value) {
    // finish the code for CAS-based atomic increment
}

void* CASWorker(void* arg) {
    long maxcount = *(long*)arg;
    for (long i = 0; i < maxcount; i++) {
        atomic_increment(&counter_cas);
    }
    return NULL;
}

void tas_lock_acquire(volatile int* lock) {
    // finish the code for TAS lock 
}

void tas_lock_release(volatile int* lock) {
    // finish the code for TAS lock 
}
// 4. Test-and-Set Worker, finish the code with TAS lock 
void* testAndSetWorker(void* arg) {
    long maxcount = *(long*)arg;
    for (long i = 0; i < maxcount; i++) {
        counter_tas++;
    }
    return NULL;
}

// 5. Dum worker
void *DumWorker(void *arg) {
    long maxcount = *(long*)arg;
    for (int i = 0; i < maxcount; i++) {
        counter_dum++;
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <NumThread> <MaxCount> \n", argv[0]);
        exit(1);
    }
    int num_threads = atoi(argv[1]);
    long maxcount = atol(argv[2]);

    long long elapsed;
    pthread_t threads[num_threads];

    // Dum increment   

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, DumWorker, &maxcount);
    }
    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // measure the execution time and print final counter value
    printf("Dum Time [%lld], final counter value: %lld \n", elapsed, counter_dum);


    // CAS increment

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, CASWorker, &maxcount);
    }
    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("CAS Time [%lld], final counter value: %lld \n", elapsed, counter_cas);

    // TAS increment   

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, testAndSetWorker, &maxcount);
    }
    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("TAS Time [%lld], final counter value: %lld \n", elapsed, counter_tas);


    // Mutex increment   

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, mutexWorker, &maxcount);
    }
    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("MUT Time [%lld], final counter value: %lld \n", elapsed, counter_mutex);

    // Semaphore increment   
    // Initialize the semaphore here
    sem_init(&semaphore, 0, 1);

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, semaphoreWorker, &maxcount);
    }
    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("SEM Time [%lld], final counter value: %lld \n", elapsed, counter_semaphore);

    // Destroy mutex and semaphore
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
    return 0;
}