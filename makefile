

CC = gcc
CFLAGS = -Wall
SRCS = parent.c child.c
OBJS = $(SRCS:.c=.o)

default: compile run

compile: $(OBJS)
	$(CC) -o parent parent.c
	$(CC) -o child child.c

run:
	./parent

clean:
	rm -f parent.o child.o


# 	CC = gcc
# CFLAGS = -Wall
# SRCS = file1.c file2.c file3.c file4.c file5.c file6.c file7.c file8.c file9.c file10.c
# OBJS = $(SRCS:.c=.o)

# LIBS = -lglut -lGLU -lGL -lm

# # This is a comment

# my_program: $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $<

# clean:
# 	rm -f $(OBJS) my_program
