#ifndef MAP_H_
#define MAP_H_

#define M_PI 3.14159265358979323846

#include <vector>
#include <set>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "unit.h"
#include "common.h"
#include "engine.h"
using namespace std;


enum AmmoMunitions {
	CONFETIS,
	ILLUMINER
};

class AnimationAmmo {
	public :
		Unit* cible;
		int x, y;
		double vx, vy;
		int frame = 0;
		AmmoMunitions typeAmmo;
		int degats;
};

struct Tile {
	Unit* occupying_unit;
};

class Map 
{
	public:
		Map();

		Camera camera;

		ALLEGRO_BITMAP* mapsImg[3]; // Bitmaps de toute les maps
		int mapsImgWidth, mapsImgHeight;

		Unit* spawn_unit(UnitType type, Position pos, Size size);
		Unit* spawn_unit(NPC_Ship npc, UnitType type, Position pos, Size size);

		Tile tileMap[MAP_HEIGHT][MAP_WIDTH];
		//vector<Tile> tileMaps; // Tableau a 3 dimensions qui contiendras tout les niveaux de l'editeur de map.
		std::set<Unit*> g_units;
		//std::set<AttackAnimationAmmo*> g_animationsAmmo;

	public:
		void load();
		void update();
		void draw();

		bool fogOfWar(int playerX, int playerY, int unitX, int unitY, int playerVision);
		void circleFogOfWar(int playerX, int playerY, int distanceVision);
		double mathDistance(double x1, double y1, double x2, double y2);
		double mathAngleDegres(double x1, double y1, double x2, double y2);
		double mathAngleRadians(double x1, double y1, double x2, double y2);
		double cosinus(double x);
		double sinus(double y);
};

#endif