#ifndef OBSTACOL_H_INCLUDED
#define OBSTACOL_H_INCLUDED

#include "DefinesAndConstants.h"
#include "ImagesAndMap.h"

typedef struct Obstacol Obstacol;
struct Obstacol{
    int x, y;
    int speed;
    int type;
    Obstacol *next;
}*Top = NULL;


void AdaugaObstacol(int x, int y, int speed, int type);//adauga un obstacol in stiva
void StergeObstacol();//sterge un obstacol ce nu mai este pe ecran
void StergeVarf();//sterge obstacolul din varful stivei
void SpawnObstacol();//adauga un obstacol nou
int GasesteObstacolStanga(int linie);//gaseste cel mai din stanga obstacol de pe linia linie
void AnimateObstacol();//misca obstacolele spre dreapta
void SetLine();//scade linia
void DrawObstacol();//deseneaza obstacolele
int CheckColision();//verifica coliziunea dintre player si obstacol

int CheckColision(){
    if(frog -> life <= 0)
        return 0;
    Obstacol *aux = Top;
    while(aux){
        if(frog -> x * SpriteSize + SpriteSize / 2 >= aux -> x && frog -> x * SpriteSize + SpriteSize / 2 <= aux -> x + 2*SpriteSize)
            if(frog -> y == aux -> y){
                frog -> life--;
                return true;
            }
        aux = aux -> next;
    }
    return false;
}

void SpawnObstacol(){
    int i;
    for(i = 1; i < MAP_HEIGHT; i++){
        if(Map[i][0] == sosea){
            int distanta = rand() % SpriteSize + 5 * SpriteSize;
            int pozitie = GasesteObstacolStanga(i);
            AdaugaObstacol(pozitie - distanta, i, VitezaObstacol, masina);
        }

        if(Map[i][0] == apa){
            int distanta = rand() % SpriteSize + 5 * SpriteSize;
            int pozitie = GasesteObstacolStanga(i);
            AdaugaObstacol(pozitie - distanta, i, VitezaObstacol, bustean);
        }
    }
}

void DrawObstacol(){
    Obstacol *aux = Top;
    while(aux){
        if(aux -> y > 0)
            DrawTexture(aux -> x, (aux -> y - 1) * SpriteSize, 2 * SpriteSize, SpriteSize, Sprites[aux -> type]);
        aux = aux -> next;
    }
}

void AnimateObstacol(){
    Obstacol *aux = Top;
    while(aux){
        aux -> x += aux -> speed;
        aux = aux -> next;
    }
}

void SetLine(){
    Obstacol *aux = Top;
    while(aux){
        aux -> y += 1;
        aux = aux -> next;
    }
}

int GasesteObstacolStanga(int linie){
    int minim = -1;
    Obstacol *aux = Top;
    if(!Top)
        return 0;
    while(aux){
        if(aux -> y == linie){
            if(minim == -1 || minim > aux -> x)
                minim = aux -> x;
        }
        aux = aux -> next;
    }
    return minim;
}

void AdaugaObstacol(int x, int y, int speed, int type){
    Obstacol *aux = (Obstacol*)malloc(sizeof(Obstacol));
    aux -> next = Top;
    aux -> speed = speed;
    aux -> x = x;
    aux -> y = y;
    aux -> type = type;
    Top = aux;
}

void StergeVarf(){
    Obstacol *aux = Top;
    Top = Top -> next;
    free(aux);
}

void StergeObstacol(){
    Obstacol *aux = Top;
    Obstacol *aux2 = Top -> next;

    while(aux2){
        if( aux2 -> x > ScreenWidth || aux2 -> y > MAP_HEIGHT){
            aux -> next = aux2 -> next;
            free(aux2);
            aux2  = aux -> next;
        }else{
            aux = aux -> next;
            aux2 = aux -> next;
        }
    }

}

#endif // OBSTACOL_H_INCLUDED
