#include<bits/stdc++.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "game.h"
#include "draw.h"
using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* surface[12];
SDL_Texture*  texture[12];
TTF_Font *font = NULL;
SDL_Color textColor = {255, 255, 255};
Mix_Music *gMusic = NULL;


void init()
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) >= 0 )

	{
		window = SDL_CreateWindow( "HAPPY FARM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window != NULL )
		{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer != NULL )
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		}
		Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

	}
	for(int i = 0; i<numItems+2; i++)
    {
        surface[i] = IMG_Load( nameItems[i].c_str());
        texture[i] = SDL_CreateTextureFromSurface( renderer, surface[i] );
    }
    surface[8] = IMG_Load("background.png");
    texture[8] = SDL_CreateTextureFromSurface( renderer, surface[8] );
    surface[9] = IMG_Load("choose.png");
    texture[9] = SDL_CreateTextureFromSurface( renderer, surface[9] );
    surface[10] = IMG_Load("start.png");
    texture[10] = SDL_CreateTextureFromSurface( renderer, surface[10] );
    surface[11] = IMG_Load("end.png");
    texture[11] = SDL_CreateTextureFromSurface( renderer, surface[11] );

    TTF_Init();
    font = TTF_OpenFont("staravenue.ttf",50);

     gMusic = Mix_LoadMUS( "music.mp3" );

}

void loadMedia(int ob, int x,int y)
{
    SDL_Rect dest = {x, y,surface[ob]->w,surface[ob]->h};
    SDL_RenderCopy(renderer, texture[ob], NULL, &dest);
}
void startGame()
{
    loadMedia(10,0,0);

    SDL_RenderPresent( renderer );
}
void printText(string text,int x, int y)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(),textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect dest;
    dest.x = x+(15*(10-text.size())/2);
    dest.y = y;
    dest.w = 15*text.size();
    dest.h = 30;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
 	SDL_FreeSurface(surface);
}
void Game::printScoreAndTime()
{
    char buffer[2];
    itoa(score,buffer,10);
    std::string text = string(buffer);
    printText(text,825,223);
    int m = timeLeft/60;
    int s = timeLeft - m*60;
    itoa(m,buffer,10);
    text = string(buffer);
    itoa(s,buffer,10);

    if (s <10) text += " : 0" +string(buffer);
    else text += " : " +string(buffer);
    printText(text, 825, 145);
}
int gcd(int x,int y)
{
    if (x == y || x==-y) return abs(x);
    if (x*y==0) return abs(x+y);
    return gcd(x%y,y%x);
}
void Game::drawGame()
{
    if( Mix_PlayingMusic() == 0 )
    {
        Mix_PlayMusic( gMusic, -1 );
    }
    timeLeft = timeGame   - ((std::clock() - startTime) / (double)CLOCKS_PER_SEC);
    bool check = false;
    loadMedia(8,0,0);
     printScoreAndTime();
    for(int i = 0;i<numCell;i++)
        for(int j =0; j<numCell; j++)
            if(posY[i][j] != desY[i][j] || posX[i][j] != desX[i][j])
            {

                check = true;
                int drX = desX[i][j] - posX[i][j];
                int drY = desY[i][j] - posY[i][j];
                int d = gcd(drX,drY);
                drX = drX/d;
                drY = drY/d;
                posX[i][j] += drX*itemsSpeed;
                posY[i][j] += drY*itemsSpeed;
            }
    for(int i = 0;i<numCell;i++)
        for(int j =0; j<numCell; j++)
        {
            int typeItems = items[i][j];
            if (typeItems != -1) loadMedia(typeItems,posX[i][j],posY[i][j]);
        }
    if (countSelected == 1)
    {
        loadMedia(9,posX[selected[0]][selected[1]]-2,posY[selected[0]][selected[1]]-2);

    }

    if (endGame) loadMedia(11,startX,startY);
    SDL_RenderPresent( renderer );
  //  Sleep(20);
    if (check) drawGame();
}

void close()
{
	for(int i = 0; i<12;i++)
        SDL_DestroyTexture(texture[i]);

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}




