#ifndef HEART_H
#define HEART_H

#include "Function.h"

extern  int number ;
class mang { 
	public : 
	void SetNumber(const int& num);
	bool loadFromFile( std::string path );
	void render( int x, int y );
	void AddPos(const int& pos);
	void init();
	void decrease();
	void show();
	bool isDecreased(int i, int j);
	void setDecreased(int i, int j, bool value);

	private : 
	SDL_Texture* mTexture;
	int mWidth, mHeight;
	std::vector<int> pos_list; 
	bool decreased[9][9];
};

#endif 
