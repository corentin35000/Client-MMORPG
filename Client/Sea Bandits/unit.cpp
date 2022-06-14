#include "unit.h"

Unit::Unit() {
    spritesheet = al_load_bitmap("assets/ships/Player/1-pinnace/pinnace.png");
    ring = al_load_bitmap("assets/misc/fx/rings.png");

    spriteWidth = (al_get_bitmap_width(spritesheet) / 8);
    spriteHeight = al_get_bitmap_height(spritesheet);
    spriteOpacity = 0.0f;

    type = UnitType::Enemy;
    pos = Position(2, 2);
    direction = Direction::en_southwest;
    state = State::stopped;

    name = "";
    size = 0;
    frame = 0;
    tileW = 0, tileH = 0;
    x = 0, y = 0;
    inMotion = false;
    hpMax = 75000;
    hp = 20000;
}

Unit::~Unit() {
    al_destroy_bitmap(spritesheet);
}

void Unit::Update() 
{
    if (state == State::stopped)
        return;

    double xDelta = tileW / 2.0 / frame;
    double yDelta = tileH / 2.0 / frame;
    switch (direction) 
    {
        case Direction::en_northwest:
            x += xDelta;
            y -= yDelta;
            break;
        case Direction::en_northeast:
            x -= xDelta;
            y -= yDelta;
            break;
        case Direction::en_southwest:
            x += xDelta;
            y += yDelta;
            break;
        case Direction::en_southeast:
            x -= xDelta;
            y += yDelta;
            break;
        default:
            break;
    }

    switch (size) 
    {
        case SMALL:
            startX = 347;
            sizeX = 83;
            sizeY = 53;
            break;
        case MEDIUM:
            startX = 226;
            sizeX = 119;
            sizeY = 73;
            offsetX = sizeX / 5;
            offsetY = sizeY / 5;
            break;
        case LARGE:
            startX = 0;
            sizeX = 223;
            sizeY = 152;
            offsetX = sizeX / 3;
            offsetY = sizeY / 3;
            break;
    }
}

void Unit::Draw(int cameraX, int cameraY) const 
{
    // Si les HP actuel sont inférieur a 40% des HP du navire total alors afficher le bateau avec les voiles. SINON avec les voiles baissée
    if (hp > ((40 * hpMax) / 100))
        al_draw_tinted_bitmap_region(spritesheet, al_map_rgba_f(1, 1, 1, spriteOpacity), ((int)direction / 2) * spriteWidth, 0, spriteWidth, spriteHeight, x - 29 - cameraX, y - 33 - cameraY, 0);
    else
        al_draw_tinted_bitmap_region(spritesheet, al_map_rgba_f(1, 1, 1, spriteOpacity), ((int)direction / 2 + 4) * spriteWidth, 0, spriteWidth, spriteHeight, x - 29 - cameraX, y - 33 - cameraY, 0);
}

void Unit::setPosition(int xx, int yy)
{
    x = xx;
    y = yy;
}

void Unit::setTileSize(int w, int h)
{
    tileW = w;
    tileH = h;
}

void Unit::animationAttack(int posXAttack, int posYAttack, int posX, int posY, AmmoType typeAmmoMunition, int degats)
{
    //spritesheet = al_load_bitmap("assets/ammo_munitions/illuminer.png");
}