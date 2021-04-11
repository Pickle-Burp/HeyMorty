# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = main.c execution.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: exec

exec: main.o execution.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} exec

# END
