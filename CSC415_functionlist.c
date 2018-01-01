/* homework  2 - csc415- SFSU- Fall 2017 - Gabriel Equitz, id: 915254839 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//initialize the buffer
#define BUFFERSIZE 1024

/* prints Hello World */
void function1() {
	printf("Hello World! \n");
}

/* lists the files in the current directory */
void function2(){
    execlp("/bin/ls","ls",NULL);
}

/* prints invalid choice message */
void function3(){
    printf("Invalid Choice\n");
}


/* prints the options and returns the answer to the caller */
int print_options(){

	int ans;
	char line[BUFFERSIZE]; 
	printf("1. Hello World!\n");
	printf("2. List Files \n" );
	printf("3. Exit \n");
	printf(" Please select : ");
	fgets (line, sizeof(line), stdin);
	ans = atoi(line);
        return ans; /* returns the answer */


}


int main(int argc, const char * argv[]) {

 
	int answer;
        pid_t pid; 

        answer = print_options();


        while (answer != 3){ 



            if (answer ==1 )  {
              pid = fork();
	   
              if (pid < 0 ) {
	       fprintf(stderr, "Fork Failed ");	      
	      }
	      else if (pid == 0) {/* child process */ 
               function1();
	       return(0);
	      }
	      else {  /* parent process */
		wait(NULL);
	      }

	    }     // end if 
          

	    else if (answer == 2 )  {
	      pid = fork();
              if (pid < 0 ) {
	       fprintf(stderr, "Fork Failed ");	      
	      }
	      else if (pid == 0) {/* child process */ 
               function2();
	       return(0);
	      }
	      else {  /* parent process */
		wait(NULL);
	      }

	    }     // end if 

	    
	    else if (answer  < 0  || answer > 3 ){
	      pid = fork();
              if (pid < 0 ) {
	       fprintf(stderr, "Fork Failed ");	      
	      }
	      else if (pid == 0) {/* child process */ 
                function3();
	        return(0);
		
	      }
	      else {  /* parent process */
		wait(NULL);
	      }



	    }

	        printf("\n");
                answer = print_options();
	        printf("\n");

        }  // end while


	return 0;
}
