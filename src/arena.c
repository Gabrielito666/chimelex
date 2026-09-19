#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <stdbool.h>
#include "arena.h"

#define CHUNKS_ARRAY_LIMIT 100

struct Chunk {
	size_t size;
	size_t current_space_used;
	void *first_empty_position;
	void *first_position;
};

struct Arena
{
	size_t chunk_size;
	unsigned int length;
	unsigned int limit;
	Chunk* chunks;
};


size_t align_size(size_t size, size_t alignment)
{
	return (size + alignment - 1) & ~(alignment - 1);
}

Chunk chunk__create(size_t chunk_size)
{
	void *dataPtr = malloc(chunk_size);
	if(dataPtr == NULL)
	{
		fprintf(stderr, "[CHIMELEX ARENA ERROR] chunk creation malloc falied \n");
		abort();
	}
	Chunk chunk = {
		.size = chunk_size,
		.current_space_used = 0,
		.first_position = dataPtr,
		.first_empty_position = dataPtr
	};

	return chunk;
}

void *chunk__push(Chunk *chunk, size_t element_size, void *element)
{
	size_t aligned_size = align_size(element_size, 8);

	if(chunk->size - chunk->current_space_used < aligned_size) return NULL;

	void *element_ptr = chunk->first_empty_position;
	memcpy(element_ptr, element, element_size);	//copy just nesesary
	chunk->first_empty_position = (char *) chunk->first_empty_position + aligned_size; //align
	chunk->current_space_used += aligned_size; //align
	return element_ptr;
}

Arena *arena__create(size_t chunk_size)
{
	Chunk first_chunk = chunk__create(chunk_size);
	Chunk *chunks_arr = malloc(sizeof(Chunk) * CHUNKS_ARRAY_LIMIT);

	if(chunks_arr == NULL)
	{
		fprintf(stderr, "[CHIMELEX ARENA ERROR] arena chunks array creation malloc falied\n");
		abort();
	}

	chunks_arr[0] = first_chunk;

	Arena *arena_ptr = malloc(sizeof(Arena));

	if(arena_ptr == NULL)
	{
		fprintf(stderr, "[CHIMELEX ARENA ERROR] arena creation malloc falied\n");
		abort();
	}

	*arena_ptr = (Arena){
		.chunk_size = chunk_size,
		.length = 1,
		.limit = CHUNKS_ARRAY_LIMIT,
		.chunks = chunks_arr,
	};

	return arena_ptr;
}

void arena__append_chunk(Arena *arena)
{
	if(arena->length == arena->limit)
	{
		arena->limit += CHUNKS_ARRAY_LIMIT;
		void *temp = realloc(arena->chunks, sizeof(Chunk) * arena->limit);

		if(temp == NULL)
		{
			fprintf(stderr, "[CHIMELEX ARENA ERROR] falied to realloc chunks array\n");
			abort();
		}
		arena->chunks = temp;
	}

	Chunk new_chunk = chunk__create(arena->chunk_size);
	arena->chunks[arena->length] = new_chunk;
	arena->length++;
}

void *arena__push(Arena *arena, size_t element_size, void *element)
{
	void *ptr = chunk__push(&arena->chunks[arena->length -1], element_size, element);

	if(ptr) return ptr;

	//if not space, append a chunk and reintent
	arena__append_chunk(arena);
	
	void *ptr2 = chunk__push(&arena->chunks[arena->length -1], element_size, element);
	
	if(ptr2 == NULL)
	{
		fprintf(stderr, "[CHIMELEX ARENA ERROR] arena push a element greater than chunk size\n");
		abort();
	}

	return ptr2;
}

void arena__free_all(Arena *arena)
{
	for(unsigned int i = 0; i < arena->length; i++)
	{
		Chunk chunk = arena->chunks[i];
		free(chunk.first_position);
	}

	free(arena->chunks);
}
