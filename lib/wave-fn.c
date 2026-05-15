//====WAVE_FN====//
#include "wave-fn.h"
#include <stddef.h>
#include <string.h>

#define WAVE_FN_PARAMS_SIZE 64

/**
 * WaveFn__create
 * metodo para crear funciones de onda
 */
//implementación 
WaveFn wave_fn__create(WaveBaseFn wave_base_fn, void *params, size_t params_size)
{
	WaveFn wave_fn;
	wave_fn.wave_base_fn = wave_base_fn;
	memcpy(wave_fn.params, params, params_size);
	return wave_fn;
};

/**
 * WaveFn__create_stack
 * metodo para crear una pila de funciones de onda
 */
//implementación
WaveFnsStack wave_fn__create_stack(int length, WaveFn *wave_fns)
{
	const WaveFnsStack wave_fns_stack = { length, wave_fns };
	return wave_fns_stack;
};

/**
 * WaveFn__add
 * metodo para sumar funciones de onda de un stack
 */
double wave_base_fn_add(double t, void *params)
{
	WaveFnsStack *stack = (WaveFnsStack *)params;
	double result = 0.0;
	for(int i = 0; i < stack->length; i++) result += stack->wave_fns[i].wave_base_fn(t, stack->wave_fns[i].params);
	return result;
};

//implementacion
WaveFn wave_fn__add(WaveFnsStack wave_fns_stack)
{
	WaveFn wave_fn_result = wave_fn__create(wave_base_fn_add, &wave_fns_stack, sizeof(WaveFnsStack));
	return wave_fn_result;
};

double wave_fn__eval(WaveFn *wave_fn, double t)
{
	return wave_fn->wave_base_fn(t, &wave_fn->params);
}

//Mapper
WaveFnOpts wave_fn_opts =
{
    .create = wave_fn__create,
    .create_stack = wave_fn__create_stack,
    .add = wave_fn__add,
    .eval = wave_fn__eval
};
