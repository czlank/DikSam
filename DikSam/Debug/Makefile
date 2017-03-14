TARGET = dbg.o
CC = gcc
CFLAGS = -c -g -Wall -DDBG_NO_DEBUG -ansi -pedantic -gdwarf-2
CFLAGSD = -c -g -Wall -ansi -pedantic -gdwarf-2
OBJS = Debug.o
INCLUDES = -I..

$(TARGET) : $(OBJS)
	ld -r -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGSD) $(INCLUDES) $*.c

Debug.o : Debug.c

clean:
	rm -f *.o