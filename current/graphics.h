#include "SDL2/SDL.h"
#include "geom.h"

#define D 20 //profondeur de champ
#define L 65 //largeur de champ


typedef enum
{
    QUIT, CONTINUE
} status;


void draw_laby(matrice *, int, SDL_Renderer *);
void draw_segment(SDL_Renderer *renderer, point *, point *);
void draw_croix(SDL_Renderer *, point *);
void draw_murs(int, point *, SDL_Renderer *, int, int, int, int);
void draw_cone(observer *, SDL_Renderer *);
                 
