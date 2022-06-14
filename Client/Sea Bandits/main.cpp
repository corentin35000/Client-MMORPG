#include "main.h"

void setNextState(int& nextState, int var)
{
	nextState = var;
}

void changeState(Gamestate*& currentState, int& stateID, int& nextState) {
	// If the state needs to be changed
	if (nextState != STATE_NULL)
	{
		// Delete the current state if the next state is not set to exit state.
		if (nextState != STATE_EXIT)
		{
			delete currentState;
		}

		// Change state depending on nextState variable.
		switch (nextState)
		{
			case STATE_MENU:
				currentState = new Menu(nextState, engine->display, engine->screen_width, engine->screen_height, engine->font, engine->charPressed, engine->mouseX, engine->mouseY, engine->mouseZ, engine->mouseW, engine->mouseB);
				break;

			case STATE_PLAY:
				currentState = new Play(nextState, engine->display, engine->screen_width, engine->screen_height, engine->font, engine->charPressed, engine->mouseX, engine->mouseY, engine->mouseZ, engine->mouseW, engine->mouseB);
				break;

			case STATE_EDITORMAP:
				currentState = new EditorMap(nextState, engine->screen_width, engine->screen_height, engine->font, engine->charPressed, engine->mouseX, engine->mouseY, engine->mouseZ, engine->mouseW, engine->mouseB);
				break;
		}

		// Change the current state ID
		stateID = nextState;

		// Null the next State ID
		nextState = STATE_NULL;
	}
}

void checkInput(ALLEGRO_EVENT& event) {
	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		keyPressed = event.keyboard.keycode;

		switch (event.keyboard.keycode)
		{
			case ALLEGRO_KEY_UP:
				engine->key[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				engine->key[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				engine->key[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				engine->key[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				engine->key[SPACE] = true;
				break;
			case ALLEGRO_KEY_F1:
				engine->key[FOO1] = true;
				break;
			case ALLEGRO_KEY_E:
				engine->key[E] = true;
				break;
		}
	}
	else if (event.type == ALLEGRO_EVENT_KEY_UP)
	{
		keyPressed = -1;
		switch (event.keyboard.keycode)
		{
			case ALLEGRO_KEY_UP:
				engine->key[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				engine->key[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				engine->key[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				engine->key[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				engine->key[SPACE] = false;
				break;
			case ALLEGRO_KEY_F1:
				engine->key[FOO1] = false;
				break;
			case ALLEGRO_KEY_E:
				engine->key[E] = false;
				break;
		}
	}

	if (event.type == ALLEGRO_EVENT_KEY_CHAR)
	{
		engine->charPressed = event.keyboard.unichar;
	}
	else
	{
		engine->charPressed = -1;
	}

	if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		engine->mouseX = event.mouse.x;
		engine->mouseY = event.mouse.y;
		engine->mouseZ = event.mouse.z;
		engine->mouseW = event.mouse.w;
	}

	if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		engine->mouseB = event.mouse.button;
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		engine->mouseB = 0;
	}
}

void Load()
{
	
}

void UpdateReseauUDP()
{
	/*
	printf("PING : %u \n", peer->roundTripTime);
	printf("GIGUE (ping) : %u \n", peer->roundTripTime);
	printf("PACKETLOSS : %u \n", peer->packetsLost);
	printf("bandePassanteEntrante : %u \n", peer->incomingBandwidth);
	printf("bandePassanteSortante : %u \n", peer->outgoingBandwidth);
	printf("CLIENT HORODOTAGE - TIMESTAMP (for packet) : %u \n", client->serviceTime);
	*/
}





int main(int argc, char* argv[]) {
	// TEST 
	/*json dataJSON = {
		{
			{ "idDB", 3 },
			{ "HMAC", "le hash" },
			{ "packetEventAction", "ATTACK" },
			{ "packetRequeteSequence", 43 },
			{ "packetSendTimeStamp", 19883282 }, // Quand on envoie un packet du CLIENT au SERVER et que le SERVER redistribu le packet on recevra le timestamp lors de l'envoie - le timestamp actuelle du Client
			{ "map", 2 },
			{ "visible", true },
			{ "x", 10 },
			{ "y", 20 },
			{ "HP", 75000 },
			{ "idDesign", "elite21" },
			{ "pseudo", "shoutteurVEX" },
			{ "rangPVE", 0 },
			{ "rangPVP", 0 },
			{ "ponts", {
				{ "or", 3 },
				{ "elite", 5 }
			}}
		},
	};
	std::string dataPacket = dataJSON.dump();
	int lengthPacket = dataPacket.length();*/

	// Create Engine (init ENET, Allegro, Screen width/height client..)
	engine = new Engine(1920, 1080);

	// Create SceneManager et creation de la scene de Menu.
	currentScene = new Menu(nextState, engine->display, engine->screen_width, engine->screen_height, engine->font, engine->charPressed, engine->mouseX, engine->mouseY, engine->mouseZ, engine->mouseW, engine->mouseB);
	stateID = STATE_MENU; // Premiere scene qui sera charger.
	nextState = STATE_NULL;

	// GameLoop
	Load();
	while (stateID != STATE_EXIT) 
	{
		ALLEGRO_EVENT event;
		do {
			// La file d'attente des événements.
			al_wait_for_event(engine->queue, &event);

			// Check -> Event Allegro : KEYBOARD, MOUSE...
			checkInput(event);

			// Check -> EVENT Allegro : TIMER, CLOSE WINDOW, RESIZE WINDOW
			if (event.type == ALLEGRO_EVENT_TIMER)
			{
				if (event.timer.source == engine->timer)
				{
					currentScene->Process(engine->key);
					UpdateReseauUDP();
					currentScene->Update(engine->deltaTime);
				}
			}
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				setNextState(nextState, STATE_EXIT);
			}
			else if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) 
			{
				// BUG
				/*
				al_acknowledge_resize(engine->display);
				ALLEGRO_TRANSFORM trans;
				al_build_transform(&trans, 0, 0, 0, 0, 0);
				al_identity_transform(&trans);
				al_scale_transform(&trans, al_get_display_width(engine->display) / 640, al_get_display_height(engine->display) / 480);
				al_use_transform(&trans);
				*/
			}
		} while (! al_is_event_queue_empty(engine->queue)); // Renvoie true si la file d'attente d'événements spécifiée est actuellement vide.

		// Change de Scene si changement d'etat de scene.
		changeState(currentScene, stateID, nextState);

		// Selectionne le doublebackbuffer sur le qu'elle on vas dessiner à l'écran qui est une image en mémoire qu'ont ne vois pas.
		al_set_target_bitmap(al_get_backbuffer(engine->display));

		// Dessine à l'écran en fonction de la scene de jeu.
		currentScene->Draw();

		// Efface l'ecran
		al_flip_display();
 
		// Attend le nombre de secondes spécifié.
		// Cela indique au système de mettre en pause le thread en cours pendant la durée donnée de pendant 0,001 seconde : pour des raisons de performances.
		al_rest(0.001);
	}

	al_destroy_event_queue(engine->queue);
	al_destroy_timer(engine->timer);
	al_destroy_display(engine->display);

	return 0;
}