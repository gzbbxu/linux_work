

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm04_mulfork dm02_mysystem sig dm02_signal
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)




