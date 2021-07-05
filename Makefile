#
# "Makefile" for the Producer Consumer Task.
#

CC=gcc #-Wall -ggdb3
FLAGS=-g -D_REENTRANT
LIBS=-pthread -lrt

all: vaccine
.PHONY: all clean

vaccine: vaccine.o queue.o
	$(CC) $(LIBS) $(FLAGS) -o vaccine vaccine.o queue.o

vaccine.o: vaccine.c queue.h
	$(CC) $(LIBS) $(FLAGS) -c vaccine.c

queue.o: queue.c queue.h
	$(CC) $(FLAGS) -c queue.c

clean:
	rm -rf *.o vaccine
