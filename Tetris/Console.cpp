#include "Console.h"
// Hàm thay đổi kích cỡ Console.
void SetWindowSize(int width,int height,int top,int left)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT Size;
    Size.Top=top;
    Size.Left=left;
    Size.Right=width-1;
    Size.Bottom=height-1;
    SetConsoleWindowInfo(h,true,&Size);
}
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
// Hàm thay đổi kích cỡ Screen Buffer.
void SetScreenBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;
    SetConsoleScreenBufferSize(hStdout, NewSize);
}

// Chặn thay đổi kích cỡ Screen Buffer và Screen Console thủ công
void DisResizeScreen()
{
    HWND hwnd = GetConsoleWindow();
    SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
}
// Hàm ẩn con trỏ chuột
void SetCursorVisible(bool Visible)
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible=Visible;
    Info.dwSize=20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&Info);
}

// Hàm tô màu.
void SetColor(int color)
{
    HANDLE handle;
    handle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle,color);
}
void ColorStringXY(char chars[], int color,int x, int y)
{
    HANDLE handle;
    handle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle,color);

    COORD Position = {x,y};
    SetConsoleCursorPosition(handle, Position);

    cout << chars;
}
void ColorStringXY(char chars, int color,int x, int y)
{
    HANDLE handle;
    handle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle,color);

    COORD Position = {x,y};
    SetConsoleCursorPosition(handle, Position);

    cout << chars;
}

// Hàm dịch chuyển con trỏ đến tọa độ x,y.
void Gotoxy(int x,int y)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position = {x,y};
    SetConsoleCursorPosition(hStdout, Position);
}
