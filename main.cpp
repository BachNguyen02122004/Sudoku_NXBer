#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "Area.h"
#include "CellData.h"
#include <SDL_mixer.h>
#include "Board.h"
#include <cmath>

#define COLOR_SELECT   	0x00, 0x50, 0x50, 0x40
#define COLOR_GRID     	0x00, 0x00, 0x00, 0xFF 
#define COLOR_BAD_CELL 	0xFF, 0xB8, 0xCF, 0xFF

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};


struct Texture {
	SDL_Texture* texture;
	int width;
	int height;
	int x, y;
};

int loadTexture (const std::string &file, Texture &texture, int x, int y);


SDL_Surface* loadSurface (const std::string &file);
void renderTexture(Texture& texture);

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int CELL_SIZE = 40;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* gSurface = NULL;

 SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;

SDL_Surface* icon = NULL;

TTF_Font* gFont = NULL;
Texture bgTxtr;
Texture Hint;
LTexture gTextTexture;
LTexture HintText;
Texture digTxtrs [9] ;
Mix_Music *gMusic = NULL;

bool Hintclick = false;
int Hint_number = 3;
int sel_x = 0;
int sel_y = 0;

CellData data [9][9] ;

Area area_rows [9];
Area area_cols [9];
Area area_boxs [9];


SudokuBoard ans, su;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}
bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}


void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool init ()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	window = SDL_CreateWindow(
			"SDL Tutorial", 
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN );
	if ( window == NULL )
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	
	gSurface = SDL_GetWindowSurface( window );
	if ( gSurface == NULL )
	{
		printf( "Surface could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	gRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( gRenderer == NULL )
	{
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	SDL_SetRenderDrawBlendMode (gRenderer, SDL_BLENDMODE_BLEND);
	
	gTexture = SDL_CreateTexture(gRenderer, gSurface->format->format,
		SDL_TEXTUREACCESS_TARGET, 9*CELL_SIZE+2, 9*CELL_SIZE+2); // might be 1 too little
	if ( gTexture == NULL )
	{
		printf( "Texture could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	
	if ( TTF_Init () < 0 )
	{
		printf( "TTF could not initialize! TTF_Error: %s\n", TTF_GetError() );
		return -1;
	}

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return -1;
	}
	
	// SDL_Color color = { 0x00, 0x00, 0x00 };
	// TTF_Font* font = TTF_OpenFont( "res/lazy.ttf", 28 );
	// if ( font != NULL)
	// {
	// 	for (int i = 0; i < 9; i++)
	// 	{
	// 		std::string s = std::to_string(i+1);
	// 		SDL_Surface* textSurface = TTF_RenderText_Blended( font, s.c_str(), color); // viet chu len man
	// 		if( textSurface == NULL )
	// 		{
	// 			printf( "Unable to render text surface! SDL_ttf error: %s\n", TTF_GetError() );
	// 		}
	// 		else
	// 		{
	// 			//Create texture from surface pixels
	// 			SDL_Texture * texture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
	// 			if( texture == NULL )
	// 			{
	// 				printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
	// 			}
	// 			else
	// 			{
	// 				Texture ft;
	// 				ft.texture = texture;
	// 				ft.width = textSurface->w;
	// 				ft.height = textSurface->h;
	// 				digTxtrs[i] = ft;
	// 			}

	// 			//Get rid of old surface
	// 			SDL_FreeSurface( textSurface );
	// 		}
	// 	}
	// 	TTF_CloseFont (font);
	// }
	// else 
	// {
	// 	printf ("Could not load font! SDL error: %s\n", TTF_GetError());
	// }
	
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return -1;
	}
	
	if (loadTexture("res/phong_nen.png", bgTxtr, 0, 0) < 0)
	{
		printf("FAIL\n");
		return -1;
	}
	
	if(loadTexture("res/hint3.png", Hint, 600, 150) < 0 ){
		printf("Fail");
		return -1;
	}
	
	
	icon = loadSurface ("res/avatar.png");
	SDL_SetWindowIcon (window, icon);	
	SDL_SetWindowTitle (window, "Sudoku");
	
	for (int i = 0; i < 9; i++)
	{
		area_cols[i].init(AREA_COLUMN, i );
	}
	for (int i = 0; i < 9; i++)
	{
		area_rows[i].init(AREA_ROW, i );
	}
	for (int i = 0; i < 9; i++)
	{
		area_boxs[i].init(AREA_BOX, i );
	}
	
	return 0;
 }
bool LoadMedia(){
	bool success = true;
	 SDL_Color color = { 0x00, 0x00, 0x00 };
	TTF_Font* font = TTF_OpenFont( "res/lazy.ttf", 28 );
	if ( font != NULL)
	{
		for (int i = 0; i < 9; i++)
		{
			std::string s = std::to_string(i+1);
			SDL_Surface* textSurface = TTF_RenderText_Blended( font, s.c_str(), color); // viet chu len man
			if( textSurface == NULL )
			{
				printf( "Unable to render text surface! SDL_ttf error: %s\n", TTF_GetError() );
				success = false;
			}
			else
			{
				//Create texture from surface pixels
				SDL_Texture * texture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
				if( texture == NULL )
				{
					printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
					success = false;
				}
				else
				{
					Texture ft;
					ft.texture = texture;
					ft.width = textSurface->w;
					ft.height = textSurface->h;
					digTxtrs[i] = ft;
				}

				//Get rid of old surface
				SDL_FreeSurface( textSurface );
			}
		}
		TTF_CloseFont (font);
	}
	else 
	{
		printf ("Could not load font! SDL error: %s\n", TTF_GetError());
		success = false;
	}
	gFont = TTF_OpenFont( "res/lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		if( !gTextTexture.loadFromRenderedText( "Time ", color ) ) // hien thi ten 
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}


	}
	TTF_Font* gHintText = TTF_OpenFont( "res/lazy.ttf", 14);
	if(gHintText == NULL){
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else { 
		if( !HintText.loadFromRenderedText( "Hint" , color ) ) // hien thi ten 
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	gMusic = Mix_LoadMUS( "Ngu 1 minh.mp3" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success ;
}

int loadTexture (const std::string &file, Texture &texture, int x, int y) // load anh
{
	SDL_Surface* image = loadSurface(file);
	if (image == NULL)
	{
		printf("Can't load %s! SDL error: %s\n", file.c_str(), IMG_GetError());
		return -1;
	}
	
	SDL_Texture* bg = SDL_CreateTextureFromSurface(gRenderer, image);
	texture.width = image->w;
	texture.height = image->h;
	texture.texture = bg;
	texture.x = x;
	texture.y = y;
	SDL_FreeSurface(image);
	return 0;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

SDL_Surface* loadSurface (const std::string &file) //load anh
{
	SDL_RWops *rwop = SDL_RWFromFile(file.c_str(), "rb"); // mo tep 
	if (rwop == NULL)
	{
		printf("Can't find %s!\n", file.c_str());
		return NULL;
	}
	
	SDL_Surface *image;
	std::string sfx_gif = ".gif";
	std::string sfx_png = ".png";
	if (hasEnding(file, sfx_gif))
        image = IMG_LoadGIF_RW (rwop);
    else if (hasEnding(file, sfx_png) )
		image = IMG_Load ( file.c_str() );    

	if (image == NULL)
	{
		printf("Failed to load %s: SDL error: %s\n", file.c_str(), SDL_GetError());
	}
	SDL_SetColorKey( image, SDL_TRUE, SDL_MapRGBA( image->format, 0xFF , 0xFF, 0xFF, 0xFF ) );
	return image;
}

void drawBackground( )
{
	for (int x = 0 ; x < SCREEN_WIDTH; x += bgTxtr.width)
	{
		for (int y = 0 ; y < SCREEN_HEIGHT; y += bgTxtr.height)
		{
			SDL_Rect rect = { x, y, bgTxtr.width, bgTxtr.height };
			SDL_RenderCopy (gRenderer, bgTxtr.texture, NULL, &rect);
		}
	}
}

void drawHint(){ // vẽ gợi ý 
	SDL_Rect rect = { 600, 150, Hint.width, Hint.height };
	SDL_RenderCopy (gRenderer, Hint.texture, NULL, &rect);
		}
	

void drawGrid ()
{
	int fullsize = 9 * CELL_SIZE + 1;
	
	SDL_SetRenderDrawColor ( gRenderer, COLOR_GRID );		
	for (int i = 0; i < 10; i++){
		int x = i * CELL_SIZE;
		int y = i * CELL_SIZE;
		SDL_RenderDrawLine (gRenderer, 0, y, fullsize, y);
		SDL_RenderDrawLine (gRenderer, x, 0, x, fullsize);
		if (i % 3 == 0)
		{
			SDL_RenderDrawLine (gRenderer, 0, y+1, fullsize, y+1);
			SDL_RenderDrawLine (gRenderer, x+1, 0, x+1, fullsize);
		}
	}
}

void drawSelection ()
{	
	int x = sel_x * CELL_SIZE;
	int y = sel_y * CELL_SIZE;
	
	SDL_SetRenderDrawColor ( gRenderer, COLOR_SELECT );
	
	int startx = sel_x % 3 == 0 ? x + 2 : x +1;
	int width = sel_x % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
	int starty = sel_y % 3 == 0 ? y + 2 : y + 1;
	int height = sel_y % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
	
	SDL_Rect select = { startx, starty, width, height };
	SDL_RenderFillRect (gRenderer, &select);
}

void drawInvalidCells ()
{
	SDL_SetRenderDrawColor ( gRenderer, COLOR_BAD_CELL );
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (data[i][j].vldcnt != 0 )
			{
				int x = i * CELL_SIZE;
				int y = j * CELL_SIZE;
	
				int startx = i % 3 == 0 ? x + 2 : x + 1;
				int width = i % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
				int starty = j % 3 == 0 ? y + 2 : y + 1;
				int height = j % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
	
				SDL_Rect select = { startx, starty, width, height };
				SDL_RenderFillRect (gRenderer, &select);
			}	
		}
	}
}
void RandomBoard(){
	srand(time(NULL));
	int idx = rand() % (sizeof(manyBoard)/sizeof(manyBoard[0])) ;
	su =  manyBoard[idx];
	ans = ansBoards[idx];
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if(su.board[i][j] != 0) {
				int box = 3 * (j / 3) + (i / 3);
				data[i][j].digit = su.board[i][j];
				data[i][j].used = 1;
				area_cols[i].DigitAdded (data[i][j].digit, data, i, j);
				area_rows[j].DigitAdded (data[i][j].digit, data, i, j);
				area_boxs[box].DigitAdded (data[i][j].digit, data, i, j);
		
			}	
		}
	}
}

void hintBoard(){
	bool ok = 0;
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9; j++){
			if(data[i][j].digit == 0){
				data[i][j].digit  = ans.board[i][j];
				int box = 3 * (j / 3) + (i / 3);
				area_cols[i].DigitAdded (data[i][j].digit, data, i, j);
				area_rows[j].DigitAdded (data[i][j].digit, data, i, j);
				area_boxs[box].DigitAdded (data[i][j].digit, data, i, j);
				return;
			}
		}
	}
}

void drawText () // dien so 
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			
			if (data[i][j].digit != 0)
			{
				
				Texture ft = digTxtrs[data[i][j].digit - 1];				
				
				int x = i * CELL_SIZE;
				int y = j * CELL_SIZE;
	
				int startx = (i % 3 == 0 ? x + 2 : x + 1) + (CELL_SIZE - ft.width) / 2;
				int starty = (j % 3 == 0 ? y + 2 : y + 1) + (CELL_SIZE - ft.height) / 2;
				
				if(data[i][j].digit != ans.board[i][j]){
					int startx_color = i % 3 == 0 ? x + 2 : x + 1;
					int width = i % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
					int starty_color = j % 3 == 0 ? y + 2 : y + 1;
					int height = j % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
	
					SDL_SetRenderDrawColor(gRenderer, COLOR_BAD_CELL);
					SDL_Rect rect = {startx_color, starty_color, width, height}; // vi tri in mau 
					SDL_RenderFillRect(gRenderer, &rect);

				}
				SDL_Rect renderQuad = {startx, starty, ft.width, ft.height }; // vi tri in chu 
				//Render to screen
				SDL_RenderCopy( gRenderer, ft.texture, NULL, &renderQuad ); 
				if (data[i][j].used)
   	 			{
        			SDL_SetRenderDrawColor(gRenderer, 100, 100, 200, 80);
					SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
					SDL_RenderFillRect(gRenderer, &rect);
   				}	
			}	
		}
	}
}

void close ()
{
	
	if (icon != NULL)
		SDL_FreeSurface(icon);
	if (gRenderer != NULL)
		SDL_DestroyRenderer(gRenderer);
	if (gSurface != NULL)
		SDL_FreeSurface(gSurface);
	if (window != NULL)
		SDL_DestroyWindow(window);
	if (gTexture != NULL)
		SDL_DestroyTexture(gTexture);
	if (bgTxtr.texture != NULL)
		SDL_DestroyTexture(bgTxtr.texture);
	if(Hint.texture != NULL){
		SDL_DestroyTexture(Hint.texture);
	}
	for (int i = 0; i < 9; i++)
		SDL_DestroyTexture(digTxtrs[i].texture);
	Mix_FreeMusic( gMusic );
	gFont = NULL;
	gMusic = NULL;
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void handleKey (SDL_Event &event)
{
	int x, y;
    switch (event.type)
    {
        case SDL_KEYDOWN:
        {	if(data[sel_x][sel_y].used == 0)
			{ // khong cho nhap vao so tao random ban dau 
			int sym = event.key.keysym.sym; 
			switch( sym )
			{
				// movement
				case SDLK_s : 
				{
					if( Mix_PlayingMusic() == 0 )
									{
										//Play the music
										Mix_PlayMusic( gMusic, -1 );
									}
									//If music is being played
									else
									{
										//If the music is paused
										if( Mix_PausedMusic() == 1 )
										{
											//Resume the music
											Mix_ResumeMusic();
										}
										//If the music is playing
										else
										{
											//Pause the music
											Mix_PauseMusic();
										}
									}
									break;
				}
				case SDLK_p:
				{
					//Stop the music
					Mix_HaltMusic();
					break;
				}

				case SDLK_UP : 
				{
					if(data[sel_x][sel_y - 1].used == 0){
						sel_y--;
						if (sel_y < 0) sel_y = 8;
						break;
					}else{ 
						sel_y = sel_y;
						break;
					}
				}
				case SDLK_DOWN: 
				{
					if(data[sel_x][sel_y + 1].used == 0){
						sel_y++;
						if (sel_y > 8)	sel_y = 0;		
						break;
					}else{
						sel_y = sel_y;
						break;
					}
				}
				case SDLK_LEFT:
				{
					if(data[sel_x-1][sel_y].used == 0){
						sel_x--; 
						if (sel_x < 0) sel_x = 8;
						break;
					}else{
						sel_x = sel_x;
						break;
					}
				}
				case SDLK_RIGHT:
				{
					if(data[sel_x + 1][sel_y].used == 0){
						sel_x++;
						if (sel_x > 8)	sel_x = 0;
						break;
					}else{
						sel_x = sel_x;
						break;
					}
				}

				// case SDLK_KP_1: case SDLK_KP_2: case SDLK_KP_3:
				// case SDLK_KP_4: case SDLK_KP_5: case SDLK_KP_6:
				// case SDLK_KP_7:	case SDLK_KP_8:	case SDLK_KP_9:
				case SDLK_1: case SDLK_2: case SDLK_3: case SDLK_4: 
				case SDLK_5: case SDLK_6: case SDLK_7: case SDLK_8: case SDLK_9:
				{
					int prev = data [sel_x][sel_y].digit;
					
					int nr = sym - SDLK_1 + 1;
					if (prev == nr)
						break;
					
					int box = 3 * (sel_y / 3) + (sel_x / 3);
					
					if (prev != 0)
					{
						data [sel_x][sel_y].digit = 0;
						area_cols[sel_x].DigitRemoved (prev, data, sel_x, sel_y);
						area_rows[sel_y].DigitRemoved (prev, data, sel_x, sel_y);
						area_boxs[box].DigitRemoved (prev, data, sel_x, sel_y);
					}
					
					data [sel_x][sel_y].digit = nr;
					area_cols[sel_x].DigitAdded (nr, data, sel_x, sel_y);
					area_rows[sel_y].DigitAdded (nr, data, sel_x, sel_y);
					area_boxs[box].DigitAdded (nr, data, sel_x, sel_y);
					break;
				}
				case SDLK_BACKSPACE:
				{
					if (data [sel_x][sel_y].digit == 0)
						break;
					
					int nr = data [sel_x][sel_y].digit;
					data [sel_x][sel_y].digit = 0;
					
					int box = 3 * (sel_y / 3) + (sel_x / 3);
					
					area_cols[sel_x].DigitRemoved (nr, data, sel_x, sel_y);
					area_rows[sel_y].DigitRemoved (nr, data, sel_x, sel_y);
					area_boxs[box].DigitRemoved (nr, data, sel_x, sel_y);
					break;
				}
			}
		}
		else {
			break;
		}
		}
		
		}
			if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_LEFT))
            {
                int x = event.button.x;
                int y = event.button.y;
                // Calculate the row and column of the clicked cell
                int row = (y - 120) / CELL_SIZE;
                int col = (x - 220) / CELL_SIZE;
                // Make sure the click is inside the board
                if (row >= 0 && row <= 8 && col >= 0 && col <= 8)
                {
                    // Update the selected cell
                    sel_x = col;
                    sel_y = row;
				}
				if(x >= 600 && x <= 650  && y>=150 && y <= 180){
					Hintclick = true;
				}
			}
}			
			



int main( int argc, char* args[] )
{
	if (init () < 0)
		return -1;
	else {
		if(!LoadMedia()){
			return -1;
		}
	}

	RandomBoard();
	
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			//User requests quit
			if( event.type == SDL_QUIT )
			{
				quit = true;
			} 
			else if ( event.type == SDL_KEYDOWN )
			{
				handleKey ( event );
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				handleKey (event);
			}
		}

		//Fill the surface white
		SDL_SetRenderDrawColor ( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);
		drawBackground ();
		drawHint();
		SDL_SetRenderTarget(gRenderer, gTexture);
		SDL_RenderClear(gRenderer);
		drawGrid();
		drawInvalidCells();
		drawSelection ();
		drawText();
		if(Hint_number != 0){
			if(Hintclick == true){
				Hint_number--;
				hintBoard();
				Hintclick = false;
			}
		}
		SDL_SetRenderTarget(gRenderer, NULL);
		
		int size = 9 * CELL_SIZE + 2;
		SDL_Rect dst = { ( SCREEN_WIDTH - size ) / 2, ( SCREEN_HEIGHT - size ) / 2, size, size }; // thay doi vi tri
		gTextTexture.render(700,0);
		HintText.render(650, 200);
		SDL_RenderCopy(gRenderer, gTexture, NULL, &dst);
		
		SDL_RenderPresent(gRenderer);
		
		SDL_Delay (15);
	}
	close();

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
	
	return 0;
}