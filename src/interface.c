#include "waveshow.h"
#include "interface.h"

void start_interface()
{
	SDL_Surface *screen;

	screen = SDL_SetVideoMode(get_uint("window_xsize"), get_uint("window_ysize"),
			get_uint("window_colourdepth"), SDL_DOUBLEBUF);

	if (!screen)
	{
		fprintf(stderr, "Unable to create screen: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_WM_SetCaption("Waveform Display", NULL);
}
