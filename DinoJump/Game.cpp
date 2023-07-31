#include "Game.h"
const int HEIGHT=20;
const int WIDTH=30;
const int LENGTH=25;
Block A[HEIGHT][10];
Block Dino[7][3];
int SizeBlock=0;
int down;
int downDino=14;
bool direct;
bool isJump=false;
int timed=40;
bool run;
int BOUNS=0;
void Game()
{
    char f;
    srand(time(NULL));
    Map();
    Get();
    GetDino();
    GraphicsDino();
    Border();
    run=true;
    while (run)
    {
        Graphics();
        if (kbhit())
        {
            f=getch();
            isJump=true;
            direct=true;
            timed=timed/2;
        }
        if (isJump)
        {
            jump();
        }
        if (impact()) continue;
        Sleep(timed);
        Move();
        ColorStringXY("",4,15,2);
        printf("%d",++BOUNS);
        if (BOUNS%400==0) timed-=2;
    }
}
void Map()
{
    FILE* fr=fopen("MAP.txt","r");
    if (fr==NULL) printf("dd");
    FILE* fw=fopen("SOURCE.txt","w");
    char c=0;
    int d1=0,d2=0;
    while (c!=EOF)
    {
        c=fgetc(fr);
        if (c=='*')
        {
            d1++;
            if (d2!=0)
            {
                fprintf(fw,"%d ",-d2);
                d2=0;
            }
        }
        else if (c==' ')
        {
            d2++;
            if (d1!=0)
            {
                fprintf(fw,"%d ",d1);
                d1=0;
            }
        }
        if (c=='\n')
        {
            if (d1!=0)
            {
                fprintf(fw,"%d ",d1);
                d1=0;
            }
            if (d2!=0)
            {
                fprintf(fw,"%d ",-d2);
                d2=0;
            }
            fprintf(fw,"\n");
        }
    }
    fclose(fr);
    fclose(fw);
}
void Get()
{
    FILE* fr=fopen("SOURCE.txt","r");
    int rands=rand()%6;
    int d1,d2,finder,i,j=0;
    while (j<rands)
    {
        fscanf(fr,"%d",&finder);
        if (finder==10) j++;
    }
    i=1;
    d1=0;
    d2=0;
    while (true)
    {
        fscanf(fr,"%d",&finder);
        if (finder==10) break;
        else if (finder<0)
        {
            d1-=finder;
        }
        else if (finder>0)
        {
            A[i][++d2].Set=finder;
            A[i][d2].X=d1+WIDTH;
            d1+=finder;
        }
        if (d1==10)
        {
            A[i][0].X=1;
            A[i][0].Set=d2;
            i++;
            d1=0;
            d2=0;
        }
    }
    A[0][0].X=i;
    down=HEIGHT-i+1;
    fclose(fr);
}
void Graphics()
{
    int i,j,t,y,d1,d2;
    int bot=A[0][0].X;
    for (i=1;i<bot;i++)
    {
        y=i+down;
        SetColor(2*17);
        j=A[i][0].X;
        d1=A[i][j].Set;
        d2=A[i][j].X;
        for (t=0;t<d1;t++)
        {
            if (t+d2>=0)
            {
                Gotoxy((t+d2)*2,y);
                for (;t<d1&&t+d2<=LENGTH;t++)
                {
                    printf("  ");
                }
            }
        }
        j++;
        d1=A[i][0].Set;
        for (;j<=d1;j++)
        {
            d2=A[i][j].Set;
            Gotoxy(A[i][j].X*2,y);
            for (t=0;t<d2&&t+A[i][j].X<=LENGTH;t++)
                printf("  ");
        }
    }
}
void Move()
{
    int i,j,d1,d2;
    int a,b,c;
    a=A[0][0].X;
    for (i=1;i<a;i++)
    {
        b=A[i][0].Set;
        for (j=A[i][0].X;j<=b;j++)
        {
            A[i][j].X--;
            ColorStringXY("  ",0,(A[i][j].X+A[i][j].Set)*2,i+down);
        }
        d1=A[i][0].X;
        d2=A[i][d1].X;
        if (d2+A[i][d1].Set<=0&&d1<=b) A[i][0].X++;
    }
    j=0;
    for (i=1;i<a;i++)
        if (A[i][0].X>A[i][0].Set) j++;
        else i=a;
    if (j==a-1) Get();
}
void GetDino()
{
    Dino[0][0].X=1;
    Dino[0][0].Set=1;
    Dino[0][1].X=4;
    Dino[0][1].Set=2;
    Dino[1][0].X=1;
    Dino[1][0].Set=2;
    Dino[1][1].X=4;
    Dino[1][1].Set=1;
    Dino[1][2].X=6;
    Dino[1][2].Set=1;
    Dino[2][0].X=1;
    Dino[2][0].Set=1;
    Dino[2][1].X=4;
    Dino[2][1].Set=3;
    Dino[3][0].X=1;
    Dino[3][0].Set=1;
    Dino[3][1].X=3;
    Dino[3][1].Set=3;
    Dino[4][0].X=1;
    Dino[4][0].Set=2;
    Dino[4][1].X=0;
    Dino[4][1].Set=1;
    Dino[4][2].X=2;
    Dino[4][2].Set=5;
    Dino[5][0].X=1;
    Dino[5][0].Set=1;
    Dino[5][1].X=1;
    Dino[5][1].Set=5;
    Dino[6][0].X=1;
    Dino[6][0].Set=2;
    Dino[6][1].X=3;
    Dino[6][1].Set=1;
    Dino[6][2].X=5;
    Dino[6][2].Set=1;
}
void Border()
{
    int i;
    Gotoxy(0,0);
    SetColor(8*17);
    for (i=0;i<LENGTH+1;i++) printf("  ");
    Gotoxy(0,HEIGHT+1);
    for (i=0;i<LENGTH+1;i++) printf("  ");
}
void GraphicsDino()
{
    int i,j,d1,d2,t;
    SetColor(15*17);
    for (i=0;i<7;i++)
    {
        d1=Dino[i][0].Set;
        for (j=Dino[i][0].X;j<=d1;j++)
        {
            d2=Dino[i][j].Set;

            for (t=0;t<d2;t++)
                if (t+Dino[i][j].X>=0)
                {
                    Gotoxy((t+Dino[i][j].X)*2,i+downDino);
                    while (t<d2)
                    {
                        printf("  ");
                        t++;
                    }
                }
        }
    }
}
void jump()
{
    int i,j,d1,d2,t;
    SetColor(0);
    for (i=0;i<7;i++)
    {
        d1=Dino[i][0].Set;
        for (j=Dino[i][0].X;j<=d1;j++)
        {
            d2=Dino[i][j].Set;

            for (t=0;t<d2;t++)
                if (t+Dino[i][j].X>=0)
                {
                    Gotoxy((t+Dino[i][j].X)*2,i+downDino);
                    while (t<d2)
                    {
                        printf("  ");
                        t++;
                    }
                }
        }
    }
    if (!direct&&downDino<14)
    {
        if (downDino<8) downDino++;
        else if (downDino<14) downDino+=2;
        GraphicsDino();
    }
    else if (direct)
    {
        if (downDino>8) downDino-=2;
        else if (downDino>=4) downDino--;
        GraphicsDino();
    }
    if (direct&&downDino==4)
    {
        GraphicsDino();
        Sleep(timed);
        Move();
        Graphics();
    }
    if (direct&&downDino==3)
    {
        GraphicsDino();
        for (i=0;i<4;i++)
        {
            Sleep(timed);
            Move();
            Graphics();
        }
        direct=false;
    }
    if (!direct&&downDino==14)
    {
        isJump=false;
        timed=timed*2;
    }
}
void GameOver()
{
    ColorStringXY("DEATH",4,4,2);
    run=false;
}
bool impact()
{
    int i,j,d1,t,d2,d3;
    for (i=1;i<A[0][0].X;i++)
    {
        d1=A[i][0].Set;
        for (j=1;j<=d1;j++)
        {
            for (t=0;t<6;t++)
            {
                d2=Dino[t][0].Set;
                if ((A[i][j].X==Dino[t][d2].X+Dino[t][d2].Set&&i+down==t+downDino))
                {
                    GameOver();
                    return true;
                }
            }
            for (t=1;t<=2;t++)
            {
                d3=A[i][j].X;
                d2=Dino[6][t].X+1;
                if ((d3<=d2&&d3+A[i][j].Set>=d2&&i+down==7+downDino))
                {
                    GameOver();
                    return true;
                }
            }
        }
    }
    return false;
}
