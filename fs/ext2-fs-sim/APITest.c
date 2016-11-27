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
#include <stdlib.h>

#define STRING_SIZE 10000
#define ITERATIONS 1000

void checkCmd(char* cmd) {
  char* fileName = malloc(sizeof(char) * 256);
  char* data = malloc(sizeof(char) *256);

  if (!strcmp(cmd, "ls")) {
    List();
  }
  else if (!strcmp(cmd, "write")) {
    printf("name:");
    scanf("%s", fileName);
    printf("data:");
    scanf("%s", data);
    if (!Write(fileName, strlen(data), data))
      printf("Error writing to file %s\n", fileName);
  }
  else if (!strcmp(cmd, "read")) {
    printf("name:");
    scanf("%s", fileName);
    if (!Read(fileName, 1023, data))
      printf("file does not exist\n");
    else {
      printf("%s\n", data);
    }
  }
  else if (!strcmp(cmd, "mkdir")) {
    printf("name:");
    scanf("%s", fileName);
    if (!Mkdir(fileName))
      printf("Cannot make dictionary\n");
  }
  else if (!strcmp(cmd, "del")) {
    printf("name:");
    scanf("%s", fileName);
    if (!Delete(fileName))
      printf("Cannot delete file\n");
  }
  else if (!strcmp(cmd, "rmdir")) {
    printf("name:");
    scanf("%s", fileName);
    if (!Rmdir(fileName))
      printf("Cannot delete dictionary\n");
  }
  else if (!strcmp(cmd, "su")) {
    printf("username:");
    scanf("%s", fileName);
    printf("password:");
    scanf("%s", data);
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
	char cmd[1024];
  printf("EXT2 file system simulator.\n");

	// Create the file system
	if (!Format())
		printf("Failed to create file system\n");

  while(1) {
    scanf("%s", cmd);
    checkCmd(cmd);
  }
  
  Clean();

	return 0;
}
