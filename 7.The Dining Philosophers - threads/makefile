NAME = philosophers

COMPILER = gcc

compile: philosophers.o
	$(COMPILER) $(FLAGS) -o $(NAME) philosophers.o -lpthread

philosophers.o:
	$(COMPILER) -c philosophers.c

clean:
	rm philosophers.o $(NAME)
