#include<bits/stdc++.h>

#include<windows.h>

#include <SDL.h>
#include "game.h"
#include "draw.h"
using namespace std;

int Game::newItem()
{
    return rand()%numItems;
}
bool Game::checkInit()
{
        bool check = false;
        for(int i = 0; i<numCell; i++)
        {
            for(int j = 0; j<numCell; j++)
            {
                if(j + 2 < numCell)
                {
                    if (items[i][j]==items[i][j+1]&& items[i][j]==items[i][j+2])
                    {
                        items[i][j] = newItem();
                        items[i][j+1] = newItem();
                        items[i][j+2] = newItem();
                        check = true;
                    }
                }
                if(i + 2 < numCell)
                {
                    if (items[i][j]==items[i+1][j]&& items[i+2][j]==items[i][j])
                    {
                        items[i][j] = newItem();
                        items[i+1][j] = newItem();
                        items[i+2][j] = newItem();
                        check = true;
                    }
                }
            }
        }
        return check;
    }
void Game::initGame()
{
    srand(time(NULL));
    for (int i = 0;i<numCell;i++)
        for(int j = 0;j<numCell;j++)
            items[i][j] = newItem();
    while(checkInit()) checkInit();
    for(int i = 0; i<numCell; i++)
        for(int j = 0; j<numCell;j++)
    {
        desX[i][j] = startX + i*itemLength;
        desY[i][j] = startY + j*itemLength;
        posX[i][j] = startX + i*itemLength;
        posY[i][j] = startY + j*itemLength - itemLength*numItems;
    }
    score = 0;
    countSelected = 0;
    restart = false;
    endGame = false;
    timeLeft = timeGame;
    startTime = std::clock();
}

void Game::updateTouch(int mouseX, int mouseY)
{
        int row = (mouseX-startX)/(itemLength);
        int col = (mouseY-startY)/(itemLength);
        if (row<0 || col<0 || row>=numCell || col>=numCell) return;
        if (countSelected == 1)
        {
          //  cout<<row<<" "<<col<<endl;
            if((row == selected[0]-1 && col == selected[1]) ||
               (row == selected[0]+1 && col == selected[1]) ||
               (row == selected[0] && col == selected[1]-1) ||
               (row == selected[0] && col == selected[1]+1))
            {
                selected[2] = row;
                selected[3] = col;
                countSelected = 2;
                return;
            }
            else countSelected = 0;
        }
        else
        {
             selected[0] = row;
             selected[1] = col;
             countSelected = 1;
        }
}

int Game::horizontal(int x, int y)
{
    int i1 = y-1, i2 = y+1;
    if (items[x][y]<0) return 0;
    while(i1>=0 && items[x][y]==items[x][i1]) i1--;
    while(i2<numCell && items[x][y]==items[x][i2]) i2++;
    if(i2-i1-1>2)
    {
        for(int i = i1+1;i<i2;i++)
            if(i!= y) items[x][i] = -1;

        return i2-i1-1;
    }
    return 0;
}
int Game::vertical(int x, int y)
{
    if (items[x][y]<0) return 0;
    int i1 = x-1, i2 = x+1;
    while(i1>=0 && items[x][y]==items[i1][y]) i1--;
    while(i2<numCell && items[x][y]==items[i2][y]) i2++;
    if(i2-i1-1>2)
    {
        for(int i = i1+1;i<i2;i++)
            if (i != x) items[i][y] = -1;
        return i2-i1-1;
    }
    return 0;
}
bool Game::checkCanEat(int x, int y)
{

    int can1,can2;
    can1 = horizontal(x,y);
    can2 = vertical(x,y);
    if(can1 == 5 || can2 == 5) items[x][y] = numItems;
    else if (can1 != 0 && can2 != 0) items[x][y] = numItems+1;
    else if (can1 != 0 || can2 != 0) items[x][y] = -1;
    return (can1+can2 > 2);
}
void Game::eatStar(int x, int y,int color)
{
    for(int i = 0; i < numCell; i++)
                for(int j = 0; j < numCell; j++)
                    if (color == -1 || items[i][j] == color ) items[i][j] = -1;
}

bool Game::checkCanEat(int x, int y, int u, int v)
{
    if (items[x][y] == numItems && items[u][v] == numItems)
    {
        eatStar(x,y,-1);
        return true;
    }
    if (items[x][y] == numItems || items[u][v] == numItems)
    {
            int color = items[x][y]+ items[u][v] - numItems;
            if (color < 0 || color >= numItems) color = newItem();
            eatStar(x,y,color);
            items[x][y] = -1;
            items[u][v] = -1;
        return true;
    }
    bool r = checkCanEat(x,y);
    r = checkCanEat(u,v) || r;
    return r;
}
void Game::eatItems(){
    //disappear fruits
    for(int i = 0; i<numCell; i++)
        for(int j = 0; j<numCell; j++)
        {
            if (items[i][j] < 0 )
            {
                score++;
            }
        }
}
void Game::swapItems(int x, int y, int u, int v)
{
    desX[x][y] = posX[u][v];
    desY[x][y] = posY[u][v];
    desX[u][v] = posX[x][y];
    desY[u][v] = posY[x][y];
    drawGame();
    swap(posX[x][y],posX[u][v]);
    swap(posY[x][y],posY[u][v]);
    desX[x][y] = posX[x][y];
    desY[x][y] = posY[x][y];
    desX[u][v] = posX[u][v];
    desY[u][v] = posY[u][v];
    swap(items[x][y],items[u][v]);
}
void Game::generateItems()
{
    //drop
    int c1, r1, r2;
    for(c1 = 0; c1<numCell; c1++)
    {
        r1 = numCell-1;
        while(r1>=0)
        {
            while(r1>=0 && (items[c1][r1] != -1) ) r1--;
            r2 = r1-1;
            while(r2>=0 && (items[c1][r2]< 0)) r2--;
            if (r2<0) break;
            swap(items[c1][r1],items[c1][r2]);
            posX[c1][r1] = posX[c1][r2];
            posY[c1][r1] = posY[c1][r2];
        }
    }

    for(r1 = numCell; r1>=0;r1--)
        {
            r2 = -1;
            for(c1 = numCell-1; c1>=0; c1--)
            if (items[r1][c1] == -1 )
            {

                posX[r1][c1] = startX + r1*itemLength;
                if (r2 == -1 && items[r1][c1] == -1) r2 = itemLength;
                else if(items[r1][c1] == -1) r2 +=itemLength;

                posY[r1][c1] = startY - r2;
                items[r1][c1] = newItem();
            }
        }
    drawGame();
}

bool Game::checkClear()
{
    int cnt, row, col;
    bool can = false;
    for(int i=0;i<numCell;i++)
        for(int j=0;j<numCell;j++)
            if(items[i][j]>=0)
    {
        bool d1 = false,d2=false, d3 =false, d4=false;
        if(j>1 &&  items[i][j-1]==items[i][j] && items[i][j-2]==items[i][j]) d1 = true;
        if(j<numCell-2 &&  items[i][j+1]==items[i][j] && items[i][j+2]==items[i][j]) d2 = true;
        if(i>1 && items[i-1][j]==items[i][j] && items[i-2][j]==items[i][j]) d3 = true;
        if(i<numCell-2 &&  items[i+1][j]==items[i][j] && items[i+2][j]==items[i][j]) d4 = true;
        if(d1==true && d3==true && d2==false)
        {
            items[i][j-1]= -1; items[i][j-2]= -1; items[i][j] = numItems+1;
            items[i-1][j]= -1; items[i-2][j]= -1;
            can = true;
        }
        else if(d1==true && d4==true && d2==false)
        {
            items[i][j-1]= -1; items[i][j-2]= -1; items[i][j] = numItems+1;
            items[i+1][j]= -1; items[i+2][j]= -1;
            can = true;
        }
        else if(d2==true && d4==true && d1==false)
        {
            items[i][j+1]= -1; items[i][j+2]= -1; items[i][j] = numItems+1;
            items[i+1][j]= -1; items[i+2][j]= -1;
            can = true;
        }
        else if(d2==true && d3==true && d1==false)
        {
            items[i][j+1]= -1; items[i][j+2]= -1; items[i][j] = numItems+1;
            items[i-1][j]= -1; items[i-2][j]= -1;
            can = true;
        }
    }

    for(row = 0; row < numCell; row++)
    {
        cnt = 1;
        for(col = 1;col <= numCell; col++)
            if (col<numCell && items[row][col]>=0&&items[row][col] == items[row][col - 1]) cnt++;
            else
            {
                if (cnt>2)
                {
                    can = true;
                    for(int i = col - 1; i>=col-cnt;i--) items[row][i] = -1;
                    if (cnt == 5) items[row][col-cnt] = numItems;
                }
                cnt = 1;
            }


    }
     for(col = 0; col < numCell; col++)
    {
        cnt = 1;
        for(row = 1;row <= numCell; row++)
            if (row<numCell && items[row][col]>=0 && items[row][col] == items[row-1][col]) cnt++;
            else
            {
                if (cnt>2)
                {
                    can = true;
                    for(int i = row - 1; i>=row-cnt;i--) items[i][col] = -1;
                    if (cnt == 5) items[row-cnt][col] = numItems;
                }
                cnt = 1;
            }
    }
    return can;
}
void Game::updateBoard()
{
    while(1)
    {
                eatItems();
                drawGame();
                Sleep(timeSleep);
                generateItems();
                if (checkClear()==false) break;

    }
}
void Game::eatBoom(int x,int y)
{
    for(int i = 0;i<9;i++)
    {
        if(x+hx[i]>=0 && x+hx[i]<numCell && y+hy[i]>=0 && y+hy[i]<numCell)
        {
            if(items[x+hx[i]][y+hy[i]] == numItems+1)
            {
                 items[x+hx[i]][y+hy[i]]=-1;
                 eatBoom(x+hx[i],y+hy[i]);
            }
            else if(items[x+hx[i]][y+hy[i]] == numItems)
            {
                items[x+hx[i]][y+hy[i]]=-1;
                eatStar(x+hx[i],y+hy[i],rand()%numItems);
            }
            else
            items[x+hx[i]][y+hy[i]]=-1;

        }
    }
}



void Game::updateGame()
{
    int x = selected[0], y = selected[1], u = selected[2], v = selected[3];
    if(countSelected == 2)
    {
        countSelected = 0;
        swapItems(x,y,u,v);


        if (checkCanEat(x,y,u,v))
        {
                updateBoard();
        }
        else
        {
            swapItems(x,y,u,v);
        }
    }
    else if(countSelected == 1 && items[x][y] == numItems+1)
    {
        countSelected = 0;
        eatBoom(x,y);
         updateBoard();

    }
}
void Game::run()
{
    int mouseX, mouseY;
    SDL_Event e;
    init();
    startGame();

    const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

    while( !quit )
    {
        frameStart = SDL_GetTicks();

		while( SDL_PollEvent( &e ) != 0 )
		{
               //cout<<mouseX<<" "<<mouseY<<endl;
			if( e.type == SDL_QUIT ) quit = true;
			if(played || ( e.type == SDL_MOUSEBUTTONDOWN && mouseX>=375 && mouseX<=661 &&mouseY>=292 && mouseY<=354))
            {
                played = true;
                if (restart)
                {
                    initGame();
                    e.type = NULL;
                }


                if (!endGame) drawGame();
                if( e.type == SDL_MOUSEBUTTONDOWN && mouseX>=712 && mouseX<=982 &&mouseY>=301 && mouseY<=353)
                {
                    restart = true;

                }
                if(!endGame && e.type == SDL_MOUSEBUTTONDOWN)
                {
                    updateTouch(mouseX, mouseY);
                    e.type = NULL;
                }

                if(!endGame) updateGame();
            }
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime)
            {
                SDL_Delay(frameDelay - frameTime);
            }


            if( e.type == SDL_MOUSEMOTION){
                mouseX = e.button.x;
                mouseY = e.button.y;
			}

        }
        if (played)
        {
            timeLeft = timeGame  - ((std::clock() - startTime) / (double)CLOCKS_PER_SEC);
            if (!endGame && timeLeft <=0)
            {
                endGame = true;
                drawGame();
            }
            if (!endGame) drawGame();
        }

    }

	close();

}
