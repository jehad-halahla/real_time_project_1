
CC := gcc
CFLAGS := -Wall
SRCS := parent.c child.c
OBJS = $(SRCS:.c=.o)
LIBS_GUI = -lglut -lGLU -lGL -lm
LIBS = -lm

default: compile run

compile:

	@echo "Compiling..."

	@$(CC) parent.c -o parent $(LIBS)
	@$(CC) child.c -o child $(LIBS)

run:
	@./parent

gui: 
	@$(CC) ballpass.c -o ballpass $(LIBS_GUI)
	@./ballpass

clean:
	@rm -f parent child ballpass
