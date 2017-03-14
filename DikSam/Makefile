TARGET = DiskSam

CC = gcc

OBJS = \
    ./Debug/dbg.o \
    ./Memory/mem.o \
    ./Compiler/Compiler.o

INCLUDES = \

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $@ -lm -lmsvcp60

./Debug/dbg.o:
	cd ./Debug; $(MAKE);

./Memory/mem.o:
	cd ./Memory; $(MAKE);

./Compiler/Compiler.o:
	cd ./Compiler; $(MAKE);

#=====================================
#=============== clean ===============
#=====================================
clean:
	rm -f $(TARGET).exe
	cd ./Debug; $(MAKE) clean;
	cd ./Memory; $(MAKE) clean;
	cd ./Compiler; $(MAKE) clean;