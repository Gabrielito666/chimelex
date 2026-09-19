#ifndef CHIMELEX_ARENA
#define CHIMELEX_ARENA
#include <stddef.h>

typedef struct Chunk Chunk;
typedef struct Arena Arena;

Arena *arena__create(size_t chunk_size);
void *arena__push(Arena *arena, size_t element_size, void *element);
void arena__free_all(Arena *arena);
#endif
