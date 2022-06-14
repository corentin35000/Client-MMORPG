#include <stdlib.h>
#include <string>
#include "common.h"
using namespace std;


struct AABB
{
    int x;
    int y;
    int w;
    int h;
};

class SystemCoords
{
    public:
        bool clickContinueScrolling; // Return si il click en continue
        string stateClickCoterScrolling; // Renvoie si il click à gauche, droite, en haut a droite..etc
        float pointCoordsHorizontal = NOMBRE_COLONNE / 60;
        float pointCoordsVertical = NOMBRE_LIGNE / 120;

        SystemCoords();

	public:
        void Load();
        void Update();
        void Draw();
};
