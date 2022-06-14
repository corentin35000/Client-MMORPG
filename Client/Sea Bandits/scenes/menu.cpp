#include "menu.h"

Menu::Menu(int &nextState, ALLEGRO_DISPLAY*& display, int &screenWidth, int &screenHeight, std::vector<ALLEGRO_FONT*>& font, int& charPressed, int& mouseX, int& mouseY, int& mouseZ, int& mouseW, int& mouseB)
{
	this->nextState = &nextState;
	this->display = display;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->font = font;
	this->charPressed = &charPressed;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->mouseZ = &mouseZ;
	this->mouseW = &mouseW;

	Menu::Load();
}

Menu::~Menu() {}


void Menu::Load()
{

}


void Menu::Update(double dt) 
{

}


void Menu::Draw()
{
	al_clear_to_color(al_map_rgb(100, 100, 100));
}


void Menu::Process(bool key[MAX_KEY])
{
	if (mouseB)
	{
		*nextState = STATE_PLAY;
	}
}