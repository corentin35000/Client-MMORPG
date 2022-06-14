#include "common.h"

int keyPressed;

Point tileCenter(int row, int column)
{
	Point ret;
	ret.x = (row % 2) ? 0 : TILE_WIDTH / 2;
	ret.y = TILE_HEIGHT * (row + 1) / 2;
	ret.x += column * TILE_WIDTH;

	return ret;
}

// Récupère la positionX et Y de la souris par exemple lors d'un click pour récupère la Tile cliquer
Point getTile(int x, int y) 
{
	for (int row = 0; row <= NOMBRE_LIGNE; row++)
		for (int column = 0; column <= NOMBRE_COLONNE; column++)
		{
			Point center = tileCenter(row, column);
			int dx = abs(x - center.x);
			int dy = abs(y - center.y);

			if (dx / (TILE_WIDTH * 0.5) + dy / (TILE_HEIGHT * 0.5) <= 1)
			{
				return Point(row, column);
			}
		}

	return Point(-1, -1);
};

Rect getTileRect(int row, int column, Camera camera)
{
	Point center = tileCenter(row, column);

	Rect ret;
	ret.polygon[0] = center.x - camera.x - (TILE_WIDTH / 2);
	ret.polygon[1] = center.y - camera.y;
	ret.polygon[6] = center.x - camera.x;
	ret.polygon[7] = center.y - camera.y - (TILE_HEIGHT / 2);
	ret.polygon[4] = center.x - camera.x + (TILE_WIDTH / 2);
	ret.polygon[5] = center.y - camera.y;
	ret.polygon[2] = center.x - camera.x;
	ret.polygon[3] = center.y - camera.y + (TILE_HEIGHT / 2);

	return ret;
}