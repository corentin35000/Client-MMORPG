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


class DesignBoat
{
	string name;
	string pathImgSpriteSheet;
	int imgSpriteSheet;
	int imgCurrentAnimation;

	DesignBoat()
	{

	}
};

class Quest
{

};

class NPC
{
	string nameNPC;
	string designBoat;
	int mapDisponible;
	int HP;
	int recompenseOR;
	int recompensePERLE;
	int recompensePE;

	NPC()
	{
		//NPC::NPC();
	}
};

class Monster 
{
	string nameMonster;
	string designBoat;
	int mapDisponible;
	int HP;
	int recompenseOR;

	Monster()
	{

	}
};

class Cannon
{
	string nameCannon;
	int hitProbality;
	int range;
	int couldownFire;
	int price;
};

class CannonBall
{
	string nameCannonBall;
	int damage;
	string bonus;
	int priceOR;
	int pricePERLE;
};

class Item
{
	string nameItem;
	int damage;
	int range;
	int couldownBonus;
	int speedBonus;
	int durationBonus;
	int hitProbality;
};

class PlayerMessageChat
{	
	int idBDD;
	string message;
};

class Chat
{
	int width;
	int height;
	int positionX;
	int positionY;
	vector<string> messageAll;
};

class Player 
{     
  public:
	int idBDD;
	string currentServer; // player connected for server (FR, EN..)
    string pseudo; // pseudo player for inscription in BDD.
	ENetPeer *peer; // ref client for peer ENET.
	int ping; // Ping du jour en : ms (millisecondes)
	boolean visible;
	int speed;
	int HP;
	int PE;
	int level;
	int badgeAnneeJeu;
	int badgePVP;
	int badgePVE;
	int currentMap;
	int positionX;
	int positionY;
	vector<Quest> quest;
	vector<DesignBoat> designBoat; // Contient tout les design(bateau) du joueur
	vector<Cannon> cannon; // Contient tout les cannons du joueur
	vector<CannonBall> cannonball; // Contient tout les boulets de cannon du joueur
	vector<Item> items; // Contient tout les items du joueur
	vector<Cannon> cannonEquiper; // Contient les cannons du joueur qui est équiper actuellement sur le bateau
	CannonBall cannonballEquiper; // Boulet de cannon qui est selectionner actuellement sur le bateau
	int range;
	int hitProbality;
	int couldownFire = 0;
	boolean attack = false;

	Player(string currentServer2, string pseudo2, ENetPeer *peer2, boolean visible2, int speed2, int HP2, int PE2, int level2, int badgeAnneeJeu2, int badgePVP2, int badgePVE2, int currentMap2, int positionX2, int positionY2, string designBoat2)
	{
		currentServer = currentServer2;
		pseudo = pseudo2;
		peer = peer2;
		visible = visible2;
		speed = speed2;
		HP = HP2;
		PE = PE2;
		level = level2;
		badgeAnneeJeu = badgeAnneeJeu2;
		badgePVP = badgePVP2;
		badgePVE = badgePVE2;
		currentMap = currentMap2;
		positionX = positionX2;
		positionY = positionY2;
		//designBoat = designBoat2;

		printf("Object Player creer. \n");
	}

	void test() 
	{

	}
};