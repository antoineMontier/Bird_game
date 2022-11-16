#include "SDL_Basics.h"


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
    double precision = 0.5;
    for(int a = cx - radius; a <= cx + radius ; a++){
        for(int b = cy-radius ; b <= cy+radius ; b++){
            if(filled){
                if(dist(a, b, cx, cy) <= radius)
                    point(r, a, b);
            }else{
                if(dist(a, b, cx, cy) <= radius + precision && dist(a, b, cx, cy) >= radius - precision)
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

    if(TTF_Init() != 0)
        SDL_ExitWithError("TTF initialisation failed");

}

void closeSDL(SDL_Window**w, SDL_Renderer**r){
    TTF_Quit();
    SDL_DestroyRenderer(*r);
    SDL_DestroyWindow(*w);
    SDL_Quit();
}

void background(SDL_Renderer* r, int red, int green, int blue, int w, int h, int p){
    color(r, red, green, blue, 255);
    rect(r, 0, 0, w, h, 1);
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
    else if(b < a && b < c)
        return b;
    else if(c < a && c < b)
        return c;
}

double max(double a, double b, double c){
    if(a > b && a > c)
        return a;
    else if(b > a && b > c)
        return b;
    else if(c >a && c >b)
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

void roundRect(SDL_Renderer* r, int x, int y, int width, int height, int filled, int curve){
    if(curve <= 0){
        if(filled){
            for(int a = x ; a <= x + width ; a++){
                for(int b = y ; b <= y + height ; b++){
                    point(r, a, b);
                }
            }
            return;
        }else{
            line(r, x, y, x + width, y);
            line(r, x, y, x, y + height);
            line(r, x + width, y + height, x + width, y);
            line(r, x + width, y + height, x , y + height);
            return;
        }
    }

    int smaller = 1;// = -1 if it's height ; 1 if it's width or equal
    if(width > height){
        smaller =-1;
    }
    if(curve >= width/2 || curve >= height/2){//if curve is at its max value
        curve = min(width/2, height/2, height);
    }

    //here curve is between 1 and it's max :
    if(filled){
        if(smaller == -1){//long rectangle
            //draw the middle part :
            roundRect(r, x + curve, y, width - 2*curve, height, 1, 0);
            //fill the left gap :
            roundRect(r, x, y + curve, curve, height - curve*2, 1, 0);
            //fill the right gap
            roundRect(r, x + width - curve, y + curve, curve, height - 2*curve, 1, 0);

            //fill the top left :
            circle(r, x + curve, y + curve, curve, 1);

            //fill the top right :
            circle(r, x + width -curve, y + curve, curve, 1);

            //fill the bottom right :
            circle(r, x + width -curve, y +height- curve, curve, 1);

            //fill the bottom left :
            circle(r, x +  curve, y + height - curve, curve, 1);
        }else if(smaller == 1){//high rectangle
            //draw the middle part :
            roundRect(r, x, y + curve, width, height - curve*2, 1, 0);
            //fill the upper gap :
            roundRect(r, x + curve, y, width - 2* curve, curve, 1, 0);
            //fill the bottom gap :
            roundRect(r, x + curve, y + height - curve, width - 2* curve, curve, 1, 0);

            //fill the top left :
            circle(r, x + curve, y + curve, curve, 1);

            //fill the top right :
            circle(r, x + width -curve, y + curve, curve, 1);

            //fill the bottom right :
            circle(r, x + width -curve, y +height- curve, curve, 1);

            //fill the bottom left :
            circle(r, x +  curve, y + height - curve, curve, 1);
        }



    }else if(filled == 0){
        if(smaller == -1){//long rectangle
            //draw the middle part :
            line(r, x + curve, y, x + width - curve, y);//top
            line(r, x + curve, y + height, x + width - curve, y + height);//bottom
            line(r, x, y+ curve, x, y + height - curve);//left
            line(r, x + width, y+ curve, x + width, y + height - curve);//right

            double precision = 0.5;

            //top left
            for(int a = x ; a <= x + curve ; a++){
                for(int b = y ; b <= y + curve ; b++){
                    if(dist(a, b, x + curve, y+curve) <= curve + precision && dist(a, b, x + curve, y+curve) >= curve - precision)
                        point(r, a, b);
                }
            }

            //bottom left
            for(int a = x ; a <= x + curve ; a++){
                for(int b = y + height - curve; b <= y + height ; b++){
                    if(dist(a, b, x + curve, y+ height - curve) <= curve + precision && dist(a, b, x + curve, y+height - curve) >= curve - precision)
                        point(r, a, b);
                }
            }

            //top right
            for(int a = x + width - curve ; a <= x + width ; a++){
                for(int b = y ; b <= y + curve ; b++){
                    if(dist(a, b, x + width - curve, y+curve) <= curve + precision && dist(a, b, x + width - curve, y+curve) >= curve - precision)
                        point(r, a, b);
                }
            }

            //top left
            for(int a = x + width - curve ; a <= x + width ; a++){
                for(int b = y + height - curve; b <= y + height ; b++){
                    if(dist(a, b, x + width - curve, y+height-curve) <= curve + precision && dist(a, b, x + width - curve, y+height-curve) >= curve - precision)
                        point(r, a, b);
                }
            }



        }else if(smaller == 1){//high rectangle

            //draw the middle part :
            line(r, x + curve, y, x + width - curve, y);//top
            line(r, x + curve, y + height, x + width - curve, y + height);//bottom
            line(r, x, y+ curve, x, y + height - curve);//left
            line(r, x + width, y+ curve, x + width, y + height - curve);//right

            double precision = 0.5;

            //top left
            for(int a = x ; a <= x + curve ; a++){
                for(int b = y ; b <= y + curve ; b++){
                    if(dist(a, b, x + curve, y+curve) <= curve + precision && dist(a, b, x + curve, y+curve) >= curve - precision)
                        point(r, a, b);
                }
            }

            //bottom left
            for(int a = x ; a <= x + curve ; a++){
                for(int b = y + height - curve; b <= y + height ; b++){
                    if(dist(a, b, x + curve, y+ height - curve) <= curve + precision && dist(a, b, x + curve, y+height - curve) >= curve - precision)
                        point(r, a, b);
                }
            }

            //top right
            for(int a = x + width - curve ; a <= x + width ; a++){
                for(int b = y ; b <= y + curve ; b++){
                    if(dist(a, b, x + width - curve, y+curve) <= curve + precision && dist(a, b, x + width - curve, y+curve) >= curve - precision)
                        point(r, a, b);
                }
            }

            //top left
            for(int a = x + width - curve ; a <= x + width ; a++){
                for(int b = y + height - curve; b <= y + height ; b++){
                    if(dist(a, b, x + width - curve, y+height-curve) <= curve + precision && dist(a, b, x + width - curve, y+height-curve) >= curve - precision)
                        point(r, a, b);
                }
            }

        }
    }

}

void setFont(TTF_Font**font, char*font_file, int size){
    *font = TTF_OpenFont(font_file, size);
    if (*font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
}

void text(SDL_Renderer*r, int x, int y, char*text, TTF_Font*font, int red, int green, int blue){
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color textColor = {red, green, blue, 0};

    surface = TTF_RenderText_Blended(font, text, textColor);
    texture = SDL_CreateTextureFromSurface(r, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = text_width;
    rectangle.h = text_height;
    SDL_RenderCopy(r, texture, NULL, &rectangle);
    SDL_DestroyTexture(texture);
}

void toChar(char*c, int n){
    if(n < 0)
        n = 0;

    c[2] = '0' + (char)(n%10);
    c[1] = '0' + (char)(n/10 % 10);
    c[0] = '0' + (char)(n/100);


    c[3] = '\0';

}

double dist(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
