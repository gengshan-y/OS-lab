/*
 *	Source file APITest.c
 *
 *	Test program for file system API for CSCI 460 Assignment 3
 *
 *	David Bover, WWU, August 2008
 */

#include <stdio.h>
#include <string.h>
#include "FileSysAPI.h"
#include "Driver.h"

#define STRING_SIZE 10000
#define ITERATIONS 1000

void stress_test(){
    char string[STRING_SIZE];
    char buf[STRING_SIZE+1];
    int i;
    int len;

    printf("Stress testing with %d byte string for %d iterations... \n", STRING_SIZE, ITERATIONS);
	if (!CSCI460_Format())
		printf("Failed to create file system\n");

    for(i = 0; i < STRING_SIZE-1; i++){
        string[i] = 'a';
    }

    len = strlen(string);
    for(i = 0; i < ITERATIONS; i++){
        CSCI460_Write("file", len, string);
        CSCI460_Read("file", len+1, buf);
        if(strcmp(string, buf) != 0){
            printf("Tests failed!\n");
            printf("Expected: \n%s\n", string);
            printf("Got: \n%s\n", buf);
            return;
        }
        CSCI460_Delete("file");
    }

    printf("Tests passed\n");
}

int main() {

	char *stuff = "Friends! Romans! Countrymen!  Lend me your ears";
	char *morestuff = "I come to bury Caesar, not to praise him";
	char *evenmore = "The evil that men do lives after them; the good is oft interred with their bones.  So let it be with Caesar";
	char data[1024];

	// Create the file system
	if (!CSCI460_Format())
		printf("Failed to create file system\n");

	if (!CSCI460_Write("mydata", strlen(stuff), stuff))
		printf("Error writing to file mydata\n");
	else
		printf("Success on writing mydata\n");

	if (!CSCI460_Write("mydata2", strlen(morestuff), morestuff))
		printf("Error writing to file mydata2\n");
	else
		printf("Success on writing mydata2\n");

	if (!CSCI460_Write("junk", strlen(evenmore), evenmore))
		printf("Error writing to file junk\n");
	else
		printf("Success on writing junk\n");

	// Now read it back
	if (!CSCI460_Read("mydata", 1023, data))
		printf("Error reading file mydata\n");
	else 
		printf("<<%s>>\n", data);

	if (!CSCI460_Read("mydata2", 1023, data))
		printf("Error reading file mydata2\n");
	else
		printf("<<%s>>\n", data);

	if (!CSCI460_Read("junk", 1023, data))
		printf("Error reading file junk\n");
	else
		printf("<<%s>>\n", data);


	//Now delete a file
	if (!CSCI460_Delete("mydata2"))
		printf("Cannot delete mydata2\n");
	else
		printf("Success on deleting mydata2\n");

	//Try to read it again
	printf("Try to read a deleted file\n");
	if (!CSCI460_Read("mydata2", 1023, data))
		printf("Error reading file mydata2\n");
	else
		printf("<<%s>>\n", data);


    stress_test();
	return 0;
}
