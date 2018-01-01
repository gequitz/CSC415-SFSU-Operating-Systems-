// CSC415 - project 5
// Producer-Consumer Project
// Gabriel Equitz - SFSU ID: 915254839

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<stdlib.h>


#define BUFFERLENGTH 16


sem_t full;
sem_t empty;
pthread_mutex_t lock;
int buffer[BUFFERLENGTH];
int items_per_consumer;
int items_per_producer;
int counter = 0;

struct producer_thread{
    int producer_thread_index;
    int producer_quantity;
};


void *consume(void *param); //consumer thread
void *produce(void *param); //producer thread

int main(int argc, const char * argv[]){

    if (argc != 4){
	    fprintf(stderr, "Usage: ./a.out <#producers> <#consumers> <#items>");
	    return 1;
    }



    int number_of_producers, number_of_consumers, number_of_items_produced, i;

    number_of_producers = atoi(argv[1]); //number of producers
    number_of_consumers = atoi(argv[2]); //number of consumers
    number_of_items_produced = atoi(argv[3]); //number of items


    // display input information
    printf("Producers: %i\n", number_of_producers);
    printf("Consumers: %i\n", number_of_consumers);
    printf("Items produced: %i\n\n", number_of_items_produced);

    pthread_t *producers;
    pthread_t *consumers;
    
    // Initializing the semaphore
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFERLENGTH);
    
    //exit if cannot initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error in initializing the mutex\n");
        return 1;
    } 
    
    
    items_per_producer = number_of_items_produced / number_of_producers;
    items_per_consumer = number_of_items_produced / number_of_consumers;
    
    
    producers = (pthread_t *)malloc(number_of_producers * sizeof(pthread_t));
    consumers = (pthread_t *)malloc(number_of_consumers * sizeof(pthread_t));
    
    struct producer_thread pInfo[number_of_producers];
    
    // intialize thread ids
    for(i = 0; i < number_of_producers; i++){
        pInfo[i].producer_thread_index = i;
        pInfo[i].producer_quantity = items_per_producer;
        
	//create producer thread
        if(pthread_create(&producers[i], NULL, produce, &pInfo[i]) != 0){
            fprintf(stderr,"Error creating producer threads\n");
            return 1;
        } 
    } 
    
    // Create consumer threads
    for(i = 0; i < number_of_consumers; i++){
        if(pthread_create(&consumers[i], NULL, consume, &items_per_consumer) != 0){
            fprintf(stderr, "Error creating consumer threads\n");
            return 1;
        } 
    } 
    
    // Join the consumer and producer threads
    for(i = 0; i < number_of_producers; i++){
        pthread_join(producers[i], NULL);
    } 
    
    for( i=0; i < number_of_consumers; i++){
        pthread_join(consumers[i], NULL);
    } 
    
    
    // free producer and consumer threads
    free(producers);
    free(consumers);
    pthread_mutex_destroy(&lock);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    printf("\nProducers and Consumers are done!\n");
    
    return 0;
}


//consumer thread
void *consume(void *param){
    int item, i= 0;
    int numberConsumed = *(int *) param;
    
    while(i < numberConsumed){
        
        sem_wait(&full);
        pthread_mutex_lock(&lock);
        
        if(counter > 0){
            counter--;
            item = buffer[counter];
            printf("Consumed %i from slot %i\n", item, counter);
            buffer[counter] = 0;
        }else{
            exit(1);
        } 
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
        i++;
    } 
    
    pthread_exit(0);
} 

//producer thread
void *produce(void *param){
    int item, i = 0;
    struct producer_thread* pinfo = (struct producer_thread*) param;
    
    while(i < pinfo->producer_quantity){
        item = pinfo->producer_thread_index * pinfo->producer_quantity + i;
        
        sem_wait(&empty);
        pthread_mutex_lock(&lock);
        
        if(counter < BUFFERLENGTH){
            buffer[counter] = item;
            printf("Producer %i adds %i to slot %i\n", pinfo->producer_thread_index,item, counter);
            counter++;
        }else{
            exit(1);
        } 
        
        pthread_mutex_unlock(&lock);
        sem_post(&full);
        i++;
    }
    
    pthread_exit(0);
} 

