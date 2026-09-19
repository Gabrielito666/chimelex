#include "wave-fn.h"
#include "global-arena.h"
#include "arena.h"
#include <stddef.h>
#include <string.h>

#define WAVE_FN_PARAMS_SIZE 64

struct WaveFn
{
	WaveBaseFn wave_base_fn;
	void *params;
	size_t params_size;
};

struct WaveFnsStack
{
	int length;
	WaveFn **wave_fns;
};

/**
 * metodo para crear funciones de onda
 */
WaveFn *wave_fn__create(WaveBaseFn wave_base_fn, void *params, size_t params_size)
{
	Arena *_global_arena = global_arena__get();
		 
	WaveFn wave_fn;
	wave_fn.wave_base_fn = wave_base_fn;
	wave_fn.params = arena__push(_global_arena, params_size, params);
	wave_fn.params_size = params_size;

	return arena__push(_global_arena, sizeof(WaveFn), &wave_fn);
};

/**
 * metodo para crear una pila de funciones de onda
 */
WaveFnsStack *wave_fn__create_stack(int length, WaveFn **wave_fns)
{
	WaveFnsStack wave_fns_stack = { length, wave_fns };
	Arena* _global_arena = global_arena__get();

	return arena__push(_global_arena, sizeof(WaveFnsStack), &wave_fns_stack);
};

/**
 * metodo para sumar funciones de onda de un stack
 */
double wave_base_fn_add(double t, void *params, SoundContext* sound_context)
{
	WaveFnsStack *stack = (WaveFnsStack *)params;
	double result = 0.0;
	for(int i = 0; i < stack->length; i++)
	{
		result += stack->wave_fns[i]->wave_base_fn(t, stack->wave_fns[i]->params, sound_context);
	}

	return result;
};

WaveFn *wave_fn__add(WaveFnsStack *wave_fns_stack)
{
	WaveFn *wave_fn_result = wave_fn__create(wave_base_fn_add, wave_fns_stack, sizeof(WaveFnsStack));
	return wave_fn_result;
};

double wave_fn__eval(WaveFn *wave_fn, double t, SoundContext* sound_context)
{
	return wave_fn->wave_base_fn(t, wave_fn->params, sound_context);
}
