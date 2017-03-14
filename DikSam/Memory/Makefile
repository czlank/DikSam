TARGET = mem.o
CC = gcc
CFLAGS = -c -g -Wall -ansi -pedantic -gdwarf-2
CFLAGSD = -c -g -Wall -DDEBUG -ansi -pedantic -gdwarf-2
OBJS = Memory.o Storage.o
INCLUDES = -I..

$(TARGET) : $(OBJS)
	ld -r -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGSD) $(INCLUDES) $*.c

Memory.o : Memory.c
Storage.o : Storage.c

clean:
	rm -f *.o