NAME = copy

COMPILER = cc

CFLAGS = -Wall -std=c99

compile: copy.o
	$(COMPILER) $(FLAGS) -o $(NAME) copy.o

copy.o:
	$(COMPILER) -c copy.c

clean:
	rm copy.o $(NAME)
