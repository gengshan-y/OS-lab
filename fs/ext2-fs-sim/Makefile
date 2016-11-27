objects = EasyTest APITest
CC = gcc
FLAGS = 
 
all : $(objects)

debug: FLAGS += -g -Wall -DDEBUG
debug: all

EasyTest: Driver.o EasyTest.c FileSysAPI.c Queue_LinkedList.c 
	$(CC) $(FLAGS) -o EasyTest EasyTest.c FileSysAPI.c Queue_LinkedList.c Driver.o

APITest: Driver.o APITest.c FileSysAPI.c Queue_LinkedList.c
	$(CC) $(FLAGS) -o APITest APITest.c FileSysAPI.c Queue_LinkedList.c Driver.o

Driver.o: Driver.h
 
clean:
	rm -rf $(objects)
