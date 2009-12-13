#include "waveshow.h"

int main (int argc, char ** argv)
{
	char *data;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	/* Initialize libavcodec and all of the codecs */
	avcodec_init();
	avcodec_register_all();

	if (argc > 1)
	{
		data = load_file(argv[1]);
	}

	if (data)
	{
		start_interface();
	}
	else
	{
		fprintf(stderr, "No data to display!\n");
	}

	SDL_Quit();
	return 0;
}
