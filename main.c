#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_Functii.h"
#include "DefinesAndConstants.h"
#include "ImagesAndMap.h"

void GameLoop();
void LoadGameAssets();
void Game();


int main(int argc, char *argv[])
{
    if(!Init(&Window, &Renderer, &Timer)){
        printf("Eroare la initializare. SDL_Error: %s", SDL_GetError());
    }else{
        LoadGameAssets();
        GameLoop();
        Close(&Renderer, &Window,  Sprites);
        free(frog);
    }
    return 0;
}

void Game(){

    //desenez harta: sosea, pamant, apa
    DrawMap();

    //adauga obstacole pe harta
    SpawnObstacol();
    DrawObstacol();
    AnimateObstacol();
    StergeObstacol();

    switch(menuNumber){
        case 0: //jocul nu a inceput sau player-ul a fost lovit de un obstacol
            DrawTexture(200, 150, 400, 300, Sprites[menu]);
            DrawText(225, 280, 350, "Press Enter");
            DrawText(225, 330, 350, "To Start");
            break;
        case 1: //jocul a inceput
            //deseneaza player
            DrawTexture(frog -> x * SpriteSize, (frog -> y - 1) * SpriteSize, SpriteSize, SpriteSize, Sprites[player]);

            //pregeteste si deseneaza scorul
            char mesaj[10];
            itoa(frog -> puncte, mesaj, 10);
            DrawTexture(0,0, ScreenWidth, ScoreHeight, Sprites[score]);
            DrawText(0, 5, 200, "Score: ");
            DrawText(200, 5, 100, mesaj);

            DrawText(500, 5, 200, "Record: ");

            itoa(HighScore, mesaj, 10);
            DrawText(700, 5, 100, mesaj);

            //creste dificultatea jocului
            if(VitezaObstacol < VitezaObstacolMax)
                if(frog->puncte % 15 == 0)
                    VitezaObstacol += frog -> puncte /15;
            break;
        default: printf("meniul nu exista\n");
    }


    //verific daca exista coliziuni intre player si obstacole
    if(CheckColision()){
        menuNumber = 0;
        if(frog -> puncte > HighScore)
            HighScore = frog -> puncte;
        SDL_Delay(500);
        ResetPlayer();
        InitializareHarta();
        Obstacol *aux = Top;
        while(aux){
            Top = Top -> next;
            free(aux);
            aux = Top;
        }
    }


}

void GameLoop(){
    int quit = false;
    int coolDown = false;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT)
                quit = true;

            //verific input in timpul meniului
            if(menuNumber == 0 && e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_RETURN:
                        menuNumber = 1;
                        break;
                }

            }

            //verific coolDown
            if(e.type == SDL_KEYUP || frog -> life == 0 || menuNumber != 1)
                coolDown = false;
            //input pentru controlul playerului
            if(e.type == SDL_KEYDOWN && !coolDown && frog -> life > 0 && menuNumber == 1)
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        coolDown = true;
                        int i, j;
                        if(frog -> y - 1 >= frog -> Y_lim){
                            SetPlayerPosition(frog -> x, frog -> y - 1);
                        }else{
                            //generez o linie noua
                            int mapType = rand() % (sosea + 1);
                            if(mapType == Map[1][0])
                                mapType = (mapType + rand() + 1) % (sosea + 1);
                            SetMapLine(Map[0], mapType);
                            //generez masini pe prima linie daca este sosea

                            if(Map[0][0] == sosea){
                                int distanta = rand() % SpriteSize + 4 * SpriteSize;
                                for(i = 0; i < ScreenWidth; i+= distanta)
                                    AdaugaObstacol(i, 0, VitezaObstacol, masina);
                            }

                            if(Map[0][0] == apa){
                                int distanta = rand() % SpriteSize + 4 * SpriteSize;
                                for(i = 0; i < ScreenWidth; i+= distanta)
                                    AdaugaObstacol(i, 0, VitezaObstacol, bustean);
                            }

                            //mut liniile mai jos
                            for(i = MAP_HEIGHT - 1; i > 0; i--)
                                for(j = 0; j < MAP_WIDTH; j++)
                                    Map[i][j] = Map[i - 1][j];
                            //mut masinile mai jos
                            SetLine();
                        }
                        //puncte
                        if(frog -> y <= frog -> lastY){
                            frog -> puncte++;
                            frog -> lastY = frog -> y;
                        }

                        break;
                    case SDLK_DOWN:
                        coolDown = true;
                        SetPlayerPosition(frog -> x, frog -> y + 1);
                        break;
                    case SDLK_LEFT:
                        coolDown = true;
                        SetPlayerPosition(frog -> x - 1, frog -> y);
                        break;
                    case SDLK_RIGHT:
                        coolDown = true;
                        SetPlayerPosition(frog -> x + 1, frog -> y);
                        break;
                    default:printf("comanda gresita\n");
                }
        }

        if(SDL_GetTicks() - Timer >= FRAMES_PER_SECOND){
            Game();
            SDL_RenderPresent(Renderer);
            Timer = SDL_GetTicks();
        }
    }
}

void LoadGameAssets(){
    //alocare dinamica si initializare a unor variabile
    frog = (struct Player*)malloc(sizeof(struct Player));
    ResetPlayer();

    //incarcare imagini
    Sprites[pamant] = loadTexture("Images/pamant.png");
    Sprites[apa] = loadTexture("Images/apa.png");
    Sprites[sosea] = loadTexture("Images/sosea.png");
    Sprites[player] = loadTexture("Images/player.png");
    Sprites[masina] = loadTexture("Images/masina.png");
    Sprites[bustean] = loadTexture("Images/bustean.png");
    Sprites[score] = loadTexture("Images/score.png");
    Sprites[menu] = loadTexture("Images/menu.png");

    //initializare harta
    InitializareHarta();

    //initializare scor
    FILE *saveGame = NULL;
    saveGame = fopen("SaveGame.dat", "rb");
    if(saveGame != NULL){
        fread(&HighScore, sizeof(int), 1, saveGame);
    }else{
        HighScore = 0;
    }
    fclose(saveGame);
}
