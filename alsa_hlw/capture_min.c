/*
 *  This extra small demo sends a random samples to your speakers.
 */
#include <alsa/asoundlib.h>

#define FRAMES 1600 //100ms

static char *device = "plughw:0,1";            /* playback device */
unsigned char buffer[2*8*FRAMES];              /* some random data */

int main(void)
{
	int err;
	unsigned int i;
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;

	if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf(stderr, "Playback open error: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
    if ((err = snd_pcm_set_params(handle,
                                  SND_PCM_FORMAT_S16_LE,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  8,
                                  16000,
                                  1,
                                  100000)) < 0) {   /* 0.1sec */
        fprintf(stderr, "Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

	{
		snd_pcm_uframes_t buffer_size;
		snd_pcm_uframes_t period_size;
		if ((err = snd_pcm_get_params(handle,
			&buffer_size,
			&period_size)) < 0 ) {
				fprintf(stderr, "Playback open error: %s\n", snd_strerror(err));
				exit(EXIT_FAILURE);
		}
		fprintf(stderr, "buffer_size %ld period_size %ld\n", buffer_size, period_size);
	}
	
    for (i = 0; i < 1000; i++) {
		frames = snd_pcm_readi(handle, buffer, FRAMES);
		if (frames < 0)
			frames = snd_pcm_recover(handle, frames, 0);
		if (frames < 0) {
			fprintf(stderr, "snd_pcm_readi failed: %s\n", snd_strerror(frames));
			break;
		}
		fprintf(stderr, "frames %li\n", frames);

		if (frames > 0 && frames < FRAMES)
			fprintf(stderr, "short read (expected %li, wrote %li)\n", FRAMES, frames);

		fwrite(buffer, 1, sizeof(buffer), stdout);
    }
    snd_pcm_close(handle);
    return 0;
}