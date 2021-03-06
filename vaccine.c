/*

 * Each line corresponds to person arrive for vaccinationn 
 * and is formatted as:
 *
 * <person_id>:<arrival_time>,<service_time>
 * 
 * NOTE: All times represented in `Tenths of a Second`.
 * 
 * Expected Sample Output:
 * -----------------------
 * Person 1: Arrived at 3.
 * Person 1: Added to the queue.
 * Vaccine Station 1: START Person 1 Vaccination.
 * Vaccine Station 1: FINISH Person 1 Vaccination.
 * 
 */

   
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>


/*
 * A queue structure provided to you by the teaching team. This header
 * file contains the function prototypes; the queue routines are
 * linked in from a separate .o file (which is done for you via
 * the `makefile`).
 */
#include "queue.h"


/* 
 * Some compile-time constants related to assignment description.
 */
#define MAX_VAC_STATIONS 10
#define MAX_INPUT_LINE 100
#define TENTHS_TO_SEC 100000


/*
 * Here are variables that are available to all threads in the
 * process. Given these are global, you need not pass them as
 * parameters to functions. However, you must properly initialize
 * the queue, the mutex, and the condition variable.
 */
Queue_t *queue;
pthread_mutex_t queue_mutex;
pthread_cond_t queue_condvar;
unsigned int num_vac_stations;
unsigned int is_vac_completed = false;
int totalPatient=0;


/*
 * Function: reg_desk
 * ------------------
 *  Registration Desk Thread.
 *  Reads the input file and adds the vaccination persons to the
 *  queue as per their arrival times.
 * 
 *  arg: Input file name
 *  
 *  returns: null
 */
void *reg_desk(void *arg) {
    int sizeQue;
    char *filename = (char *)arg;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "File \"%s\" does not exist.\n", filename);
        exit(1);
    }

    char line[MAX_INPUT_LINE];
    unsigned int current_time = 0;

    while (fgets(line, sizeof(line), fp)) {
        // printf("---------------------\n");
        totalPatient++;
        int person_id;
        int person_arrival_time;
        int person_service_time;

        int vars_read = sscanf(line, "%d:%d,%d", &person_id, 
            &person_arrival_time, &person_service_time);

        if (vars_read == EOF || vars_read != 3) {
            fprintf(stderr, "Error reading from the file.\n");
            exit(1);
        }

        if (person_id < 0 || person_arrival_time < 0 || 
            person_service_time < 0)
        {
            fprintf(stderr, "Incorrect file input.\n");
            exit(1);
        }

        int arrival_time = person_arrival_time;

        // Sleep to simulate the persons arrival time.
        usleep((arrival_time - current_time) * TENTHS_TO_SEC);
        fprintf(stdout, "Person %d: arrived at time %d.\n", 
            person_id, arrival_time);

        // Update the current time based on simulated time elapsed.
        current_time = arrival_time;

        // TODO ... Insert your Code Here
        // For what to implement, please refer to the function
        // description above. Beware that you are now working in a 
        // multi-threaded scenario.

        //Lock the mutex
        pthread_mutex_lock(&queue_mutex);
        PersonInfo_t *person=new_person();//Initialize struct person node
        person->id=person_id;                       
        person->arrival_time=person_arrival_time;
        person->service_time=person_service_time;
        enqueue(queue,person);
        sizeQue=queue_size(queue);
        printf("Person %d: Added to the queue.\n",person->id);
        //Unlock the mutex
        pthread_mutex_unlock(&queue_mutex);

        pthread_cond_broadcast(&queue_condvar);
        
    //^^^^^^Queue Area^^^^^^^^//
    }
    

    fclose(fp);
    
    // TODO ... Insert your Code Here
    // Notify all waiting threads that the vaccination drive is now 
    // completed.
    
 


    // return NULL;
    //self define
    // char *filename = (char *)arg;
}


/*
 * Function: vac_station
 * ---------------------
 *  Vaccination Station Thread.
 *  Vaccinate the persons from the queue as per their service times.
 *
 *  arg: Vaccination station number
 *
 *  returns: null
 *
 * Remember: When performing a vaccination, the vac_station 
 * must sleep for the period of time required to "service"
 * that "person". (This is part of the simulation). Assuming
 * the "person" to be serviced is a pointer to an instance of
 * PersonInfo, the sleep would be something like:
 *
 *      usleep(person->service_time * TENTHS_TO_SEC);
 *
 */
void *vac_station(void* arg) {//void *arg
    //shared Value(index of Vac_Stations)
    int station_num = *((int *)arg) ;
    deallocate(arg);
    //checking for the end of the service
    while (is_vac_completed==false) {////////////!!!!!!!!!!!!
    //     // TODO ... Insert your Code Here
    //     // For what to implement, please refer to the function
    //     // description above and the assignment description.
    pthread_mutex_lock(&queue_mutex);
    while(is_empty(queue)==1){
        // printf("Waiting...\n");//TESTING Threads checking the quantity inside Queue
        pthread_cond_wait(&queue_condvar,&queue_mutex);
    }
   
    PersonInfo_t *finPerson;
    finPerson=dequeue(queue);
    printf("Vaccine Station %d : START Person %d Vaccination\n",station_num,finPerson->id);//KEEPER
    pthread_mutex_unlock(&queue_mutex);
    usleep(finPerson->service_time * TENTHS_TO_SEC);
    printf("Vaccine Station %d : FINISH Person %d Vaccination\n",station_num,finPerson->id);
    totalPatient--;
    // printf("=================================================%d==num_vac_stations=%d\n",totalPatient,num_vac_stations);
    if(totalPatient==num_vac_stations-1){// #of station -1 = how many station should left to deal with the rest of patients
        is_vac_completed=true;
    }
    
        
    }/////////!!!!!!!!!!!!
    // return NULL;

 
}


/*
 * Function: validate_args
 * -----------------------
 *  Validate the input command line args.
 *
 *  argc: Number of command line arguments provided
 *  argv: Command line arguments
 */
void validate_args(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid number of input args provided! "
                        "Expected 2, received %d\n", 
                        argc - 1);
        exit(1);
    }

    num_vac_stations = atoi(argv[1]);
    if ((num_vac_stations <= 0) || (num_vac_stations > MAX_VAC_STATIONS)) {
        fprintf(stderr, "Vaccine stations exceeded the MAX LIMIT.%d\n", 
            argc - 1);
        exit(1);
    }

}

/*
 * Function: initialize_vars
 * -------------------------
 *  Initialize the mutex, conditional variable and the queue.
 */
void initialize_vars(int argc, char *argv[]) {
    queue=init_queue();

    
    //Initialize  mutex
    if (pthread_mutex_init(&queue_mutex, NULL) != 0) {
        printf("\n mutex init has failed\n");
    }
    //Initialize conditional variables
    if (pthread_cond_init(&queue_condvar, NULL) != 0) {
        printf("\n condition variables init has failed\n");
    }
    // TODO ... Insert your Code Here
    // For what to implement, please refer to the function
    // description above.
   
    
}


/*
 * Function: main
 * -------------------------
 * argv[0]: Number of Vaccination Stations 
 * argv[1]: Input file/test case.
 */
int main(int argc, char *argv[]) {
    int i, status;
    int numOfStationThreads=atoi(argv[1])+1;
    pthread_t th[numOfStationThreads];
    int x,y;
    validate_args(argc, argv);
    initialize_vars(argc, argv);//argv[2]
    

    // TODO ... Insert your Code Here
    // 1. Create threads.
    for(x=0;x<numOfStationThreads;x++){//threads created

        int* a=malloc(sizeof(int));
        *a=x;
        if(x==0){
            if(pthread_create(&th[x],NULL,&reg_desk,argv[2])!=0){
                perror("Failed to create Threads");
            }
            // printf("Desk Thread %d had been Created\n",x);//TESTING
        }
        else{
            if(pthread_create(&th[x],NULL,&vac_station,a)!=0){
                perror("Failed to create Threads");
            }
            // printf("Station Thread %d had been Created\n",x);//TESTING
        }
        
    }
    // 2. Wait for threads to complete.
    for(y=0;y<numOfStationThreads;y++){//threads jointed
        if(y==0){
            if(pthread_join(th[y],NULL)!=0){
                printf("WHAT");
            }
        // printf("Desk Thread %d had Finished Excution\n",x);//TESTING 
        
        }
    else{
        if(pthread_join(th[y],NULL)!=0){
            printf("WHAT");
        }
        // printf("Station Thread %d had Finished Excution\n",x);//TESTING
    }
        
    }
    // 3. Clean up.
    
    
    
    
    
    pthread_mutex_destroy(&queue_mutex);//destroy mutex
    pthread_cond_destroy(&queue_condvar);//destroy conditional variables
}

