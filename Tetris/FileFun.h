#ifndef FILEFUN_H_INCLUDED
#define FILEFUN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "Console.h"
bool flinedown(FILE* f);
bool flNumber(FILE* f,int line);
void GameStart();
void boundary();
void getblock(FILE *f);
void draw();
void movedown();
void Even();
bool Checknext();
void updMap();
bool checkMap(int mode);
void getBouns();
bool GameOver();
#endif // FILEFUN_H_INCLUDED
