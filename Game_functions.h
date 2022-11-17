#include <stdio.h>
#include <stdlib.h>

#include "SDL_Basics.h"

#define BIRD_MAX_SIZE 200
#define MAX_JUMP 50
#define FRAMES_PER_SECOND 36
#define BUTTON_WIDTH 70
#define BUTTON_HEIGHT 70
#define PALETTE 4
#define NB_SPIKES 10 
#define SPIKE_SPACE_PX 20
#define WING_SPEED 7
#define BIRD_MAX_SPEED 30
#define MAX_GRAVITY 50
#define MIN_SPIKE_DIFFICULTY 20
#define ORIGINAL_BIRD_SPEED 8
#define ORIGINAL_GRAVITY 9.81
#define ORIGINAL_JUMP_PWR 20
#define ORIGINAL_BIRD_SIZE 40
#define ORIGINAL_SPIKE_INCREASE 7


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

void drawLandscape(SDL_Renderer* r, Color*c, int p);
void printRestartButton(SDL_Renderer* r, Color*c, int p);
void jump(double*y, double*vy, int*sj);
void drawSpikes(SDL_Renderer* r, int*s_l, int*s_r, int *spike_nb, double size, double a_l, double a_r, Color*c, int p);
void drawBackground(SDL_Renderer* r, int lvl, TTF_Font*score_font, Color*c, int p);
void spikeUpdate(int *sl, int*sr, int spike_nb, int lvl, double*a_l, double*a_r, int facing, int*u_l, int*u_r, int spike_increase);
void drawBird(SDL_Renderer* r, bird b, int facing, int*j, Color*c, int p, double bird_size);
void moveBird(bird *b, int *facing, int* lvl, int size, double sp_sz, double bird_speed, double gravity, double bird_size);
int birdTouchSpike(bird b, int facing, int spike_sz, int *s_l, int*s_r, int spike_nb, double bird_size);
void startGame(bird*b, int*facing, int*pfacing, int*palette, int*lvl, double*a_l, double*a_r, int*u_l, int*u_r, int*s_l, int*s_r, int sn, int*jumped, int*menu, double bird_speed, double bird_size);
void printSettingButton(SDL_Renderer* r, Color*c, int p);
void printReturnButton(SDL_Renderer* r, Color*c, int p);
void printResetSettingsButton(SDL_Renderer* r, TTF_Font*f, Color*c, int p);
