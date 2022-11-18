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
    double menu = 0;
    int jumped, prev_facing, facing = 1, update_r, update_l, level = 0, high_level = 0, tick_count, palette = rand() % PALETTE , k = 0, s = 0, re = 0, spike_number = NB_SPIKES*2;
    //        0for starting bg
    //        1for play
    //       -1for settings
    int setting_selectionned = 0; // 0 if no one is selectionned


    double bird_speed, gravity, bird_jump_pwr, bird_size;//game parameters
    int spike_increase, animation;

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

    //=============== open file containing the high score :
    FILE* best_score = fopen("./bestscore.txt", "r");
    if(best_score == NULL)
        fprintf(stderr, "failed to open file");
    fscanf(best_score, "%d", &high_level);//get the previous high score
    //remove("./best_score.txt");
    fclose(best_score);

    FILE* best_scoree = fopen("./bestscore.txt", "wt");

    //==============

    resetSettingsAndCursors(cursor_positions, &bird_speed, &gravity, &bird_jump_pwr, &bird_size, &spike_increase, &animation, spike_size);


    startGame(&birdy, &facing, &prev_facing, &palette, &level, &app_l, &app_r, &update_l, &update_r, s_l, s_r, spike_number, &jumped, &menu, bird_speed, bird_size);


    
    drawBackground(ren, level, score_font, colors, palette, 1);
    //draw landscape
    drawSpikes(ren, s_l, s_r, &spike_number, spike_size, app_l, app_r, colors, palette);
    //draw bird
    drawBird(ren, birdy, facing, &jumped, colors, palette, bird_size, 1);

    menu = 0.0;


    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    while(program_launched){//principal loop
        SDL_Event evt;

        




        if(menu == 1.0){
            //update level
            if(level > high_level)
                high_level = level;
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
            animation = cursor_positions[6]*(MAX_ANIMATION_SPEED+1)/(WIDTH - 2*(spike_size/4 + WIDTH/10 + 1));


            moveBird(&birdy, &facing, &level, spike_size, spike_size, bird_speed, gravity, bird_size);
            spikeUpdate(s_l, s_r, spike_number, level, &app_l, &app_r, facing, &update_l, &update_r, spike_increase);
            //draw background
            drawBackground(ren, level, score_font, colors, palette, 1);
            printHighScore(ren, setting_font_small, tmp, high_level, colors, palette, 1);

            //draw landscape
            drawSpikes(ren, s_l, s_r, &spike_number, spike_size, app_l, app_r, colors, palette);
            //draw bird
            drawBird(ren, birdy, facing, &jumped, colors, palette, bird_size, 1);
            if(birdTouchSpike(birdy, facing, spike_size, s_l, s_r, spike_number, bird_size))
                menu = 0; 
            }else if(menu == 0.0){
            drawBackground(ren, level, score_font, colors, palette, 1);
            printHighScore(ren, setting_font_small, tmp, high_level, colors, palette, 1);
            drawSpikes(ren, s_l, s_r, &spike_number, spike_size, app_l, app_r, colors, palette);
            drawBird(ren, birdy, facing, &jumped, colors, palette, bird_size, 1);
            printRestartButton(ren, colors, palette, 1);
            printSettingButton(ren, colors, palette, 1);
            if(k){
                startGame(&birdy, &facing, &prev_facing, &palette, &level, &app_l, &app_r, &update_l, &update_r, s_l, s_r, spike_number, &jumped, &menu, bird_speed, bird_size);
                k = 0;
            }
            if(s){
                menu = -1.99;
                s = 0;
            }
        }else if(menu > -2.0 && menu <= -0.0001){
            //parameters
            
            if(re){//exit parameters
                menu = -0.49;
                re = 0;
            }
            printSettingMenu(ren, setting_font_big, setting_font_small, score_font, cursor_positions, spike_size, colors, palette, tmp, &bird_size, &menu, level, high_level, s_r, s_l, &spike_number, birdy, facing, &jumped, bird_size, &animation);
            if(level == 0){
                birdy.x = WIDTH/2 - bird_size/2;
            }
            
            //printf("\n\nm : %f\n", menu);
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
                    if(menu == 1.0){
                        birdy.vy = -bird_jump_pwr;
                        jumped = 90;
                    }
                    if(menu == 0.0){
                        k = rollover(evt.button.x, evt.button.y, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 + BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT);
                        s = rollover(evt.button.x, evt.button.y, WIDTH - (WIDTH/2 + BUTTON_WIDTH/3), HEIGHT/2 + BUTTON_HEIGHT*2, 2*BUTTON_WIDTH/3, 2*BUTTON_HEIGHT/3);
                    }
                    if(menu == -1.0){
                        //setting_selectionned = 0;
                        re = rollover(evt.button.x, evt.button.y, WIDTH - BUTTON_WIDTH, BUTTON_HEIGHT/2, BUTTON_WIDTH/2, BUTTON_HEIGHT/2);
                        for(int i = 1 ; i < 8 && setting_selectionned == 0; i++){
                            if(rollover(evt.button.x, evt.button.y, spike_size/4 + WIDTH/10 + cursor_positions[i], spike_size*2 + i*HEIGHT/10 - 5, 10, 20))
                                setting_selectionned = i;
                        }
                        if(rollover(evt.button.x, evt.button.y, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT/2))
                            resetSettingsAndCursors(cursor_positions, &bird_speed, &gravity, &bird_jump_pwr, &bird_size, &spike_increase, &animation, spike_size);
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if(menu == -1.0 && setting_selectionned != 0.0)
                        cursor_positions[setting_selectionned] = evt.button.x - (spike_size/4 + WIDTH/10);
                    break;

                case SDL_MOUSEBUTTONUP:
                    if(menu == -1.0)
                        setting_selectionned = 0;
                    break;

                

                default:
                    break; 

            }
        }
        tick_count++; 
        /////red lines
        /*int x = 0;
        color(ren, 255, 0, 0, 255);
        while(x < WIDTH){
            line(ren, x, 0, x, HEIGHT);
            x += WIDTH/40.0;
        }
        x= 0;
        while(x < HEIGHT){
            line(ren, 0, x, WIDTH, x);
            x+=HEIGHT/40.0;
        }*/

        SDL_Delay(1000/FRAMES_PER_SECOND);
        SDL_RenderPresent(ren);//refresh the render
    }
    //===== update file
    fprintf(best_score, "%d", high_level);
    fclose(best_scoree);
    //=====
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
