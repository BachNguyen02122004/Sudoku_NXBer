#include "show.h"
#include "Function.h"
#include "LTexture.h"
bool quit = false;


bool checkpositionforrect(const int& x, const int& y, SDL_Rect rect)
{
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
    {
        return true;
    }
        return false;
}

int showMenu(SDL_Event e)
{
    SDL_Rect item[2];
    item[0].x = 328;
    item[0].y = 270;
    item[1].x = 328;
    item[1].y = 370;

    int x, y;
    LTexture text_menu[2];
	text_menu[0].loadFromFile("res/start.png");
    text_menu[0].confirmOx_Oy(item[0].x, item[0].y);
    text_menu[0].SetRect();

    text_menu[1].loadFromFile("res/quit.png");
    text_menu[1].confirmOx_Oy(item[1].x, item[1].y);
    text_menu[1].SetRect();

    bool Selected[2] = {0, 0};
    
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                quit = true;
                break;
                case SDL_MOUSEMOTION:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkpositionforrect(x, y, text_menu[i].getRect()))
                        {
                            if(Selected[i] == 0)
                            {
                                Selected[i] = 1;
                                text_menu[i].SetColor(1);
                            }
                        }
                        else
                        {
                            if(Selected[i] == 1)
                            {
                                Selected[i] = 0;
                                text_menu[i].SetColor(2);
                            }
                        }
                    }
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkpositionforrect(x, y, text_menu[i].getRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
                case SDL_KEYDOWN:
                {
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 1;
                    }
                }
                default:
                break;
            }
        }
        SDL_RenderClear( gRenderer );

        Menu.render(0, 0);
        for(int i = 0; i < 2; i++)
        {
            if(i == 0) text_menu[i].loadFromFile("res/start.png");
			if(i == 1) text_menu[i].loadFromFile("res/quit.png");
            text_menu[i].render(text_menu[i].getOx(), text_menu[i].getOy());
        }

        SDL_RenderPresent(gRenderer);
    }
	
    return 1;
}
int showDifficulty(SDL_Event e)
{
    SDL_Rect item[3];
    item[0].x = 300;
    item[0].y = 250;
    item[1].x = 300;
    item[1].y = 350;
	item[2].x = 300;
	item[2].y =	450;
	
    int x, y;
    LTexture text_choose[3];
	text_choose[0].loadFromFile("res/easy1.png");
	text_choose[0].confirmOx_Oy(item[0].x, item[0].y);
	text_choose[0].SetRect();

   text_choose[1].loadFromFile("res/medium1.png");
    text_choose[1].confirmOx_Oy(item[1].x, item[1].y);
    text_choose[1].SetRect();

   	text_choose[2].loadFromFile("res/hard1.png");
    text_choose[2].confirmOx_Oy(item[2].x, item[2].y);
   text_choose[2].SetRect();
    
bool Selected[3] = {0, 0, 0};
    
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                quit = true;
                break;
                case SDL_MOUSEMOTION:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 3; i++)
                    {
                        if(checkpositionforrect(x, y, text_choose[i].getRect()))
                        {
                            if(Selected[i] == 0)
                            {
                                Selected[i] = 1;
                                text_choose[i].SetColor(1);
                            }
                        }
                        else
                        {
                            if(Selected[i] == 1)
                            {
                                Selected[i] = 0;
                                text_choose[i].SetColor(2);
                            }
                        }
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 3; i++)
                    {
                        if(checkpositionforrect(x, y, text_choose[i].getRect()))
                        {
                            return i + 3;
                        }
                    }
                }
                break;
               
                break;
            }
        }
        SDL_RenderClear( gRenderer );

        Menu.render(0, 0);

        for(int i = 0; i < 3; i++)
        {
            if(i == 0) text_choose[0].loadFromFile("res/easy1.png");
            if(i == 1) text_choose[1].loadFromFile("res/medium1.png");
            if(i == 2) text_choose[2].loadFromFile("res/hard1.png");
            text_choose[i].render(text_choose[i].getOx(), text_choose[i].getOy());
        }

        SDL_RenderPresent(gRenderer);
    }
    return 1;
}


int showGameOver(SDL_Event e)
{
    SDL_Rect item[2];
    item[0].x = 328;
    item[0].y = 400;
    item[1].x = 328;
    item[1].y = 500;

    int x, y;
    LTexture text_GameOver[2];
   
    text_GameOver[0].loadFromFile("res/start.png");
   	text_GameOver[0].confirmOx_Oy(item[0].x, item[0].y);
    text_GameOver[0].SetRect();

    text_GameOver[1].loadFromFile("res/quit.png");
    text_GameOver[1].confirmOx_Oy(item[1].x, item[1].y);
    text_GameOver[1].SetRect();

    bool Selected[2] = {0, 0};
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                quit = true;
                break;
                case SDL_MOUSEMOTION:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkpositionforrect(x, y, text_GameOver[i].getRect()))
                        {
                            if(Selected[i] == 0)
                            {
                                Selected[i] = 1;
                                text_GameOver[i].SetColor(1);
                            }
                        }
                        else
                        {
                            if(Selected[i] == 1)
                            {
                                Selected[i] = 0;
                               text_GameOver[i].SetColor(2);
                            }
                        }
                    }
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkpositionforrect(x, y, text_GameOver[i].getRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
                case SDL_KEYDOWN:
                {
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 1;
                    }
                }
                default:
                break;
            }
        }
        SDL_RenderClear( gRenderer );
        Gameover.render(0, 0);
        for(int i = 0; i < 2; i++)
        {
            if(i == 0) text_GameOver[i].loadFromFile("res/start.png");
            if(i == 1) text_GameOver[i].loadFromFile("res/quit.png");
           text_GameOver[i].render(text_GameOver[i].getOx(), text_GameOver[i].getOy());
        }

        SDL_RenderPresent(gRenderer);
    }
	
    return 1;
}

int showWin(SDL_Event e)
{
    SDL_Rect item[2];
    item[0].x = 328;
    item[0].y = 400;
    item[1].x = 328;
    item[1].y = 500;

    int x, y;
    LTexture text_win[2];
  
	text_win[0].loadFromFile("res/start.png");
    text_win[0].confirmOx_Oy(item[0].x, item[0].y);
    text_win[0].SetRect();

    
    text_win[1].loadFromFile("res/quit.png");
    text_win[1].confirmOx_Oy(item[1].x, item[1].y);
    text_win[1].SetRect();

    bool Selected[2] = {0, 0};
   
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                quit = true;
                break;
                case SDL_MOUSEMOTION:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkpositionforrect(x, y, text_win[i].getRect()))
                        {
                            if(Selected[i] == 0)
                            {
                                Selected[i] = 1;
                                text_win[i].SetColor(1);
                            }
                        }
                        else
                        {
                            if(Selected[i] == 1)
                            {
                                Selected[i] = 0;
                                text_win[i].SetColor(2);
                            }
                        }
                    }
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_GetMouseState( &x, &y );
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkpositionforrect(x, y, text_win[i].getRect()))
                        {
                            return i;
                        }
                    }
                }
                break;
                case SDL_KEYDOWN:
                {
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 1;
                    }
                }
                default:
                break;
            }
        }
        SDL_RenderClear( gRenderer );

        Winner.render(200, 140);
		gYourtime.render(250, 120);
        for(int i = 0; i < 2; i++)
        {
            if(i == 0) text_win[i].loadFromFile("res/start.png");
            if(i == 1) text_win[i].loadFromFile("res/quit.png");;
            text_win[i].render(text_win[i].getOx(), text_win[i].getOy());
        }

        SDL_RenderPresent(gRenderer);
    }
	
    return 1;
}




