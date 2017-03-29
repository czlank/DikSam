TARGET = lex.yy.o y.tab.o

CC = gcc
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -gdwarf-2 -DGB2312_SOURCE
CFLAGSD = -c -g -Wall -Wswitch-enum -ansi -pedantic -gdwarf-2 -DGB2312_SOURCE -DDEBUG

######## flex ########
lex.yy.c : DikSam.l DikSam.y y.tab.h
	flex DikSam.l

######## bison ########
y.tab.h : DikSam.y
	bison --yacc -dv DikSam.y
y.tab.c : DikSam.y
	bison --yacc -dv DikSam.y

######## y.tab.o/lex.yy.o ########
y.tab.o : y.tab.c
	$(CC) -c -g $*.c $(INCLUDES)

lex.yy.o : lex.yy.c
	$(CC) -c -g $*.c $(INCLUDES)

clean:
	rm -f *.o lex.yy.c y.tab.h y.tab.c y.output