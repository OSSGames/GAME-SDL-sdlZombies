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

#ifndef __HOLE_H
#define __HOLE_H

#include "sdl_util.h"

typedef struct
{
	int x, y;

	int w, h;	/* half width and height for collision */

} Hole_t;

extern void InitHole(Hole_t *hole, SDL_Rect * coord_rect);

extern void PutHole(Hole_t *hole, SDL_Surface * dest);



#endif /* __HOLE_H */
