#include "Function.h"

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* gSurface = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;

SDL_Surface* icon = NULL;

TTF_Font* gFont = NULL;

Mix_Music *gMusic = NULL;
Mix_Chunk *gGameover = NULL;
Mix_Chunk* gWin = NULL;