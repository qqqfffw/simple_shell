CC      = gcc
OBJS    = main.o utils.o

all: main

main: $(OBJS)
	$(CC) $(OBJS) -o $@

main.o: main.c cell.h
utils.o: utils.c cell.h

run: main
	./main

clean:
	rm -f main $(OBJS)
