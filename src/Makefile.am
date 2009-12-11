## Process this file with automake to produce Makefile.in

INCLUDES = @AVCODEC_CFLAGS@ @AVFORMAT_CFLAGS@ @SDL_CFLAGS@

bin_PROGRAMS = waveshow


waveshow_SOURCES = \
        main.c waveshow.h \
        config_manager.c config_manager.h \
        interface.c interface.h \
        load.c load.h

waveshow_LDADD = @AVCODEC_LIBS@ @AVFORMAT_LIBS@ @SDL_LIBS@

