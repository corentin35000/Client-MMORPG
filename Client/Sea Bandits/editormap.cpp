#include "editormap.h"

EditorMap::EditorMap(int& nextState, int& screenWidth, int& screenHeight, std::vector<ALLEGRO_FONT*>& font, int& charPressed, int& mouseX, int& mouseY, int& mouseZ, int& mouseW, int& mouseB)
{
	this->nextState = &nextState;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->font = font;
	this->charPressed = &charPressed;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->mouseW = &mouseW;
	this->mouseZ = &mouseZ;

	EditorMap::Load();
}


EditorMap::~EditorMap() {}


void EditorMap::Load()
{
	// Background color du fond d'écran de l'éditeur de map.
	backgroundColorEditorMap = al_map_rgb(137, 137, 137); // Grey

	// Init data maps global to all levels maps.
	stateOrientationTiles = ISOMETRIC; // Draw grilleMap to Isometric OR Orthogonale.
	map_widthText = "MAP_WIDTH : " + std::to_string(MAP_WIDTH);
	map_heightText = "MAP_HEIGHT : " + std::to_string(MAP_HEIGHT);
	tile_widthText = "TILE_WIDTH : " + std::to_string(TILE_WIDTH);
	tile_heightText = "TILE_HEIGHT : " + std::to_string(TILE_HEIGHT);
	map_pixelsText = "MAP_PIXELS : " + std::to_string(NOMBRE_LIGNE) + " x " + std::to_string(NOMBRE_COLONNE) + " pixels";
}


void EditorMap::Update(double dt)
{

}


void EditorMap::Draw()
{
	// Background color in the editormap
	al_clear_to_color(backgroundColorEditorMap);


	// Affiche d'une grille 2D Pur par-dessus la Map
	int x = 0;
	int y = 0;
	for (int row = 0; row <= MAP_HEIGHT; row++)
	{
		for (int column = 0; column <= MAP_WIDTH; column++)
		{
			al_draw_rectangle(x, y, x + TILE_WIDTH, y + TILE_HEIGHT, al_map_rgb(15, 15, 15), 0);
			x += TILE_WIDTH;
		}

		y += TILE_HEIGHT;
		x = 0;
	}


	// Affiche d'une grille 2D Isometric par-dessus la Map
	/*for (int row = 0; row <= MAP_HEIGHT; row++)
		for (int column = 0; column <= MAP_WIDTH; column++)
		{
			//Rect tile = getTileRect(row, column, camera);
			//al_draw_polygon(tile.polygon, 4, ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(0, 255, 0), 1, 1);
		}
		*/


	// Affiche les données des maps.
	al_draw_text(font[0], al_map_rgb(255, 255, 255), 5, 0, 0, (char*)map_widthText.c_str());
	al_draw_text(font[0], al_map_rgb(255, 255, 255), 50, 20, 1, (char*)map_heightText.c_str());
	al_draw_text(font[0], al_map_rgb(255, 255, 255), 45, 40, 1, (char*)tile_widthText.c_str());
	al_draw_text(font[0], al_map_rgb(255, 255, 255), 45, 52, 1, (char*)tile_heightText.c_str());
	al_draw_text(font[0], al_map_rgb(255, 255, 255), 83, 70, 1, (char*)map_pixelsText.c_str());
}


void EditorMap::Process(bool key[MAX_KEY])
{
	if (mouseB)
	{

	}

	if (key[FOO1])
	{
		*nextState = STATE_PLAY;
	}
}