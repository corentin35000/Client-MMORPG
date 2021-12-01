#include <stdio.h> // maloc..
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h> // boolean C
#include <stdlib.h>
#include <string.h> // string C
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h> // SDL2
#include "SDL2/SDL_image.h" // SDL2_Image
#include "SDL2/SDL_ttf.h" // SDL2_Font
#include <enet/enet.h> // Lib ENET / UDP
#include <curl/curl.h> // Lib CURL / Request HTTP.
#include "SDL2/SDL_mixer.h"
#include <iostream> // C++
#include <vector> // tableau dynamique - C++
using namespace std; // C++
#include "sceneSplashScreen.cpp"
#include "sceneLogin.cpp"
#include "sceneGame.cpp"


string stateScene = "SplashScreen"; // Etat de la scene de jeu.

void static sceneManager_load()
{
	sceneSplashScreen_load();
	sceneLogin_load();
	sceneGame_load();
}

void static sceneManager_update()
{	
	if (stateScene == "SplashScreen")
	{
		sceneSplashScreen_update();
	}
	else if (stateScene == "Login")
	{
		sceneLogin_update();
	}
	else if (stateScene == "Game")
	{
		sceneGame_update();
	}
}

void static sceneManager_draw()
{
	if (stateScene == "SplashScreen")
	{
		sceneSplashScreen_draw();
	}
	else if (stateScene == "Login")
	{
		sceneLogin_draw();
	}
	else if (stateScene == "Game")
	{
		sceneGame_draw();
	}
}

void static sceneManager_keypressed()
{
	if (stateScene == "SplashScreen")
	{
		sceneSplashScreen_keypressed();
	}
	else if (stateScene == "Login")
	{
		sceneLogin_keypressed();
	}
	else if (stateScene == "Game")
	{
		sceneGame_keypressed();
	}
}

void static sceneManager_mousepressed()
{
	if (stateScene == "SplashScreen")
	{
		sceneSplashScreen_mousepressed();
	}
	else if (stateScene == "Login")
	{
		sceneLogin_mousepressed();
	}
	else if (stateScene == "Game")
	{
		sceneGame_mousepressed();
	}
}

void static sceneManager_textinput()
{
	if (stateScene == "SplashScreen")
	{
		sceneSplashScreen_textinput();
	}
	else if (stateScene == "Login")
	{
		sceneLogin_textinput();
	}
	else if (stateScene == "Game")
	{
		sceneGame_textinput();
	}
}