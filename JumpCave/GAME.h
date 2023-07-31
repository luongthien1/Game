#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "Console.h"
typedef struct Block
{
    int X;
    int Set;
} Block;
void start();
void getMap();
void Get(Block Ai[7][10]);
void Graphics(Block Ai[7][10]);
void Move(Block Ai[7][10]);
void background(int color);
void MovePro();
void Impact();
void GameOver();
void KeyEvent();
#endif // GAME_H_INCLUDED
