#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// MAX_ARIPLANES == THREAD_COUNT
#define MAX_AIRPLANES 20
#define THREAD_COUNT 20

sem_t runway; 
sem_t control_tower; 

int queue[MAX_AIRPLANES]; // 0 represents empty
int flying[MAX_AIRPLANES]; // 0 represents gound, 1 represents flying - all planes start grounded
int timer[MAX_AIRPLANES]; // counts how many loops it took to complete

double counter = 0;

void* action(void *x){ 
    int n = *(int *)x;

    // place plane in queue
    for(int i = 0; i < MAX_AIRPLANES; i++){
        if(queue[i] == 0){ // search until an empty (0) spot is found
            queue[i] = n; // put the plane number in the first empty spot
            break; // end early so we dont waste time
        }
    }

    int status = 0; // set status as 0 (incompelte)
    while(status == 0){ // keep looping until status is 1 (complete)

        // if something is waiting a while, put it at the front of the queue
        double limit = (double)THREAD_COUNT;
        if( timer[n] > ((limit) + ((limit/5)*counter)) ){ // arbitrialy chosen as the amount of threads
                                                         // increases each time to ensure that not too many are pushed at the same time
            for (int i = n; i > 0; i--) { // start at the chosen queue spot (n)
                queue[i] = queue[i - 1]; // shift everything over to the right by (the original will be overridden)
            }
            queue[0] = n; // place n at the front
            counter += 1;
        }

        if(n == queue[0]){ // wait until first in queue


            //wait for control tower
            sem_wait(&control_tower);

            // wait for runway
            sem_wait(&runway); 
            printf("Runway is clear for Plane %d\n", n);
            
            // do action
            printf("Plane %d is taking off/landing after confirming with control\n", n);
            sleep(2);

            // resize queue
            for (int i = 0; i < MAX_AIRPLANES - 1; i++) {
                queue[i] = queue[i + 1]; // move queue forward by 1
            }
            queue[MAX_AIRPLANES - 1] = 0; // clear last slot

            // clear
            sem_post(&runway); 
            sem_post(&control_tower); 
            status = 1; // signal completion
        }else{
            timer[n] += 1;
        }
        sleep(1); // limit the refresh rate so the computer isnt too strained
    }
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
        // For sake of control and cleanliness, I made this using loops rather than making each thread 
        // So we cant have for example plane1doing something 7 times and plane4 doing something 3 times etc...
        // each plane will do more or less the same amound of stuff (+/- 1)
    }

    // join all threads
    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(thread[i], NULL); 
    }

    // destroy semaphores once complete
    sem_destroy(&runway);
    sem_destroy(&control_tower);

    //print the log
    printf("\nWaiting time log:\n");
    for(int i = 1; i <= MAX_AIRPLANES; i++){
        printf("Plane %d was in queue for a total of %d iterations\n", i, timer[i]); // roughly in seconds
    }
    
    return 0;
}
