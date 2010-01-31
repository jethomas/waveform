#include "waveshow.h"

int main (int argc, char ** argv)
{
	uint16_t *data, max = 0, min = 0;
	unsigned int i, data_size;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	/* Initialize libavcodec and all of the codecs */
	av_register_all();

	if (argc > 1)
	{
		data = load_file(argv[1], &data_size);
		if ((data) && (data_size > 0))
		{
			printf("Managed to load file correctly!\n");
			printf("Data size: %d\n", data_size);
			min = max = data[0];
		}
		else
		{
			return 1;
		}

		for (i = 0; i < data_size; i++)
		{
			if (data[i] > max)
				max = data[i];
			if (data[i] < min)
				min = data[i];
		}
		printf("max: %d, min: %d\n", max, min);
	}

	if (data)
	{
		start_interface();
	}
	else
	{
		fprintf(stderr, "No data to display!\n");
		return 1;
	}

	SDL_Quit();
	return 0;
}
