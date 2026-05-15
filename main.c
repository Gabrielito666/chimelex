#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "sin.h"

double random_double(double min, double max)
{
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

Sound sin__create_random()
{
	double fq = random_double(2200, 22000);
	double amp = random_double(0.5, 0.7);
	double dur = random_double(0.04, 0.08);

	Sound sin_sound = sound_sin__create(fq, amp, dur);
	return sin_sound;
}

int main()
{
	for(int i = 0; i < 20; i++)
	{

		Sound sin_sound = sin__create_random();

		sound_opts.play(sin_sound);
	};

	/**	
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

	**/
	return 0;
};
