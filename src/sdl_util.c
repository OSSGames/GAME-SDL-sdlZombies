/***************************************************************************
                        SdlZombies - Simple zombies game
                             -------------------
    begin                : Sun Apr 15 16:55:07 CEST 2001
    copyright            : (C) 2001 by Philippe Brochard
    email                : hocwp@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "sdl_util.h"
#include "global.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif




/*===========================================================
 *		Initialisation et fermeture
 ============================================================*/
 
static Uint32 videoflags;
static Uint8  video_bpp;


/* This is a way of telling whether or not to use hardware surfaces */
static Uint32 FastestFlags(Uint32 flags)
{
	const SDL_VideoInfo *info;

	/* Hardware acceleration is only used in fullscreen mode */
	flags |= SDL_FULLSCREEN;

	/* Check for various video capabilities */
	info = SDL_GetVideoInfo();
	if ( info->blit_hw_CC && info->blit_fill ) {
		/* We use accelerated colorkeying and color filling */
		flags |= SDL_HWSURFACE;
	}
	/* If we have enough video memory, and will use accelerated
	   blits directly to it, then use page flippeng.
	 */
	if ( (flags & SDL_HWSURFACE) == SDL_HWSURFACE ) {
		/* Direct hardware blitting without double-buffering
		   causes really bad flickering.
		 */
		if ( info->video_mem > MAXX*MAXY ) {
			flags |= SDL_DOUBLEBUF;
		} else {
			flags &= ~SDL_HWSURFACE;
		}
	}

	/* Return the flags */
	return(flags);
}



static void Usage(char *name)
{
	fprintf(stderr, "SdlZombies %s\n", VERSION);
	fprintf(stderr, "Usage: %s [Options]\n", name);

	fprintf(stderr, "       --bpp value \t set color depth (value=8,16,24,32)\n");
	fprintf(stderr, "  -hw, --hardware \t enable hardware surface\n");
	fprintf(stderr, "       --flip \t\t enable double buffer\n");
	fprintf(stderr, "       --fast \t\t enable fast blitting\n");
	fprintf(stderr, "  -f,  --fullscreen \t enable fullscreen\n");
	fprintf(stderr, "  -w,  --window \t enable window\n");
	fprintf(stderr, "  -ns, --nosound \t disable sounds\n");
	fprintf(stderr, "  -s,  --sound \t enable sounds\n");
	exit(1);
}


SDL_Surface * Sdl_Init(int argc, char ** argv)
{
	SDL_Surface * screen;

	/* initialisation de SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
	
	/* Test des arguments */
	videoflags = SDL_SWSURFACE|SDL_ANYFORMAT;
	video_bpp = 8;
	while ( argc > 1 ) {
		--argc;
		if ( strcmp(argv[argc-1], "--bpp") == 0 ) {
			video_bpp = atoi(argv[argc]);
			printf("* Set bpp = %d\n", video_bpp);
			videoflags &= ~SDL_ANYFORMAT;
			--argc;
		} else
		if ( strcmp(argv[argc], "--fast") == 0 ) {
			printf("* Enable fast\n");
			videoflags = FastestFlags(videoflags);
		} else
		if ( strcmp(argv[argc], "-hw") == 0 || 
				strcmp(argv[argc], "--hardware") == 0 ) {
			printf("* Enable harware surface\n");
			videoflags ^= SDL_HWSURFACE;
		} else
		if ( strcmp(argv[argc], "--flip") == 0 ) {
			printf("* Enable double buffer\n");
			videoflags ^= SDL_DOUBLEBUF;
		} else
		if ( strcmp(argv[argc], "--nosound") == 0 ||
				strcmp(argv[argc], "-ns") == 0 ) {
			printf("* Disable sound\n");
			use_sound = 0;
		} else
		if ( strcmp(argv[argc], "--sound") == 0 ||
				strcmp(argv[argc], "-s") == 0 ) {
			printf("* Enable sound\n");
			use_sound = 1;
		} else
		if ( strcmp(argv[argc], "-f") == 0 ||
				strcmp(argv[argc], "--fullscreen") == 0 ) {
			printf("* Enable fullscreen\n");
			fullscreen = 1;
		} else
		if ( strcmp(argv[argc], "-w") == 0 ||
				strcmp(argv[argc], "--window") == 0 ) {
			printf("* Enable window\n");
			fullscreen = 0;
		} else {
			Usage(argv[0]);
		}
	}

	if (fullscreen == 1) videoflags ^= SDL_FULLSCREEN;
	
	
	/* Set MAXXxMAXY (640x480) video mode */
	if ( (screen=SDL_SetVideoMode(MAXX,MAXY,video_bpp,videoflags)) == NULL ) 
	{
		fprintf(stderr, "Couldn't set %dx%d video mode: %s\n", 
			MAXX, MAXY, SDL_GetError());
		exit(2);
	}


	/* Init SDL Audio: */
	if (use_sound == 1)
	{
		if (SDL_Init(SDL_INIT_AUDIO) < 0)
		{
			fprintf(stderr,
				"\nWarning: I could not initialize audio!\n"
				"The Simple DirectMedia error that occured was:\n"
				"%s\n\n", SDL_GetError());
			use_sound = 0;
		}
	}

	/* Open sound: */
#ifndef NOSOUND
	
	if (use_sound == 1)
	{
		if (Mix_OpenAudio(22050, AUDIO_S16, 2, 512) < 0)
		{
			fprintf(stderr,
				"\nWarning: I could not set up audio for 22050 Hz "
				"16-bit stereo.\n"
				"The Simple DirectMedia error that occured was:\n"
				"%s\n\n", SDL_GetError());
			use_sound = 0;
		}
		SetSoundVolume(sound_volume);
	}
#endif

	/* Random initialization (annexe) */
	InitRandom();

	return( screen );
}


void Sdl_Close(SDL_Surface * screen)
{
	SDL_FreeSurface(screen);

#ifndef NOSOUND
	Mix_CloseAudio();
#endif

	SDL_Quit();
}


void Sdl_SwapFullScreen(SDL_Surface ** screen)
{
	videoflags ^= SDL_FULLSCREEN;

	SDL_FreeSurface( * screen );
	
	/* Set MAXXxMAXY (640x480) video mode */
	if ( (* screen=SDL_SetVideoMode(MAXX,MAXY,video_bpp,videoflags)) == NULL ) 
	{
		fprintf(stderr, "Couldn't set %dx%d video mode: %s\n", 
			MAXX, MAXY, SDL_GetError());
		exit(2);
	}
}


void Sdl_WaitNoEvent(void)
{
	SDL_Event event;
	int done;

	do {
		done = 0;
	
		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_KEYDOWN : done = 1; break;	

				case SDL_MOUSEBUTTONDOWN : done = 1; break;
			}
		}
	} while(done == 1);
}


int Sdl_Pause(void)
{
	int done = SDL_EVT_NULL;
	SDL_Event event;

	while(done == SDL_EVT_NULL)
	{
		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_QUIT : done = SDL_EVT_EXIT; break;

				case SDL_KEYDOWN : done = SDL_EVT_VALID;
					if (event.key.keysym.sym == key_quit) done = SDL_EVT_QUIT;
				break;

				case SDL_MOUSEBUTTONUP : done = SDL_EVT_VALID; break;
			}
		}
	}

	return done;
}



void Sdl_PutPixel(SDL_Surface *screen, int x, int y, 
		Uint8 red, Uint8 green, Uint8 blue)
{
	Uint32   pixel;
	Uint8   *bits, bpp;
	
	pixel = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
	
	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 )
			return;
	}
	
	bpp = screen->format->BytesPerPixel;
	bits = ((Uint8 *)screen->pixels)+y*screen->pitch+x*bpp;
	
	/* Set the pixel */
	switch(bpp) {
		case 1:
			*((Uint8 *)(bits)) = (Uint8)pixel;
		break;
		case 2:
			*((Uint16 *)(bits)) = (Uint16)pixel;
		break;
		case 3: { /* Format/endian independent */
			Uint8 r, g, b;
			r = (pixel>>screen->format->Rshift)&0xFF;
			g = (pixel>>screen->format->Gshift)&0xFF;
			b = (pixel>>screen->format->Bshift)&0xFF;
			*((bits)+screen->format->Rshift/8) = r; 
			*((bits)+screen->format->Gshift/8) = g;
			*((bits)+screen->format->Bshift/8) = b;
		}
		break;
		case 4:
			*((Uint32 *)(bits)) = (Uint32)pixel;
		break;
	}
	
	/* Update the display */
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect(screen, x, y, 1, 1);
}






void Sdl_PutSurface(SDL_Surface * screen, int x, int y, SDL_Surface * source)
{
	SDL_Rect dstrect;
	
	dstrect.x = x; dstrect.y = y;
	dstrect.w = source->w; dstrect.h = source->h;
	SDL_BlitSurface(source, NULL, screen, &dstrect);
}



int Sdl_XYOnRect(int x, int y, SDL_Rect * rect)
{
	if (x >= rect->x && y >= rect->y &&
		x <= rect->x + rect->w && y <= rect->y + rect->h) return 1;

	return 0;
}



void ComplainAndExit(char *msg)
{
	fprintf(stderr, "Error : %s\n", msg);

	exit( 1 );
}


void InitRandom(void)
{
	time_t curtime;
	int i;
	
	time( &curtime );
	srand( (unsigned int) curtime );
	
	for(i = 0; i < Random(0, 1000); i++)
		Random(0, 1000);
}

int Random(int rmin, int rmax)
{
	return rmin + (int) (1.0*(rmax-rmin+1) * rand()/(RAND_MAX+1.0) );
}
