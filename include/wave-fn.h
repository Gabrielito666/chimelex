#ifndef CHIMELEX_WAVE_FN
#define CHIMELEX_WAVE_FN
#include <stddef.h>

#define WAVE_FN_PARAMS_SIZE 64

typedef struct
{
	double duration;
} SoundContext;
typedef double (*WaveBaseFn)(double t, void *params, SoundContext* sound_context);

typedef struct WaveFn WaveFn;
typedef struct WaveFnsStack WaveFnsStack;

WaveFn *wave_fn__create(WaveBaseFn wave_base_fn, void *params, size_t params_size);
WaveFnsStack *wave_fn__create_stack(int length, WaveFn **wave_fns);
double wave_base_fn_add(double t, void *params, SoundContext* sound_context);
WaveFn *wave_fn__add(WaveFnsStack *wave_fns_stack);
double wave_fn__eval(WaveFn *wave_fn, double t, SoundContext* sound_context);
#endif
