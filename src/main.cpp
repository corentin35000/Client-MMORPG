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
#include "sceneManager.cpp"
#include "game.cpp"



SDL_Texture *LoadTexture(SDL_Renderer *renderer, char *path)
{
	SDL_Texture *texture = NULL;
	texture = IMG_LoadTexture(renderer, path);

	if (texture == NULL)
	{
		printf("Impossible de creer la texture, erreur : %s \n", path, IMG_GetError());
		return NULL;
	}

	return texture;
}


// Initialisation variables, pointeurs.. des librairies de la SDL2.
SDL_Window *window; // Window
SDL_Renderer *renderer; // Renderer

// Initialisation variables, pointeurs.. de la librairie : ENET 
ENetHost *client; // Client ENET

ENetAddress address; // Adress to connection for Server (ip + port)
ENetEvent eventENET;
ENetPeer *peer;
ENetPacket *packet;

// Initialisation variables, pointeurs.. de la librairie : curl 
CURL *curl;
CURLcode res;

// DeltaTime


//
class GameServer
{
	public:
		vector<Player> Players;
};



// Functions for GameLoop
SDL_Texture *playerImg; // Player Data
int playerWidth;
int playerHeight;
int playerX = 0;
int playerY = 0;
int playerEndX = 0;
int playerEndY = 0;

Uint32 mouseButtons; // State Mouse.
int mouseX;
int mouseY;

const Uint8 *keyState = SDL_GetKeyboardState(NULL); // State Keyboard.

TTF_Font *police;
SDL_Color policeColor;
SDL_Texture *textureFont;
SDL_Surface *surfaceMessage;

void load()
{
	sceneManager_load();
	
	police = TTF_OpenFont("./assets/fonts/arial.ttf", 10);
	policeColor = { 255, 255, 255 };
	surfaceMessage = TTF_RenderText_Solid(police, "TRDDFFSDF", policeColor);
	textureFont = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	playerImg = LoadTexture(renderer, (char*)"./assets/images/1.png");
	SDL_QueryTexture(playerImg, NULL, NULL, &playerWidth, &playerHeight);
}

void update()
{	
	sceneManager_update();
	
	mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

	if (playerEndX != 0 || playerEndY != 0)
	{
		if (playerX != playerEndX)
		{
			if (playerEndX < playerX)
			{
				playerX -= 1;

				printf("ENVOIE PACKET");
					/*packet = enet_packet_create(&objectPlayer, sizeof(objectPlayer) + 1, ENET_PACKET_FLAG_RELIABLE);                                
				enet_peer_send(peer, 0, packet);
				enet_host_service(client, &eventENET, 0);*/
			}
			else if (playerEndX > playerX)
			{
				playerX += 1;
				packet = enet_packet_create("test", strlen("test") + 1, ENET_PACKET_FLAG_RELIABLE);                                
				enet_peer_send(peer, 0, packet);
				enet_host_service(client, &eventENET, 0);
			}
		}
		else
		{
			playerEndX = 0;
		}

		if (playerY != playerEndY)
		{
			if (playerEndY < playerY)
			{
				playerY -= 1;
				//printf("ENVOIE PACKET");
				/*packet = enet_packet_create(&objectPlayer, sizeof(objectPlayer) + 1, ENET_PACKET_FLAG_RELIABLE);                                
				enet_peer_send(peer, 0, packet);
				enet_host_service(client, &eventENET, 0);*/
			}
			else if (playerEndY > playerY)
			{
				playerY += 1;
				/*packet = enet_packet_create(&objectPlayer, sizeof(objectPlayer) + 1, ENET_PACKET_FLAG_RELIABLE);                                
				enet_peer_send(peer, 0, packet);
				enet_host_service(client, &eventENET, 0);*/
			}
		}
		else
		{
			playerEndY = 0;
		}
	}

}

void draw()
{
	sceneManager_draw();

	SDL_Rect rectDest = { playerX, playerY, playerWidth, playerHeight };
	SDL_RenderCopy(renderer, playerImg, NULL, &rectDest);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(textureFont, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { 0, 0, texW, texH };
	SDL_Rect t = { 100, 100, texW, texH };

	SDL_RenderCopy(renderer, textureFont, NULL, &dstrect);
	SDL_RenderCopy(renderer, textureFont, NULL, &t);
}

void keypressed()
{
	sceneManager_keypressed();

	if (keyState[SDL_SCANCODE_RETURN]) 
	{
		printf("<RETURN> is pressed.\n");
	}
}

void mousepressed()
{
	sceneManager_mousepressed();

	if ((mouseButtons & SDL_BUTTON_LMASK) != 0) 
	{
		SDL_Log("Mouse cursor is at %d, %d", mouseX, mouseY);
		SDL_Log("Position player %d, %d", playerX, playerY);

		playerEndX = mouseX - (playerWidth / 2);
		playerEndY = mouseY - (playerHeight / 2);
	}
}

void textinput()
{
	sceneManager_textinput();
}

void updateReseauUDP()
{
	enet_host_service(client, &eventENET, 0);

    switch (eventENET.type)
    {
        case ENET_EVENT_TYPE_RECEIVE:
            printf("Un paquet de %u octets contenant %s a etais recue du server sur le canal %u \n",
            eventENET.packet->dataLength,
            eventENET.packet->data,
            eventENET.channelID);
            
            //printf("%s \n", eventENET.packet->data)

            enet_packet_destroy(eventENET.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("Server disconnected.");
    }

	enet_peer_ping(peer);
	printf("PING : %u \n", peer->roundTripTime);
}









// Main Program
int main(int argc, char *argv[])
{
	// Initialisation de la SDL avec toute les dépendances.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("Impossible d'initialiser la SDL : %s \n", SDL_GetError());
		return -1;
	}


	// Create window
	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 900;
	window = SDL_CreateWindow("Seafight - Client",
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  SCREEN_WIDTH,
										  SCREEN_HEIGHT,
										  SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_Log("Impossible de creer la fenetre: %s \n", SDL_GetError());
		return -1;
	}


	// Create Renderer in Window.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_Log("Impossible de creer le renderer : %s \n", SDL_GetError());
		return -1;
	}


	// Initialisation de la librairie : SDL_Image (PNG loading)
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if(! (IMG_Init(imgFlags) & imgFlags))
	{
		printf( "SDL_image non initialiser ! SDL_image Error: %s \n", IMG_GetError());
		return -1;
	}


	// Initialisation de la librairie : SDL2_Font.tff
	if (!TTF_WasInit() && TTF_Init() != 0) 
	{
		printf("SDL_FontTFF non initialiser : %s \n", TTF_GetError());
		return -1;
	}


	// Initialisation de la librairie : SDL2_mixer (sound)
	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	int initted = Mix_Init(flags);

	if (initted & flags != flags) 
	{
		printf("SDL2_mixer OGG and MP3 non supporter \n");
		printf("Mix_Init : %s \n", Mix_GetError());

		return -1;
	}


	// Initialisation de la librairie : curl (request HTTP - GET / POST)
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl == NULL)
    {
		printf("CURL non initialiser ! \n");
        return -1;
    }
	

	//SCENE LOGIN
	/*while (true)
	{

	}*/


	// Initialisation de la librairie : ENET (sockets UDP)
    if (enet_initialize () != 0)
    {
		printf("ENET non initialiser !");
        return -1;
    }


	// Create client (ENET / UDP)
	client = enet_host_create(NULL, 1, 2, 0, 0);
	if (client == NULL)
	{
		printf("Erreur create Client-ENET \n");
		return -1;
	}


	// Set Host et établie la connection au server.
	enet_address_set_host(&address, "141.94.23.58");
	address.port = 56289;
	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) 
	{
		fprintf(stderr, "Aucun homologue disponible pour l'établissement d'une connexion ENet \n");
		return -1;
	}


	// Attendez jusqu'à 5 secondes pour que la tentative de connexion réussisse
	if (enet_host_service(client, &eventENET, 5000) > 0 && eventENET.type == ENET_EVENT_TYPE_CONNECT) 
	{
		printf("Connection to server. \n");
		enet_peer_ping_interval(peer, ENET_PEER_PING_INTERVAL);
		enet_peer_ping(peer);
		peer->roundTripTime = peer->lastRoundTripTime;
	}
	else 
	{
		enet_peer_reset(peer);
		printf("Connection server failed. \n");
	}


	// GameLoop House
	load();

	while (true)
	{
		SDL_PumpEvents();
		SDL_Event eventSDL;
		if (SDL_PollEvent(&eventSDL))
		{
			if (eventSDL.type == SDL_QUIT)
			{
				break;
			}
		}

		updateReseauUDP();

		keypressed();
		mousepressed();
		textinput();

		update();
		SDL_RenderClear(renderer); // Efface l'écran
		draw();
		SDL_RenderPresent(renderer); // Présente à l'écran
	}
	

	// Destroy instance for memory (Free memory)
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(textureFont);
	SDL_DestroyTexture(playerImg);
	IMG_Quit();
	TTF_CloseFont(police);
	TTF_Quit();
	while(Mix_Init(0))
		Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	enet_peer_disconnect_later(peer, 0);
	enet_peer_reset(peer);
	enet_host_destroy(client);
	enet_deinitialize();
	curl_global_cleanup();
	SDL_Quit();

	return 0;
}