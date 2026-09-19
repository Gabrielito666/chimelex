CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SRC = src/sound.c src/wave-fn.c src/sin.c src/arena.c src/global-arena.c src/chimelex.c
OBJ = $(SRC:.c=.o)

TARGET = main

all: $(TARGET)

# Compilar el ejecutable directamente
$(TARGET): $(OBJ) main.c
	$(CC) $(CFLAGS) main.c $(OBJ) -o $(TARGET) -lasound -lm

# Regla genérica: .c -> .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
