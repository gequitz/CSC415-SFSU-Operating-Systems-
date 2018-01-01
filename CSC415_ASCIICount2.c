//  CSC415 - SFSU 
//  homework 4
//  program that counts the ascii characters of a text file
//  usage: a.out filename.txt
//  date: 10/11/2017
//  
//
//  Gabriel Equitz - SFSU ID: 915254839
//
//

#include <unistd.h>
#include <stdio.h>
#include <pthread.h> /* header file for the Pthread API */
#include <time.h>


#define N_ASCII 128 /* Number of ascii characters */
#define THREAD_NUM 8  /* number of child threads */
#define BUFFER_SIZE 65536 /* buffer size */

int asciiChar[N_ASCII];
char buffer[BUFFER_SIZE];

//structure with thread details 
typedef struct{
    int threadNumber;
    int partitionSize;
} thread_details;


void *ascii_counter(void *t_detail); //function that counts the ascii characters

int main(int argc, char* argv[]){

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    pthread_t th_ids[THREAD_NUM]; /* thread identifiers for the child threads */
    thread_details t_detail[THREAD_NUM]; //set the attributes for each thread

    pthread_attr_t attr; /*thread attribute */
    pthread_attr_init(&attr); /*initialize thread attribute variable with default values */

    //print error message if the filename is not given and return
    if (argc !=2){
	    fprintf(stderr, "usage: a.out <filename>\n");
	    return -1;
    }

     
    // open the input file
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp != NULL){
    size_t aLen = fread(buffer, sizeof(char), BUFFER_SIZE, fp); //reads input file into the buffer

     
    
     // if file is empty report error and exit 
       if (aLen == 0){
           fprintf(stderr, "File is empty!\n");
           return -1;
	  } 
       else{
          buffer[aLen++] = '\0';
       }
    
    
       // find the size of the partition 
       int partiontSize = aLen / THREAD_NUM;

    
       // set the threads with number and partition boundaries
       for(int i = 0; i < THREAD_NUM; i++) {
           t_detail[i].threadNumber = i;
           t_detail[i].partitionSize = partiontSize;
        
        // create thread 
           int myThread = pthread_create(&th_ids[i], &attr, &ascii_counter, &t_detail[i]);
        
        // return if unable to create the thread
           if(myThread != 0) {
               fprintf(stderr, "Unable to create thread  %i\n", i);
	       return (-1);
  	   }
       } //end for
    
       // combine the threads
       for (int i =0; i< THREAD_NUM; i++){
	   pthread_join(th_ids[i], NULL);
       } 
    

       // print the results
       for(int i = 0; i < N_ASCII; i++) {
           int charTotal = 0;
               charTotal += asciiChar[i];
        
           printf("%i occurrences of ", charTotal);
        
           if(i < 33 || i == 127){
               printf("%#0x\n", i);
           }
           else{
               printf("%c\n", i);
           }

       } 
       //close the input file
       fclose(fp); 
    } // close if
    else{
	fprintf(stderr,"error in opening the file \n");
    }


    end = clock();    
    cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    //printf("cpu time used : %f", cpu_time_used);
    
    return 0;
} 

// function to count the ascii characters
void *ascii_counter(void *t_detail) {
    
    thread_details threadDet = *(thread_details*) t_detail; //variable of thread struct
    int threadPosition = threadDet.threadNumber * threadDet.partitionSize; //thread position
    char currChar = buffer[threadPosition]; 
   
    // count the ascii characters
    for(int i = 0; i < threadDet.partitionSize; i++) {
           currChar = buffer[threadPosition++];
           //printf("%c\n", currChar);
           asciiChar[currChar]++;
    } 
    
    pthread_exit(0);
} 
