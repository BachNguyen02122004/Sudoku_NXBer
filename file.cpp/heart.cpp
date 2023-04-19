#include "heart.h"

int number = 3;

void mang::SetNumber(const int& num)
{
    number = num;
}
bool mang::isDecreased(int i, int j)  {
    return decreased[i][j];
}

void mang::setDecreased(int i, int j, bool value) {
    decreased[i][j] = value;
}



bool mang::loadFromFile( std::string path )
{
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
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGBA( loadedSurface->format, 0xFF , 0xFF, 0xFF, 0xFF ) );

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

void mang::render( int x, int y )
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

void mang::AddPos(const int& pos)
{
    pos_list.push_back(pos);
}

void mang::show()
{
    if(number == pos_list.size())
    {
        for(int i = 0; i < pos_list.size(); i++)
        {
            render(pos_list[i], 0);
        }
    }
}
void mang::init()
{
    // number = 3;
    if(pos_list.size() > 0)
    {
        pos_list.clear();
    }

    AddPos(20);
    AddPos(60);
    AddPos(100);
}

void mang::decrease()
{
	if(pos_list.size() > 0) {
		number--;
		std::cout << number ;
		pos_list.pop_back();
	}
}

