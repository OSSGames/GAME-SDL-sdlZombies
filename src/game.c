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

#include "sdl_util.h"
#include <stdio.h>
#include "global.h"
#include "level.h"
#include "option.h"
#include "menu.h"
#include "f_option.h"



static SDL_Surface * CreateBackground(void);
static void CloseBackground(SDL_Surface * background);


static int RunGame(Option_t *opt_beg, Option_t * opt_end)
{
	int i;
	int done = SDL_EVT_NULL;
	Option_t opt;

	opt.status = opt_beg->status;

	for(i = 0; i < opt_beg->status.nb_level; i++)
	{
		HaltSound();
	
		opt.status.level = i;
	
		SetOptions(opt_beg, opt_end, &opt);

		done = RunLevel(&opt);
		if (done == SDL_EVT_QUIT || done == SDL_EVT_EXIT) {
			PlaySound(SND_MENU_VAL);
			break;
		}

		if (opt.status.nb_life <= 0) {
			HaltSound();
			PlaySound(SND_EVIL);
			break;
		}
	}

	if (i == opt_beg->status.nb_level) {
		HaltSound();
		PlaySound(SND_FINAL);
	}

	return done;
}



static int MenuGlobalOptions(Option_t * opt_beg, Option_t * opt_end,
			SDL_Surface * background)
{
	Menu_t menu;
	int ret = 0;
	int done = SDL_EVT_NULL;
	
	if (InitMenu(&menu, screen, 5, background) < 0) return -1;

	SetMenuItem(&menu, 0, images[IMG_MENU_OPT_LEVEL], 
		images[IMG_MENU_OPT_LEVEL_S], 320, 180);
		
	switch (sound_volume) {
		case 0 : SetMenuItem(&menu, 1, images[IMG_MENU_OPT_NO_SOUND],
			images[IMG_MENU_OPT_NO_SOUND_S], 320, 240); break;
		case 1 : SetMenuItem(&menu, 1, images[IMG_MENU_OPT_LOW_SOUND],
			images[IMG_MENU_OPT_LOW_SOUND_S], 320, 240); break;
		case 2 : SetMenuItem(&menu, 1, images[IMG_MENU_OPT_MED_SOUND],
			images[IMG_MENU_OPT_MED_SOUND_S], 320, 240); break;
		case 3 : SetMenuItem(&menu, 1, images[IMG_MENU_OPT_HIG_SOUND],
			images[IMG_MENU_OPT_HIG_SOUND_S], 320, 240); break;
	}
	
	switch(use_map) {
		case 0 : SetMenuItem(&menu, 2, images[IMG_MENU_OPT_NO_MAP], 
					images[IMG_MENU_OPT_NO_MAP_S], 320, 300); break;
		case 1 : SetMenuItem(&menu, 2, images[IMG_MENU_OPT_TRANS_MAP], 
					images[IMG_MENU_OPT_TRANS_MAP_S], 320, 300); break;
		case 2 : SetMenuItem(&menu, 2, images[IMG_MENU_OPT_UNDER_MAP], 
					images[IMG_MENU_OPT_UNDER_MAP_S], 320, 300); break;
		case 3 : SetMenuItem(&menu, 2, images[IMG_MENU_OPT_OVER_MAP], 
					images[IMG_MENU_OPT_OVER_MAP_S], 320, 300); break;
	}
	
	SetMenuItem(&menu, 3, images[IMG_MENU_OPT_FULL], 
		images[IMG_MENU_OPT_FULL_S], 320, 360);
	if (fullscreen == 1) {
		SetMenuItem(&menu, 3, images[IMG_MENU_OPT_WINDOW],
			images[IMG_MENU_OPT_WINDOW_S], 320, 360);
	}
	
	SetMenuItem(&menu, 4, images[IMG_MENU_OPT_QUIT], 
		images[IMG_MENU_OPT_QUIT_S], 320, 420);



	while( done == SDL_EVT_NULL )
	{
		ret = ActiveMenu(&menu);

		switch(ret)
		{
			case SDL_EVT_EXIT : done = SDL_EVT_EXIT; break;
			case SDL_EVT_QUIT : done = SDL_EVT_QUIT; break;
			case 0 : if (MenuSetOptions(opt_beg, opt_end) == SDL_EVT_EXIT)
						done = SDL_EVT_EXIT;
			break;
			case 1 : sound_volume += 1; if (sound_volume > 3) sound_volume = 0;
				switch (sound_volume) {
					case 0 : 
						SetMenuItem(&menu, 1, images[IMG_MENU_OPT_NO_SOUND],
							images[IMG_MENU_OPT_NO_SOUND_S], 320, 240); break;
					case 1 : 
						SetMenuItem(&menu, 1, images[IMG_MENU_OPT_LOW_SOUND],
							images[IMG_MENU_OPT_LOW_SOUND_S], 320, 240); break;
					case 2 : 
						SetMenuItem(&menu, 1, images[IMG_MENU_OPT_MED_SOUND],
							images[IMG_MENU_OPT_MED_SOUND_S], 320, 240); break;
					case 3 :
						SetMenuItem(&menu, 1, images[IMG_MENU_OPT_HIG_SOUND],
							images[IMG_MENU_OPT_HIG_SOUND_S], 320, 240); break;
				}
				SetSoundVolume(sound_volume);
			break;
			case 2 : use_map += 1; if (use_map > 3) use_map = 0;
				switch(use_map) {
					case 0 : 
						SetMenuItem(&menu, 2, images[IMG_MENU_OPT_NO_MAP], 
							images[IMG_MENU_OPT_NO_MAP_S], 320, 300); break;
					case 1 : 
						SetMenuItem(&menu, 2, images[IMG_MENU_OPT_TRANS_MAP], 
							images[IMG_MENU_OPT_TRANS_MAP_S], 320, 300); break;
					case 2 : 
						SetMenuItem(&menu, 2, images[IMG_MENU_OPT_UNDER_MAP], 
							images[IMG_MENU_OPT_UNDER_MAP_S], 320, 300); break;
					case 3 : 
						SetMenuItem(&menu, 2, images[IMG_MENU_OPT_OVER_MAP], 
							images[IMG_MENU_OPT_OVER_MAP_S], 320, 300); break;
				}
			break;
			case 3 : fullscreen = 1 - fullscreen;
				if (fullscreen == 1) {
					SetMenuItem(&menu, 3, images[IMG_MENU_OPT_WINDOW],
						images[IMG_MENU_OPT_WINDOW_S], 320, 360);
				} else {
					SetMenuItem(&menu, 3, images[IMG_MENU_OPT_FULL],
						images[IMG_MENU_OPT_FULL_S], 320, 360);
				}
				Sdl_SwapFullScreen(& screen);
			break;
			case 4 : done = SDL_EVT_VALID; break;
		}
	}

	CloseMenu(&menu);

	Sdl_WaitNoEvent();

	SaveGameOptions(opt_beg, opt_end);

	return done;
}


void MainLoop(Option_t * opt_beg, Option_t * opt_end)
{
	int ret = 0;
	int done = SDL_EVT_NULL;
	Menu_t menu;
	SDL_Surface * background;

	Sdl_WaitNoEvent();

	background = CreateBackground();

	if (InitMenu(&menu, screen, 3, background) < 0) return;

	SetMenuItem(&menu, 0, images[IMG_MENU_PLAY], 
		images[IMG_MENU_PLAY_S], 320, 220);
	SetMenuItem(&menu, 1, images[IMG_MENU_OPTION], 
		images[IMG_MENU_OPTION_S], 320, 310);
	SetMenuItem(&menu, 2, images[IMG_MENU_QUIT], 
		images[IMG_MENU_QUIT_S], 320, 400);


	while( done == SDL_EVT_NULL )
	{
		ret = ActiveMenu(&menu);

		switch(ret)
		{
			case SDL_EVT_EXIT : done = SDL_EVT_EXIT; break;
			case SDL_EVT_VALID : done = SDL_EVT_VALID; break;
			case 0 : 
				if (menu.type_valid == MENU_KEYBOARD) use_keyboard = 1;
					else use_keyboard = 0;
				if (RunGame(opt_beg,  opt_end) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
			break;
			case 1 :
				if (MenuGlobalOptions(opt_beg, opt_end, background) 
						== SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
			break;
			case 2 : done = SDL_EVT_VALID; break;
		}

	}

	CloseMenu(&menu);

	CloseBackground(background);
}




static SDL_Surface * CreateBackground(void)
{
	int i, j;
	SDL_Surface * background;
	SDL_Surface * img;

	background = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
		MAXX, MAXY, screen->format->BitsPerPixel, 0,0,0,0);
	if (background == NULL) ComplainAndExit("Allocation Background");

	img = images[ IMG_MENU_BACK ];

	for(i = 0; i <= MAXX; i+= img->w)
	for(j = 0; j <= MAXY; j+= img->h)
		Sdl_PutSurface(background, i, j, img);

	img = images[ IMG_MENU_TITLE ];
	Sdl_PutSurface(background, (MAXX - img->w) / 2, 20, img);

	return background;
}

static void CloseBackground(SDL_Surface * background)
{
	if (background != NULL) SDL_FreeSurface(background);
}
