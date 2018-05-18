#include<bits/stdc++.h>
#include "initGame.h"
using namespace std;
//const int numCol = 8;
//const int numRow = 10;
//const int numItems = 5;
int items[numRow][numCol];
int selected[4];
int newItem()
{
    return rand()%numItems;
}
bool checkInit()
{
        bool check = false;
        for(int i = 0; i<numRow; i++)
        {
            for(int j = 0; j<numCol; j++)
            {
                if(j + 2 < numCol)
                {
                    if (items[i][j]==items[i][j+1]&& items[i][j]==items[i][j+2])
                    {
                        items[i][j] = newItem();
                        items[i][j+1] = newItem();
                        items[i][j+2] = newItem();
                        check = true;
                    }
                }
                if(i + 2 < numRow)
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
void initGame()
{
    srand(time(NULL));
    for (int i = 0;i<numRow;i++)
        for(int j = 0;j<numCol;j++)
            items[i][j] = newItem();
    while(checkInit()) checkInit();
}
