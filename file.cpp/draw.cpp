#include "draw.h"


int sel_x = 0;
int sel_y = 0;
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
			if (data[i][j].vldcnt != 0 && data[i][j].digit != ans.board[i][j])
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


void drawText () // dien so 
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			
			if (data[i][j].digit != 0)
			{
				
				LTexture ft = digTxtrs[data[i][j].digit - 1];				
				
				int x = i * CELL_SIZE;
				int y = j * CELL_SIZE;
	
				int startx = (i % 3 == 0 ? x + 2 : x + 1) + (CELL_SIZE - ft.getWidth()) / 2;
				int starty = (j % 3 == 0 ? y + 2 : y + 1) + (CELL_SIZE - ft.getHeight()) / 2;
				
				if(data[i][j].digit != ans.board[i][j]){
					if (!heart.isDecreased(i, j)){
                        heart.decrease();
                        heart.setDecreased(i, j, true);
					}
					int startx_color = i % 3 == 0 ? x + 2 : x + 1;
					int width = i % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
					int starty_color = j % 3 == 0 ? y + 2 : y + 1;
					int height = j % 3 == 0 ? CELL_SIZE - 2 : CELL_SIZE - 1;
					SDL_SetRenderDrawColor(gRenderer, COLOR_BAD_CELL);
					SDL_Rect rect = {startx_color, starty_color, width, height}; // vi tri in mau 
					SDL_RenderFillRect(gRenderer, &rect);

				}
				SDL_Rect renderQuad = {startx, starty, ft.getWidth(), ft.getHeight() }; // vi tri in chu 
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