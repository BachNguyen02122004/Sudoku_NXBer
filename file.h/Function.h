#ifndef FUNCTION_H
#define FUNCTION_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include<bits/stdc++.h>
#include"LTexture.h"


//The window we'll be rendering to
extern SDL_Window* window;

//The surface contained by the window
extern SDL_Surface* gSurface;

extern SDL_Renderer* gRenderer;

extern SDL_Texture* gTexture;

extern SDL_Surface* icon;

extern TTF_Font* gFont;

extern Mix_Music *gMusic;
extern Mix_Chunk *gGameover;
extern Mix_Chunk* gWin;




#define COLOR_SELECT   	0x00, 0x50, 0x50, 0x40
#define COLOR_GRID     	0x00, 0x00, 0x00, 0xFF 
#define COLOR_BAD_CELL 	0xFF, 0xB8, 0xCF, 0xFF

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int CELL_SIZE = 40;

#endif
