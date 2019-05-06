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


#ifndef __SDLUTIL_H
#define __SDLUTIL_H

#include <SDL.h>
#include <SDL_image.h>
#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#define MAXX	640
#define MAXY	480

#define SDL_EVT_QUIT	-1
#define SDL_EVT_EXIT	-2
#define SDL_EVT_NULL	-3
#define SDL_EVT_VALID	-4

extern SDL_Surface * Sdl_Init(int argc, char ** argv);
extern void Sdl_Close(SDL_Surface * screen);

extern void Sdl_SwapFullScreen(SDL_Surface ** screen);

extern void Sdl_WaitNoEvent(void);
extern int Sdl_Pause(void);

extern void Sdl_PutPixel (SDL_Surface *screen, int x, int y, 
		Uint8 red, Uint8 green, Uint8 blue);

extern void Sdl_PutSurface(SDL_Surface * screen, int x, int y,
		SDL_Surface * source);

extern int Sdl_XYOnRect(int x, int y, SDL_Rect * rect);

extern void ComplainAndExit(char *msg);

extern void InitRandom(void);
extern int Random(int min, int max);


#endif
