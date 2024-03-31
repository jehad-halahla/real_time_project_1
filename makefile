
CC = gcc
CFLAGS = -Wall
SRCS = parent.c child.c
OBJS = $(SRCS:.c=.o)
LIBS_GUI = -lglut -lGLU -lGL -lm
LIBS = -lm

default: compile run

compile:
	@echo "Compiling..."
	@$(CC) -o parent parent.c $(LIBS)
	@$(CC) -o child child.c $(LIBS)

run:
	@./parent

clean:
	@rm -f parent child parent.o child.o