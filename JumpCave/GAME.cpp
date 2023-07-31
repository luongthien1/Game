#include "GAME.h"
const int WIDTH=20;
const int HEIGHT=7;
const int LENGTH=WIDTH/2;
int Time=300;
COORD Pro={2,1};
bool direct=true;
bool Map[HEIGHT-1];
Block A[7][10];
Block B[7][10];
int amount=0;
bool run=true;
int BOUNS;
int times=150;
void start()
{
    int mode=-WIDTH;
    srand(time(NULL));
    getMap();
    Get(A);
    Get(B);
    background(5*17);
    while (run)
    {

        mode++;
        if (mode>WIDTH*2) mode=1;
        if (mode==WIDTH) Get(A);
        if (mode==WIDTH*2) Get(B);
        if (mode<=WIDTH/2||mode>WIDTH) Graphics(A);
        if (mode>0&&mode<=WIDTH*3/2) Graphics(B);
        MovePro();
        if (mode<=WIDTH/2||mode>WIDTH) Move(A);
        if (mode>0&&mode<=WIDTH*3/2) Move(B);
        Gotoxy(20,7);
        SetColor(4);
        printf("%d",++BOUNS);
        if (BOUNS%400==0) times-=20;

    }
}
void getMap()
{
    FILE* fw=fopen("SOURCE.txt","w");
    FILE* fr=fopen("MAP.txt","r");
    char c=0;
    int dem1=0;
    int dem2=0;
    int i;
    while (c!=EOF)
    {
        c=fgetc(fr);
        if (c!='\n'&&c!=EOF)
        {
            if (c=='*')
            {
                dem2++;
                if (dem1!=0) fprintf(fw," %d",-dem1);
                dem1=0;
            }
            else
            {
                dem1++;
                if (dem2!=0) fprintf(fw," %d",dem2);
                dem2=0;
            }
        }
        else
        {
            if (!dem1) fprintf(fw," %d",dem2);
            if (!dem2) fprintf(fw," %d",-dem1);
            dem1=0;
            dem2=0;
            fprintf(fw,"\n");
        }
    }
    fclose(fw);
    fclose(fr);
}
void Get(Block Ai[7][10])
{
    FILE* fr=fopen("SOURCE.txt","r");
    int finder;
    int j=0,i=rand()%7;
    while (j!=i)
    {
        fscanf(fr,"%d",&finder);
        if (finder==WIDTH) j++;
    }
    i=0;
    Ai[0][0].Set=0; Ai[0][0].X=0;
    while (true)
    {
        fscanf(fr,"%d",&finder);
        if (finder==WIDTH) break;
        else if (finder<0) Ai[i][0].X-=finder;
        else if (finder>0)
        {
            Ai[i][++amount].X=Ai[i][0].X+(WIDTH/2);
            Ai[i][0].X+=finder;
            Ai[i][amount].Set=finder;
        }
        if (Ai[i][0].X==WIDTH)
        {
            Ai[i][0].Set=amount;
            amount=0;
            Ai[i][0].X=1;
            i++;
            Ai[i][0].Set=0;
            Ai[i][0].X=0;
        }
    }
    fclose(fr);
}

void Graphics(Block Ai[7][10])
{
    int j,i,t,d,m;
    for (i=0;i<HEIGHT-2;i++)
    {
        SetColor(5*16);
        d=Ai[i][0].X;

        for (t=0;t<Ai[i][d].Set;t++)
            if (Ai[i][d].X+t>-1&&Ai[i][d].X+t<LENGTH)
            {
                Gotoxy((Ai[i][d].X+t)*2,i+1);
                printf("  ");
            }
        m=Ai[i][0].Set;

        for (j=d+1;j<=m;j++)
        {
            Gotoxy(Ai[i][j].X*2,i+1);
            for (t=0;t<Ai[i][j].Set;t++)
                if (t+Ai[i][j].X<LENGTH) printf("  ");
                else
                {
                    j=m;
                    break;
                }
        }
    }
}
void Move(Block Ai[7][10])
{
    int i,j,d1,d2,t;
    for (i=0;i<HEIGHT-2;i++)
    {
        d2=Ai[i][0].X;
        d1=Ai[i][0].Set;
        for (j=Ai[i][0].X;j<=d1;j++)
        {
            Ai[i][j].X--;
            t=Ai[i][j].X+Ai[i][j].Set;
            if (t>-1) ColorStringXY("  ",0,t*2,i+1);
        }

        if (Ai[i][d2].X+Ai[i][d2].Set<0)
        {
            if (Ai[i][0].X<d1) Ai[i][0].X++;
        }
    }
    Gotoxy(0,9);
    SetColor(4);
    for (i=0;i<HEIGHT-2;i++)
        if (Ai[i][Ai[i][0].X].X==Pro.X&&i+1==Pro.Y)
        {
            Impact();
            j=-1;
            break;
        }
    if (j!=-1)
        for (i=0;i<HEIGHT-2;i++)
        {
            d2=Ai[i][0].X;
            d1=Ai[i][d2].X;
            if (d1==Pro.X) Map[i+1]=true;
            if (d1+Ai[i][d2].Set==Pro.X) Map[i+1]=false;
        }
}
void MovePro()
{
    int d1,d2;
    clock_t a,b;
    a=clock();
    do
    {
        KeyEvent();
        d1=Pro.Y;
        d2=Pro.X*2;
        if (direct)
        {
            if (d1<5)
            if (!Map[d1+1])
            {
                ColorStringXY("  ",0,d2,d1);
                Pro.Y++;
            }
        }
        else
        {
            if (d1>1)
            if (!Map[d1-1])
            {
                ColorStringXY("  ",0,d2,d1);
                Pro.Y--;
            }
        }
        ColorStringXY("  ",7*17,d2,Pro.Y);
        Sleep(20);
    b=clock();
    } while (b-a<=times);
}
void Impact()
{
    ColorStringXY("  ",0,Pro.X*2,Pro.Y);
    Pro.X--;
    if (Pro.X<0)
    {
        ColorStringXY("DEATH",4,10,14);
        GameOver();
        getch();
    }
    ColorStringXY("  ",7*17,Pro.X*2,Pro.Y);
}
void GameOver()
{
    run=false;
}
void background(int color)
{
    int i;
    Gotoxy(0,0);
    SetColor(color);
    for (i=0;i<LENGTH;i++)
    printf("  ");
    Gotoxy(0,HEIGHT-1);
    for (i=0;i<LENGTH;i++)
    printf("  ");
}
void KeyEvent()
{
    HANDLE h=GetStdHandle(STD_INPUT_HANDLE);
    DWORD NoOfEvent=0;
    DWORD NoOfEventRead=0;
    GetNumberOfConsoleInputEvents(h,&NoOfEvent);
    if (NoOfEvent)
    {
        INPUT_RECORD *EventBuffer = new INPUT_RECORD[NoOfEvent];
        ReadConsoleInput(h,EventBuffer,NoOfEvent,&NoOfEventRead);

        for (DWORD i=0;i<NoOfEvent;i++)
        {
            if (EventBuffer[i].EventType == KEY_EVENT)
            {
                if(EventBuffer[i].Event.KeyEvent.bKeyDown)
                {
                    switch (EventBuffer[i].Event.KeyEvent.wVirtualKeyCode)
                    {
                    case VK_UP:
                        if (direct) direct=false;
                        break;
                    case VK_DOWN:
                        if (!direct) direct=true;
                        break;
                    }
                }
            }
        }
    }

}
