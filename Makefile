# Makefile
OBJS =  main.c
TARG = mosaico 
CFLAG = -Wall -lm
CC = gcc
INCLUDE = mosaic_class.c
LIBS = -lncurses

hello:${OBJS}
	${CC} -o  ${TARG} ${OBJS} ${INCLUDE} ${LIBS} ${CFLAG}

clean:
	-rm -f *.o space_invaders