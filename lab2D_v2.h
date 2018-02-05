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
point *cast_vertical(matrice *, point *, point *, int);
point *cast_horizontal(matrice *, point *, point *, int);
point *plus_proche(point *, point *, point *);
void ray_cast(matrice *, observer *, SDL_Renderer *, SDL_Renderer *, int, int, 
              int, point *);
