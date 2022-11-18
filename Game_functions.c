#include "Game_functions.h"

void drawLandscape(SDL_Renderer* r, Color*c, int p){
    int alpha = 255;
    for(int y = 3*HEIGHT/4 ; y < HEIGHT && alpha > 0; y ++){
        color(r, c[4*p+1].r, c[4*p+1].g, c[4*p+1].b, alpha);
        line(r, 0, y, WIDTH, y);
        alpha -= 1;
    }
    
}

void printRestartButton(SDL_Renderer* r, Color*c, int p, double animation){
    if(animation == 0.0)
        return;
    
    color(r, animation*c[4*p].r + (1-animation)*c[4*p + 3].r , animation*c[4*p].g + (1-animation)*c[4*p + 3].g, animation*c[4*p].b + (1-animation)*c[4*p + 3].b, 255);
    roundRect(r, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 + animation*BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, 1, BUTTON_HEIGHT/3);
    color(r, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b, 255);
    triangle(r  , WIDTH/2 -  (BUTTON_WIDTH/2 -  3*BUTTON_WIDTH/10)
                , HEIGHT/2 - (BUTTON_HEIGHT/2 - 3*BUTTON_HEIGHT/10) + (1-animation)*BUTTON_HEIGHT/2 + BUTTON_HEIGHT
                , WIDTH/2 -  (BUTTON_WIDTH/2 -  3*BUTTON_WIDTH/10)
                , HEIGHT/2 - animation*(BUTTON_HEIGHT/2 - 7*BUTTON_HEIGHT/10) + (1-animation)*2*BUTTON_HEIGHT/3 + BUTTON_HEIGHT
                , WIDTH/2 -  (BUTTON_WIDTH/2 - 7.5*BUTTON_WIDTH/10)
                , HEIGHT/2 + (1-animation)*BUTTON_HEIGHT/2 + BUTTON_HEIGHT
                , 1);
}

void printSettingButton(SDL_Renderer* r, Color*c, int p, double animation){
    if(animation == 0)
        return;
    color(r, animation*c[4*p+2].r + (1-animation)*c[4*p + 3].r , animation*c[4*p+2].g + (1-animation)*c[4*p + 3].g, animation*c[4*p+2].b + (1-animation)*c[4*p + 3].b, 255);
    roundRect(r, WIDTH - animation*(WIDTH/2 + BUTTON_WIDTH/3), HEIGHT/2 + BUTTON_HEIGHT*2, 2*BUTTON_WIDTH/3, 2*BUTTON_HEIGHT/3, 0, BUTTON_HEIGHT/6);//external rect

    //cx = WIDTH/2 //cy = HEIGHT/2 + 4*BUTTON_HEIGHT/3

    circle(r, animation*WIDTH/2, HEIGHT/2 + 7*BUTTON_HEIGHT/3, 4*BUTTON_WIDTH/20, 1);

    double angle = 0;

    while(angle <= 2*3.1415){
        circle(r, animation*WIDTH/2 +  (4*BUTTON_WIDTH/20)*cos(angle),HEIGHT/2 + 7*BUTTON_HEIGHT/3 + (4*BUTTON_WIDTH/20)*sin(angle), BUTTON_WIDTH/20, 1);
        angle += 3.1415/5;
    }
    color(r, c[4*p+3].r, c[4*p+3].g, c[4*p+3].b, 255);
    circle(r, animation*WIDTH/2, HEIGHT/2 + 7*BUTTON_HEIGHT/3, 2.2*BUTTON_WIDTH/20, 1);
}

void printReturnButton(SDL_Renderer* r, Color*c, int p, double animation){
    color(r, c[4*p].r, c[4*p].g, c[4*p].b, 255);
    roundRect(r, WIDTH - animation*BUTTON_WIDTH, animation*BUTTON_HEIGHT/2, BUTTON_WIDTH/2, BUTTON_HEIGHT/2, 1, BUTTON_HEIGHT/6);
    color(r, 255, 255, 255, 255);

    roundRect(r, WIDTH - animation*BUTTON_WIDTH + 2*BUTTON_WIDTH/10, BUTTON_HEIGHT/2 + BUTTON_HEIGHT/5, BUTTON_WIDTH/3 - BUTTON_WIDTH/10, 5, 1, BUTTON_HEIGHT/6);//arrow body

    triangle(r,  WIDTH - animation*BUTTON_WIDTH + 5*BUTTON_WIDTH/20, BUTTON_HEIGHT/2 + 2*BUTTON_WIDTH/20//top
                ,WIDTH - animation*BUTTON_WIDTH + 5*BUTTON_WIDTH/20, BUTTON_HEIGHT - 2*BUTTON_HEIGHT/20//down
                ,WIDTH - animation*BUTTON_WIDTH + 2*BUTTON_WIDTH/20, 3*BUTTON_HEIGHT/4, 1);//middle
}

void drawBackground(SDL_Renderer* r, int lvl, TTF_Font*font, Color*c, int p, double animation){
    if(animation == 0.0)
        return;
    char*score = malloc(5);
    toChar(score, lvl);
    color(r, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b, 255);
    rect(r, 0, 0, WIDTH, HEIGHT, 1);
    text(r, WIDTH/2 - 18*300/30, 150, score, font, animation*c[4*p+2].r + (1-animation)*c[4*p + 3].r , animation*c[4*p+2].g + (1-animation)*c[4*p + 3].g, animation*c[4*p+2].b + (1-animation)*c[4*p + 3].b);
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

void drawBird(SDL_Renderer* r, bird b, int facing, int*j,  Color*c, int p, double bird_size, double animation){
    if(animation == 0)
        return;
    color(r, animation*c[4*p].r + (1-animation)*c[4*p + 3].r , animation*c[4*p].g + (1-animation)*c[4*p + 3].g, animation*c[4*p].b + (1-animation)*c[4*p + 3].b, animation);
    roundRect(r, b.x, animation*b.y, bird_size, bird_size, 1, 10);


    color(r, animation*c[4*p+1].r + (1-animation)*c[4*p + 3].r , animation*c[4*p+1].g + (1-animation)*c[4*p + 3].g, animation*c[4*p+1].b + (1-animation)*c[4*p + 3].b, 255);
    //========================================================================================
    if(facing == 1){//right
        triangle(r, b.x + 4.8*bird_size/5, 
                    animation*b.y + 1.2*bird_size/5,//top
                    b.x + 4.8*bird_size/5,
                    animation*b.y + 3.2*bird_size/5,//down
                    b.x + 6*bird_size/5,
                    animation*b.y + 2.2*bird_size/5, 1);//middle
        //eye
        circle(r, b.x + 3*bird_size/5, animation*b.y + 1*bird_size/4, bird_size/8, 1);
        color(r, animation*c[4*p+2].r + (1-animation)*c[4*p + 3].r , animation*c[4*p+2].g + (1-animation)*c[4*p + 3].g, animation*c[4*p+2].b + (1-animation)*c[4*p + 3].b, 255);
        //wing if not jumped
        if(*j <= 0){
            triangle(r, b.x + bird_size/10, animation*b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, animation*b.y + bird_size/2,//left
                        b.x + bird_size/10, animation*b.y + 9*bird_size/10, 1);//down
        }else{//wing if jumped
            triangle(r, b.x + bird_size/10, animation*b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, animation*b.y + bird_size/2,//left
                        b.x + bird_size/10, animation*b.y + (9 - (*j)/10 )*bird_size/10, 1);//down
            (*j)-= WING_SPEED;
        }
            
    //========================================================================================
    }else if(facing == -1){//left
        triangle(r, b.x +0.2*bird_size/5, 
                    animation*b.y + 1.2*bird_size/5,//top
                    b.x +0.2*bird_size/5,
                    animation*b.y + 3.2*bird_size/5,//down
                    b.x -1*bird_size/5,
                    animation*b.y + 2.2*bird_size/5, 1);//middle
        //eye
        circle(r, b.x + 2*bird_size/5, animation*b.y + 1*bird_size/4, bird_size/8, 1);
        color(r, animation*c[4*p+2].r + (1-animation)*c[4*p + 3].r , animation*c[4*p+2].g + (1-animation)*c[4*p + 3].g, animation*c[4*p+2].b + (1-animation)*c[4*p + 3].b, 255);
        //wing if not jumped
        if(*j <= 0){
            triangle(r, b.x + 9*bird_size/10, animation*b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, animation*b.y + bird_size/2,//right
                        b.x + 9*bird_size/10, animation*b.y + 9*bird_size/10, 1);//down
        }else{//wing if jumped
            triangle(r, b.x + 9*bird_size/10, animation*b.y + bird_size/2,//top
                        b.x + 5*bird_size/10, animation*b.y + bird_size/2,//left
                        b.x + 9*bird_size/10, animation*b.y + (9 - (*j)/10 )*bird_size/10, 1);//down
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

void startGame(bird*b, int*facing, int*pfacing, int*palette, int*lvl, double*a_l, double*a_r, int*u_l, int*u_r, int*s_l, int*s_r, int sn, int*jumped, double*menu, double bird_speed, double bird_size){
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

void printResetSettingsButton(SDL_Renderer* r, TTF_Font*f, Color*c, int p, double animation){
    color(r, c[4*p + 1].r, c[4*p + 1].g, c[4*p + 1].b, 255);
    roundRect(r, WIDTH - animation*(WIDTH/2 + BUTTON_WIDTH/2), HEIGHT - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT/2, 1, 10);
    text(r, WIDTH/2 - 8*BUTTON_WIDTH/20, HEIGHT - animation*0.98*BUTTON_HEIGHT, "Reset", f, c[4*p + 3].r, c[4*p + 3].g, c[4*p + 3].b);
}

void resetSettingsAndCursors(int*cursor_positions, double*bsp, double*g, double*jp, double*bsz, int*si, int *a, int spike_size){
    *bsp = ORIGINAL_BIRD_SPEED;//if it's upper than 30 may cause bugs
    *g = ORIGINAL_GRAVITY;
    *jp = ORIGINAL_JUMP_PWR;
    *bsz = ORIGINAL_BIRD_SIZE;
    *si = ORIGINAL_SPIKE_INCREASE;
    *a = ORIGINAL_ANIMATION_SPEED;
    for(int i = 0 ; i < 8 ; i ++)
        cursor_positions[i] = 0;
    cursor_positions[1] = (*bsp)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(BIRD_MAX_SPEED);//initialize the speed at 8
    cursor_positions[2] = (*g)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_GRAVITY);//initialize the gravity at 9.81
    cursor_positions[3] = (*jp)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_JUMP);//initialize the jump at 20
    cursor_positions[4] = (*bsz)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(BIRD_MAX_SIZE);//initialize the bird's size at 40
    cursor_positions[5] = (*si)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MIN_SPIKE_DIFFICULTY);//initialize the spike difficulty at 40
    cursor_positions[6] = (*a)*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_ANIMATION_SPEED);//initialize the animation

}

void printSettingMenu(SDL_Renderer* r, TTF_Font*big, TTF_Font*small, TTF_Font*score, int*cursor_positions, int spike_size, Color*c, int p, char*tmp, double*birdsize, double*menu, int lvl, int*s_l, int*s_r, int*spike_nb, bird b, int facing, int*j, double bird_size, int *animation){
    background(r, c[4*p+3].r, c[4*p+3].g, c[4*p+3].b, WIDTH, HEIGHT);
    double text_animation = (*animation)/500.0;

    if(*menu <= -1.0){//appear
        text_animation = (*menu) + 2; //from 0 to 1
    }else if(*menu > -0.5){
        text_animation = (-((*menu)*2 ) ); //from 1 to 0
    }
    if(*menu < -1.0){//animation of entry
        drawBackground(r, lvl, score, c, p, (1-text_animation));
    }
    if(*menu > -0.5 && *menu <= -0.00001)//animation of exit
        drawBackground(r, lvl, score, c, p, (1-text_animation));
    

    //printf("%f\n", text_animation);

    color(r, c[4*p + 2].r, c[4*p + 2].g, c[4*p + 2].b, 255);
    rect(r, 0, 0, WIDTH, spike_size/4, 1);//top
    rect(r, 0, HEIGHT-spike_size/4, WIDTH, spike_size/4 + 1, 1);//bottom
    rect(r, 0, 0, spike_size/4, HEIGHT, 1);//left
    rect(r, WIDTH-spike_size/4, 0, spike_size/4 +1, HEIGHT, 1);//right
    /////blurrr

    drawSpikes(r, s_l, s_r, spike_nb, spike_size, 0, 0, c, p);

    int pwr = text_animation*(spike_size);
    for(int i = 0 ; i <  pwr ; i ++){
        color(r,  ((pwr - i)/(double)pwr) *c[4*p + 2].r + (i/(double)pwr) * c[4*p + 3].r ,
        ((pwr - i)/(double)pwr) *c[4*p + 2].g + (i/(double)pwr) * c[4*p + 3].g ,
        ((pwr - i)/(double)pwr) *c[4*p + 2].b + (i/(double)pwr) * c[4*p + 3].b , 255);
        line(r, spike_size/4 + i, spike_size/4 + i, WIDTH - spike_size/4 - i, spike_size/4 + i);//top
        line(r, spike_size/4 + i, HEIGHT - spike_size/4 - i, WIDTH - spike_size/4 - i, HEIGHT - spike_size/4 - i);//bottom
        line(r, spike_size/4 + i, spike_size/4 + i, spike_size/4 + i, HEIGHT - spike_size/4 - i);//left
        line(r, WIDTH - spike_size/4 - i, spike_size/4 + i, WIDTH - spike_size/4 - i, HEIGHT - spike_size/4 - i);//left
    }   
    //////en blurrr

    //text
    text(r, 9.7*WIDTH/40, text_animation*50, "Settings", big, text_animation*c[4*p + 1].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 1].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 1].b + (1-text_animation)*c[4*p + 3].b);
    ///////////////////replace the cursors on the bar before displaying them
    for(int i = 0 ; i < 8 ; i ++){
        if(cursor_positions[i] < 0)
            cursor_positions[i] = 0;
        else if(cursor_positions[i] > WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))
            cursor_positions[i] = WIDTH - 2*(spike_size/4 + WIDTH/10 + 2);
    }
    /////////////////end replace
    color(r, text_animation*c[4*p].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p].b + (1-text_animation)*c[4*p + 3].b, 255);;//line color
    for(int i = 1 ; i < 8 ; i++)
        roundRect(r, spike_size/4 + WIDTH/10, text_animation*spike_size*2 + i*HEIGHT/10, WIDTH - 2*(spike_size/4 + WIDTH/10), 10, 1, 20);//line
    color(r, text_animation*c[4*p+1].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p+1].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p+1].b + (1-text_animation)*c[4*p + 3].b, 255);//cursor color

    //========================================================bird speed
    text(r, text_animation*(spike_size/4 + WIDTH/10), spike_size*1.5 + 1*HEIGHT/10, "Speed", small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[1], (2-text_animation)*spike_size*2 + 1*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[1]*(BIRD_MAX_SPEED+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
    text(r, WIDTH - text_animation*(1.8*WIDTH/10), 4.3*spike_size/2 + 1*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    //=========================================================

    //=========================================================gravity
    text(r, text_animation*(spike_size/4 + WIDTH/10), spike_size*1.5 + 2*HEIGHT/10, "Gravity", small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[2], (2-text_animation)*spike_size*2 + 2*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[2]*(MAX_GRAVITY+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
    text(r, WIDTH - text_animation*1.8*WIDTH/10, 4.3*spike_size/2 + 2*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    //=========================================================

    //=========================================================jump power
    text(r, text_animation*(spike_size/4 + WIDTH/10), spike_size*1.5 + 3*HEIGHT/10, "Jump power", small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[3], (2-text_animation)*spike_size*2 + 3*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[3]*(MAX_JUMP+2)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
    text(r, WIDTH - text_animation*1.8*WIDTH/10, 4.3*spike_size/2 + 3*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    //=========================================================
        
    //=========================================================bird size
    text(r, text_animation*(spike_size/4 + WIDTH/10), spike_size*1.5 + 4*HEIGHT/10, "Size", small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[4], (2-text_animation)*spike_size*2 + 4*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[4]*(BIRD_MAX_SIZE+2)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
    text(r, WIDTH - text_animation*1.8*WIDTH/10, 4.3*spike_size/2 + 4*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    //=========================================================

    //=========================================================spike incrementation
    text(r, text_animation*(spike_size/4 + WIDTH/10), spike_size*1.5 + 5*HEIGHT/10, "Spike simplicity", small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[5], (2-text_animation)*spike_size*2 + 5*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[5]*(MIN_SPIKE_DIFFICULTY+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
    text(r, WIDTH - text_animation*1.8*WIDTH/10, 4.3*spike_size/2 + 5*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    //=========================================================

    //=========================================================animaton
    text(r, text_animation*(spike_size/4 + WIDTH/10), spike_size*1.5 + 6*HEIGHT/10, "Animation speed", small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[6], (2-text_animation)*spike_size*2 + 6*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[6]*(MAX_ANIMATION_SPEED+2)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
    text(r, WIDTH - text_animation*1.8*WIDTH/10, 4.3*spike_size/2 + 6*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);//display a number in front of the setting bar
    //=========================================================

    //=========================================================7th cursor
    roundRect(r, spike_size/4 + WIDTH/10 + cursor_positions[7], (2-text_animation)*spike_size*2 + 7*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
    toChar(tmp, cursor_positions[7]);
    text(r, WIDTH - text_animation*1.8*WIDTH/10, 4.3*spike_size/2 + 7*HEIGHT/10, tmp, small, text_animation*c[4*p + 2].r + (1-text_animation)*c[4*p + 3].r , text_animation*c[4*p + 2].g + (1-text_animation)*c[4*p + 3].g, text_animation*c[4*p + 2].b + (1-text_animation)*c[4*p + 3].b);//display a number in front of the setting bar
    //=========================================================

    //printf("%f\n", text_animation);
    //printf("%f\t", *menu);
    double animation_speed = (*animation)/500.0;
    if(*menu < -1.0){//update animation of entry
        (*menu)+= animation_speed;
        printRestartButton(r, c, p, 1 - text_animation);
    }
    if(*menu < -1.2){
        printSettingButton(r, c, p, 1- text_animation);
        drawBird(r, b, facing, j, c, p, bird_size, 1-text_animation);
    }

    if(*menu > -1.0 && *menu < - 1+ animation_speed)
        (*menu) = -1.0;
    if(*menu > -0.5 && *menu <= -0.00001){//animation of exit
        *menu += animation_speed/2;
        printRestartButton(r, c, p,  1 - text_animation);
    }
    if(*menu > -0.3 && *menu <= -0.00001){//animation of exit
        printSettingButton(r, c, p, 1- text_animation);
        drawBird(r, b, facing, j, c, p, bird_size, 1-text_animation);
    }

    if(*menu >= 0)
        *menu = 0.0;
    
    *birdsize = cursor_positions[4]*(BIRD_MAX_SIZE+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));
    *animation = cursor_positions[6]*(MAX_ANIMATION_SPEED+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));
    if(*animation < 1)
        *animation = 1;//avoid freeze

    printResetSettingsButton(r, small, c, p, text_animation);
    printReturnButton(r, c, p, text_animation);
}






