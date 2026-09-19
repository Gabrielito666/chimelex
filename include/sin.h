#ifndef CHIMELEX_SIN
#define CHIMELEX_SIN
#include "sound.h"
#include "wave-fn.h"

WaveFn* wave_fn_sin__create(double fq, double amp);
Sound* sound_sin__create(double fq, double amp, double duration);
#endif
