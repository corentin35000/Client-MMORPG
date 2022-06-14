#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "map.h"

Map::Map() 
{
	
}


// This is a sample format to be replaced by map editor format
void Map::load() 
{
	// Initialise la caméra
	camera.x = 0;
	camera.y = 0;


	// Map
	mapsImg[0] = al_load_bitmap("assets/maps/map_pve_sea.png");
	mapsImg[1] = al_load_bitmap("assets/maps/pve_middle.png");
	mapsImgWidth = al_get_bitmap_width(mapsImg[0]);
	mapsImgHeight = al_get_bitmap_height(mapsImg[0]);


	// Grille : Map -> Initialise pour chaque colonne de chaque ligne : {} (aucune donnée)
	for (int row = 0; row < MAP_HEIGHT; row++)
		for (int column = 0; column < MAP_WIDTH; column++)
		{
			tileMap[row][column] = {};
		}
}

void Map::draw()
{
	// Affichage de la Map (récupère l'image de la map puis la multiplie)
	for (int y = 0; y <= NOMBRE_LIGNE; y += mapsImgHeight)
	{
		for (int x = 0; x <= NOMBRE_COLONNE; x += mapsImgWidth)
		{
			if (x - camera.x <= 1920 && y - camera.y <= 1080)
				al_draw_bitmap(mapsImg[0], x - camera.x, y - camera.y, 0);
		}
	}

	
	// Affiche d'une grille isometric par-dessus la Map
	int x = 0;
	int y = 0;
	for (int row = 0; row <= MAP_HEIGHT; row++)
	{
		for (int column = 0; column <= MAP_WIDTH; column++)
		{
			//al_draw_rectangle(x, y, x + TILE_WIDTH, y + TILE_HEIGHT, al_map_rgb(15, 15, 15), 2);
			x += TILE_WIDTH;
		}

		y += TILE_HEIGHT;
		x = 0;
	}
}

Unit* Map::spawn_unit(UnitType type, Position pos, Size size)
{
	Unit* unit = new Unit;
	g_units.insert(unit);

	unit->spritesheet = al_load_bitmap("assets/ships/Player/1-pinnace/spritesheet.png");
	unit->spriteWidth = (al_get_bitmap_width(unit->spritesheet) / 8);
	unit->spriteHeight = al_get_bitmap_height(unit->spritesheet);

	unit->type = type;
	unit->pos = pos;
	unit->size = size;
	switch (size) {
		case SMALL:
			unit->startX = 347;
			unit->sizeX = 83;
			unit->sizeY = 53;
			break;
		case MEDIUM:
			unit->startX = 226;
			unit->sizeX = 119;
			unit->sizeY = 73;
			unit->offsetX = unit->sizeX / 5;
			unit->offsetY = unit->sizeY / 5;
			break;
		case LARGE:
			unit->startX = 0;
			unit->sizeX = 223;
			unit->sizeY = 152;
			unit->offsetX = unit->sizeX / 3;
			unit->offsetY = unit->sizeY / 3;
			break;
	}

	//en_north = 0, en_northeast = 1, en_east = 2, en_southeast = 3,
	//en_south = 4, en_southwest = 5, en_west = 6, en_northwest = 7
	unit->direction = Direction::en_northwest;
	unit->state = State::stopped;

	unit->name = "";
	unit->frame = 0;
	unit->tileW = 0, unit->tileH = 0;
	unit->x = 0, unit->y = 0;
	unit->inMotion = false;

	return unit;
}

Unit* Map::spawn_unit(NPC_Ship npc, UnitType type, Position pos, Size size)
{
	Unit* unit = new Unit;
	g_units.insert(unit);

	unit->spritesheet = al_load_bitmap("assets/ships/NPC/1-red_korsar/spritesheet.png");
	unit->spriteWidth = (al_get_bitmap_width(unit->spritesheet) / 8);
	unit->spriteHeight = al_get_bitmap_height(unit->spritesheet);

	unit->type = type;
	unit->pos = pos;
	unit->size = size;
	switch (size) {
		case SMALL:
			unit->startX = 347;
			unit->sizeX = 83;
			unit->sizeY = 53;
			break;
		case MEDIUM:
			unit->startX = 226;
			unit->sizeX = 119;
			unit->sizeY = 73;
			unit->offsetX = unit->sizeX / 5;
			unit->offsetY = unit->sizeY / 5;
			break;
		case LARGE:
			unit->startX = 0;
			unit->sizeX = 223;
			unit->sizeY = 152;
			unit->offsetX = unit->sizeX / 3;
			unit->offsetY = unit->sizeY / 3;
			break;
	}

	//en_north = 0, en_northeast = 1, en_east = 2, en_southeast = 3,
	//en_south = 4, en_southwest = 5, en_west = 6, en_northwest = 7
	unit->direction = Direction::en_northwest;
	unit->state = State::stopped;

	unit->name = "";
	unit->frame = 0;
	unit->tileW = 0, unit->tileH = 0;
	unit->x = 0, unit->y = 0;
	unit->inMotion = false;

	return unit;
}

// Renvoie true si l'objet est dans le brouillard donc l'objet sera invisible. (distanceVision = jusqu'a ou vois le bateau)
bool Map::fogOfWar(int playerX, int playerY,  int unitX, int unitY, int distanceVision)
{
	if (Map::mathDistance(playerX, playerY, unitX, unitY) <= distanceVision)
		return true;
	else
		return false;
}

void Map::circleFogOfWar(int playerX, int playerY, int distanceVision)
{
	al_draw_circle(playerX, playerY, distanceVision, al_map_rgb_f(1, 1, 1), 0);
}

double sqr(double a) {
	return a * a;
}

// Renvoie la distance entre deux points.
double Map::mathDistance(double x1, double y1, double x2, double y2) {
	return sqrt(sqr(y2 - y1) + sqr(x2 - x1));
}

// Renvoie l'angle entre deux vecteurs en supposant la même origine. 
double Map::mathAngleDegres(double x1, double y1, double x2, double y2) {
	return atan2(y2 - y1, x2 - x1) * (180 / M_PI); // return angle en degres
}

// Renvoie l'angle entre deux vecteurs en supposant la même origine. 
double Map::mathAngleRadians(double x1, double y1, double x2, double y2) {
	return atan2(y2 - y1, x2 - x1); // return angle radians
}

// Cosinus
double Map::cosinus(double x) {
	return cos(x);
}

// Sinus
double Map::sinus(double y) {
	return sin(y);
}