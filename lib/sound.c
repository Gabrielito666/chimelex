//====SOUND====//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <alsa/asoundlib.h>

#include "sound.h"


//implementación
Sound sound__create(WaveFn wave_fn, double duration, unsigned int rate)
{
	Sound sound = {wave_fn, duration, rate};
	return sound;
};

/**
 * sound__play
 * metodo para reproducir con ALSA
 */
//implementación
void sound__play(Sound sound)
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
	for (unsigned int i = 0; i < length; i++)
	{
		double t = (double)i / sound.rate;
		buffer[i] = ((short)(wave_fn_opts.eval(&sound.wave_fn, t) * 32767));
	}

	/**
	 * NOTA: Estamods duplicando un buffer que puede ser mu grande... chunkearlo quizas es lo correcto
	 */

	// Enviar al dispositivo
	unsigned int frames_written = 0;
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

SoundOpts sound_opts = { .create = sound__create, .play = sound__play };
