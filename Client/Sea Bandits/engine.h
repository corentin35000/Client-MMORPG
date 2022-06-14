#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "common.h"

#include "enet/enet.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;


class Engine
{
	public:
		Engine(int screenWidth, int screenHeight);
		~Engine();

	public :		
		// Engine/Agrello timer : Structure for queue events, fonts, display..
		ALLEGRO_BITMAP* icon;
		ALLEGRO_DISPLAY* display;
		ALLEGRO_TIMER* timer;
		ALLEGRO_EVENT_QUEUE* queue;
		std::vector<ALLEGRO_FONT*> font;

		// Engine : Keyboard, keypressed, mouse..
		bool key[MAX_KEY];
		int charPressed;
		int mouseX;
		int mouseY;
		int mouseZ;
		int mouseW;
		int mouseB;

		// Engine - Display
		int screen_width;
		int screen_height;
		int FPS = 60;
		double deltaTime;

		// Engine/ENET
		ENetHost* client; // Client ENET
		ENetAddress address; // Adress to connection for Server (ip + port)
		ENetEvent eventENET;
		ENetPeer* peer;
		ENetPacket* packet;
};

#endif