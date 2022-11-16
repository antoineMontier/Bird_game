#include "Game_functions.h"



int main(int argc, char *args[]){//compile and execute with     gcc main.c -o main -lm -lSDL2_ttf $(sdl2-config --cflags --libs) && ./main
    
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

    colors[5].r = 225;
    colors[5].g = 0;
    colors[5].b = 35;

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
    TTF_Font *score_font;//font for score display
    TTF_Font *setting_font_big;
    TTF_Font *setting_font_small;

    const double spike_size = HEIGHT/(NB_SPIKES+2);//+2 for the down and up spike border
    srand(time(0));
    openSDL(WIDTH, HEIGHT, 0, &w, &ren);

    setFont(&score_font, "BebasNeue.ttf", 300);
    setFont(&setting_font_big, "BebasNeue.ttf", 100);
    setFont(&setting_font_small, "BebasNeue.ttf", 30);

    SDL_bool program_launched = SDL_TRUE; //SDL_FALSE or SDL_TRUE
    bird birdy;
    double app_r = 0;//0 to 21 0 if fully appeared and 21 is dissapeared
    double app_l = 21;                  //-1 if facing left, 1 if facing right
    int jumped, menu = 0, prev_facing, facing = 1, update_r, update_l, level = 0, tick_count, palette = rand() % PALETTE , k = 0, s = 0, re = 0, spike_number = NB_SPIKES*2;
    //        0for starting bg
    //        1for play
    //       -1for settings
    int setting_selectionned = 0; // 0 if no one is selectionned

    double bird_speed = 8;//if it's upper than 30 may cause bugs
    double gravity = 9.81;
    double bird_jump_pwr = 20;
    double bird_size = 40;
    int spike_increase = 7;

    int*temp = malloc(NB_SPIKES*sizeof(int));
    for(int i = 0 ;i < NB_SPIKES ; i++){
        temp[i] = 1;
    }

    

    drawSpikes(ren, temp, temp, &spike_number, spike_size, app_l, app_r, colors, palette);
    free(temp);
    

    int* s_l = malloc(spike_number*sizeof(int));
    int* s_r = malloc(spike_number*sizeof(int));
    int* cursor_positions = malloc(8*sizeof(int));
    char*tmp = malloc(50*sizeof(char));
    for(int i = 0 ; i < 8 ; i ++)
        cursor_positions[i] = 0;
    cursor_positions[1] = bird_speed*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(BIRD_MAX_SPEED);//initialize the speed at 8
    cursor_positions[2] = gravity*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_GRAVITY);//initialize the gravity at 9.81
    cursor_positions[3] = bird_jump_pwr*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MAX_JUMP);//initialize the jump at 20
    cursor_positions[4] = bird_size*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(BIRD_MAX_SIZE);//initialize the bird's size at 40
    cursor_positions[5] = spike_increase*(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))/(MIN_SPIKE_DIFFICULTY);//initialize the bird's size at 40


    startGame(&birdy, &facing, &prev_facing, &palette, &level, &app_l, &app_r, &update_l, &update_r, s_l, s_r, spike_number, &jumped, &menu, bird_speed, bird_size);


    
    drawBackground(ren, level, score_font, colors, palette);
    //draw landscape
    drawSpikes(ren, s_l, s_r, &spike_number, spike_size, app_l, app_r, colors, palette);
    //draw bird
    drawBird(ren, birdy, facing, &jumped, colors, palette, bird_size);

    menu = 0;


    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while(program_launched){//principal loop
        SDL_Event evt;
        if(menu == 1){
            //update positions
            if(facing == -1 && prev_facing == 1)
                update_r = 1;
            else if(facing == 1 && prev_facing == -1)
                update_l = 1;
            prev_facing = facing;

            bird_speed = cursor_positions[1]*(BIRD_MAX_SPEED+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));
            gravity = cursor_positions[2]*(MAX_GRAVITY+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));
            bird_jump_pwr = cursor_positions[3]*(MAX_JUMP+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));
            bird_size = cursor_positions[4]*(BIRD_MAX_SIZE+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));
            spike_increase = cursor_positions[5]*(MIN_SPIKE_DIFFICULTY+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));


            moveBird(&birdy, &facing, &level, spike_size, spike_size, bird_speed, gravity, bird_size);
            spikeUpdate(s_l, s_r, spike_number, level, &app_l, &app_r, facing, &update_l, &update_r, spike_increase);
            //draw background
            drawBackground(ren, level, score_font, colors, palette);
            //draw landscape
            drawSpikes(ren, s_l, s_r, &spike_number, spike_size, app_l, app_r, colors, palette);
            //draw bird
            drawBird(ren, birdy, facing, &jumped, colors, palette, bird_size);
            if(birdTouchSpike(birdy, facing, spike_size, s_l, s_r, spike_number, bird_size))
               menu = 0;
        }else if(menu == 0){
            drawBackground(ren, level, score_font, colors, palette);
            drawSpikes(ren, s_l, s_r, &spike_number, spike_size, app_l, app_r, colors, palette);
            drawBird(ren, birdy, facing, &jumped, colors, palette, bird_size);
            printRestartButton(ren, colors, palette);
            printSettingButton(ren, colors, palette);
            if(k){
                startGame(&birdy, &facing, &prev_facing, &palette, &level, &app_l, &app_r, &update_l, &update_r, s_l, s_r, spike_number, &jumped, &menu, bird_speed, bird_size);
                k = 0;
            }
            if(s){
                menu = -1;
                s = 0;
            }
        }else if(menu == -1){
            //parameters

            //background
            background(ren, colors[4*palette+3].r, colors[4*palette+3].g, colors[4*palette+3].b, WIDTH, HEIGHT, palette);

            color(ren, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b, 255);

            rect(ren, 0, 0, WIDTH, spike_size/4, 1);//top
            rect(ren, 0, HEIGHT-spike_size/4, WIDTH, spike_size/4, 1);//bottom
            rect(ren, 0, 0, spike_size/4, HEIGHT, 1);//left
            rect(ren, WIDTH-spike_size/4, 0, spike_size/4 +1, HEIGHT, 1);//right
            /////blurrr
            int pwr = 40;
            for(int i = 0 ; i <  pwr ; i ++){
                color(ren,  ((pwr - i)/(double)pwr) *colors[4*palette + 2].r + (i/(double)pwr) * colors[4*palette + 3].r ,
                            ((pwr - i)/(double)pwr) *colors[4*palette + 2].g + (i/(double)pwr) * colors[4*palette + 3].g ,
                            ((pwr - i)/(double)pwr) *colors[4*palette + 2].b + (i/(double)pwr) * colors[4*palette + 3].b , 255);
                line(ren, spike_size/4 + i, spike_size/4 + i, WIDTH - spike_size/4 - i, spike_size/4 + i);//top
                line(ren, spike_size/4 + i, HEIGHT - spike_size/4 - i, WIDTH - spike_size/4 - i, HEIGHT - spike_size/4 - i);//bottom
                line(ren, spike_size/4 + i, spike_size/4 + i, spike_size/4 + i, HEIGHT - spike_size/4 - i);//left
                line(ren, WIDTH - spike_size/4 - i, spike_size/4 + i, WIDTH - spike_size/4 - i, HEIGHT - spike_size/4 - i);//left
            }   
            //////

            //text
            text(ren, 9.7*WIDTH/40, 50, "Settings", setting_font_big, colors[4*palette + 1].r, colors[4*palette + 1].g, colors[4*palette + 1].b);


            ///////////////////replace the cursors on the bar before displaying them
            for(int i = 0 ; i < 8 ; i ++){
                if(cursor_positions[i] < 0)
                    cursor_positions[i] = 0;
                else if(cursor_positions[i] > WIDTH - 2*(spike_size/4 + WIDTH/10 + 1))
                    cursor_positions[i] = WIDTH - 2*(spike_size/4 + WIDTH/10 + 2);
            }
            /////////////////


            color(ren, colors[4*palette].r, colors[4*palette].g, colors[4*palette].b, 255);//line color
            for(int i = 1 ; i < 8 ; i++)
                roundRect(ren, spike_size/4 + WIDTH/10, spike_size*2 + i*HEIGHT/10, WIDTH - 2*(spike_size/4 + WIDTH/10), 10, 1, 20);//line
            color(ren, colors[4*palette + 1].r, colors[4*palette + 1].g, colors[4*palette + 1].b, 255);//cursor color

            //========================================================bird speed
            text(ren, spike_size/4 + WIDTH/10, spike_size*1.5 + 1*HEIGHT/10, "Speed", setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            roundRect(ren, spike_size/4 + WIDTH/10 + cursor_positions[1], spike_size*2 + 1*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
            toChar(tmp, cursor_positions[1]*(BIRD_MAX_SPEED+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
            text(ren, WIDTH - 1.8*WIDTH/10, 4.3*spike_size/2 + 1*HEIGHT/10, tmp, setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            //=========================================================

            //=========================================================gravity
            text(ren, spike_size/4 + WIDTH/10, spike_size*1.5 + 2*HEIGHT/10, "Gravity", setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            roundRect(ren, spike_size/4 + WIDTH/10 + cursor_positions[2], spike_size*2 + 2*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
            toChar(tmp, cursor_positions[2]*(MAX_GRAVITY+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
            text(ren, WIDTH - 1.8*WIDTH/10, 4.3*spike_size/2 + 2*HEIGHT/10, tmp, setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            //=========================================================

            //=========================================================jump power
            text(ren, spike_size/4 + WIDTH/10, spike_size*1.5 + 3*HEIGHT/10, "Jump power", setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            roundRect(ren, spike_size/4 + WIDTH/10 + cursor_positions[3], spike_size*2 + 3*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
            toChar(tmp, cursor_positions[3]*(MAX_JUMP+2)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
            text(ren, WIDTH - 1.8*WIDTH/10, 4.3*spike_size/2 + 3*HEIGHT/10, tmp, setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            //=========================================================
            
            //=========================================================bird size
            text(ren, spike_size/4 + WIDTH/10, spike_size*1.5 + 4*HEIGHT/10, "Size", setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            roundRect(ren, spike_size/4 + WIDTH/10 + cursor_positions[4], spike_size*2 + 4*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
            toChar(tmp, cursor_positions[4]*(BIRD_MAX_SIZE+2)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
            text(ren, WIDTH - 1.8*WIDTH/10, 4.3*spike_size/2 + 4*HEIGHT/10, tmp, setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            //=========================================================

            //=========================================================spike incrementation
            text(ren, spike_size/4 + WIDTH/10, spike_size*1.5 + 5*HEIGHT/10, "Spike simplicity", setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            roundRect(ren, spike_size/4 + WIDTH/10 + cursor_positions[5], spike_size*2 + 5*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
            toChar(tmp, cursor_positions[5]*(MIN_SPIKE_DIFFICULTY+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1)));
            text(ren, WIDTH - 1.8*WIDTH/10, 4.3*spike_size/2 + 5*HEIGHT/10, tmp, setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);
            //=========================================================


            for(int i = 6 ; i < 8 ; i++){
                roundRect(ren, spike_size/4 + WIDTH/10 + cursor_positions[i], spike_size*2 + i*HEIGHT/10 - 5, 10, 20, 1, 20);//cursor
                toChar(tmp, cursor_positions[i]);
                text(ren, WIDTH - 1.8*WIDTH/10, 4.3*spike_size/2 + i*HEIGHT/10, tmp, setting_font_small, colors[4*palette + 2].r, colors[4*palette + 2].g, colors[4*palette + 2].b);//display a number in front of the setting bar
            }
            bird_size = cursor_positions[4]*(BIRD_MAX_SIZE+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));

            


            printReturnButton(ren, colors, palette);
            if(re){//exit parameters
                menu = 0;
                re = 0;
            }
    









        }





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
                            birdy.vy = -bird_jump_pwr;
                            jumped = 90;
                            break;

                        default:
                            break;
                    }


                case SDL_MOUSEBUTTONDOWN:
                    if(menu == 1){
                        birdy.vy = -bird_jump_pwr;
                        jumped = 90;
                    }
                    if(menu == 0){
                        k = rollover(evt.button.x, evt.button.y, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT);
                        s = rollover(evt.button.x, evt.button.y, WIDTH/2 - BUTTON_WIDTH/3, HEIGHT/2 + BUTTON_HEIGHT, 2*BUTTON_WIDTH/3, 2*BUTTON_HEIGHT/3);
                    }
                    if(menu == -1){
                        //setting_selectionned = 0;
                        re = rollover(evt.button.x, evt.button.y, WIDTH - BUTTON_WIDTH, BUTTON_HEIGHT/2, BUTTON_WIDTH/2, BUTTON_HEIGHT/2);
                        for(int i = 1 ; i < 8 && setting_selectionned == 0; i++){
                            if(rollover(evt.button.x, evt.button.y, spike_size/4 + WIDTH/10 + cursor_positions[i], spike_size*2 + i*HEIGHT/10 - 5, 10, 20))
                                setting_selectionned = i;
                        }
                        //update the selectionned setting :




                    }
                    break;

                case SDL_MOUSEMOTION:
                    if(menu == -1 && setting_selectionned != 0)
                        cursor_positions[setting_selectionned] = evt.button.x - (spike_size/4 + WIDTH/10);
                    break;

                case SDL_MOUSEBUTTONUP:
                    if(menu == -1)
                        setting_selectionned = 0;
                    break;

                

                default:
                    break; 

            }
        }
        tick_count++;
        SDL_Delay(1000/FRAMES_PER_SECOND);
        SDL_RenderPresent(ren);//refresh the render
    }
    TTF_CloseFont(setting_font_small);
    TTF_CloseFont(setting_font_big);
    TTF_CloseFont(score_font);
    closeSDL(&w, &ren);
    free(tmp);
    free(cursor_positions);
    free(s_l);
    free(s_r);
    printf("closed successfully !\n");
    return 0;
}
