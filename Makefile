CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SRC = lib/sound.c lib/wave-fn.c lib/sin.c
OBJ = $(SRC:.c=.o)

TARGET = main

all: $(TARGET)

# Compilar el ejecutable directamente
$(TARGET): $(OBJ) main.c
	$(CC) $(CFLAGS) main.c $(OBJ) -o $(TARGET) -lasound -lm

# Regla genérica: .c -> .o
lib/%.o: lib/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f lib/*.o $(TARGET)
