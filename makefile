CC := gcc
CFLAGS := -Wall
SRCS := parent.c child.c
OBJS = $(SRCS:.c=.o)
LIBS_GUI = -lglut -lGLU -lGL -lm
LIBS = -lm

default: run

compile:
	@echo "Compiling..."
	@$(CC) parent.c -o parent $(LIBS)
	@$(CC) child.c -o child $(LIBS)

run: compile
	@./parent

compile_gui:
	@echo "Compiling..."
	@$(CC) parent.c -o parent $(LIBS) -D__GUI__
	@$(CC) child.c -o child $(LIBS) -D__GUI__
	@$(CC) ballpass.c -o ballpass $(LIBS_GUI)


compile_gui_dbg:
	@echo "Compiling..."
	@$(CC) parent.c -o parent $(LIBS) -D__GUI__ -g
	@$(CC) child.c -o child $(LIBS) -D__GUI__ -g
	@$(CC) ballpass.c -o ballpass $(LIBS_GUI) -g

run_gui: compile_gui
	@./parent

qui_dbg: compile_gui_dbg
	@gdb ./parent

test_gui: 
	@$(CC) ballpass.c -o ballpass $(LIBS_GUI)
	@./ballpass

clean:
	@rm -f parent child ballpass

