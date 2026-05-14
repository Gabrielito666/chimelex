#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "wave-fn.h"
#include "sound.h"

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

double random_double(double min, double max)
{
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

SinParams sin_params__create(double fq, double amp)
{
	SinParams sin_params = {fq, amp};
	return sin_params;
}

Sound sin__create_random()
{
	SinParams sin_params = sin_params__create(random_double(1000.0, 22000.0), random_double(0.3, 0.5));
	WaveFn sin_wave_fn = wave_fn_opts.create(sin_wave_base_fn, &sin_params, sizeof(SinParams));
	double dur = random_double(0.04, 0.08);

	Sound sin_sound = sound_opts.create(sin_wave_fn, dur, 44100);
	return sin_sound;
}

int main()
{
	for(int i = 0; i < 20; i++)
	{

		Sound sin_sound = sin__create_random();

		sound_opts.play(sin_sound);
	};

	SinParams c_params = {261.626, 0.2};
	SinParams e_params = {329.628, 0.2};
	SinParams g_params = {391.995, 0.2};

	WaveFn wave_fn_C = wave_fn_opts.create(sin_wave_base_fn, &c_params, sizeof(SinParams));
	WaveFn wave_fn_E = wave_fn_opts.create(sin_wave_base_fn, &e_params, sizeof(SinParams));
	WaveFn wave_fn_G = wave_fn_opts.create(sin_wave_base_fn, &g_params, sizeof(SinParams));

	WaveFn chord_arr[3] = {wave_fn_C, wave_fn_E, wave_fn_G};

	WaveFnsStack chord_stack = wave_fn_opts.create_stack(3, chord_arr);

	WaveFn chord_wave_fn = wave_fn_opts.add(chord_stack);

	Sound chord_sound = sound_opts.create(chord_wave_fn, 3, 44100);

	sound_opts.play(chord_sound);

	return 0;
};
