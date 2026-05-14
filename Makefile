CC = gcc

CFLAGS = -Iinclude -g -fsanitize=address
LDFLAGS = -lm -lasound

SRC = main.c include/sound.c include/wave-fn.c

main:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o main
