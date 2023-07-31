#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;

void SetWindowSize(int width,int height,int top=0,int left=0);
void resizeConsole(int width, int height);
void SetScreenBufferSize(SHORT width,SHORT height);
void DisResizeScreen();
void SetCursorVisible(bool Visible);
void SetColor(int color);
void ColorStringXY(char chars[], int color,int x, int y);
void ColorStringXY(char chars, int color,int x, int y);
void Gotoxy(int x,int y);
#endif // SONSOLE_H_INCLUDED
