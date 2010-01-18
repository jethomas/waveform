#include "waveshow.h"
#include "load.h"

#define INBUF_SIZE 4096

int16_t *load_file(const char *filename)
{
	AVFormatContext *formatContext;
	AVCodec *codec;
	AVCodecContext *c;
	AVPacket avpkt;
	int len, i, outsize;
	FILE *f;
	uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
	int16_t *outbuf;

	if ((!filename) || (filename[0] == '\0'))
		return NULL;

	f = fopen(filename, "rb");
	if (!f)
	{
		fprintf(stderr, "Couldn't open file %s!\n", filename);
		return NULL;
	}

	if (av_open_input_file(&formatContext, filename, NULL, 0, NULL) != 0)
	{
		fprintf(stderr, "Couldn't open file %s!\n", filename);
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
			codec = avcodec_find_decoder(
					formatContext->streams[i]->codec->codec_id);
			if (!codec)
			{
				fprintf(stderr, "Couldn't find a codec for this file!\n");
				return NULL;
			}
		}
	}
	av_close_input_file(formatContext);

	if (!codec)
	{
		fprintf(stderr, "File doesn't contain any audio streams!\n");
		return NULL;
	}

	c = avcodec_alloc_context();

	if (avcodec_open(c, codec) < 0)
	{
		fprintf(stderr, "Couldn't open codec!\n");
		return NULL;
	}

	outbuf = malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
	if (!outbuf)
	{
		fprintf(stderr, "Couldn't allocate raw data!\n");
		return NULL;
	}

	av_init_packet(&avpkt);
	while (1)
	{
		avpkt.size = fread(inbuf, sizeof(uint8_t), INBUF_SIZE, f);
		if (avpkt.size == 0)
			break;

		avpkt.data = inbuf;
		while (avpkt.size > 0)
		{
			outsize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
			len = avcodec_decode_audio3(c, outbuf, &outsize, &avpkt);
			if (len < 0)
			{
				fprintf(stderr, "Error while decoding file!\n");
				return NULL;
			}
			avpkt.size -= len;
			avpkt.data += len;

			/* XXX DEBUG XXX */
			goto DEBUG_EXIT;
		}
	}

DEBUG_EXIT:
	fclose(f);
	avcodec_close(c);
	av_free(c);
	return outbuf;
}
