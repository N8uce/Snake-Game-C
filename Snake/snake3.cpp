#include <stdlib.h>
#include <time.h>
#include "graphics.h"

//#include <math.h>

//���������� ��������� 
#define WIHT 800 
#define HIHT 600 

#define GOR 64 //���������� ������ �� �����������
#define VER 48 // ���������� ������ �� ��������� 

#define TEN 10 // ��������� ����� 10, ������ ����� ��� �������� �� "������" � "�������" 

#define KEY_SPACE 32

int apple[2] = {20, 20}; // ������ �� ���������
int head[2] = {15, 8}; // ������ ���� �� ��������, ������� �������� ������ ���� ���� 
int upr[2] = {1, 0}; // ���������� ������������ ����������� �� ���������
int snake[512][3] = {{13, 8}, {14,8}, {15, 8}};// ���� ���� �� ���������
int length = 3; // ����� ���� ������ , � ����� ��� � ���� ������
int speed = 200; // �������� ���������� � �� , ������������ ��� ��������� �������� ����
int pause = 0; // ����������, ������� ����� ��� ����� ����
int p = 1; //������� ������� ��������
int g=1;// ���������� �������� �� ���������� �����  ����
int sr = 13;// ��������� ��������� ���� ���� �� x


enum button_values { NONE = -1,
                     GAME, N1,N2,N3, ABOUT, EXIT,
                     N_BUTTONS };

typedef struct Button
{
   int left;
   int top;
   int width;
   int height;
   IMAGE *image;
} Button;

void create_button(int, int, int, const char*);
int  select_button();

void load();
void start();
void menu();
void game();
void about();
void close();

Button buttons[N_BUTTONS];
IMAGE *image_menu, *image_about,*image_N1,*image_N2,*image_N3;
                     
int main()
{ 
   initwindow(WIHT, HIHT,"Snake");
   srand(time(NULL)); //������������� ��������� ����� ��� ����������� ��������� ������
   load();
   start();
   menu();
   closegraph();
   return 0;
}

//������� ������� ��������� ��� ������� � ����������, � ����� ������ ���������� �����������, ���� ����������� ������� ����������� 
void process (char key)
{
  if (key == KEY_ESC)
    exit (0);
  else if (key == KEY_SPACE)
    pause = !pause;
  else if (key == 0)
    switch (getch ())
    {
      case KEY_UP:
         if (upr[1]!=1)
         {
        upr[0] = 0;
        upr[1] = -1;
         }
        break;
         
           case KEY_DOWN:
       if(upr[1] !=-1)
       {
        upr[0] = 0;
        upr[1] = 1;
       }
        break;
      
      case KEY_LEFT:
         if(upr[0]!=1)
         {
        upr[0] = -1;
        upr[1] = 0;
         }
        break;
         
      case KEY_RIGHT:
         if(upr[0]!=-1)
         {
        upr[0] = 1;
        upr[1] = 0;
         }
        break;
    }
    
}


//�����������  ������
void object_apple (int x, int y, int color)
{
  setfillstyle (SOLID_FILL, color);
  fillellipse(x*TEN,  y*TEN, 7, 7);
}
//����������� ���� ����
void object_body (int x, int y, int color)
{
  setfillstyle (SOLID_FILL, color);
  //bar (x * TEN+2, y * TEN+2, x * TEN + TEN, y * TEN + TEN);
   fillellipse(x*TEN,  y*TEN, 7, 7);
}
//����������� �������� � ������ ����
void headh (int x, int y)
{
    setfillstyle (SOLID_FILL, BLUE);
    fillellipse(x*TEN,y*TEN,6,6);
}

//��� ���� � ������� ����
void game_pole()
{   
   setfillstyle (SOLID_FILL, BLACK);
   bar (0, 0, WIHT,  HIHT); 
   
   setfillstyle (SOLID_FILL, WHITE);
   rectangle((WIHT-GOR*TEN)/2, (HIHT-VER*TEN)/2, GOR*TEN+((WIHT-GOR*TEN)/2),VER*TEN+10);
}
//����������� ����� ������
void game_point()
{
  bgiout << length; 
  settextstyle(GOTHIC_FONT, HORIZ_DIR, 12);
  outtextxy((WIHT-GOR*TEN)/2,40,"��� ����:");
  outstreamxy (((WIHT-GOR*TEN)/2)+120, 40);
}
//������� ��������� ������ 
int isdead ()
{
  if (head[0] <= 8 || head[0] >= GOR+8 || head[1] <= 6 || head[1] >= VER+1) 
     return 0; //1
  
  for (int i = 0; i < length; i++)
    if (head[0] == snake[i][0] && head[1] == snake[i][1])
          return 0; //1
   
    return 1; //0
}

//������� �������
void refresh ()
{
   
  int i;
 if(pause)
   { 
      p = 0;
      return;
   }
   
     head[0] += upr[0];
     head[1] += upr[1];
   
   game_pole();
 if(!isdead())
 {  
    g = 0;
    outtextxy(((WIHT-GOR*TEN)/2)+240,40,"�� ���������");
    return;
 }
  if (head[0] == apple[0] && head[1]== apple[1])
  {
    again:
      //��������� ������� � �������� �������� ����
     apple[0] = ((rand()) % (GOR-8))+16; 
     apple[1] = ((rand()) % (VER-6))+7 ;
   
    for (i = 0; i < length; i++)
      if (snake[i][0] == apple[0] && snake[i][1] == apple[1])
      goto again;
  }
  else
   {
    length--;
    for (i = 0; i < length; i++)
     {
      snake[i][0] = snake[i + 1][0];
      snake[i][1] = snake[i + 1][1];
     }
   }

     if (length < 512)
    {
    snake[length][0] = head[0];
    snake[length][1] = head[1];
    length++;
    }
   game_point();
  for (i = 0; i < length; i++)
   {
  object_body (snake[i][0], snake[i][1], WHITE);
   }
   
   object_apple (apple[0], apple[1], RED);
   
   headh(head[0],head[1]);
   
}
//�������� ��������� � ������ � �������� ������
void load()
{
     
   image_menu  = loadBMP("window_menu.bmp");
   image_about = loadBMP("window_about.bmp");
   image_N1 = loadBMP("N1.bmp");
   image_N2 = loadBMP("N2.bmp");
   image_N3 = loadBMP("N3.bmp");
   
   create_button(GAME,  300, 200, "button_game.bmp");
   create_button(ABOUT, 300, 400, "button_about.bmp");
   create_button(N1,300,320,"N1.bmp");
   create_button(N2,375,320,"N2.bmp");
   create_button(N3,450,320,"N3.bmp");
   create_button(EXIT,  300, 500, "button_exit.bmp");
}
//��������
void start()
{
   IMAGE *image;
   image = loadBMP("window_start.bmp");
   putimage(0, 0, image, COPY_PUT);
   freeimage(image);
   getch();
}
//����
void menu()
{
   int state;
   
   while (1)
   {
      putimage(0, 0, image_menu, COPY_PUT);
      for (int i = 0; i < N_BUTTONS; i++)
      {
         putimage(buttons[i].left, buttons[i].top,
                  buttons[i].image, COPY_PUT);
      }
      
      state = NONE;
      while (state == NONE)
      {
         while (mousebuttons() != 1);
         state = select_button();
      }
      
      switch (state)
      {
         case GAME: 
          g = 1;
          head[0] = 15;
          head[1] = 8;
          apple[0] = 20; 
          apple[1] = 20;
          upr[0] = 1; 
          upr[1] = 0;
          length = 3; 
          sr = 13;
          for (int i=0; i < length; i++)
         {
            snake[i][0] = sr++;
            snake[i][1] = 8;
         }
            game(); 
            break;
         case N1:speed = 150; break;
         case N2:speed = 100; break;
         case N3:speed = 50; break;
         case ABOUT: about(); break;
         case EXIT: close(); return;
      }
   }
}
//�� ��������� 
void about()
{
   putimage(0, 0, image_about, COPY_PUT);
   getch();
}
//�������� ���������
void close()
{
   freeimage(image_menu);
   freeimage(image_about);
   for (int i = 0; i < N_BUTTONS; i++)
   {
      freeimage(buttons[i].image);
   }
}
// ������� ��������� ������
void create_button(int i, int left, int top,
                   const char *file_name)
{
   buttons[i].image  = loadBMP(file_name);
   buttons[i].left     = left;
   buttons[i].top     = top;
   buttons[i].width  = imagewidth(buttons[i].image);
   buttons[i].height = imageheight(buttons[i].image);
}
//������� ������� ��������� ������ �� ������
int select_button()
{
   int x, y;
   x = mousex();
   y = mousey();
   for (int i = 0; i < N_BUTTONS; i++)
   {
      if (x > buttons[i].left &&
          x < buttons[i].left + buttons[i].width &&
          y > buttons[i].top &&
          y < buttons[i].top + buttons[i].height)
      {
         return i;
      }
   }
   
   return NONE;
}
//������� ����������� ����
void game()
{
      while (g)
    {
      p = 1-p;
      setactivepage(p); //�������� �������� == 1-�������
       
      while (kbhit())
      {
      process (getch ());
      }
      
      refresh ();
      setvisualpage(p); // ������ �������� �������� �������
      delay (speed);
      //printf("%d",g);
    }
   getch();
}
