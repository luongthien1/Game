#include "FileFun.h"
COORD block[100];
int MAP[21][22]={0};
int n,Color;
const int WIDTH=39;
const int HEIGHT=22;
int TIMEDOWN=500;
bool run=true;
int BOUNS=0;
int COMBO=0;
int Level=1;
bool flinedown(FILE* f)
{
    char c=0;
    do
    {
        c=fgetc(f);
    } while (c!='\n'&&c!=EOF);
    if (c==EOF) return false;
    else return true;
}
bool flNumber(FILE* f,int line)
{
    rewind(f);
    for (int i=1;i<line;i++)
        if (!flinedown(f))
        {
            rewind(f);
            return false;
        }
    return true;
}
void GameStart()
{
    boundary();
    srand(time(NULL));
    FILE *f=fopen("oct.txt","r");
    getBouns();
    ColorStringXY("LEVEL: ",8,41,3);
    printf("%d",Level);
    do
    {   getblock(f);
        draw();
        movedown();
        updMap();
        if (checkMap(4)) getBouns();
        if (GameOver())
        {
            Sleep(400);
            run=false;
            while (!kbhit())
            {
                Color=(rand()%15+1);
                ColorStringXY("~~---GAMEOVER---~~",Color,8,11);
                Sleep(500);
                Color=(rand()%15+1);
                ColorStringXY("~G~A~M~E~~O~V~E~R~",Color,8,11);
                Sleep(500);
            }
        }
    } while (run);
    SetColor(0);
    fclose(f);
}
void boundary()
{
    int i;
    printf("%c",201);
    for (i=1;i<WIDTH;i++) printf("%c",205);
    printf("%c",187);
    for (i=1;i<HEIGHT;i++)
    {
        Gotoxy(0,i);
        printf("%c",186);
        Gotoxy(WIDTH,i);
        printf("%c",186);
    }
    Gotoxy(0,HEIGHT);
    printf("%c",200);
    for (i=1;i<WIDTH;i++) printf("%c",205);
    printf("%c",188);
    for (i=0;i<=HEIGHT;i++)
    {
        MAP[0][i]=-1;
        MAP[(WIDTH+1)/2][i]=-1;
    }
    for (i=1;i<=(WIDTH-1)/2;i++) MAP[i][HEIGHT]=-1;
}
void getblock(FILE *f)
{
    static int line;
    if (line==12) line =13;
    else line=rand()%(Level+4)+1;
    Color=(rand()%15+1)*17;
    flNumber(f,line);
    fscanf(f,"%d",&n);
    for (int i=0;i<n;i++)
    {
        fscanf(f,"%d",&block[i].X);
        fscanf(f,"%d",&block[i].Y);
        block[i].X=(block[i].X+9)*2+1;
        block[i].Y++;
    }
    Sleep(400);
}
void draw()
{
    for (int i=0;i<n;i++)
    {
        ColorStringXY("  ",Color,block[i].X,block[i].Y);
    }
}
void movedown()
{
    clock_t a,b;
    while(Checknext())
    {
        TIMEDOWN=500;
        for (int i=0;i<n;i++)
            ColorStringXY("  ",0,block[i].X,block[i].Y);
        for (int i=0;i<n;i++)
        {
            block[i].Y++;
            ColorStringXY("  ",Color,block[i].X,block[i].Y);
        }
        a=clock();
        do
        {
            if (kbhit()) Even();
            b=clock();
        }
        while (b-a<=TIMEDOWN);
    }

}
void Even()
{
    int i;
    char c=getch();
    if (c==-32)
    {
        c=getch();
        switch(c)
        {
        case 75:
            if (checkMap(2))
            {
                for (i=0;i<n;i++)
                ColorStringXY("  ",0,block[i].X,block[i].Y);
                for (i=0;i<n;i++)
                {
                    block[i].X-=2;
                    ColorStringXY("  ",Color,block[i].X,block[i].Y);
                }
            }
        break;
        case 77:
            if (checkMap(3))
            {
                for (i=0;i<n;i++)
                ColorStringXY("  ",0,block[i].X,block[i].Y);
                for (i=0;i<n;i++)
                {
                    block[i].X+=2;
                    ColorStringXY("  ",Color,block[i].X,block[i].Y);
                }
            }
        break;
        case 80:
            TIMEDOWN=0;;
        break;
        }
    }
    else if (c==115)
    {
        for (i=1;i<n;i++)
            {
                if (MAP[block[0].Y-block[i].Y+(block[0].X+1)/2][(block[i].X-block[0].X)/2+block[0].Y]!=0) return;
            }
        for (i=0;i<n;i++)
            ColorStringXY("  ",0,block[i].X,block[i].Y);
        for (i=1;i<n;i++)
        {
            int k;
            k=(block[i].X-block[0].X)/2+block[0].Y;
            block[i].X=(block[0].Y-block[i].Y)*2+block[0].X;
            block[i].Y=k;
        }
        draw();
    }
}
bool Checknext()
{
    for (int i=0;i<n;i++)
    {
        if (block[i].Y==HEIGHT-1) return false;
    }
    if (checkMap(1)) return false;
    return true;
}
bool checkMap(int mode)
{
    int i=0,j;
    if (mode==1)
    {
        for (;i<n;i++)
            if (MAP[(block[i].X+1)/2][block[i].Y+1]!=0) return true;
        return false;
    }
    if (mode==2)
    {
        for (;i<n;i++)
            if (block[i].X==1||MAP[(block[i].X-1)/2][block[i].Y]!=0) return false;
        return true;
    }
    if (mode==3)
    {
        for (;i<n;i++)
            if (block[i].X==WIDTH-2||MAP[(block[i].X+3)/2][block[i].Y]!=0) return false;
        return true;
    }
    if (mode==4)
    {
        int k,h;
        for (i=1;i<HEIGHT;i++)
            for (j=1;j<(WIDTH+1)/2;j++)
            {
                if (MAP[j][i]==0) break;
                if (j==(WIDTH-1)/2&&MAP[j][i]!=0)
                {
                    COMBO++;
                    for (k=1;k<(WIDTH+1)/2;k++)
                    {
                        h=i+1;
                        do
                        {
                            h--;
                            MAP[k][h]=MAP[k][h-1];
                            ColorStringXY("  ",MAP[k][h],2*k-1,h);
                        } while (h>2);
                    }
                }
            }
        if (COMBO) return true;
    }
}
void getBouns()
{
    BOUNS=BOUNS+15*COMBO+(COMBO)*(COMBO)*5;
    ColorStringXY("BOUNS: ",8,41,1);
    printf("%d",BOUNS);
    ColorStringXY("!COMBO!: ",8,41,2);
    printf("%d",COMBO);
    if (BOUNS>Level*300)
    {
        ColorStringXY("!LEVEL!: ",8,41,3);
        printf("%d",++Level);
    }
    COMBO=0;
}
void updMap()
{
    for (int i=0;i<n;i++)
    {
        MAP[(block[i].X+1)/2][block[i].Y]=Color;
    }
}
bool GameOver()
{
    for (int i=1;i<(WIDTH+1)/2;i++)
    {
        if (MAP[i][2]!=0) return true;
    }
    return false;
}
