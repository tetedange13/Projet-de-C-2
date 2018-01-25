#include "SDL2/SDL.h"
#include "source2.h"


typedef enum
{
    QUIT, CONTINUE
} status;


void draw_laby(matrice *pm, int marg, SDL_Renderer *renderer);

void draw_U (double theta, point *ori, int marg, SDL_Renderer *renderer);

point *coord_pix(int fact, point *ori, double COSINUS, double SINUS, int k);

vect *unitaire(point *ori, point *Pk);
