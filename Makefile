#Use the following to compile under unix or cygwin
CC = g++
LD = g++
SFLAGS=

#Uncomment the following line to make CYGWIN produce stand-alone Windows executables
INCLUDE= 
CFLAGS=  $(SFLAGS) -O3  -w                     # release C-Compiler flags
LFLAGS=  $(SFLAGS) -O3  -w                     # release linker flags
LIBS=-lm -L.

all: main

lcs.o: sentence_lcs.cpp lcs.h
	$(CC) -c $(CFLAGS) sentence_lcs.cpp -o lcs.o $(INCLUDE)

main.o: main.cpp lcs.h 
	$(CC) -c $(CFLAGS) main.cpp -o main.o $(INCLUDE) 

main:lcs.o main.o
	$(LD) $(LFLAGS) main.o lcs.o -o main $(LIBS)

clean:
	rm *.o main

