#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<alsa/asoundlib.h>
#include<time.h>
//====WAVE_FN====//
typedef double (*WaveBaseFn)(double t, void *params);

typedef struct
{
	WaveBaseFn wave_base_fn;
	void *params;
} WaveFn;

WaveFn wave_fn_create(WaveBaseFn wave_base_fn, void *params)
{
	WaveFn wave_fn = {wave_base_fn, params};
	return wave_fn;
};

typedef struct
{
	size_t length;
	WaveFn *wave_fns;
} WaveFnsStack;

WaveFnsStack wave_fns_stack_create(size_t length, WaveFn *wave_fns)
{
	WaveFnsStack wave_fns_stack = { length, wave_fns };
	return wave_fns_stack;
};

double wave_base_fn_add(double t, void *params)
{
	WaveFnsStack *p = (WaveFnsStack *)params;
	double result = 0.0;
	for(int i = 0; i < p->length; i++) result += p->wave_fns[i].wave_base_fn(t, p->wave_fns[i].params);
	return result;
};

WaveFn wave_fn_add(WaveFnsStack *wave_fns_stack)
{
	WaveFn wave_fn_result = wave_fn_create(wave_base_fn_add, wave_fns_stack);
	return wave_fn_result;
};

//====SOUND====//
typedef struct
{
	WaveFn wave_fn;
	double duration;
	unsigned int rate;
} Sound;

Sound sound_create(WaveFn wave_fn, double duration, unsigned int rate)
{
	Sound sound = {wave_fn, duration, rate};
	return sound;
};

// Reproducir con ALSA
void sound_playALSA(Sound sound)
{
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	unsigned int rate = sound.rate;
	int channels = 1;   // mono
	snd_pcm_uframes_t frames = 32;
	int pcm;

	// Abrir dispositivo de sonido
	if ((pcm = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		fprintf(stderr, "ERROR: no se pudo abrir el dispositivo (%s)\n", snd_strerror(pcm));
		exit(1);
	}

	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(pcm_handle, params);
	snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
	snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0);
	snd_pcm_hw_params_set_period_size_near(pcm_handle, params, &frames, 0);

	if ((pcm = snd_pcm_hw_params(pcm_handle, params)) < 0)
	{
		fprintf(stderr, "ERROR: no se pudieron establecer los parámetros (%s)\n", snd_strerror(pcm));
		exit(1);
	};

	unsigned int length = floor(sound.rate * sound.duration);
	// Convertir double (-1.0 a 1.0) → int16
	short *buffer = malloc(length * sizeof(short));
	for (int i = 0; i < length; i++)
	{
		double t = (double)i / sound.rate;
		buffer[i] = (short)(sound.wave_fn.wave_base_fn(t, sound.wave_fn.params) * 32767);
	}

	/**
	 * NOTA: Estamods duplicando un buffer que puede ser mu grande... chunkearlo quizas es lo correcto
	 */

	// Enviar al dispositivo
	int frames_written = 0;
	while (frames_written < length)
	{
		int ret = snd_pcm_writei(pcm_handle, buffer + frames_written, length - frames_written);

		if (ret == -EPIPE)
		{
			snd_pcm_prepare(pcm_handle); // Recuperar en caso de underrun
		}
		else if (ret < 0)
		{
			fprintf(stderr, "ERROR al escribir en PCM (%s)\n", snd_strerror(ret));
			break;
		}
		else
		{
			frames_written += ret;
		}
	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buffer);
}


// IMPLEMENTACION
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

double random_double(double min, double max) {
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

int main()
{
	for(int i = 0; i < 100; i++)
	{
		double fq = random_double(2200.0, 22000.0);
		double amp = random_double(0.3, 0.5);
		double dur = random_double(0.04, 0.08);
		SinParams sin_params = {fq, amp};

		WaveFn wave_fn = wave_fn_create(sin_wave_base_fn, &sin_params);
		Sound sound = sound_create(wave_fn, dur, 44100);
		sound_playALSA(sound);
	};
	
	return 0;
};
