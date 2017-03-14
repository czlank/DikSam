TARGET = Compiler.o

CC = gcc
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -gdwarf-2 -DGB2312_SOURCE
CFLAGSD = -c -g -Wall -Wswitch-enum -ansi -pedantic -gdwarf-2 -DGB2312_SOURCE -DDEBUG

OBJS =  lex.yy.o \
        y.tab.o \
        Main.o

INCLUDES = -I../Include

$(TARGET) : $(OBJS)
	ld -r -o $@ $(OBJS)

######## flex ########
lex.yy.c : DiskSam.l DiskSam.y y.tab.h
	flex DiskSam.l

######## bison ########
y.tab.h : DiskSam.y
	bison --yacc -dv DiskSam.y
y.tab.c : mycrowbar.y
	bison --yacc -dv DiskSam.y

######## y.tab.o/lex.yy.o ########
y.tab.o : y.tab.c
	$(CC) -c -g $*.c $(INCLUDES)

lex.yy.o : lex.yy.c
	$(CC) -c -g $*.c $(INCLUDES)

.c.o:
	$(CC) $(CFLAGSD) $(INCLUDES) $*.c

Main.o : Main.c

clean:
	rm -f *.o