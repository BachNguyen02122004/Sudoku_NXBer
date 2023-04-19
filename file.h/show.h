#ifndef SHOW_H
#define SHOW_H
#include "Function.h"
#include "LTexture.h"


extern LTexture Menu;
extern LTexture Gameover;
extern LTexture Winner;
extern bool quit ;
extern LTexture gYourtime;


int showMenu(SDL_Event e);
int showDifficulty(SDL_Event e);
int showGameOver(SDL_Event e);
int showWin(SDL_Event e);
bool checkpositionforrect(const int& x, const int& y, SDL_Rect rect);


#endif