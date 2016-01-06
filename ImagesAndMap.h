#ifndef IMAGESANDMAP_H_INCLUDED
#define IMAGESANDMAP_H_INCLUDED

#include "DefinesAndConstants.h"

void DrawMap();
void SetMapLine(int *Line, int Type);
void DrawTexture(int x, int y, int spriteW, int spriteH, SDL_Texture *sprite);
void DrawText(int x, int y, int w, char *mesaj);
void SetPlayerPosition(int x, int y);
void InitializareHarta();
void ResetPlayer();
SDL_Texture *loadTexture(char *path);

void DrawText(int x, int y, int w, char *mesaj){
    Color.r = 255;
    Color.g = 255;
    Color.b = 255;
    Color.a = 255;
    Text = TTF_RenderText_Solid(Font, mesaj, Color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(Renderer,Text);
    DrawTexture(x,y - ScoreHeight, w, ScoreHeight, textTexture);
}

void ResetPlayer(){
    frog -> Y_lim = MAP_HEIGHT - 5;
    frog -> y = MAP_HEIGHT - 1;
    frog -> x = MAP_WIDTH/2 - 1;
    frog -> lastY = frog -> y;
    frog -> puncte = 0;
    frog -> life = 1;
    VitezaObstacol = 5;
}

void InitializareHarta(){
    int i;
    SetMapLine(Map[MAP_HEIGHT - 1], pamant);
    SetMapLine(Map[MAP_HEIGHT - 2], pamant);
    SetMapLine(Map[MAP_HEIGHT - 3], pamant);
    srand(time(NULL));
    for(i = MAP_HEIGHT - 4; i > 0; i--){
        int mapType = rand() % (sosea + 1);
        if(mapType == Map[i-1][0])
            mapType = (mapType + rand() + 1) % (sosea + 1);
        int number = rand() % 3 + 1;
        while(number > 0 && i > 0){
            SetMapLine(Map[i--], mapType);
            number--;
        }
    }
}

void SetPlayerPosition(int x, int y){
    if(x >= 0 && x < MAP_WIDTH)
        frog -> x = x;
    if(y >= 1 && y <MAP_HEIGHT)
        frog -> y = y;
}

void DrawTexture(int x, int y, int spriteW, int spriteH, SDL_Texture *sprite){
    SDL_Rect imgPos;
    imgPos.x = x;
    if(sprite == Sprites[score])
        imgPos.y = y;
    else
        imgPos.y = y + ScoreHeight;
    imgPos.w = spriteW;
    imgPos.h = spriteH;
    SDL_RenderSetClipRect(Renderer, NULL);
    SDL_RenderCopy(Renderer, sprite, NULL, &imgPos);
}

void DrawMap(){
    int i, j;
    for(i = 1; i < MAP_HEIGHT; i++)
        for(j = 0; j < MAP_WIDTH; j++){
            DrawTexture(j*SpriteSize, (i-1)*SpriteSize, SpriteSize, SpriteSize, Sprites[Map[i][j]]);
        }
}

SDL_Texture *loadTexture(char *path){
    SDL_Surface *img = IMG_Load(path);
    SDL_Texture *t = SDL_CreateTextureFromSurface(Renderer, img);
    SDL_FreeSurface(img);
    return t;
}

void SetMapLine(int *Line, int Type){
    int i = 0;
    for(i = 0; i < MAP_WIDTH; i++)
        Line[i] = Type;
}


#endif // IMAGESANDMAP_H_INCLUDED
