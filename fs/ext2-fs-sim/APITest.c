/***
 * Filename:        APITest.c
 * Date:            Dec 14 2016
 * Last Edited by:  Gengshan Yang
 * Description:     A simple EXT2-OS simulation. With command "ls" "write" 
 *                  "read" "mkdir" "del" "rmdir" "su" "exit" "quit"
 ***/


#include <stdio.h>
#include <string.h>
#include "FileSysAPI.h"
#include "Driver.h"
#include <stdlib.h>

#define STRING_SIZE 10000
#define ITERATIONS 1000
#define IN_BUFF_LEN 1024


/* from http://stackoverflow.com/questions/314401/how-to-read-a-line-from-the-console-in-c 
   To avoid trunation by space */
void getLine(char* line) {
    char* oLine = line;
    int c;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if((*line++ = c) == '\n')
            break;
    }
    *--line = '\0';
}


void checkCmd(char* cmd) {
  char* fileName = malloc(sizeof(char) * 256);
  char* data = malloc(sizeof(char) *256);

  if (!strcmp(cmd, "ls")) {
    List();
  }
  else if (!strcmp(cmd, "write")) {
    printf("name:");
    getLine(fileName);
    printf("data:");
    getLine(data);
    if (!Write(fileName, strlen(data), data))
      printf("Error writing to file %s\n", fileName);
  }
  else if (!strcmp(cmd, "read")) {
    printf("name:");
    getLine(fileName);
    if (!Read(fileName, 1023, data))
      printf("file does not exist\n");
    else {
      printf("%s\n", data);
    }
  }
  else if (!strcmp(cmd, "mkdir")) {
    printf("name:");
    getLine(fileName);
    if (!Mkdir(fileName))
      printf("Cannot make dictionary\n");
  }
  else if (!strcmp(cmd, "del")) {
    printf("name:");
    getLine(fileName);
    if (!Delete(fileName))
      printf("Cannot delete file\n");
  }
  else if (!strcmp(cmd, "rmdir")) {
    printf("name:");
    getLine(fileName);
    if (!Rmdir(fileName))
      printf("Cannot delete dictionary\n");
  }
  else if (!strcmp(cmd, "su")) {
    printf("username:");
    getLine(fileName);
    printf("password:");
    getLine(data);
    if(login(fileName, data)) {
      printf("login success\n");
    }
  } 
  else if (!strcmp(cmd, "exit")) {
    logout();
  } 
  else if (!strcmp(cmd, "quit")) {
    exit(0);
  }
  else {
    printf("invalid command\n");
  }
}


int main() {
	char cmd[IN_BUFF_LEN];
  printf("EXT2 file system simulator.\n");

	// Create the file system
	if (!Format())
		printf("Failed to create file system\n");

  while(1) {
    getLine(cmd);
    checkCmd(cmd);
  }
  
  Clean();

	return 0;
}
