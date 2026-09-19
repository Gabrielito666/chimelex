#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "chimelex.h"

double random_double(double min, double max)
{
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

Sound* sin__create_random()
{
	double fq = random_double(2200, 22000);
	double amp = random_double(0.5, 0.7);
	double dur = random_double(0.04, 0.08);

	Sound* sin_sound = sound_sin__create(fq, amp, dur);
	return sin_sound;
}

int main()
{
	chimelex__init();
	for(int i = 0; i < 20; i++)
	{

		Sound* sin_sound = sin__create_random();

		sound__play(sin_sound, 44100);
	};

	WaveFn* wave_fn_C = wave_fn_sin__create(261.626, 0.1);
	WaveFn* wave_fn_E = wave_fn_sin__create(329.628, 0.1);
	WaveFn* wave_fn_G = wave_fn_sin__create(391.995, 0.1);

	WaveFn* chord_arr[3] = {wave_fn_C, wave_fn_E, wave_fn_G};

	WaveFnsStack* chord_stack = wave_fn__create_stack(3, chord_arr);
	WaveFn* chord_wave_fn = wave_fn__add(chord_stack);

	Sound* chord_sound = sound__create(chord_wave_fn, 3);

	sound__play(chord_sound, 44100);

	chimelex__close();
	return 0;
};
