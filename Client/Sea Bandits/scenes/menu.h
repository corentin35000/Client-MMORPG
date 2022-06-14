#ifndef MENU_H_
#define MENU_H

#include "gamestate.h"


class Menu : public Gamestate
{
	private:
		std::vector<ALLEGRO_FONT*> font;
		int *nextState;
		ALLEGRO_DISPLAY* display;
		int screenHeight, screenWidth;
		int *charPressed;
		int* mouseX, * mouseY, * mouseB, * mouseW, * mouseZ;

		std::vector<ALLEGRO_BITMAP*> membitmap;
		std::vector<ALLEGRO_BITMAP*> bitmap;

	public:
		Menu(int &nextState, ALLEGRO_DISPLAY*& display, int& screenWidth, int& screenHeight, std::vector<ALLEGRO_FONT*>& font, int& charPressed, int& mouseX, int& mouseY, int& mouseZ, int& mouseW, int& mouseB);
		~Menu();

	public:
		void Load();
		void Update(double dt);
		void Process(bool key[MAX_KEY]);
		void Draw();
};

#endif