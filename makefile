
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



compile_gui:
	@echo "Compiling..."
	@$(CC) parent.c -o parent $(LIBS) -D__GUI__
	@$(CC) child.c -o child $(LIBS)
	@$(CC) ballpass.c -o ballpass $(LIBS_GUI)


run_gui: compile_gui
	@./parent

test_gui: 
	@$(CC) ballpass.c -o ballpass $(LIBS_GUI)
	@./ballpass

clean:
	@rm -f parent child ballpass
