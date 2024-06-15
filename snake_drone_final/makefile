CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = snake.o
DEPS = snake.h

all: snake_game

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

snake_game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o snake_game
