#include<math.h>
#include "sin.h"
#include "global-arena.h"
#include "arena.h"

#define FADE_TIME 0.001

double sin_wave_implementation(double t, double fq, double amp)
{
	return amp * sin(fq * 2.0 * M_PI *  t);
};

typedef struct
{
	double fq;
	double amp;
} SinParams;

double sin_wave_base_fn(double t, void *params, SoundContext* sound_context)
{
	SinParams *sin_params = (SinParams *)params;

	double fade_factor = 1.0;
	if(t < FADE_TIME) fade_factor = t / FADE_TIME;
	if(t > sound_context->duration - FADE_TIME) fade_factor = (sound_context->duration - t) / FADE_TIME;

	return sin_wave_implementation(t, sin_params->fq, sin_params->amp) * fade_factor;
};

SinParams* sin_params__create(double fq, double amp)
{
	Arena* _global_arena = global_arena__get();
	SinParams sin_params = {fq, amp};

	return arena__push(_global_arena, sizeof(SinParams), &sin_params);
}

WaveFn* wave_fn_sin__create(double fq, double amp)
{
	SinParams* sin_params = sin_params__create(fq, amp);
	WaveFn* wave_fn_sin = wave_fn__create(sin_wave_base_fn, sin_params, sizeof(SinParams));

	return wave_fn_sin;
}

Sound* sound_sin__create(double fq, double amp, double duration)
{
	WaveFn* wave_fn_sin = wave_fn_sin__create(fq, amp);
	Sound* sound_sin = sound__create(wave_fn_sin, duration);

	return sound_sin;
}
