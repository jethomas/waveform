#include "waveform.h"

int main (int argc, char ** argv)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	/* Initialize libavcodec and all of the codecs */
	avcodec_init();
	avcodec_register_all();

	start_interface();

	return 0;
}
