#ifndef SDL_FUNCTII_H_INCLUDED
#define SDL_FUNCTII_H_INCLUDED

#include "DefinesAndConstants.h"
#include "Obstacol.h"

int Init();
void Close();

int Init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        printf("eroare la initializare video: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        Window = SDL_CreateWindow("Frogger",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
        if(Window == NULL)
        {
            printf("Window creation error: %s\n", SDL_GetError());
            return false;
        }
        else
        {
            Renderer = SDL_CreateRenderer(Window, -1, 0);
            Timer = SDL_GetTicks();
        }
    }
    if( TTF_Init() == -1 ){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }else{
        Font = TTF_OpenFont("font.ttf", 28);
    }
    return true;
}

void Close(){
    FILE *saveGame = NULL;
    saveGame = fopen("SaveGame.dat", "wb");
    fwrite(&HighScore, sizeof(int), 1, saveGame);
    fclose(saveGame);
    int i;
    //sterge texturi
    for(i = 0; i < SPRITE_NUMBER; i++)
        SDL_DestroyTexture(Sprites + i);
    //sterg player
    free(frog);
    //sterge obstacole
    Obstacol *aux = Top;
    while(aux){
        Top = Top -> next;
        free(aux);
        aux = Top;
    }
    TTF_CloseFont(Font);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

#endif // SDL_FUNCTII_H_INCLUDED
