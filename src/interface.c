#include "waveshow.h"
#include "interface.h"

void event_loop(SDL_Surface *screen)
{
	SDL_Event ev;

	while (1)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_KEYDOWN:
					switch (ev.key.keysym.sym)
					{
						case SDLK_q:
							return;
							break;
					}
					break;

				case SDL_QUIT:
					return;
					break;
			}
		}
	}
}

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

	event_loop(screen);
}
