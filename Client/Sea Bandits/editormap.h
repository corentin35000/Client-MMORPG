#ifndef EDITORMAP_H_
#define EDITORMAP_H

#include "./scenes/gamestate.h"
#include "common.h"
#include "map.h"

enum OrientationTilesGrille {
	ORTHOGONALE,
	ISOMETRIC
};

class EditorMap : public Gamestate
{
	private:
		std::vector<ALLEGRO_FONT*> font;
		int* nextState;
		int screenHeight, screenWidth;
		int* charPressed;
		int* mouseX, * mouseY, * mouseB, * mouseW, * mouseZ;

		std::vector<ALLEGRO_BITMAP*> membitmap;
		std::vector<ALLEGRO_BITMAP*> bitmap;

		ALLEGRO_COLOR backgroundColorEditorMap;

		int stateOrientationTiles;
		int mapLigne;
		int mapColonne;
		int mapNiveau;
		std::string map_widthText;
		std::string map_heightText;
		std::string tile_widthText;
		std::string tile_heightText;
		std::string map_pixelsText;
		std::string map_ligneText;
		std::string map_colonneText;
		std::string map_niveau_currentText;

		vector<Tile> tileMaps; // Tableau a 3 dimensions qui contiendras tout les niveaux de l'editeur de map.

	public:
		EditorMap(int& nextState, int& screenWidth, int& screenHeight, std::vector<ALLEGRO_FONT*>& font, int& charPressed, int& mouseX, int& mouseY, int& mouseZ, int& mouseW, int& mouseB);
		~EditorMap();

	public:
		void Load();
		void Update(double dt);
		void Process(bool key[MAX_KEY]);
		void Draw();

};

#endif