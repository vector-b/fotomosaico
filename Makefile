# Makefile
OBJS =  main.c
TARG = mosaico
TO_C = mosaico mosaico.o mosaico_class.o
CFLAG = -Wall 
CC = gcc
INCLUDE = mosaic_class.c
LIBS = -lm

all: hello

hello:${OBJS}
	${CC} -o  ${TARG} ${OBJS} ${INCLUDE} ${LIBS} ${CFLAG}

mosaico.o:   main.c mosaic_class.h
mosaic_class.o: mosaic_class.c mosaic_class.h

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f ${TO_C}