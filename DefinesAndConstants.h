#ifndef DEFINESANDCONSTANTS_H_INCLUDED
#define DEFINESANDCONSTANTS_H_INCLUDED

#define FRAMES_PER_SECOND 60
#define FRAME_RATE 1000/FRAMES_PER_SECOND

#define false 0
#define true 1
#define MAP_WIDTH 20
#define MAP_HEIGHT 17
#define ScoreHeight 40
#define SPRITE_NUMBER 8
#define SpriteSize 40
#define VitezaObstacolMax 8

const int ScreenWidth = 800;
const int ScreenHeight = (640 + ScoreHeight);

enum{
    pamant,
    apa,
    sosea,
    player,
    masina,
    bustean,
    score,
    menu
};

typedef struct Player Player;
struct Player{
    int x, y;
    int Y_lim;
    int lastY;
    int puncte;
    int life;
};

int VitezaObstacol = 5;
int Map[MAP_HEIGHT][MAP_WIDTH];
int Timer;
int NrObstacole = 0;
int HighScore = 0;
/*
    0 = inca nu a inceput jocul
    1 = jocul a inceput sau player-ul a fos tlovit de un obstacol
*/
int menuNumber = 0;
Player *frog;

SDL_Window *Window = NULL;
SDL_Texture *Sprites[SPRITE_NUMBER];
SDL_Surface *Text = NULL;
SDL_Color Color;
SDL_Renderer *Renderer;
TTF_Font *Font;

#endif // DEFINESANDCONSTANTS_H_INCLUDED
