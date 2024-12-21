CC = gcc
CFLAGS_DBG = -g -Wall
CFLAGS_REL = -Wall
OBJ = day*.o ../utils/utils.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS_DBG) -c -o $@ $<

solution: $(OBJ)
	gcc $(CFLAGS_DBG) -o $@ $^

clean:
	rm *.o ../utils/*.o
