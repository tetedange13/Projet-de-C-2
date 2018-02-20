#include "graphics.h"



short in_laby(point *coeff, int h, int l);
short est_angle(double, double, int, matrice *);
point *cast_vertical(matrice *, point *, point *, int);
point *cast_horizontal(matrice *, point *, point *, int);
point *plus_proche(point *, point *, point *);
void ray_cast(matrice *, observer *, SDL_Renderer *, SDL_Renderer *, int, int, 
              int, point *);
short meme_case(point *, point *, int);
void trapez_cast(matrice *, observer *, SDL_Renderer *, SDL_Renderer *, int, 
                 int, int, point *, short*);              
                 
