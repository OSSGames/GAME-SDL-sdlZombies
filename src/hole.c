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

#include "hole.h"
#include "global.h"


void InitHole(Hole_t *hole, SDL_Rect * coord_rect)
{
	hole->x = Random(0, coord_rect->w); 
	hole->y = Random(0, coord_rect->h);

	hole->w = images[IMG_HOLE]->w / 3;
	hole->h = images[IMG_HOLE]->h / 3;
}

void PutHole(Hole_t *hole, SDL_Surface * dest)
{
	SDL_Rect drect;
	SDL_Surface * img = images[IMG_HOLE];
	
	drect.x = hole->x - img->w / 2; 
	drect.y = hole->y - img->h / 2;
	drect.w = img->w; drect.h = img->h;

	SDL_BlitSurface(img, NULL, dest, & drect);
}
