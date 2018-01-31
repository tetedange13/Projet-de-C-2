#include "SDL2/SDL.h"
#include "source2.h"



typedef enum
{
    QUIT, CONTINUE
} status;


void draw_laby(matrice *pm, int marg, SDL_Renderer *renderer);
void draw_U (double theta, point *, int marg, SDL_Renderer *renderer);
point *coord_pix(int fact, point *, double COSINUS, double SINUS, int k);
vect *unitaire(point *, point *Pk);
int dist_mur(int ind, int marg, int c);
short in_laby(point *coeff, int h, int l);
short check_vert(point *A, point *B);
point *coli_vert(int k, matrice *pm, point *, double COSINUS, double SINUS, 
                  int width, int marg, int h, int l, SDL_Renderer *renderer2D);
short check_horizontal(point *A, point *B);
