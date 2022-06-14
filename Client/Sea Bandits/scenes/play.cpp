#include "play.h"

Play::Play(int& nextState, ALLEGRO_DISPLAY*& display, int& screenWidth, int& screenHeight, std::vector<ALLEGRO_FONT*>& font, int& charPressed, int& mouseX, int& mouseY, int& mouseZ, int& mouseW, int& mouseB)
{
	this->nextState = &nextState;
	this->display = display;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->font = font;
	this->charPressed = &charPressed;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->mouseZ = &mouseZ;
	this->mouseW = &mouseW;
	this->valueMouseZ = 0;
	this->valueMouseW = 0;

	Play::Load();
}

Play::~Play() {}

void Play::Load()
{
	// Permet de pouvoir jouer avec des bitmaps transparent avec la fonction : al_draw_tinted_bitmap()
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);


	// Load la map
	map = new Map();
	map->load();


	// Insert les unités dans le tableau a deux dimensions.
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (x == 50 && y == 50)
			{
				player = map->spawn_unit(UnitType::Player, { x, y }, MEDIUM);
				player->setTileSize(TILE_WIDTH, TILE_HEIGHT);
				player->setFrameCount(10);
				player->direction = Direction::en_southeast;
	
				Point cur = tileCenter(player->pos.x, player->pos.y);
				player->setPosition(cur.x, cur.y);
				map->tileMap[y][x].occupying_unit = player;
			}
			else if ((rand() % 600) == 0 && x <= 150 && y <= 80)
			{
				Unit* npc = map->spawn_unit(NPC_Ship::red_korsar_1, UnitType::NPC, { x, y }, SMALL);
				npc->setTileSize(TILE_WIDTH, TILE_HEIGHT);
				npc->setFrameCount(10);

				Point cur = tileCenter(npc->pos.x, npc->pos.y);
				npc->setPosition(cur.x, cur.y);
				map->tileMap[y][x].occupying_unit = npc;
			}
		}
	}


	/*** MiniMap ***/
	// Load l'image de la MiniMap.
	minimap = al_load_bitmap("assets/misc/minimap/minimap.png");
	// GUI de la Mini Map 
	minimapPositionX = screenWidth - al_get_bitmap_width(minimap) - 30;
	minimapPositionY = 30;
	// GUI de l'interieur de la minimap. 
	minimapInterieurPositionX = minimapPositionX + 28;
	minimapInterieurPositionY = minimapPositionY + 40;
	minimapInterieurWidth = 90;
	minimapInterieurHeight = 90;
	widthPixelsEntity = 2;
	heightPixelsEntity = 2;


	/*** Barre de jeu ***/
	// BtnCenterShip
	btnCenterShip = al_load_bitmap("assets/barredejeu/btncentermap.png");
	btnCenterShipWidth = al_get_bitmap_width(btnCenterShip);
	btnCenterShipHeight = al_get_bitmap_height(btnCenterShip);
	btnCenterShipPositionX = (screenWidth / 2 - (btnCenterShipWidth / 2));
	btnCenterShipPositionY = (screenHeight - 300 - (btnCenterShipHeight / 2));
	centerShipContinue = false;


	// Player : RangPVE, rangPVP, guild, pseudo
	m_isMoving = false;
	m_left = 0;
	rangPVE = al_load_bitmap("assets/rangPVE/1_rangpve.png");
	rangPVP = al_load_bitmap("assets/rangPVP/1_rangpvp.png");
	nameGuild = "[O$$]";
	ponts = al_load_bitmap("assets/ponts/pontselite.png");
	pseudo = "ShoutteurVEX";
	distanceVision = 2000;
	cursorOnTheTarget = false;
	attack = false;
	doubleClickAttack = 0;
	rangeCannon = 1000;
	timerCooldownAttack = 0;


	// Items
	imgAmmoMunitions[0] = al_load_bitmap("assets/ammo_munitions/spritesheet_confetis.png");


	// Coords (01 - 60 / AA - CH)
	nombreCoordsTotalHorizontal = 60; // Le nombre de coordonnées en largeur
	nombreCoordsTotalVertical = 60; // Le nombre de coordonnées en hauteur 
	pointCoordsHorizontal = NOMBRE_COLONNE / nombreCoordsTotalHorizontal; // Le nombre de pixels entre chaque point de cordonnées en largeur
	pointCoordsVertical = NOMBRE_LIGNE / nombreCoordsTotalVertical; // Le nombre de pixels entre chaque point de cordonnées en hauteur


	// Affichage barre de Scrolling + Affichage rectangle dans les angles.
	alphaScrollingNoClick = 51;
	alphaScrollingClickContinue = 25;
	colorScrollingNoClick = al_map_rgba(255, 255, 255, alphaScrollingNoClick);
	colorScrollingClickContinue = al_map_rgba(255, 255, 255, alphaScrollingClickContinue);
	widthScrolling = 20;
	heightScrolling = 40;
	rectAngleWidthHeight = 40;


	// Click in map
	alphaClickTileInMap = 0.8f;
	cursorClickTileInMap = false;


	// Savoir quel côté est cliquer du Scrolling (haut, droite, bas, gauche, haut droite, bas droitre, bas gauche, haut gauche)
	clickContinueScrolling = false;
	stateClickCoterScrolling = "";


	centerShip();
}




void Play::Update(double dt)
{
	// Dès que la personne a attaquer, timer du cooldown lancer.
	if (attack == true && timerCooldownAttack < 180) // 180 = 60 x 3 (pour 3 SECONDS)
		timerCooldownAttack++;
	else
		timerCooldownAttack = 0;


	// Attack en continue - Ajoute les tire d'animation en + dans la liste d'affichage des animations de boulets
	// Si le joueur est en pleine attaque je test si les cannons sont toujours dans la porter.
	// Si il dépasse la range des cannons, mettre attack a FALSE.
	if (attack == true && timerCooldownAttack == 0)
	{
		if (checkRangeCannon(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), targetAttack->x - 29 + (targetAttack->spriteWidth / 2), targetAttack->y - 33 + (targetAttack->spriteHeight / 2)))
		{
			if (m_left == 0)
				changeSpriteForAttackToAngle(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), targetAttack->x - 29 + (targetAttack->spriteWidth / 2), targetAttack->y - 33 + (targetAttack->spriteHeight / 2));
			
			cout << "ATTACK UPDATE \n";

			// Ajoute a la liste des animations de boulets pour l'update et draw.
			double angleRadians = map->mathAngleRadians(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), targetAttack->x - 29 + (targetAttack->spriteWidth / 2), targetAttack->y - 33 + (targetAttack->spriteHeight / 2));

			AnimationAmmo* animation = new AnimationAmmo;
			animation->cible = targetAttack;
			animation->x = player->x - 29 + (player->spriteWidth / 2);
			animation->y = player->y - 33 + (player->spriteHeight / 2);
			animation->vx = map->cosinus(angleRadians);
			animation->vy = map->sinus(angleRadians);

			g_animationsAmmo.insert(animation);
		}
		// Si dans les prochaine x seconds il n'es plus à côté pour tirer sur le navire, je stop l'attaque. 
		// Permet d'éviter quand on recule de trop de pas arrêter l'attaque pour rien si ont reviens rapidement avant le cooldown de la prochaine attaque
		else 
		{
			attack = false;
			targetAttack = nullptr;
			cout << "STOP ATTACK \n";
		}
	}


	// Update des animations de boulets.
	for (auto& animationAmmo : g_animationsAmmo)
	{
		if (map->mathDistance(animationAmmo->x, animationAmmo->y, animationAmmo->cible->x, animationAmmo->cible->y) <= 2)
		{
			//g_animationsAmmo.erase(animationAmmo);
			//cout << "ANIM FINISH";
			// ENLEVER DE LA LIST.
		}
		else
		{
			double angleRadians = map->mathAngleRadians(animationAmmo->x, animationAmmo->y, animationAmmo->cible->x, animationAmmo->cible->y);
			animationAmmo->vx = map->cosinus(angleRadians);
			animationAmmo->vy = map->sinus(angleRadians);

			animationAmmo->x += (animationAmmo->vx * 12);
			animationAmmo->y += (animationAmmo->vy * 12);
		}
	}


	// Centre le navire en continue, au centre de l'écran.
	if (centerShipContinue == true)
	{
		centerShip();
	}


	// MOUSE MOLETTE Z/W : On sélectionne le bateau le plus près du curseur, même si le curseur n'est pas sur le bateau.
	if (*mouseZ != valueMouseZ || *mouseW != valueMouseW)
	{
		valueMouseZ = *mouseZ;
		valueMouseW = *mouseW;

		// Récupère les NPCs / Enemy dans le champ de vision puis récupère le bateau le plus proche du curseur à l'écran.
		double distanceUnitToPlayer;
		double distanceUnitToPlayerCurrent;

		int i = 0;
		for (auto& unit : map->g_units)
		{
			if (unit->type == UnitType::NPC || unit->type == UnitType::Enemy)
			{
				if (map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), unit->x - 29 + (unit->spriteWidth / 2), unit->y - 33 + (unit->spriteHeight / 2), distanceVision))
				{
					distanceUnitToPlayerCurrent = map->mathDistance(*mouseX + map->camera.x, *mouseY + map->camera.y, unit->x - 29 + (unit->spriteWidth / 2), unit->y - 33 + (unit->spriteHeight / 2));

					if (i == 0)
					{
						distanceUnitToPlayer = distanceUnitToPlayerCurrent;
						target = unit;
					}
					else
					{
						if (distanceUnitToPlayer > distanceUnitToPlayerCurrent)
						{
							target = unit;
							distanceUnitToPlayer = distanceUnitToPlayerCurrent;
						}
					}

					i++;
				}
			}
		}
	}


	// Click Left
	if (*mouseB & 1) 
	{
		// Selectionne le navire (target) : CLICK LEFT
		for (auto &unit : map->g_units)
		{
			if (unit->type == UnitType::NPC || unit->type == UnitType::Enemy)
			{
				if (*mouseX + map->camera.x >= unit->x - 29 && *mouseX + map->camera.x <= unit->x - 29 + unit->spriteWidth &&
					*mouseY + map->camera.y >= unit->y - 29 && *mouseY + map->camera.y <= unit->y - 29 + unit->spriteHeight &&
					map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), unit->x - 29 + (unit->spriteWidth / 2), unit->y - 33 + (unit->spriteHeight / 2), distanceVision))
				{
					target = unit;
					break;
				}
			}
		}


		// Si il y a déjà un navire selectionner, si il y a un double click dessus alors il attack.
		/*if(target)
		{
			if (*mouseX + map->camera.x >= target->x - 29 && *mouseX + map->camera.x <= target->x - 29 + target->spriteWidth &&
				*mouseY + map->camera.y >= target->y - 29 && *mouseY + map->camera.y <= target->y - 29 + target->spriteHeight &&
				map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), target->x - 29 + (target->spriteWidth / 2), target->y - 33 + (target->spriteHeight / 2), distanceVision))
			{
				doubleClickAttack++;

				if (doubleClickAttack == 2)
				{
					attack = true;
					cout << "DOUBLE CLICK";
				}
			}
			else
			{
				doubleClickAttack = 0;
			}
		}*/


		// Click in minimap and move
		if (*mouseX >= minimapInterieurPositionX && *mouseX <= minimapInterieurPositionX + minimapInterieurWidth && *mouseY >= minimapInterieurPositionY && *mouseY <= minimapInterieurPositionY + minimapInterieurHeight)
		{
			moveMiniMap();
		}
		// Click btnCenterShip
		else if (*mouseX >= btnCenterShipPositionX && *mouseX <= btnCenterShipPositionX + btnCenterShipWidth && *mouseY >= btnCenterShipPositionY && *mouseY <= btnCenterShipPositionY + btnCenterShipHeight)
		{
			centerShipContinue = true;
		}
		// Click sur les barre de Scrolling pour déplacer la caméra.
		else if (*mouseX >= 0 && *mouseX <= 20 && *mouseY >= 40 && *mouseY <= screenHeight - 40) // Gauche
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "left";

			if (map->camera.x <= 0)
				map->camera.x = 0;
			else
				map->camera.x -= 20;
		}
		else if (*mouseX >= 40 && *mouseX <= screenWidth - 40 && *mouseY >= screenHeight - 20 && *mouseY <= screenHeight) // Bas
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "down";

			if (map->camera.y >= NOMBRE_LIGNE - screenHeight)
				map->camera.y = NOMBRE_LIGNE - screenHeight;
			else
				map->camera.y += 20;
		}
		else if (*mouseX >= screenWidth - 20 && *mouseX <= screenWidth && *mouseY >= 0 + 40 && *mouseY <= screenHeight - 40) // Droite
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "right";

			if (map->camera.x >= NOMBRE_COLONNE - screenWidth)
				map->camera.x = NOMBRE_COLONNE - screenWidth;
			else
				map->camera.x += 20;
		}
		else if (*mouseX >= 40 && *mouseX <= screenWidth - 40 && *mouseY >= 0 && *mouseY <= 20) // Haut
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "up";

			if (map->camera.y <= 0)
				map->camera.y = 0;
			else
				map->camera.y -= 20;
		}
		else if (*mouseX >= 0 && *mouseX <= 40 && *mouseY >= 0 && *mouseY <= 40) // HAUT GAUCHE
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "up-left";

			// Gauche
			if (map->camera.x <= 0)
				map->camera.x = 0;
			else
				map->camera.x -= 20;

			// Haut
			if (map->camera.y <= 0)
				map->camera.y = 0;
			else
				map->camera.y -= 20;
		}
		else if (*mouseX >= 0 && *mouseX <= 40 && *mouseY >= screenHeight - 40 && *mouseY <= screenHeight) // BAS GAUCHE
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "down-left";

			// Bas
			if (map->camera.y >= NOMBRE_LIGNE - screenHeight)
				map->camera.y = NOMBRE_LIGNE - screenHeight;
			else
				map->camera.y += 20;

			// Gauche
			if (map->camera.x <= 0)
				map->camera.x = 0;
			else
				map->camera.x -= 20;
		}
		else if (*mouseX >= screenWidth - 40 && *mouseX <= screenWidth && *mouseY >= screenHeight - 40 && *mouseY <= screenHeight) // BAS DROITE
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "down-right";

			// Bas
			if (map->camera.y >= NOMBRE_LIGNE - screenHeight)
				map->camera.y = NOMBRE_LIGNE - screenHeight;
			else
				map->camera.y += 20;

			// Droite
			if (map->camera.x >= NOMBRE_COLONNE - screenWidth)
				map->camera.x = NOMBRE_COLONNE - screenWidth;
			else
				map->camera.x += 20;
		}
		else if (*mouseX >= screenWidth - 40 && *mouseX <= screenWidth && *mouseY >= 0 && *mouseY <= 40) // HAUT DROITE
		{
			centerShipContinue = false;

			clickContinueScrolling = true;
			stateClickCoterScrolling = "up-right";

			// Haut
			if (map->camera.y <= 0)
				map->camera.y = 0;
			else
				map->camera.y -= 20;

			// Droite
			if (map->camera.x >= NOMBRE_COLONNE - screenWidth)
				map->camera.x = NOMBRE_COLONNE - screenWidth;
			else
				map->camera.x += 20;
		}
		// Click in map to movement
		else
		{
			if (cursorOnTheTarget == false)
				clickOnTheMapAndMove();
		}
	}
	else
	{
		clickContinueScrolling = false;
	}	


	//
	for (auto& unit : map->g_units)
	{
		if (unit->type == UnitType::Player) 
		{
			if (!m_isMoving) 
			{
				if (m_left)
				{
					map->tileMap[unit->pos.y][unit->pos.x].occupying_unit = nullptr;
					unit->Move((Direction)m_dir);
					m_left--;
					unit->Update();
					map->tileMap[unit->pos.y][unit->pos.x].occupying_unit = unit;
				}
				else
				{
					if (!Path.empty())
					{
						PathInfo p = Path.top();
						Path.pop();
						Point cen = tileCenter(p.x, p.y);
						Rect tile = getTileRect(p.x, p.y, map->camera);
						m_isMoving = true;

						if (p.dir != -1) 
						{
							m_left = 10;
							m_dir = p.dir;
						}

						m_isMoving = false;

						for (auto& unit : map->g_units)
						{
							if (unit->type == UnitType::Player)
								unit->pos = Position(p.x, p.y);
						}
					}
				}
			}
		}
		else if (unit->type == UnitType::NPC)
		{
			// Test
			unit->x += 0.2;			
			unit->y += 0.2;
		}
		else if(unit->type == UnitType::Enemy) 
		{

		}
	}
}




void Play::Draw()
{
	// Eviter les déchirure d'écran.
	al_clear_to_color(al_map_rgb_f(0.0, 0.0, 0.0));


	// Cela permet un dessin efficace pour de nombreux bitmaps qui partagent un bitmap parent, tels que des sous-bitmaps d'une feuille de tuiles
	if (hold_bitmap_drawing) 
	{
		al_hold_bitmap_drawing(true);
	}


	// Affichage de la map + Circle Fog
	map->draw();
	map->circleFogOfWar(player->x - 29 - map->camera.x + (player->spriteWidth / 2), player->y - 33 - map->camera.y + (player->spriteHeight / 2), distanceVision);


	// Affichage des cercle vert du player
	al_draw_bitmap_region(player->ring, player->startX, 0, player->sizeX, player->sizeY, player->x + ((al_get_bitmap_width(player->spritesheet) / 8) / 2) - 70 - map->camera.x - player->offsetX, player->y + 20 + (al_get_bitmap_height(player->spritesheet) / 8) - map->camera.y - player->offsetY, 0);

	// Mettre une opacity au rond jaune
	// Affichage un cercle jaune autour du bateau qui à était selectionner (la target) (Mouse molette ou click gauche)
	if (target && map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), target->x - 29 + (target->spriteWidth / 2), target->y - 33 + (target->spriteHeight / 2), distanceVision))
		al_draw_bitmap_region(target->ring, target->startX, 460, target->sizeX, target->sizeY, target->x - al_get_bitmap_width(target->spritesheet) / 8 - map->camera.x + 20, target->y - al_get_bitmap_height(target->spritesheet) - map->camera.y + 30, 0);


	// Affichage des : NPCs / ENEMIES / TOWERS / MONSTER / SCINTILLE.
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			Tile& tile = map->tileMap[y][x];

			if (tile.occupying_unit)
			{
				Unit& unit = *tile.occupying_unit;

				// Affichage + Opaciter lors du spawn, opacité quand il sors du champ de vision ou rentre dans le champ de vision
				if (map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), unit.x - 29 + (unit.spriteWidth / 2), unit.y - 33 + (unit.spriteHeight / 2), distanceVision))
				{
					if (unit.spriteOpacity < 1.0f)
					{
						unit.spriteOpacity += 0.01f;
					}
					else
					{
						unit.spriteOpacity = 1.0f;
					}

					unit.Draw(map->camera.x, map->camera.y);
				}
				else
				{
					if (unit.spriteOpacity > 0.0f)
					{
						unit.spriteOpacity -= 0.01f;
						unit.Draw(map->camera.x, map->camera.y);
					}
					else
					{
						unit.spriteOpacity = 0.0f;
					}
				}
			}
		}
	}


	// Affichage des animations de boulets.
	for (auto& animation_ammo : g_animationsAmmo)
	{
		if (animation_ammo->frame == 0)
		{
			al_draw_bitmap_region(imgAmmoMunitions[0], 0, 0, 17, 17, animation_ammo->x - map->camera.x, animation_ammo->y - map->camera.y, 0); // Couleur de l'interieur de la minimap.
			animation_ammo->frame++;
		}
		else if (animation_ammo->frame == 4)
		{
			al_draw_bitmap_region(imgAmmoMunitions[0], animation_ammo->frame + 17, 0, 17, 17, animation_ammo->x - map->camera.x, animation_ammo->y - map->camera.y, 0); // Couleur de l'interieur de la minimap.
			animation_ammo->frame = 0;
		}
		else
		{
			al_draw_bitmap_region(imgAmmoMunitions[0], animation_ammo->frame + 17, 0, 17, 17, animation_ammo->x - map->camera.x, animation_ammo->y - map->camera.y, 0); // Couleur de l'interieur de la minimap.
			animation_ammo->frame++;
		}
	}


	// Affichage d'un nouveau curseur de souris lors du survol au-dessus d'un NPCs or Enemy
	for (auto& unit : map->g_units)
	{
		if (unit->type == UnitType::NPC || unit->type == UnitType::Enemy)
		{
			if (*mouseX + map->camera.x >= unit->x - 29 && *mouseX + map->camera.x <= unit->x - 29 + unit->spriteWidth &&
				*mouseY + map->camera.y >= unit->y - 29 && *mouseY + map->camera.y <= unit->y - 29 + unit->spriteHeight && 
				 map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), unit->x - 29 + (unit->spriteWidth / 2), unit->y - 33 + (unit->spriteHeight / 2), distanceVision)
				)
			{
				al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
				cursorOnTheTarget = true;
				break;
			}
			else
			{
				al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
				cursorOnTheTarget = false;
			}
		}
	}


	// Affichage du polygon lors du click sur la map.
	if (cursorClickTileInMap == true)
	{
		al_draw_filled_polygon(tileClicker.polygon, 4, al_map_rgba_f(1.0f * alphaClickTileInMap, 1.0f * alphaClickTileInMap, 1.0f * alphaClickTileInMap, alphaClickTileInMap));
		cursorClickTileInMap = false;
	}


	// Barre HP, pseudo, name guilds, rangPVE, rangPVP..
	al_draw_text(font[1], al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f), player->x - map->camera.x - 5, player->y + 100 - map->camera.y, 0, (char*)pseudo.c_str()); // Pseudo (20 caracter max)
	al_draw_text(font[1], al_map_rgba(230, 213, 11, 255), player->x - map->camera.x - 45, player->y + 100 - map->camera.y, 0, (char*)nameGuild.c_str()); // Guild
	al_draw_filled_rectangle(player->x - 30 - map->camera.x, player->y + 120 - map->camera.y, player->x - 50 + 150 - map->camera.x, player->y + 120 + 2 - map->camera.y, al_map_rgba(0, 148, 3, 255)); // Barre de HP.
	al_draw_bitmap(rangPVE, player->x - map->camera.x - 70, player->y + 100 - map->camera.y, 0); // RangPVE
	al_draw_bitmap(rangPVP, player->x - map->camera.x + 100, player->y + 97 - map->camera.y, 0); // RangPVP
	al_draw_bitmap(ponts, player->x - map->camera.x + 15, player->y + 130 - map->camera.y, 0); // Ponts gold and elite


	// GUI de la Mini Map 
	al_draw_bitmap(minimap, minimapPositionX, minimapPositionY, 0);

	// GUI de la couleur/image de l'interieur de la minimap. 
	al_draw_bitmap_region(map->mapsImg[0], 0, 0, minimapInterieurWidth, minimapInterieurHeight, minimapInterieurPositionX, minimapInterieurPositionY, 0); // Couleur de l'interieur de la minimap.

	// Affichage des entity graphiquement en pixels sur l'interieur de la minimap.
	for (auto& unit : map->g_units)
	{
		switch (unit->type)
		{
			case UnitType::Player:
				al_draw_filled_rectangle(minimapInterieurPositionX + ((player->x - 29 + (player->spriteWidth / 2) - (widthPixelsEntity / 2)) / minimapInterieurWidth), minimapInterieurPositionY + ((player->y - 33 + (player->spriteHeight / 2) - (heightPixelsEntity / 2)) / minimapInterieurHeight), minimapInterieurPositionX + ((player->x - 29 + player->spriteWidth / 2) / minimapInterieurWidth) + widthPixelsEntity, minimapInterieurPositionY + ((player->y - 33 + player->spriteHeight / 2) / minimapInterieurHeight) + heightPixelsEntity, al_map_rgba(0, 255, 0, 255));
				break;
			case UnitType::NPC:
				if (map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), unit->x - 29 + (unit->spriteWidth / 2), unit->y - 33 + (unit->spriteHeight / 2), distanceVision))
					al_draw_filled_rectangle(minimapInterieurPositionX + ((unit->x - unit->spriteWidth) / minimapInterieurWidth), minimapInterieurPositionY + ((unit->y - unit->spriteHeight) / minimapInterieurHeight), minimapInterieurPositionX + ((unit->x - unit->spriteWidth) / minimapInterieurWidth) + widthPixelsEntity, minimapInterieurPositionY + ((unit->y - unit->spriteHeight) / minimapInterieurHeight) + heightPixelsEntity, al_map_rgba(255, 153, 0, 255));
				break;
			case UnitType::Enemy:
				if (map->fogOfWar(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), unit->x - 29 + (unit->spriteWidth / 2), unit->y - 33 + (unit->spriteHeight / 2), distanceVision))
					al_draw_filled_rectangle(minimapInterieurPositionX + ((unit->x - unit->spriteWidth) / minimapInterieurWidth), minimapInterieurPositionY + ((unit->y - unit->spriteHeight) / minimapInterieurHeight), minimapInterieurPositionX + ((unit->x - unit->spriteWidth) / minimapInterieurWidth) + widthPixelsEntity, minimapInterieurPositionY + ((unit->y - unit->spriteHeight) / minimapInterieurHeight) + heightPixelsEntity, al_map_rgba(255, 13, 0, 255));
				break;

				//al_draw_filled_rectangle(100, 100, 102, 102, al_map_rgba(222, 255, 0, 255)); // Scintille
		}
	}

	// Rectangle de la MiniMap
	al_draw_rectangle(minimapInterieurPositionX + (map->camera.x / minimapInterieurWidth), minimapInterieurPositionY + (map->camera.y / minimapInterieurHeight), minimapInterieurPositionX + ((map->camera.x + screenWidth) / minimapInterieurWidth), minimapInterieurPositionY + ((map->camera.y + screenHeight) / minimapInterieurHeight), al_map_rgba(255, 255, 255, 255), 0);


	// Barre de jeu
	al_draw_bitmap(btnCenterShip, btnCenterShipPositionX, btnCenterShipPositionY, 0); // btnCenterShip


	// Affiche les cordonnées du navire et de type -> (exemple : 58 - BA) par rapport a la position x,y du navire. 
	al_draw_text(font[0], al_map_rgba(255, 13, 0, 255), (screenWidth / 2), (screenHeight / 2), 0, (char*)pointsCoordsXScrolling[indexCoordsX].c_str());
	al_draw_text(font[0], al_map_rgba(255, 13, 0, 255), (screenWidth / 2) + 20, (screenHeight / 2), 0, (char*)pointsCoordsYScrolling[indexCoordsY].c_str());


	// Affichage barre de Scrolling + Affichage rectangle dans les angles.
	// Haut Gauche
	if (clickContinueScrolling == true && stateClickCoterScrolling == "up-left")
		al_draw_filled_rectangle(0, 0, rectAngleWidthHeight, rectAngleWidthHeight, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(0, 0, rectAngleWidthHeight, rectAngleWidthHeight, colorScrollingNoClick);

	// Bas Gauche
	if (clickContinueScrolling == true && stateClickCoterScrolling == "down-left")
		al_draw_filled_rectangle(0, screenHeight, rectAngleWidthHeight, screenHeight - rectAngleWidthHeight, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(0, screenHeight, rectAngleWidthHeight, screenHeight - rectAngleWidthHeight, colorScrollingNoClick);

	// Bas Droite
	if (clickContinueScrolling == true && stateClickCoterScrolling == "down-right")
		al_draw_filled_rectangle(screenWidth, screenHeight, screenWidth - rectAngleWidthHeight, screenHeight - rectAngleWidthHeight, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(screenWidth, screenHeight, screenWidth - rectAngleWidthHeight, screenHeight - rectAngleWidthHeight, colorScrollingNoClick);

	// Haut Droite
	if (clickContinueScrolling == true && stateClickCoterScrolling == "up-right")
		al_draw_filled_rectangle(screenWidth, 0, screenWidth - rectAngleWidthHeight, rectAngleWidthHeight, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(screenWidth, 0, screenWidth - rectAngleWidthHeight, rectAngleWidthHeight, colorScrollingNoClick);

	// Haut
	if (clickContinueScrolling == true && stateClickCoterScrolling == "up")
		al_draw_filled_rectangle(heightScrolling, 0, screenWidth - heightScrolling, widthScrolling, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(heightScrolling, 0, screenWidth - heightScrolling, widthScrolling, colorScrollingNoClick);

	// Gauche
	if (clickContinueScrolling == true && stateClickCoterScrolling == "left")
		al_draw_filled_rectangle(0, heightScrolling, widthScrolling, screenHeight - heightScrolling, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(0, heightScrolling, widthScrolling, screenHeight - heightScrolling, colorScrollingNoClick);

	// Bas
	if (clickContinueScrolling == true && stateClickCoterScrolling == "down")
		al_draw_filled_rectangle(heightScrolling, screenHeight, screenWidth - heightScrolling, screenHeight - widthScrolling, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(heightScrolling, screenHeight, screenWidth - heightScrolling, screenHeight - widthScrolling, colorScrollingNoClick);

	// Droite
	if (clickContinueScrolling == true && stateClickCoterScrolling == "right")
		al_draw_filled_rectangle(screenWidth, heightScrolling, screenWidth - widthScrolling, screenHeight - heightScrolling, colorScrollingClickContinue);
	else
		al_draw_filled_rectangle(screenWidth, heightScrolling, screenWidth - widthScrolling, screenHeight - heightScrolling, colorScrollingNoClick);


	// Affichage les cordonnées de 01-60 à AA-CH sur les barre de Scrolling
	int k = 0;
	for (float i = pointCoordsHorizontal; i < NOMBRE_COLONNE; i += pointCoordsHorizontal)
	{
		al_draw_text(font[0], al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f), i - map->camera.x, 2, 0, (char*)pointsCoordsXScrolling[k].c_str());
		k++;
	}
	
	k = 0;
	for (float i = pointCoordsVertical; i < NOMBRE_LIGNE; i += pointCoordsVertical)
	{
		al_draw_text(font[0], al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f), 2, i - map->camera.y, 0, (char*)pointsCoordsYScrolling[k].c_str());
		k++;
	}


	// Affiche les cordonnées du navire et le type de cordonnées est exemple : 58 - BA on calcule par rapport a la position x,y du centre du navire. 
	distanceRestantWidth = NOMBRE_COLONNE - player->x + (player->spriteWidth / 2); // Récupère la distance qui reste jusqu'a parcourir jusqu'a la fin de la map en LARGEUR.
	tt = distanceRestantWidth / (NOMBRE_COLONNE / nombreCoordsTotalHorizontal);
	pointCoordsXApproximatif = nombreCoordsTotalHorizontal - tt;
	indexCoordsX = floor(pointCoordsXApproximatif) - 1;

	distanceRestantHeight = NOMBRE_LIGNE - player->y + (player->spriteHeight / 2); // Récupère la distance qui reste jusqu'a parcourir jusqu'a la fin de la map en Hauteur.
	tt2 = distanceRestantHeight / (NOMBRE_LIGNE / nombreCoordsTotalVertical);
	pointCoordsYApproximatif = nombreCoordsTotalVertical - tt2;
	indexCoordsY = floor(pointCoordsYApproximatif) - 1;


	// Debug
	//map->circleFogOfWar(player->x - 29 - map->camera.x + (player->spriteWidth / 2), player->y - 33 - map->camera.y + (player->spriteHeight / 2), 2);
	//al_draw_rectangle(player->x - 29 - map->camera.x, player->y - 33 - map->camera.y, player->x - 29 - map->camera.x + player->spriteWidth, player->y - 33 - map->camera.y + player->spriteHeight, al_map_rgb(0, 0, 255), 0);
	//al_draw_line(1920 / 2, 0, 1920 / 2, 1080, al_map_rgb(0, 255, 0), 0);
	//al_draw_line(0, 1080 / 2, 1920, 1080 / 2, al_map_rgb(0, 255, 0), 0);
}




// Déplacement de la caméra sur la map et également sur la minimap.
void Play::Process(bool key[MAX_KEY]) {
	if (key[LEFT])
	{
		centerShipContinue = false;

		if (map->camera.x <= 0)
			map->camera.x = 0;
		else
			map->camera.x -= 20;
	}
	else if (key[RIGHT])
	{
		centerShipContinue = false;

		if (map->camera.x >= NOMBRE_COLONNE - 1920)
			map->camera.x = NOMBRE_COLONNE - 1920;
		else
			map->camera.x += 20;
	}
	else if (key[UP])
	{
		centerShipContinue = false;

		if (map->camera.y <= 0)
			map->camera.y = 0;
		else
			map->camera.y -= 20;
	}
	else if (key[DOWN])
	{
		centerShipContinue = false;

		if (map->camera.y >= NOMBRE_LIGNE - 1080)
			map->camera.y = NOMBRE_LIGNE - 1080;
		else
			map->camera.y += 20;
	}	
	else if (key[SPACE])
	{
		centerShipContinue = true;
	}
	else if (key[FOO1])
	{
		*nextState = STATE_EDITORMAP;
	}
	else if (key[E])
	{
		// Si un navire est déjà selectionner, alors on regarde pour attaquer.
		if (target)
		{
			// Si il essaye d'attaquer, alors qu'il est déjà en pleine attaque avec la même target alors ne rien faire
			if (targetAttack)
			{
				if (target == targetAttack)
					return;
			}

			// Si le timer n'es pas a 0 alors qu'il essaye de tirer sur une autre cible alors des que le timer sera OK pour tirer, il tirera sur la nouvelle cible.
			if (attack == true)
			{
				targetAttack = target;
				return;
			}

			// Si les cannon ont la porter pour attacker (renvoie true)
			if (checkRangeCannon(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), target->x - 29 + (target->spriteWidth / 2), target->y - 33 + (target->spriteHeight / 2)))
			{
				// Si le joueur n'est pas en déplacement, changer le sprite par rapport à l'angle
				if (m_left == 0)
					changeSpriteForAttackToAngle(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), target->x - 29 + (target->spriteWidth / 2), target->y - 33 + (target->spriteHeight / 2));

				cout << "ATTACK (E) \n";
				targetAttack = target;
				attack = true;
				timerCooldownAttack++;


				// Ajoute a la liste des animations de boulets pour l'update et draw.
				double angleRadians = map->mathAngleRadians(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), targetAttack->x - 29 + (targetAttack->spriteWidth / 2), targetAttack->y - 33 + (targetAttack->spriteHeight / 2));

				AnimationAmmo* animation = new AnimationAmmo;
				animation->cible = targetAttack;
				animation->x = player->x - 29 + (player->spriteWidth / 2);
				animation->y = player->y - 33 + (player->spriteHeight / 2);
				animation->vx = cos(angleRadians);
				animation->vy = sin(angleRadians);

				g_animationsAmmo.insert(animation);
			}
		}
	}
}

void Play::aStarSearch(Pair src, Pair dest)
{
	if (isValid(src.first, src.second) == false)
	{
		printf("Source is invalid\n");
		return;
	}

	if (isValid(dest.first, dest.second) == false)
	{
		printf("Destination is invalid\n");
		return;
	}

	if (isUnBlocked(src.first, src.second) == false || isUnBlocked(dest.first, dest.second) == false)
	{
		printf("Source or the destination is blocked\n");
		return;
	}

	if (isDestination(src.first, src.second, dest) == true)
	{
		printf("We are already at the destination\n");
		return;
	}

	bool closedList[MAP_HEIGHT][MAP_WIDTH];
	memset(closedList, false, sizeof(closedList));


	int i, j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
			cellDetails[i][j].dir = -1;
		}
	}

	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	std::set<pPair> openList;

	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	bool foundDest = false;

	while (!openList.empty())
	{
		pPair p = *openList.begin();

		openList.erase(openList.begin());

		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		double gNew, hNew, fNew;

		int d = (i % 2 == 0) ? 4 : 0;
		for (int k = 0; k < 4; k++)
		{
			int nx = i + dx[d + k];
			int ny = j + dy[d + k];
			if (isValid(nx, ny) == true)
			{
				if (isDestination(nx, ny, dest) == true)
				{
					cellDetails[nx][ny].parent_i = i;
					cellDetails[nx][ny].parent_j = j;
					cellDetails[nx][ny].dir = k * 2 + 1;
					printf("X: %i, Y: %i", nx, ny);
					printf("The destination cell is found\n");
					tracePath(dest);
					foundDest = true;

					return;
				}
				else if (closedList[nx][ny] == false && isUnBlocked(nx, ny) == true)
				{
					gNew = cellDetails[i][j].g + 1.0;
					hNew = calculateHValue(nx, ny, dest);
					fNew = gNew + hNew;
					if (cellDetails[nx][ny].f == FLT_MAX || cellDetails[nx][ny].f > fNew)
					{
						openList.insert(std::make_pair(fNew, std::make_pair(nx, ny)));
						cellDetails[nx][ny].f = fNew;
						cellDetails[nx][ny].g = gNew;
						cellDetails[nx][ny].h = hNew;
						cellDetails[nx][ny].parent_i = i;
						cellDetails[nx][ny].parent_j = j;
						cellDetails[nx][ny].dir = k * 2 + 1;
					}
				}
			}
		}
	}

	if (foundDest == false)
		printf("Failed to find the Destination Cell\n");

	return;
}

bool Play::isValid(int row, int col)
{
	return (row >= 0) && (row < MAP_HEIGHT) && (col >= 0) && (col < MAP_WIDTH);
}

bool Play::isUnBlocked(int row, int col)
{
	/*if (map->grid[row][col] == 1)
		return (true);
	else
		return (false);*/

	return (true);
}

bool Play::isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

void Play::tracePath(Pair dest)
{
	int row = dest.first;
	int col = dest.second;
	while (! (cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
	{
		Path.push(PathInfo(row, col, cellDetails[row][col].dir));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(PathInfo(row, col, -1));

	while (!Path.empty() && 0)
	{
		PathInfo p = Path.top();
		Path.pop();
		Rect tile = getTileRect(p.x, p.y, map->camera);
		printf("-> (%d,%d) ", p.x, p.y);
	}

	return;
}

double Play::calculateHValue(int row, int col, Pair dest)
{
	return ((double)sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second)));
}

void Play::centerShip()
{
	if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) <= 0 && player->y - (screenHeight / 2) + (player->spriteHeight / 2) <= 0) // Haut gauche
	{
		map->camera.x = 0;
		map->camera.y = 0;
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) >= NOMBRE_COLONNE - screenWidth + (player->spriteWidth / 2) && player->y - (screenHeight / 2) + (player->spriteHeight / 2) <= 0) // Haut Droite
	{
		map->camera.x = NOMBRE_COLONNE - screenWidth;
		map->camera.y = 0;
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) >= NOMBRE_COLONNE - screenWidth + (player->spriteWidth / 2) && player->y - (screenHeight / 2) + (player->spriteHeight / 2) >= 1) // Droite
	{
		map->camera.x = NOMBRE_COLONNE - screenWidth;
		map->camera.y = player->y - (screenHeight / 2) + (player->spriteHeight / 2);
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) >= 1 && player->y - (screenHeight / 2) + (player->spriteHeight / 2) >= NOMBRE_LIGNE - screenHeight + (player->spriteHeight / 2)) // Bas
	{
		map->camera.x = player->x - (screenWidth / 2) + (player->spriteWidth / 2);
		map->camera.y = NOMBRE_LIGNE - screenHeight;
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) >= NOMBRE_COLONNE - screenWidth + (player->spriteWidth / 2) && player->y - (screenHeight / 2) + (player->spriteHeight / 2) >= NOMBRE_LIGNE - screenHeight + (player->spriteHeight / 2)) // Bas Droite
	{
		map->camera.x = NOMBRE_COLONNE - screenWidth;
		map->camera.y = NOMBRE_LIGNE - screenHeight;
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) <= 0 && player->y - (screenHeight / 2) + (player->spriteHeight / 2) >= NOMBRE_LIGNE - screenHeight + (player->spriteHeight / 2)) // Bas Gauche
	{
		map->camera.x = 0;
		map->camera.y = NOMBRE_LIGNE - screenHeight;
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) <= 0 && player->y - (screenHeight / 2) + (player->spriteHeight / 2) >= 1) // Gauche
	{
		map->camera.x = 0;
		map->camera.y = player->y - (screenHeight / 2) + (player->spriteHeight / 2);
	}
	else if (player->x - (screenWidth / 2) + (player->spriteWidth / 2) >= 1 && player->y - (screenHeight / 2) + (player->spriteHeight / 2) <= 0) // Haut
	{
		map->camera.x = player->x - (screenWidth / 2) + (player->spriteWidth / 2);
		map->camera.y = 0;
	}
	else // Autres (centre de la map..etc)
	{
		map->camera.x = player->x - (screenWidth / 2) + (player->spriteWidth / 2);
		map->camera.y = player->y - (screenHeight / 2) + (player->spriteHeight / 2);
	}
}

void Play::moveMiniMap()
{
	centerShipContinue = false;
	int positionXInMinimapInterieur = *mouseX - minimapInterieurPositionX;
	int positionYInMinimapInterieur = *mouseY - minimapInterieurPositionY;

	if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) <= 0 && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) <= 0) // Haut gauche
	{
		map->camera.x = 0;
		map->camera.y = 0;
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) >= NOMBRE_COLONNE - screenWidth && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) <= 0) // Haut Droite
	{
		map->camera.x = NOMBRE_COLONNE - screenWidth;
		map->camera.y = 0;
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) >= NOMBRE_COLONNE - screenWidth && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) >= 1) // Droite
	{
		map->camera.x = NOMBRE_COLONNE - screenWidth;
		map->camera.y = positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2);
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) >= NOMBRE_COLONNE - screenWidth && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) >= NOMBRE_LIGNE - screenHeight) // Bas Droite
	{
		cout << "BAS DROITE";

		map->camera.x = NOMBRE_COLONNE - screenWidth;
		map->camera.y = NOMBRE_LIGNE - screenHeight;
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) >= 1 && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) >= NOMBRE_LIGNE - screenHeight) // Bas
	{
		cout << "BAS";

		map->camera.x = positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2);
		map->camera.y = NOMBRE_LIGNE - screenHeight;
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) <= 0 && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) >= NOMBRE_LIGNE - screenHeight) // Bas Gauche
	{
		map->camera.x = 0;
		map->camera.y = NOMBRE_LIGNE - screenHeight;
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) >= 1 && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) >= 1) // Autres (millieu etc)
	{
		map->camera.x = positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2);
		map->camera.y = positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2);
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) <= 0 && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) >= 1) // Gauche
	{
		map->camera.x = 0;
		map->camera.y = positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2);
	}
	else if (positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2) >= 1 && positionYInMinimapInterieur * minimapInterieurHeight - (screenHeight / 2) <= 0) // Haut
	{
		map->camera.x = positionXInMinimapInterieur * minimapInterieurWidth - (screenWidth / 2);
		map->camera.y = 0;
	}
}

void Play::moveTheCameraToScrolling()
{

}

void Play::clickOnTheMapAndMove()
{
	*mouseB = 0;
	Point click = getTile(*mouseX + map->camera.x, *mouseY + map->camera.y);

	while (!Path.empty()) Path.pop();

	Point center = tileCenter(click.x, click.y);
	Rect tile = getTileRect(click.x, click.y, map->camera);

	// Lors du click gauche sur la map, récupère la tile cliquer et active l'effet de click sur la map quand le boolean est a TRUE
	cursorClickTileInMap = true;
	tileClicker = tile;

	while (!Path.empty()) Path.pop();

	Pair src;
	for (auto& unit : map->g_units)
	{
		if (unit->type == UnitType::Player)
			src = std::make_pair(unit->pos.x, unit->pos.y);
	}

	Pair dest = std::make_pair(click.x, click.y);
	aStarSearch(src, dest);
}

void Play::changeSpriteForAttackToAngle(int x1, int y1, int x2, int y2)
{
	// Je regarde l'angle entre le joueur et l'autre navire pour changer l'apparence du bateau en fonction de l'angle.
	double angle = map->mathAngleDegres(x1, y1, x2, y2);

	if (angle >= -90 && angle <= 0) // Haut droite
	{
		if (player->direction == Direction::en_northwest)
			player->direction = Direction::en_northeast;
		else if (player->direction == Direction::en_southeast)
			player->direction = Direction::en_southwest;
	}
	else if (angle >= 0 && angle <= 90) // Bas droite
	{
		if (player->direction == Direction::en_southwest)
			player->direction = Direction::en_southeast;
		else if (player->direction == Direction::en_northeast)
			player->direction = Direction::en_northwest;
	}
	else if (angle <= -90 && angle >= -180) // Haut gauche
	{
		if (player->direction == Direction::en_northeast)
			player->direction = Direction::en_northwest;
		else if (player->direction == Direction::en_southwest)
			player->direction = Direction::en_southeast;
	}
	else if (angle <= 180 && angle >= 90) // Bas gauche
	{
		if (player->direction == Direction::en_southeast)
			player->direction = Direction::en_southwest;
		else if (player->direction == Direction::en_northwest)
			player->direction = Direction::en_northeast;
	}
}

bool Play::checkRangeCannon(int x1, int y1, int x2, int y2)
{
	double distance = map->mathDistance(player->x - 29 + (player->spriteWidth / 2), player->y - 33 + (player->spriteHeight / 2), target->x - 29 + (target->spriteWidth / 2), target->y - 33 + (target->spriteHeight / 2));

	// Si les cannon ont la porter pour attacker ALORS
	if (distance <= rangeCannon)
		return true;
	else
		return false;
}