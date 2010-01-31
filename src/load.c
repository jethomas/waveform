#include "waveshow.h"
#include "load.h"

#define INBUF_SIZE 4096

int16_t *load_file(const char *filename, unsigned int *data_size)
{
	AVFormatContext *formatContext;
	AVCodec *codec;
	AVCodecContext *enc;
	AVPacket avpkt;
	uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
	uint16_t *outdata, *tdata, *samples;
	unsigned int samples_size, outsize;
	int i, error, len;
	FILE *f;

	if ((!filename) || (filename[0] == '\0') || (!data_size))
		return NULL;

	f = fopen(filename, "rb");
	if (!f)
	{
		fprintf(stderr, "Couldn't open file %s!\n", filename);
		return NULL;
	}

	error = av_open_input_file(&formatContext, filename, NULL, 0, NULL);
	if (error != 0)
	{
		fprintf(stderr, "Couldn't av_open file %s (error: %s [%d])!\n", filename,
				strerror(error * -1), error * -1);
		return NULL;
	}

	if (av_find_stream_info(formatContext) < 0)
	{
		fprintf(stderr, "Couldn't find file information!\n");
		return NULL;
	}

	codec = NULL;
	for (i = 0; i < formatContext->nb_streams; i++)
	{
		if (formatContext->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO)
		{
			enc = formatContext->streams[i]->codec;
			codec = avcodec_find_decoder(
					formatContext->streams[i]->codec->codec_id);
			if (!codec)
			{
				fprintf(stderr, "Couldn't find a codec for this file!\n");
				return NULL;
			}
			break;
		}
	}

	if (!codec)
	{
		fprintf(stderr, "File doesn't contain any audio streams!\n");
		return NULL;
	}

	if (avcodec_open(enc, codec) < 0)
	{
		fprintf(stderr, "Couldn't open codec!\n");
		return NULL;
	}

	samples = malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
	if (!samples)
	{
		fprintf(stderr, "Couldn't allocate frame buffer!\n");
		return NULL;
	}

	av_init_packet(&avpkt);
	outsize = 0;
	outdata = NULL;

	while (1)
	{
		avpkt.size = fread(inbuf, 1, INBUF_SIZE, f);
		avpkt.data = inbuf;
		while (avpkt.size > 0)
		{
			samples_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
			len = avcodec_decode_audio3(enc, samples, &samples_size, &avpkt);
			if (len < 0)
			{
				fprintf(stderr, "Error while decoding file!\n");
				if (outdata) free(outdata);
				return NULL;
			}
			avpkt.size -= len;
			avpkt.data += len;

			/* Only do this if a frame has actually been output */
			if (samples_size > 0)
			{
				printf("Requesting size of %d\n", outsize + samples_size);
				tdata = realloc(outdata, outsize + samples_size);
				if (!tdata)
				{
					fprintf(stderr, "Couldn't reallocate sample buffer!\n");
					free(outdata);
					return NULL;
				}
				else
				{
					outdata = tdata;
				}
				printf("SAMPLE: %p %p\n", samples, samples + samples_size - 1);
				printf("OUTBUF: %p %p % p\n", outdata, outdata + outsize, outdata + outsize + samples_size - 1);
				memcpy((char *)outdata + outsize, (char *)samples, samples_size);
				outsize += samples_size;
			}
		}
	}

	*data_size = outsize;
	av_close_input_file(formatContext);
	fclose(f);
	return outdata;
}
