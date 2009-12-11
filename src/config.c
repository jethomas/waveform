#include "waveform.h"
#include "config.h"

const char *get_string(const char *key)
{
	return NULL;
}

unsigned int get_uint(const char *key)
{
	if (!strcmp(key, "window_xsize"))
		return 640;
	else if (!strcmp(key, "window_ysize"))
		return 480;
	else if (!strcmp(key, "window_colourdepth"))
		return 32;

	fprintf(stderr, "Couldn't find config option!\n");
	exit(1);
}

