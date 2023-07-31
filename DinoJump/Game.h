#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Console.h"
#include <time.h>
typedef struct Block
{
    int X;
    int Set;
} Block;
void Map();
void Get();
void Game();
void Graphics();
void Move();
void GetDino();
void Border();
void GraphicsDino();
void jump();
void GameOver();
bool impact();
#endif // GAME_H_INCLUDED
