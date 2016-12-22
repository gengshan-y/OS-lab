objects = APITest
CC = gcc
FLAGS = 
 
all : $(objects)

debug: FLAGS += -g -Wall -DDEBUG
debug: all

APITest: Driver.o APITest.c FileSysAPI.c freeMan.c
	$(CC) $(FLAGS) -o APITest APITest.c FileSysAPI.c freeMan.c Driver.o

Driver.o: Driver.h
 
clean:
	rm -rf $(objects) *.o
