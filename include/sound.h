typedef struct
{
	WaveFn wave_fn;
	double duration;
	unsigned int rate;
} Sound;

typedef Sound (*Sound__createFn)(WaveFn wave_fn, double duration, unsigned int rate);
typedef void (*Sound__playFn)(Sound sound);

typedef struct {
	Sound__createFn create;
	Sound__playFn play;
} SoundOpts;

extern SoundOpts sound_opts;
