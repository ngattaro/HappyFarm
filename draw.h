#ifndef draw_h_
#define draw_h_
const int SCREEN_WIDTH  = 1040;
const int SCREEN_HEIGHT = 650;
const int startX = 50;
const int startY = 32;
const int itemLength = 74;
const std::string nameItems[numItems+2] = {"fruit1.png","fruit2.png","fruit3.png","fruit4.png","fruit5.png","cow.png","pig.png"};
void init();
void loadMedia(int ob, int x,int y);
void startGame();
void drawGame();
void close();
#endif draw_h_h;
