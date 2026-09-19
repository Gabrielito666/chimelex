#ifndef CHIMELEX_SOUND
#define CHIMELEX_SOUND
#include "wave-fn.h"

typedef struct Sound Sound;

Sound* sound__create(WaveFn* wave_fn, double duration);
void sound__play(Sound* sound, unsigned int rate);

#endif
