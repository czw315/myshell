include ../Makefile.inc

all : ${SHELL_LIB}

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))

${SHELL_LIB} : ${OBJECTS}
	${RM} ${SHELL_LIB}
	${AR} rs ${SHELL_LIB} *.o

clearn : 
	${RM} *.o ${SHELL_LIB}
