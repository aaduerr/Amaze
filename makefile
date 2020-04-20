OBJS = cell.o da.o stack.o cda.o queue.o maze.o amaze.o
OOPTS = -Wall -Wextra -g -c -std=gnu99
LOPTS = -Wall -Wextra -g -std=gnu99

amaze : $(OBJS)
	gcc $(LOPTS) -o amaze $(OBJS)

amaze.o : amaze.c maze.h
	gcc $(OOPTS) amaze.c

cell.o : cell.c cell.h
	gcc $(OOPTS) cell.c

maze.o : maze.c maze.h cell.h stack.h queue.h
	gcc $(OOPTS) maze.c

da.o : da.c da.h
	gcc $(OOPTS) da.c

stack.o : stack.c stack.h da.h
	gcc $(OOPTS) stack.c

cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c

queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c

clean :
	rm -f $(OBJS) amaze

test :
	./amaze -r 2777 -c 25 25 unready.maze -d ready.maze -s unready.maze ready.maze

valgrind :
	valgrind ./amaze
