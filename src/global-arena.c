#include <global-arena.h>
#include <arena.h>

static Arena* _global_arena = NULL;

void global_arena__init()
{
	_global_arena = arena__create(4096);
}

Arena* global_arena__get()
{
	return _global_arena;
}
