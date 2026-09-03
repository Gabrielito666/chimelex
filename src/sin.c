#include<math.h>
#include "sin.h"

double sin_wave_implementation(double t, double fq, double amp)
{
	return amp * sin(fq * 2.0 * M_PI *  t);
};

typedef struct
{
	double fq;
	double amp;
} SinParams;

double sin_wave_base_fn(double t, void *params)
{
	SinParams *sin_params = (SinParams *)params;

	return sin_wave_implementation(t, sin_params->fq, sin_params->amp);
};

SinParams sin_params__create(double fq, double amp)
{
	SinParams sin_params = {fq, amp};
	return sin_params;
}

WaveFn wave_fn_sin__create(double fq, double amp)
{
	SinParams sin_params = sin_params__create(fq, amp);
	WaveFn wave_fn_sin = wave_fn_opts.create(sin_wave_base_fn, &sin_params, sizeof(SinParams));

	return wave_fn_sin;
}

Sound sound_sin__create(double fq, double amp, double duration)
{
	WaveFn wave_fn_sin = wave_fn_sin__create(fq, amp);
	Sound sound_sin = sound_opts.create(wave_fn_sin, duration, 44100);

	return sound_sin;
}


