#ifndef game_h_
#define game_h_
const int hx[9] = {0,0,0,1,1,1,-1,-1,-1};
const int hy[9] = {-1,0,1,-1,0,1,-1,0,1};
const int numCell = 8;
const int numItems = 5;
const int timeSleep = 200;
const int itemsSpeed = 1;
const int timeGame = 300;
class Game{
    int items[numCell][numCell];
    int posX[numCell][numCell];
    int posY[numCell][numCell];
    int desX[numCell][numCell];
    int desY[numCell][numCell];
    int selected[4];
    long long score = 0;
    int startTime;
    int timeLeft;
    int countSelected = 0;
    bool quit = false;
    bool endGame = false;
    bool restart = true;
    bool played = false;
public:
    void initGame();
    void run();
    void drawGame();
    void updateTouch(int mouseX, int mouseY);
    void updateGame();
private:
    int newItem();
    bool checkInit();
    bool checkCanEat(int x, int y);
    int horizontal(int x, int y);
    int vertical(int x, int y);
    void eatStar(int x, int y,int color);
    bool checkCanEat(int x, int y, int u, int v);
    void eatItems();
    void generateItems();
    bool checkClear();
    void eatBoom(int x,int y);
    void updateBoard();
    void swapItems(int x, int y, int u, int v);
    void printScoreAndTime();

};
#endif  game_h_
