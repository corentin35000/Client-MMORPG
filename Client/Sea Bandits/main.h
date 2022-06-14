#ifndef MAIN_H_
#define MAIN_H_

#include "engine.h"
#include "gui.h"

#include "scenes/gamestate.h"
#include "scenes/menu.h"
#include "scenes/play.h"
#include "./editormap.h"


Engine* engine;

Gamestate* currentScene;
int stateID; 
int nextState;

#endif