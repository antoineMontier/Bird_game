#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 540
#define HEIGHT 940
#define BIRD_WIDTH 40
#define BIRD_HEIGHT 40
#define BIRD_JUMP_POWER 20
#define ACCELERATION 180
#define FRAMES_PER_SECOND 36
#define BUTTON_WIDTH 70
#define BUTTON_HEIGHT 70
#define PALETTE 4
#define NB_SPIKES 10 
#define SPIKE_SPACE_PX 20
#define GRAVITY 981
#define BIRD_SPEED 8

typedef struct{
    double x;
    double y;
    double vx;
    double vy;
}bird;

typedef struct{
    double x;//center x of the spike
    double y;//center y of the spike
    double r;//height of the spike
}spike;

typedef struct{
    unsigned int r;//red 0-255
    unsigned int g;//green 0-255
    unsigned int b;//blue 0-255
}Color;



void SDL_ExitWithError(const char *string);
void point(SDL_Renderer* r, int x, int y);
void mark(SDL_Renderer* r, int x, int y, int thickness);
void line(SDL_Renderer* r, int x1, int y1, int x2, int y2);
void color(SDL_Renderer* r, int red, int green, int blue, int alpha);
void rect(SDL_Renderer* r, int x, int y, int height, int width, int filled);
void circle(SDL_Renderer * r, int centreX, int centreY, int radius, int filled);
void openSDL(int x, int y, int mode, SDL_Window**w, SDL_Renderer**r);
void closeSDL(SDL_Window**w, SDL_Renderer**r);
void background(SDL_Renderer* r, Color*c, int p);
void drawLandscape(SDL_Renderer* r, Color*c, int p);
double dist(double x1, double y1, double x2, double y2);
void printRestartButton(SDL_Renderer* r, Color*c, int p);
int rollover(int mx, int my, int x, int y, int w, int h);
void jump(double*y, double*vy, int*sj);
int inTheTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double a, double b);
void triangle(SDL_Renderer* r, int x1, int y1, int x2, int y2, int x3, int y3, int filled);
void drawSpikes(SDL_Renderer* r, int*, int spike_nb, double size, double a_l, double a_r, Color*c, int p);
void drawBackground(SDL_Renderer* r, Color*c, int p);
void spikeUpdate(int *s, int spike_nb, int lvl, double*a_l, double*a_r, int*u_l, int*u_r, int facing);
void drawBird(SDL_Renderer* r, bird b, int facing, Color*c, int p);
void moveBird(bird *b, int *facing, double sp_sz);



int main(int argc, char *args[]){//compile and execute with     gcc main.c -o main -lm $(sdl2-config --cflags --libs) && ./main

    Color *colors = malloc(4*PALETTE*sizeof(Color));//store 9*4 colors that behave nicely 4 to 4

    //1 // Swap sans

    colors[0].r = 15;
    colors[0].g = 32;
    colors[0].b = 97;

    colors[1].r = 255;
    colors[1].g = 255;
    colors[1].b = 255;

    colors[2].r = 97;
    colors[2].g = 237;
    colors[2].b = 228;

    colors[3].r = 42;
    colors[3].g = 121;
    colors[3].b = 161;

    //2 //Dark codes

    colors[4].r = 0;
    colors[4].g = 12;
    colors[4].b = 67;

    colors[5].r = 25;
    colors[5].g = 25;
    colors[5].b = 25;

    colors[6].r = 68;
    colors[6].g = 68;
    colors[6].b = 68;

    colors[7].r = 253;
    colors[7].g = 170;
    colors[7].b = 50;

    //3 // lightaeon #2

    colors[8].r = 242;
    colors[8].g = 75;
    colors[8].b = 106;

    colors[9].r = 31;
    colors[9].g = 49;
    colors[9].b = 81;

    colors[10].r = 89;
    colors[10].g = 47;
    colors[10].b = 239;

    colors[11].r = 114;
    colors[11].g = 250;
    colors[11].b = 154;

     //4 // Bold decision

    colors[12].r = 232;
    colors[12].g = 157;
    colors[12].b = 86;

    colors[13].r = 26;
    colors[13].g = 58;
    colors[13].b = 70;

    colors[14].r = 205;
    colors[14].g = 84;
    colors[14].b = 29;

    colors[15].r = 115;
    colors[15].g = 133;
    colors[15].b = 100;



    SDL_Window *w;//open a window command
    SDL_Renderer *ren;//render creation

    int palette = 0;
    const double spike_size = HEIGHT/(NB_SPIKES+2);//+2 for the down and up spike border
    int* s_y = malloc(NB_SPIKES*2*sizeof(int));

    bird birdy;
    birdy.x = 150;
    birdy.y = 300;
    birdy.vx = 0;
    birdy.vy = 0;


    openSDL(WIDTH, HEIGHT, 0, &w, &ren);
    SDL_bool program_launched = SDL_TRUE; //SDL_FALSE or SDL_TRUE
    int tick_count = 0;
    int level = 0;
    double app_l = 0;//0 to 21 0 if fully appeared and 21 is dissapeared
    double app_r = 21;
    int update_l = 1;
    int update_r = 0;
    int facing = 1; //-1 if facing left, 1 if facing right

    //restartGame(bulbs, &d_x, &d_y, &d_vy, &d_ay, &tick_count, &palette);


    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while(program_launched){//principal loop
        SDL_Event evt;

                //update positions

                spikeUpdate(s_y, NB_SPIKES*2, level, &app_l, &app_r, &update_l, &update_r, facing);
                moveBird(&birdy, &facing, spike_size);
                printf("%d\n", facing);

                //draw background
                drawBackground(ren, colors, palette);

                //draw landscape
                drawSpikes(ren, s_y, NB_SPIKES*2, spike_size, app_l, app_r, colors, palette);
                //draw bird
                drawBird(ren, birdy, facing, colors, palette);
                color(ren, 255, 0, 0, 255);
                mark(ren, birdy.x, birdy.y, 4);


                //printRestartButton(ren, colors, palette);
            
                SDL_RenderPresent(ren);//refresh the render


        //controls
        while(SDL_PollEvent(&evt)){//reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch(evt.type){

                case SDL_QUIT:
                    program_launched = SDL_FALSE;//quit the program if the user closes the window
                    break;

                case SDL_KEYDOWN:                   //SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                    switch (evt.key.keysym.sym){//returns the key ('0' ; 'e' ; 'SPACE'...)

                        case SDLK_ESCAPE:
                            program_launched = SDL_FALSE;//escape the program by pressing esc
                            break;

                        case SDLK_SPACE:

                            break;

                        default:
                            break;
                    }


                case SDL_MOUSEBUTTONDOWN:

                default:
                    break; 

            }
        }


        tick_count++;

        SDL_Delay(1000/FRAMES_PER_SECOND);
    }
    free(s_y);
    closeSDL(&w, &ren);
    printf("closed successfully !\n");
    return 0;
}

void SDL_ExitWithError(const char *string){
    SDL_Log("Error : %s > %s\n", string, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void mark(SDL_Renderer* r, int x, int y, int thickness){
    for(int a = y - thickness ; a <= y + thickness ; a++){
        for(int b = x - thickness ; b <= x + thickness; b++){
            point(r, b, a);
        }
    }
}

void point(SDL_Renderer* r, int x, int y){
    if(SDL_RenderDrawPoint(r, x, y) != 0)
        SDL_ExitWithError("failed to draw point");
}

void line(SDL_Renderer* r, int x1, int y1, int x2, int y2){
    if(SDL_RenderDrawLine(r, x1, y1, x2, y2) != 0)//line
        SDL_ExitWithError("failed to draw line");
}

void color(SDL_Renderer* r, int red, int green, int blue, int alpha){
    if(SDL_SetRenderDrawColor(r, red, green, blue, alpha) != 0)
        SDL_ExitWithError("failed to set color");
}

void rect(SDL_Renderer* r, int x, int y, int width, int height, int filled){
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = width;
    rectangle.h = height;

    if(filled){
        if(SDL_RenderFillRect(r, &rectangle) != 0)
            SDL_ExitWithError("failed to draw a full rectangle");
    }
    if(!filled){
        if(SDL_RenderDrawRect(r, &rectangle) != 0)
            SDL_ExitWithError("failed to draw a full rectangle");
    }
}

void circle(SDL_Renderer * r, int cx, int cy, int radius, int filled){
   const int diameter = (radius * 2);

   int x = (radius - 1);
   int y = 0;
   int tx = 1;
   int ty = 1;
   int error = (tx - diameter);

   while (x >= y){
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(r, cx + x, cy - y);
      SDL_RenderDrawPoint(r, cx + x, cy + y);
      SDL_RenderDrawPoint(r, cx - x, cy - y);
      SDL_RenderDrawPoint(r, cx - x, cy + y);
      SDL_RenderDrawPoint(r, cx + y, cy - x);
      SDL_RenderDrawPoint(r, cx + y, cy + x);
      SDL_RenderDrawPoint(r, cx - y, cy - x);
      SDL_RenderDrawPoint(r, cx - y, cy + x);

      if (error <= 0){
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0){
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }

    if(filled){
        int s_x = cx - radius;
        int s_y = cy - radius;
        int f_x = cx + radius;
        int f_y = cy + radius;

        for(int a = s_x ; a <= f_x ; a++){
            for(int b = s_y ; b <= f_y ; b++){
                if(dist(cx, cy, a, b) < radius)
                    point(r, a, b);
            }
        }
    }

}

void openSDL(int x, int y, int mode, SDL_Window**w, SDL_Renderer**r){

    if(0 != SDL_Init(/*flag*/ SDL_INIT_VIDEO))//lots of flags like SDL_INIT_AUDIO ; *_VIDEO ; *_EVERYTHING... To separe with '|'
        SDL_ExitWithError("Initialisation SDL failed");
    //at this point, the SDL is well initialised, we can afford it because of the if


    if(SDL_CreateWindowAndRenderer(x, y, mode, w, r) !=0)
        SDL_ExitWithError("window and render creation failed");

}

void closeSDL(SDL_Window**w, SDL_Renderer**r){
    SDL_DestroyRenderer(*r);
    SDL_DestroyWindow(*w);
    SDL_Quit();
}

void background(SDL_Renderer* r, Color*c, int p){
    color(r, c[4*p+3].r, c[4*p+3].g, c[4*p+3].b, 255);
    rect(r, 0, 0, WIDTH, HEIGHT, 1);
}

void drawLandscape(SDL_Renderer* r, Color*c, int p){
    int alpha = 255;
    for(int y = 3*HEIGHT/4 ; y < HEIGHT && alpha > 0; y ++){
        color(r, c[4*p+1].r, c[4*p+1].g, c[4*p+1].b, alpha);
        line(r, 0, y, WIDTH, y);
        alpha -= 1;
    }
    
}

double dist(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void printRestartButton(SDL_Renderer* r, Color*c, int p){
    color(r, c[4*p].r, c[4*p].g, c[4*p].b, 255);
    rect(r, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, 1);
    color(r, 20, 20, 20, 255);
    rect(r, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, 0);

    color(r, c[4*p+1].r, c[4*p+1].g, c[4*p+1].b, 255);

    triangle(r  , WIDTH/2 -  BUTTON_WIDTH/2 +  BUTTON_WIDTH/5
                , HEIGHT/2 - BUTTON_HEIGHT/2 + BUTTON_HEIGHT/5
                , WIDTH/2 -  BUTTON_WIDTH/2 +  BUTTON_WIDTH/5
                , HEIGHT/2 - BUTTON_HEIGHT/2 + 4*BUTTON_HEIGHT/5
                , WIDTH/2 -  BUTTON_WIDTH/2 +  4*BUTTON_WIDTH/5
                , HEIGHT/2
                , 1);

}

int rollover(int mx, int my, int x, int y, int w, int h){
    if(mx >= x && mx < x + w && my > y && my < y + h)
        return 1;//true
    return 0;//false
}

int inTheTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double a, double b){
    int sign1 = -1, sign2 = -1, sign3 = -1;
    if(((x2-x1)*(b-y1) - (y2-y1)*(a-x1)) >= 0)
        sign1 = 1;

    if(((x3-x2)*(b-y2) - (y3-y2)*(a-x2)) >= 0)
        sign2 = 1;

    if(((x1-x3)*(b-y3) - (a-x3)*(y1-y3)) >= 0)
        sign3 = 1;

    if(sign1 == sign2 && sign2 == sign3)
        return 1;
    return 0;
}

double min(double a, double b, double c){
    if(a < b && a < c)
        return a;
    if(b < a && b < c)
        return b;
    if(c < a && c < b)
        return c;
}

double max(double a, double b, double c){
    if(a > b && a > c)
        return a;
    if(b > a && b > c)
        return b;
    if(c > a && c > b)
        return c;
}

void triangle(SDL_Renderer* r, int x1, int y1, int x2, int y2, int x3, int y3, int filled){
    line(r, x1, y1, x2, y2);
    line(r, x2, y2, x3, y3);
    line(r, x3, y3, x1, y1);
    if(filled){

        int s_x = min(x1, x2, x3);
        int s_y = min(y1, y2, y3);
        int f_x = max(x1, x2, x3);
        int f_y = max(y1, y2, y3);

        for(int a = s_x ; a <= f_x ; a++){
            for(int b = s_y ; b <= f_y ; b++){
                if(inTheTriangle(x1, y1, x2, y2, x3, y3, a, b))
                    point(r, a, b);
            }
        }
    }
}

void drawBackground(SDL_Renderer* r, Color*c, int p){
    color(r, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b, 255);
    rect(r, 0, 0, WIDTH, HEIGHT, 1);
}

void drawSpikes(SDL_Renderer* r, int*s, int spike_nb, double size, double a_l, double a_r, Color*c, int p){
    const double spike_size = WIDTH/(2*NB_SPIKES/3);
    //up and down :
    color(r, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b, 255);
    rect(r, 0, 0, WIDTH, size/4, 1);
    rect(r, 0, HEIGHT-size/4, WIDTH, size/4, 1);

    double x = spike_size;
    while(x <= WIDTH - spike_size/2){
        triangle(r, x - spike_size/2, 0, x+spike_size/2, 0, x, spike_size, 1);
        triangle(r, x - spike_size/2, HEIGHT, x+spike_size/2, HEIGHT, x, HEIGHT-spike_size, 1);
        x += spike_size;
    }
    //left and right
    double y = 1.5*spike_size;
    rect(r, 0, 0, size/4, HEIGHT, 1);
    rect(r, WIDTH-size/4, 0, size/4 +1, HEIGHT, 1);

    double delta_l = (a_l)*(2*WIDTH/3 - size/4) / 100;
    double delta_r = (a_r)*(2*WIDTH/3 - size/4) / 100;

    while(y <= HEIGHT - spike_size){
        //left
        triangle(r, 0, y - spike_size/2, 0, y + spike_size/2, spike_size - delta_l, y, 1);
        //right
        triangle(r, WIDTH, y - spike_size/2, WIDTH, y + spike_size/2, WIDTH-spike_size + delta_r, y, 1);
        y += spike_size;
    }


}

void spikeUpdate(int *s, int spike_nb, int lvl, double*a_l, double*a_r, int*u_l, int*u_r, int facing){
    srand(time(0));


    if(facing == 1){
    //clear the non faced spikes :
        if(*a_r > 0)
            (*a_r)-= 2;
    //appear the faced spikes : 
        if(*a_l < 21)
            (*a_l)+= 2;
    }


     if(facing == -1){
    //clear the non faced spikes :
        if(*a_l > 0)
            (*a_l)-= 2;
    //appear the faced spikes : 
        if(*a_r < 21)
            (*a_r)+= 2;
    }


    int ind = 0;
    int nb_spikes_visibles_per_side = 2 + lvl % 5;
    //do we need to update the non-faced side for the next-next hit ?
}

void drawBird(SDL_Renderer* r, bird b, int facing,  Color*c, int p){
    color(r, c[4*p].r, c[4*p].g, c[4*p].b, 255);
    rect(r, b.x, b.y, BIRD_WIDTH, BIRD_WIDTH, 1);
    color(r, c[4*p+1].r, c[4*p+1].g, c[4*p+1].b, 255);
    if(facing == 1){
        triangle(r, b.x + 4.8*BIRD_WIDTH/5, 
                    b.y + 1.2*BIRD_WIDTH/5,//top
                    b.x + 4.8*BIRD_WIDTH/5,
                    b.y + 3.2*BIRD_WIDTH/5,//down
                    b.x + 6*BIRD_WIDTH/5,
                    b.y + 2.2*BIRD_WIDTH/5, 1);//middle
    }else if(facing == -1){
        triangle(r, b.x +0.2*BIRD_WIDTH/5, 
                    b.y + 1.2*BIRD_WIDTH/5,//top
                    b.x +0.2*BIRD_WIDTH/5,
                    b.y + 3.2*BIRD_WIDTH/5,//down
                    b.x -1*BIRD_WIDTH/5,
                    b.y + 2.2*BIRD_WIDTH/5, 1);//middle
    }
}

void moveBird(bird *b, int *facing, double sp_sz){
    if(*facing == 1 && ((b->x + BIRD_WIDTH ) > (WIDTH - sp_sz/4)))
        *facing = -1;
    else if(*facing == -1 && ((b->x ) < sp_sz/4))
        *facing = 1;

    if(*facing == 1)
        b->vx = BIRD_SPEED;
    else if(*facing == -1)
        b->vx = -BIRD_SPEED;
    //b->vy += GRAVITY/1000.0;
    b->x += b->vx;
    b->y += b->vy;
}







