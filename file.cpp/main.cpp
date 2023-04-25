#include "Function.h"
#include "LTexture.h"
#include "Area.h"
#include "CellData.h"
#include "heart.h"
#include "show.h"
#include "draw.h"




SDL_Surface* loadSurface (const std::string &file);

LTexture bgTxtr;
LTexture gStart;
LTexture Hint;
LTexture gTextTexture;
LTexture HintText;
LTexture gYourtime;
LTexture Menu;
LTexture Gameover;
LTexture Winner;
LTexture Muc_do;
LTexture digTxtrs [9] ;
mang heart;

bool Hintclick = false;
int Hint_number = 3;
// int sel_x = 0;
// int sel_y = 0;
int num2 = 0;
// int number = 3;
// bool quit = false;

CellData data [9][9] ;

Area area_rows [9];
Area area_cols [9];
Area area_boxs [9];


SudokuBoard ans, su;

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
	
	
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return -1;
	}
	
	
	icon = loadSurface ("res/sudoku_icon.png");
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
	
	
	if(!heart.loadFromFile("res/so_mang.png")){
		success = false;
	}
	if(!gStart.loadFromFile("res/start.png")){
		success = false;
	}
	heart.init();
	
	if(!Muc_do.loadFromFile("res/anh1.png")){
		success = false;
	}
	if(!Menu.loadFromFile( "res/background.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
	if(!bgTxtr.loadFromFile("res/phong_nen.png")){
		success = false;
	}
	
	if(!Hint.loadFromFile("res/hint.png")){
		success = false;
	}

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
					LTexture ft;
					ft.texture = texture;
					ft.mWidth = textSurface->w;
					ft.mHeight = textSurface->h;
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
	
	gFont = TTF_OpenFont( "res/chu.ttf", 18);
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}


	gGameover = Mix_LoadWAV("music_game_over.wav");
	if(gGameover == NULL){
		success = false;
	}
	gWin = Mix_LoadWAV("You-win.wav");
	if(gWin == NULL){
		success = false;
	}
	
	gMusic = Mix_LoadMUS( "nhac_game.mp3" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success ;
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
	bgTxtr.render(0, 0);
}

void drawHint(){
	Hint.render(600, 150);
}

void RandomBoard(){
	SDL_Event event;
	srand(time(NULL));
	int random = (rand() % 334) + 1;
   
	int check = showDifficulty(event);
	if(check == 1) quit = true;
	if(check == 3){
		std::string chosenLine;
		std::vector<std::string> chosenProblem;
		std::vector<std::string> chosenAns;
		std::ifstream problemData("sudoku_easy.csv");
    	for(int i = 1; std::getline(problemData, chosenLine); i++)
    	{
        	if(i == random) break;
    	}
		if (chosenLine.size() >= 83) {
			chosenProblem.push_back(chosenLine.substr(0, 81));
			chosenAns.push_back(chosenLine.substr(82));
		}

		for(int i = 0; i<9; i++){
			for(int j = 0; j<9; j++){
				int problemVal = std::stoi(chosenProblem[0].substr(9 * i + j, 1));
				int ansVal = std::stoi(chosenAns[0].substr(9 * i + j, 1));
				su.board[i][j] = problemVal;
				ans.board[i][j] = ansVal;
			}
		}
	}

	if(check == 4){
		std::string chosenLine;
		std::vector<std::string> chosenProblem;
		std::vector<std::string> chosenAns;
		std::ifstream problemData("sudoku_medium.csv");
    	for(int i = 1; std::getline(problemData, chosenLine); i++)
    	{
        	if(i == random) break;
    	}
		if (chosenLine.size() >= 83) {
			chosenProblem.push_back(chosenLine.substr(0, 81));
			chosenAns.push_back(chosenLine.substr(82));
		}

		for(int i = 0; i<9; i++){
			for(int j = 0; j<9; j++){
				int problemVal = std::stoi(chosenProblem[0].substr(9 * i + j, 1));
				int ansVal = std::stoi(chosenAns[0].substr(9 * i + j, 1));
				su.board[i][j] = problemVal;
				ans.board[i][j] = ansVal;
			}
		}
	}
	if(check == 5){
		std::string chosenLine;
		std::vector<std::string> chosenProblem;
		std::vector<std::string> chosenAns;
		std::ifstream problemData("sudoku_hard.csv");
    	for(int i = 1; std::getline(problemData, chosenLine); i++)
    	{
        	if(i == random) break;
    	}
		if (chosenLine.size() >= 83) {
			chosenProblem.push_back(chosenLine.substr(0, 81)); // substr tach chuoi 
			chosenAns.push_back(chosenLine.substr(82));
		}

		for(int i = 0; i<9; i++){
			for(int j = 0; j<9; j++){
				int problemVal = std::stoi(chosenProblem[0].substr(9 * i + j, 1));
				int ansVal = std::stoi(chosenAns[0].substr(9 * i + j, 1));
				su.board[i][j] = problemVal;
				ans.board[i][j] = ansVal;
			}
		}
	}

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

void hintBoard(){ //da fix tro vao dau thi hint o do va dien duoc vao o da khoa
		if(data[sel_x][sel_y].digit == 0 || data[sel_x][sel_y].digit != ans.board[sel_x][sel_y]){
		data[sel_x][sel_y].digit  = ans.board[sel_x][sel_y];
		data[sel_x][sel_y].used = 1;
		int box = 3 * (sel_y / 3) + (sel_x / 3);
		area_cols[sel_x].DigitAdded (data[sel_x][sel_y].digit, data, sel_x, sel_y);
		area_rows[sel_y].DigitAdded (data[sel_x][sel_y].digit, data, sel_x, sel_y);
		area_boxs[box].DigitAdded (data[sel_x][sel_y].digit, data, sel_x, sel_y);
		return;
		}
		else if(data[sel_x][sel_y].used == 1)
		{
			Hint_number++;
		}
	}

// 	for(int i = 0; i<9; i++){
// 		for(int j = 0; j<9; j++){
// 			if(data[i][j].digit == 0 || data[i][j].digit != ans.board[i][j]){
// 				data[i][j].digit  = ans.board[i][j];
// 				data[i][j].used = 1;
// 				int box = 3 * (j / 3) + (i / 3);
// 				area_cols[i].DigitAdded (data[i][j].digit, data, i, j);
// 				area_rows[j].DigitAdded (data[i][j].digit, data, i, j);
// 				area_boxs[box].DigitAdded (data[i][j].digit, data, i, j);
// 				return;
// 			}
// 		}
// 	}
// }

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
				case SDLK_1: case SDLK_KP_1:
				case SDLK_2: case SDLK_KP_2:
				case SDLK_3: case SDLK_KP_3:
				case SDLK_4: case SDLK_KP_4:
				case SDLK_5: case SDLK_KP_5:
				case SDLK_6: case SDLK_KP_6:
				case SDLK_7: case SDLK_KP_7:
				case SDLK_8: case SDLK_KP_8:
				case SDLK_9: case SDLK_KP_9:
				{
					int prev = data [sel_x][sel_y].digit;
					int nr = 0;
    				if (sym >= SDLK_1 && sym <= SDLK_9) {
       					 nr = sym - SDLK_1 + 1;
   					} else if (sym >= SDLK_KP_1 && sym <= SDLK_KP_9) {
        				 nr = sym - SDLK_KP_1 + 1;
					}
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
			
void resetBoard(){ // reset all 
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9; j++){
			data[i][j].digit = 0;
			data[i][j].used = 0;
			data[i][j].vldcnt = 0;
			heart.setDecreased(i, j, false);
		}
	}
}

bool checkBoard(){
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9; j++){
			if(data[i][j].digit == 0 || data[i][j].digit != ans.board[i][j]) return false;
		}
	}
	return true;
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

	SDL_Event event;
	int check_menu = showMenu(event);
	if(check_menu == 1) quit = true;

	Mix_PlayMusic(gMusic, -1);
	num2 = (SDL_GetTicks()/1000);
	RandomBoard();
	bool play_again = false;
	

	while (!quit)
	{
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

		SDL_SetRenderDrawColor ( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);
		drawBackground ();
		drawHint();
		SDL_SetRenderTarget(gRenderer, gTexture);
		SDL_RenderClear(gRenderer);
		drawGrid();
		drawInvalidCells();
		drawSelection ();
		// checkHeart();
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
		SDL_Rect dst = { ( SCREEN_WIDTH - size ) / 2, ( SCREEN_HEIGHT - size ) / 2, size, size }; 
		// dieu chinh mang
		heart.show();
		// xu ly time
		int  minute = 0;
		
		int num1 =(SDL_GetTicks()/1000) - num2;
		
		minute =+ num1 / 60;
		num1 %= 60;	
		
	std ::string second = std::to_string(num1);
	
	std ::string minute1 = std::to_string(minute);
	if(second.size() == 1 ){
		second[0] = '0';
		second += std::to_string(num1);
	}
	if(minute1.size() == 1 ){
		minute1[0] = '0';
		minute1 += std::to_string(minute);
	}

		
		std::string abc = minute1 + ":" + second;
		gTextTexture.loadFromRenderedText( abc, gTextTexture.GetColor() );
		gTextTexture.render(650,0);

		std::string YourTime = "Your time:  " + abc;
		gYourtime.loadFromRenderedText(YourTime, gYourtime.GetColor());
		// xu ly hintText
		std::string a = "Hint:" + std::to_string(Hint_number);
		HintText.loadFromRenderedText(a,HintText.GetColor()); // hien thi ten 
		HintText.render(650, 170);
		if(checkBoard())
		{
			Winner.loadFromFile("res/win.png");
			Mix_PlayChannel(-1, gWin, 0);
			Mix_FreeMusic( gMusic );
			gMusic = NULL;
			int win= showWin(event);
			if(win == 0) 
			{
				quit = false;
    			play_again = true;
    			number = 3;
    			Hint_number = 3;
				heart.init();
    			resetBoard();
    			RandomBoard();
    			num2 = SDL_GetTicks() / 1000;
				gMusic = Mix_LoadMUS( "nhac_game.mp3");
				Mix_PlayMusic(gMusic, -1);
			}
			
			else {
				quit = true;
				play_again = false;
			}
			
		}
		
		if(number == 0) {
			// SDL_SetRenderDrawColor ( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			Gameover.loadFromFile("res/game_over1.png");
			Mix_PlayChannel(-1, gGameover, 0 );
			Mix_FreeMusic( gMusic );
			gMusic = NULL;
			int check_Gameover = showGameOver(event);
			if(check_Gameover == 1) {
				quit = true;
				play_again = false;
			}
			else{
				quit = false;
    			play_again = true;
    			number = 3;
    			Hint_number = 3;
				heart.init();
    			resetBoard();
    			RandomBoard();
    			num2 = SDL_GetTicks() / 1000;
				gMusic = Mix_LoadMUS( "nhac_game.mp3");
				Mix_PlayMusic(gMusic, -1);
			}
		}
		
		
	
		SDL_RenderCopy(gRenderer, gTexture, NULL, &dst);
		
		SDL_RenderPresent(gRenderer);
		
		SDL_Delay (16);
	}

	close();

	//Destroy window
	SDL_DestroyWindow( window);
	 //Quit SDL subsystems
    SDL_Quit();

    return 0;
}
