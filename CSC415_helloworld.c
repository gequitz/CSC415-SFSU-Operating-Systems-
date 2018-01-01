/* homework 1 - csc415- SFSU- Fall 2017 - Gabriel Equitz */

#include <stdio.h>
#include <unistd.h>

#define name "Gabriel"

int main(int argc, const char * argv[]) {
	char str[40];
	sprintf(str, "Hello CSC415, I am %s!", name, "\n");
	write(1,str,39);
	return 0;
}
