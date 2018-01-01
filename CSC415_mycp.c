//  Gabriel Equitz - id: 915254839
//  CSC415 - Homework 6
//
//  Program to copy a file. It takes two command line arguments, 
//  which are the name of file to copy from and the
//  name of the file to copy to.

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 1024

/* main function */
int main(int argc, char *argv[]){

    if (argc != 3) {
	    fprintf(stderr, "usage: ./a.exe <source file> < destination file> \n");
	    return -1;
    }

    int sourceFileNumber, destinationFileNumber;
    char aBuffer[BUFFERSIZE] = {0};
    ssize_t totalNumberBytesRead, numberBytesRead, numberBytesWritten;
    
    
     totalNumberBytesRead = 0;
     numberBytesRead = 0;
     numberBytesWritten = 0;

    // Open the source file
    sourceFileNumber = open(argv[1], O_RDONLY);
    
    // sourceFileNumber will be -1 if there is an error
    if (sourceFileNumber < 0){
        fprintf(stderr, "Cannot Open Source File");
        close(sourceFileNumber);
        return -1;
    } 
    
    //try to open destination file 
     destinationFileNumber = open(argv[2], O_CREAT | O_WRONLY | O_EXCL, S_IRWXU);
    
    // destinationFileNumber will be -1 if an error occurs
    if (destinationFileNumber < 0){
        fprintf(stderr, "Cannot Open Destination File");
        close(sourceFileNumber);
        close(destinationFileNumber);
        return -1;
    } 
    
    do{
        numberBytesRead = read(sourceFileNumber, aBuffer, BUFFERSIZE-1);
        if (numberBytesRead < 0){
            fprintf(stderr, "Error reading from file");
            close(sourceFileNumber);
            close(destinationFileNumber);
            return -1;
        } 
        
        numberBytesWritten = write(destinationFileNumber, aBuffer, numberBytesRead);
        if (numberBytesWritten < 0){
            fprintf(stderr, "Error writing to file");
            close(sourceFileNumber);
            close(destinationFileNumber);
            return -1;
        } 
        
        // increase total number bytes read from file 
        totalNumberBytesRead += numberBytesWritten;
        
    } // end do
    while (numberBytesRead != 0);
    
    printf("Copied %u bytes \n", totalNumberBytesRead);
    
    close(sourceFileNumber);
    close(destinationFileNumber);
    
    return 0;
} // end main
