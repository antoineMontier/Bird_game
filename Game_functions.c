#include "Game_functions.h"

void drawLandscape(SDL_Renderer* r, Color*c, int p){
    int alpha = 255;
    for(int y = 3*HEIGHT/4 ; y < HEIGHT && alpha > 0; y ++){
        color(r, c[4*p+1].r, c[4*p+1].g, c[4*p+1].b, alpha);
        line(r, 0, y, WIDTH, y);
        alpha -= 1;
    }
    
}

void printRestartButton(SDL_Renderer* r, Color*c, int p){
    color(r, c[4*p].r, c[4*p].g, c[4*p].b, 255);
    roundRect(r, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, 1, BUTTON_HEIGHT/3);

    color(r, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b, 255);

    triangle(r  , WIDTH/2 -  BUTTON_WIDTH/2 +  3*BUTTON_WIDTH/10
                , HEIGHT/2 - BUTTON_HEIGHT/2 + 3*BUTTON_HEIGHT/10
                , WIDTH/2 -  BUTTON_WIDTH/2 +  3*BUTTON_WIDTH/10
                , HEIGHT/2 - BUTTON_HEIGHT/2 + 7*BUTTON_HEIGHT/10
                , WIDTH/2 -  BUTTON_WIDTH/2 +  7.5*BUTTON_WIDTH/10
                , HEIGHT/2
                , 1);

}

void printSettingButton(SDL_Renderer* r, Color*c, int p){
    color(r, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b, 255);
    roundRect(r, WIDTH/2 - BUTTON_WIDTH/3, HEIGHT/2 + BUTTON_HEIGHT, 2*BUTTON_WIDTH/3, 2*BUTTON_HEIGHT/3, 0, BUTTON_HEIGHT/6);//external rect
    //cx = WIDTH/2 //cy = HEIGHT/2 + 4*BUTTON_HEIGHT/3

    circle(r, WIDTH/2, HEIGHT/2 + 4*BUTTON_HEIGHT/3, 4*BUTTON_WIDTH/20, 1);

    double angle = 0;

    while(angle <= 2*3.1415){
        circle(r, WIDTH/2 +  (4*BUTTON_WIDTH/20)*cos(angle),HEIGHT/2 + 4*BUTTON_HEIGHT/3 + (4*BUTTON_WIDTH/20)*sin(angle), BUTTON_WIDTH/20, 1);
        angle += 3.1415/5;
    }
    color(r, c[4*p+3].r, c[4*p+3].g, c[4*p+3].b, 255);
    circle(r, WIDTH/2, HEIGHT/2 + 4*BUTTON_HEIGHT/3, 3*BUTTON_WIDTH/20, 1);
}

void printReturnButton(SDL_Renderer* r, Color*c, int p){
    color(r, c[4*p].r, c[4*p].g, c[4*p].b, 255);
    roundRect(r, WIDTH - BUTTON_WIDTH, BUTTON_HEIGHT/2, BUTTON_WIDTH/2, BUTTON_HEIGHT/2, 1, BUTTON_HEIGHT/6);
    color(r, 255, 255, 255, 255);

    roundRect(r, WIDTH - BUTTON_WIDTH + 2*BUTTON_WIDTH/10, BUTTON_HEIGHT/2 + BUTTON_HEIGHT/5, BUTTON_WIDTH/3 - BUTTON_WIDTH/10, 5, 1, BUTTON_HEIGHT/6);//arrow body

    triangle(r,  WIDTH - BUTTON_WIDTH + 5*BUTTON_WIDTH/20, BUTTON_HEIGHT/2 + 2*BUTTON_WIDTH/20//top
                ,WIDTH - BUTTON_WIDTH + 5*BUTTON_WIDTH/20, BUTTON_HEIGHT - 2*BUTTON_HEIGHT/20//down
                ,WIDTH - BUTTON_WIDTH + 2*BUTTON_WIDTH/20, 3*BUTTON_HEIGHT/4, 1);//middle
}

void drawBackground(SDL_Renderer* r, int lvl, TTF_Font*font, Color*c, int p){
    char*score = malloc(5);
    toChar(score, lvl);
    color(r, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b, 255);
    rect(r, 0, 0, WIDTH, HEIGHT, 1);
    text(r, WIDTH/2 - 18*300/30, 150, score, font, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b);
    free(score);
    /*color(r, 255, 0, 0, 255);
    mark(r, WIDTH/2 - FONT_SIZE/2, 150, 5);
    mark(r, WIDTH/2 + FONT_SIZE/2, 150 + FONT_SIZE, 5);
    mark(r, WIDTH/2, 200, 2);*/

}

void drawSpikes(SDL_Renderer* r, int*s_l, int*s_r, int *spike_nb, double size, double a_l, double a_r, Color*c, int p){
    const double spike_size = WIDTH/(2*NB_SPIKES/3);
    //up and down :
    color(r, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b, 255);
    rect(r, 0, 0, WIDTH, size/4, 1);
    rect(r, 0, HEIGHT-size/4, WIDTH, size/4 + 1, 1);

    double x = spike_size;
    while(x <= WIDTH - spike_size/2){
        triangle(r, x - spike_size/2, HEIGHT, x+spike_size/2, HEIGHT, x, HEIGHT-2*spike_size/3, 1);//down
        triangle(r, x - spike_size/2, 0, x+spike_size/2, 0, x, 2*spike_size/3, 1);//up

        x += spike_size;
    }
    //left and right
    double y = 1.5*spike_size;
    rect(r, 0, 0, size/4, HEIGHT, 1);
    rect(r, WIDTH-size/4, 0, size/4 +1, HEIGHT, 1);

    double delta_l = (a_l)*(2*WIDTH/3 - size/4) / 100;
    double delta_r = (a_r)*(2*WIDTH/3 - size/4) / 100;
    (*spike_nb) = 0;
    while(y <= HEIGHT - spike_size){
        //left
        if(s_l[*spike_nb])
            triangle(r, 0, y - spike_size/2, 0, y + spike_size/2, 2*spike_size/3 - delta_l, y, 1);
        //right
        if(s_r[*spike_nb])
            triangle(r, WIDTH, y - spike_size/2, WIDTH, y + spike_size/2, WIDTH-2*spike_size/3 + delta_r, y, 1);
        (*spike_nb)++;
        y += spike_size;
    }


}

void spikeUpdate(int *s_l, int*s_r, int spike_nb, int lvl, double*a_l, double*a_r, int facing, int *u_l, int *u_r, int spike_increase){
    srand(time(0));


    if(facing == 1){
    //clear the non faced spikes :
        if(*a_r > 0)
            (*a_r)-=1.5;
    //appear the faced spikes : 
        if(*a_l < 21)
            (*a_l)+=1.5;
    }


     if(facing == -1){
    //clear the non faced spikes :
        if(*a_l > 0)
            (*a_l)-=1.5;
    //appear the faced spikes : 
        if(*a_r < 21)
            (*a_r)+=1.5;
    }


    int ind = 0;
    if(spike_increase <= 0)//anti-exception
        spike_increase = 1;
    int maxsp = 1 + lvl / spike_increase;//number of actives spikes

    if(maxsp >= spike_nb - 1){
        maxsp = spike_nb - 2;
    }
    int visibles = 1 + rand() % maxsp;

    //do we need to update the non-faced side for the next-next hit ?
    //only if u_l and u_r are non equal to 0

    if(*u_l == 1 && *a_r < 1){
        //update left spikes
        for(int i = 0 ;i < spike_nb ; i++){
            s_l[i] = 0;
        }
        while(visibles > 0){
            do{
                ind = rand() % spike_nb;
            }while(s_l[ind] == 1);
            s_l[ind] = 1;
            visibles--;
        }
        *u_l = 0;
    }else if(*u_r == 1 && *a_l < 1){
        //update left spikes
        for(int i = 0 ;i < spike_nb ; i++){
            s_r[i] = 0;
        }
        while(visibles > 0){
            do{
                ind = rand() % spike_nb;
            }while(s_r[ind] == 1);
            s_r[ind] = 1;
            visibles--;
        }
        *u_r = 0;
    }
}

void drawBird(SDL_Renderer* r, bird b, int facing, int*j,  Color*c, int p, double bird_size){
    color(r, c[4*p].r, c[4*p].g, c[4*p].b, 255);
    roundRect(r, b.x, b.y, bird_size, bird_size, 1, 10);


    color(r, c[4*p+1].r, c[4*p+1].g, c[4*p+1].b, 255);
    //========================================================================================
    if(facing == 1){//right
        triangle(r, b.x + 4.8*bird_size/5, 
                    b.y + 1.2*bird_size/5,//top
                    b.x + 4.8*bird_size/5,
                    b.y + 3.2*bird_size/5,//down
                    b.x + 6*bird_size/5,
                    b.y + 2.2*bird_size/5, 1);//middle
        //eye
        circle(r, b.x + 3*bird_size/5, b.y + 1*bird_size/4, bird_size/8, 1);
        color(r, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b, 255);
        //wing if not jumped
        if(*j <= 0){
            triangle(r, b.x + bird_size/10, b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, b.y + bird_size/2,//left
                        b.x + bird_size/10, b.y + 9*bird_size/10, 1);//down
        }else{//wing if jumped
            triangle(r, b.x + bird_size/10, b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, b.y + bird_size/2,//left
                        b.x + bird_size/10, b.y + (9 - (*j)/10 )*bird_size/10, 1);//down
            (*j)-= WING_SPEED;
        }
            
    //========================================================================================
    }else if(facing == -1){//left
        triangle(r, b.x +0.2*bird_size/5, 
                    b.y + 1.2*bird_size/5,//top
                    b.x +0.2*bird_size/5,
                    b.y + 3.2*bird_size/5,//down
                    b.x -1*bird_size/5,
                    b.y + 2.2*bird_size/5, 1);//middle
        //eye
        circle(r, b.x + 2*bird_size/5, b.y + 1*bird_size/4, bird_size/8, 1);
        color(r, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b, 255);
        //wing if not jumped
        if(*j <= 0){
            triangle(r, b.x + 9*bird_size/10, b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, b.y + bird_size/2,//right
                        b.x + 9*bird_size/10, b.y + 9*bird_size/10, 1);//down
        }else{//wing if jumped
            triangle(r, b.x + 9*bird_size/10, b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, b.y + bird_size/2,//left
                        b.x + 9*bird_size/10, b.y + (9 - (*j)/10 )*bird_size/10, 1);//down
            (*j)-= WING_SPEED;
        }
            
    }
}

void moveBird(bird *b, int *facing, int* lvl, int size, double sp_sz, double bird_speed, double gravity, double bird_size){
    if(*facing == 1 && ((b->x + bird_size ) > (WIDTH - sp_sz/4))){
        *facing = -1;
        (*lvl)++;
    }else if(*facing == -1 && ((b->x ) < sp_sz/4)){
        *facing = 1;
        (*lvl)++;
    }

    if(b->y <= size/4)
        b->vy = abs(b->vy)*0.9;//little loss of speed when bouncing
    else if(b->y + bird_size > HEIGHT - size/4)
        b->vy = -abs(b->vy)*0.9;//little loss of speed when bouncing

    if(b->y + bird_size > HEIGHT)
        b->vy = -bird_speed;
    else if(b->y < 0)
        b->vy = bird_speed;


    if(*facing == 1)
        b->vx = bird_speed;
    else if(*facing == -1)
        b->vx = -bird_speed;
    b->vy += gravity/10.0;
    b->x += b->vx;
    b->y += b->vy;
}

int birdTouchSpike(bird b, int facing, int size, int *s_l, int*s_r, int spike_nb, double bird_size){
    const double spike_size = WIDTH/(2*NB_SPIKES/3);
    //5 controls points : 0 ; 1/4 ; 2/4 ; 3/4 ; 4/4
    double x1, x2, x3, x4, x5, y1, y2, y3, y4, y5;
    if(facing == 1){//right
        x1 = b.x + bird_size;
        x2 = b.x + bird_size;
        x3 = b.x + bird_size;
        x4 = b.x + bird_size;
        x5 = b.x + bird_size;
    }else if(facing == -1){
        x1 = b.x;
        x2 = b.x;
        x3 = b.x;
        x4 = b.x;
        x5 = b.x;
    }
    y1 = b.y;
    y2 = b.y + 1*bird_size/4;
    y3 = b.y + 2*bird_size/4;
    y4 = b.y + 3*bird_size/4;
    y5 = b.y + 4*bird_size/4;


    double y = 1.5*spike_size;
    for(int i = 0 ; i < spike_nb ; i++){
        //left check
        if(facing == -1 && s_l[i] == 1){
            if(dist(size/4, y, x1, y1) <= 9*spike_size/30)
                return 1;//true
            if(dist(size/4, y, x2, y2) <= 9*spike_size/30)
                return 1;//true
            if(dist(size/4, y, x3, y3) <= 9*spike_size/30)
                return 1;//true
            if(dist(size/4, y, x4, y4) <= 9*spike_size/30)
                return 1;//true
            if(dist(size/4, y, x5, y5) <= 9*spike_size/30)
                return 1;//true
        }
        //right check
        if(facing == 1 && s_r[i] == 1){
            if(dist(WIDTH - size/4, y, x1, y1) <= 9*spike_size/30)
                return 1;//true
            if(dist(WIDTH - size/4, y, x2, y2) <= 9*spike_size/30)
                return 1;//true
            if(dist(WIDTH - size/4, y, x3, y3) <= 9*spike_size/30)
                return 1;//true
            if(dist(WIDTH - size/4, y, x4, y4) <= 9*spike_size/30)
                return 1;//true
            if(dist(WIDTH - size/4, y, x5, y5) <= 9*spike_size/30)
                return 1;//true
        }
        y += spike_size;
    }


    double x = spike_size;
    
    while(x <= WIDTH - spike_size/2){
        //up
        if(b.y < spike_size){//make the calculations easy
            y1 = y2 = y3 = b.y;
            x1 = b.x;
            x2 = b.x + bird_size/2;
            x3 = b.x + bird_size;
            if(dist(x, size/3, x1, y1) <= 9*spike_size/30)
                return 1;//true
            if(dist(x, size/3, x2, y2) <= 9*spike_size/30)
                return 1;//true
            if(dist(x, size/3, x3, y3) <= 9*spike_size/30)
                return 1;//true
        }
        
        //down
        if(b.y + bird_size >= HEIGHT - spike_size){
            y1 = y2 = y3 = b.y + bird_size;
            x1 = b.x;
            x2 = b.x + bird_size/2;
            x3 = b.x + bird_size;
            if(dist(x, HEIGHT - size/3, x1, y1) <= 9*spike_size/30)
                return 1;//true
            if(dist(x, HEIGHT - size/3, x2, y2) <= 9*spike_size/30)
                return 1;//true
            if(dist(x, HEIGHT - size/3, x3, y3) <= 9*spike_size/30)
                return 1;//true
        }
        x += spike_size;
    }

    return 0;

}

void startGame(bird*b, int*facing, int*pfacing, int*palette, int*lvl, double*a_l, double*a_r, int*u_l, int*u_r, int*s_l, int*s_r, int sn, int*jumped, int*menu, double bird_speed, double bird_size){
    //reset bird
    b->x = WIDTH/2 - bird_size/2;
    b->y = HEIGHT/3 - bird_size/2;
    b->vx = bird_speed;
    b->vy = 0;

    //reset facing and sides management 
    *facing = 1;
    *pfacing = 1;
    *a_l = 0;
    *a_r = 21;
    *u_r = 0;
    *u_l = 0;

    for(int i = 0 ;i < sn ; i++){
        s_l[i] = 0;
        s_r[i] = 0;
    }




    *jumped = 0;
    *menu = 1;
    if(*lvl == 0)
        return;

    *lvl = 0;


        //update color :
    int k = *palette;
    do{
        *palette = rand() % (PALETTE);
    }while(*palette == k);

}

void printResetSettingsButton(SDL_Renderer* r, TTF_Font*f, Color*c, int p){
    color(r, c[4*p + 1].r, c[4*p + 1].g, c[4*p + 1].b, 255);
    roundRect(r, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT/2, 1, 10);
    text(r, WIDTH/2 - 8*BUTTON_WIDTH/20, HEIGHT - 0.98*BUTTON_HEIGHT, "Reset", f, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b);
}

void resetSettingsAndCursors(int*cursor_positions, double*bsp, double*g, double*jp, double*bsz, int*si, int spike_size){
    *bsp = ORIGINAL_BIRD_SPEED;//if it's upper than 30 may cause bugs
    *g = ORIGINAL_GRAVITY;
    *jp = ORIGINAL_JUMP_PWR;
    *bsz = ORIGINAL_BIRD_SIZE;
    *si = ORIGINAL_SPIKE_INCREASE;
    for(int i = 0 ; i < 8 ; i ++)
        cursor_positions[i] = 0;
    cursor_positions[1] = (*bsp)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(BIRD_MAX_SPEED);//initialize the speed at 8
    cursor_positions[2] = (*g)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_GRAVITY);//initialize the gravity at 9.81
    cursor_positions[3] = (*jp)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_JUMP);//initialize the jump at 20
    cursor_positions[4] = (*bsz)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(BIRD_MAX_SIZE);//initialize the bird's size at 40
    cursor_positions[5] = (*si)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MIN_SPIKE_DIFFICULTY);//initialize the spike difficulty at 40
}
