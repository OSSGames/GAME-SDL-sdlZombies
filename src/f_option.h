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

#ifndef __F_OPTION_H
#define __F_OPTION_H

#include "option.h"

extern void LoadGameOptions(Option_t * opt_beg, Option_t * opt_end);
extern void SaveGameOptions(Option_t * opt_beg, Option_t * opt_end);



#endif /* __F_OPTION_H */
