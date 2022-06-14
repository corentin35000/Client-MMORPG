#ifndef UNIT_H_
#define UNIT_H_

#include <iostream>
#include <string>
#include <cmath>

#include <allegro5/allegro.h>

#include "common.h"

enum class AmmoType {
	ILLUMINER,
	CITROUILLE,
	CONFETIS
};

struct Position
{
	int x;
	int y;
	Position(int xx = 0, int yy = 0) : x(xx), y(yy) {};
};

enum class UnitType
{
	Player,
	NPC,
	Enemy,
	Monster,
	Tower
};

enum class Direction {
	//Northwest becomes down right
	//Southwest becomes down left
	//Southleft becomes Up left
	//Northleft becomes Up right
	en_northwest = 1, en_southeast = 5, en_southwest = 3, en_northeast = 7


	/*en_north = 0, en_northeast = 1, en_east = 2, en_southeast = 3,
	en_south = 4, en_southwest = 5, en_west = 6, en_northwest = 7*/
};

enum class State { stopped, moving };

enum class NPC_Ship { red_korsar_1, red_korsar_2, red_korsar_3, wild_13_1, wild_13_2, wild_13_3 };

enum Size { LARGE, MEDIUM, SMALL };

class Unit
{
	public:
		Unit();
		~Unit();

		ALLEGRO_BITMAP* spritesheet;
		ALLEGRO_BITMAP* ring;
		UnitType type;
		Position pos;
		Direction direction;
		State state;

		std::string name;
		int size, sizeX, sizeY, startX, offsetX, offsetY;
		int spriteWidth, spriteHeight;
		float spriteOpacity;
		int frame;
		int tileW, tileH;
		float x, y;
		bool inMotion;
		unsigned char speed;
		long hp;
		long hpMax;

	public:
		void Update();
		void Draw(int cameraX, int cameraY) const;
		void setPosition(int xx, int yy);
		void setTileSize(int w, int h);
		void setFrameCount(int f) { frame = f; }
		void Stop() {
			state = State::stopped;
		}
		void Move(Direction d) {
			state = State::moving;
			direction = d;
		}
		double xPos() { return x; }
		double yPos() { return y; }
		void animationAttack(int posXAttack, int posYAttack, int posX, int posY, AmmoType typeAmmoMunition, int degats);
};

#endif