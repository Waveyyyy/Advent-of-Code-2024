CC = gcc
CFLAGS_DBG = -ggdb -Wall -no-pie
CFLAGS_REL = -Wall
OBJ = day2.o ../utils/utils.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS_DBG) -c -o $@ $<

solution: $(OBJ)
	gcc $(CFLAGS_DBG) -o $@ $^

clean:
	rm *.o ../utils/*.o
