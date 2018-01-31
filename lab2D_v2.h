#include "SDL2/SDL.h"
#include "geom.h"



typedef enum
{
    QUIT, CONTINUE
} status;


void draw_laby(matrice *, int, SDL_Renderer *);
void segment_from_angle(double, int , point *, int, SDL_Renderer *);
int dist_mur(int ind, int marg, int c);
short in_laby(point *coeff, int h, int l);
void draw_segment(SDL_Renderer *renderer, point *, point *);
void draw_croix(SDL_Renderer *, int, int);
short check_vert(point *A, point *B);
point *cast_vertical(matrice *, point *, point *, int, SDL_Renderer *);
