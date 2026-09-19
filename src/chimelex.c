#include <arena.h>
#include <global-arena.h>

void chimelex__init()
{
	global_arena__init();
}

void chimelex__close()
{
	arena__free_all(global_arena__get());
}
