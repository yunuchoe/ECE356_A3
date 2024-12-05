#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// MAX_ARIPLANES == THREAD_COUNT
#define MAX_AIRPLANES 10
#define THREAD_COUNT 10

sem_t runway; 
sem_t control_tower; 


void* action(void *x){ 
    int n = *(int *)x;

    // Force an order to prevent deadlock
    //wait for control tower
    sem_wait(&control_tower);

    // wait for runway
    sem_wait(&runway); 

    printf("Runway is clear for Plane %d\n", n);
            
    // do action
    printf("Plane %d is taking off/landing after confirming with control\n", n);
    sleep(2);

    // clear
    sem_post(&control_tower); 
    sem_post(&runway); 
} 


int main(int argc, char* argv[]) {

    // ready the semaphores
    sem_init(&runway, 0, 3); // three instances allowed
    sem_init(&control_tower, 0, 1); // only one instance allowed at a time

    // create all the threads
    pthread_t thread[THREAD_COUNT];

    // plane instances
    int plane[MAX_AIRPLANES];
    for(int i = 0; i < MAX_AIRPLANES; i++){
        plane[i] = i + 1; // plane starts from 1
    }

    // create all threads
    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_create(&thread[i], NULL, action, &plane[i%MAX_AIRPLANES]); 
    }

    // join all threads
    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(thread[i], NULL); 
    }

    // destroy semaphores once complete
    sem_destroy(&runway);
    sem_destroy(&control_tower);

    return 0;
}
