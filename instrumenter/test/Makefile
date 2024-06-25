.PHONY: all clean run

CC ?= gcc

.DEFAULT_GOAL=all

./%.o: ./%.c
	@$(CC) $< -o $@ -c -w

a.out: ./test.o ./link.o
	@$(CC) $^ -o $@

all: a.out

clean:
	rm -rf *.log a.out *.o

run: a.out
	@./a.out
