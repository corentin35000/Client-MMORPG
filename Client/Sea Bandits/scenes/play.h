#ifndef PLAY_H_
#define PLAY_H_

#include "gamestate.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stack>
#include <set>
#include <queue>
#include <string>
#include <cmath>

#include "../map.h"
#include "../unit.h"
#include "../engine.h"


struct cell
{
	int parent_i, parent_j;
	int dir;
	double f, g, h;
};

struct PathInfo
{
	int x, y, dir;
	PathInfo(int xx = 0, int yy = 0, int dd = 0) :x(xx), y(yy), dir(dd) {};
};

class Play : public Gamestate
{
	private:
		int dx[8] = { -1, 1, 1, -1, -1, 1, 1, -1 };
		int dy[8] = { 0, 0, -1, -1, 1, 1, 0, 0 };


		// Mettre dans map.h
		std::set<AnimationAmmo*> g_animationsAmmo;
		ALLEGRO_BITMAP* imgAmmoMunitions[1];


		int timerCooldownAttack;
		bool stopAttack;
		Unit* target;
		Unit* targetAttack;
		Unit* player;
		Point currentPos, targetPos;
		bool m_isMoving;
		int m_left, m_dir;
		string pseudo;
		ALLEGRO_BITMAP* ponts;
		ALLEGRO_BITMAP* rangPVE;
		ALLEGRO_BITMAP* rangPVP;
		string nameGuild;
		int distanceVision;
		bool cursorOnTheTarget; // Si le curseur est sur un NPC / Enemy / Tower ne pas déplacer le navire si == true
		int rangeCannon;
		bool attack;
		int doubleClickAttack;
		int degats;


		std::vector<ALLEGRO_FONT*> font;
		ALLEGRO_DISPLAY* display;
		int* nextState;
		int screenHeight, screenWidth;
		int* charPressed;
		int* mouseX, * mouseY, * mouseB, * mouseW, * mouseZ;
		int valueMouseZ;
		int valueMouseW;


		cell cellDetails[MAP_HEIGHT][MAP_WIDTH];


		std::stack<PathInfo> Path;


		typedef std::pair<int, int> Pair;
		typedef std::pair<double, std::pair<int, int> > pPair;


		// Coords 01 - 60 / AA - CH
		int nombreCoordsTotalHorizontal; // Le nombre de coordonnées en largeur
		int nombreCoordsTotalVertical; // Le nombre de coordonnées en hauteur 
		int pointCoordsHorizontal; // Le nombre de pixels entre chaque point de cordonnées
		int pointCoordsVertical; // Le nombre de pixels entre chaque point de cordonnées
		string pointsCoordsXScrolling[59] = {
			"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",
			"27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52",
			"53", "54", "55", "56", "57", "58", "59"
		};
		string pointsCoordsYScrolling[59] = {
			"AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN", "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX", "AY", "AZ",
			"BA", "BB", "BC", "BD", "BE", "BF", "BG", "BH", "BI", "BJ", "BK", "BL", "BM", "BN", "BO", "BP", "BQ", "BR", "BS", "BT", "BU", "BV", "BW", "BX", "BY", "BZ",
			"CA", "CB", "CC", "CD", "CE", "CF", "CG"
		};


		// Affiche les cordonnées du navire et de type -> (exemple : 58 - BA) par rapport a la position x,y du navire. 
		int distanceRestantWidth; // Récupère la distance qui reste jusqu'a parcourir jusqu'a la fin de la map en LARGEUR.
		int tt;
		int pointCoordsXApproximatif;
		int indexCoordsX;

		int distanceRestantHeight;// Récupère la distance qui reste jusqu'a parcourir jusqu'a la fin de la map en Hauteur.
		int tt2;
		int pointCoordsYApproximatif;
		int indexCoordsY;


		// Affichage barre de Scrolling + Affichage rectangle dans les angles.
		float alphaScrollingNoClick;
		float alphaScrollingClickContinue;
		ALLEGRO_COLOR colorScrollingNoClick;
		ALLEGRO_COLOR colorScrollingClickContinue;
		int widthScrolling;
		int heightScrolling;
		int rectAngleWidthHeight;


		// Savoir quel côté est cliquer du Scrolling (haut, droite, bas, gauche, haut droite, bas droitre, bas gauche, haut gauche)
		bool clickContinueScrolling;
		string stateClickCoterScrolling; // recupère l'état sur la qu'elle la souris est enfoncée avec le click sur le scrolling


		// Boolean savoir si il a cliquer sur la map et récupère la tile sur la qu'elle on'as cliquer pour afficher un effet lors du click 
		float alphaClickTileInMap;
		bool cursorClickTileInMap;
		Rect tileClicker;


		// GUI de la Mini Map 
		ALLEGRO_BITMAP* minimap;
		int minimapPositionX;
		int minimapPositionY;
		// GUI de l'interieur de la minimap. 
		int minimapInterieurPositionX;
		int minimapInterieurPositionY;
		int minimapInterieurWidth;
		int minimapInterieurHeight;
		int widthPixelsEntity;
		int	heightPixelsEntity;


		// Barre de jeu (btn centership)
		ALLEGRO_BITMAP* btnCenterShip;
		int btnCenterShipWidth;
		int btnCenterShipHeight;
		int btnCenterShipPositionX;
		int btnCenterShipPositionY;
		bool centerShipContinue;

		Map* map;

		bool hold_bitmap_drawing;
	public:
		Play(int& nextState, ALLEGRO_DISPLAY*& display, int& screenWidth, int& screenHeight, std::vector<ALLEGRO_FONT*>& font, int& charPressed, int& mouseX, int& mouseY, int& mouseZ, int& mouseW, int& mouseB);
		~Play();

	public:
		void Load();
		void Update(double dt);
		void Process(bool key[MAX_KEY]);
		void Draw();

		void aStarSearch(Pair src, Pair dest);
		bool isValid(int row, int col);
		bool isUnBlocked(int row, int col);

		bool isDestination(int row, int col, Pair dest);

		void tracePath(Pair dest);

		double calculateHValue(int row, int col, Pair dest);

		void changeSpriteForAttackToAngle(int x1, int y1, int x2, int y2);
		void centerShip();
		void moveTheCameraToScrolling();
		void moveMiniMap();
		void clickOnTheMapAndMove();
		bool checkRangeCannon(int x1, int y1, int x2, int y2);
};

#endif