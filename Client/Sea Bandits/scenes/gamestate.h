#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "../engine.h"
#include "../common.h"


enum GAMESTATES
{
	STATE_NULL,
	STATE_SPLASHSCREEN,
	STATE_MENU,
	STATE_PLAY,
	STATE_REAPARITION,
	STATE_EDITORMAP,
	STATE_EXIT,
};

class Gamestate
{
	public:
		Gamestate();

		Gamestate(int& nextState, int &screenWidth, int &screenHeight, std::vector<ALLEGRO_FONT*> &font, int& charPressed, int& mouseX, int& mouseY, int& mouseB, ALLEGRO_DISPLAY& display);
		virtual ~Gamestate() {}

	public:
		virtual void Load() = 0;
		virtual void Update(double dt) = 0;
		virtual void Process(bool key[MAX_KEY]) = 0;
		virtual void Draw() = 0;
};

#endif