NAME = philosophers

COMPILER = cc

CFLAGS = -Wall -std=c99

compile: philosophers.o
	$(COMPILER) $(FLAGS) -o $(NAME) philosophers.o

philosophers.o:
	$(COMPILER) -c philosophers.c

clean:
	rm philosophers.o $(NAME)
