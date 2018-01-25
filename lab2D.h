#include "SDL2/SDL.h"
#include "source.h"


typedef enum
{
    QUIT, CONTINUE
} status;


void draw_laby(matrice *pm, int marg, SDL_Renderer *renderer);

void draw_U (double theta, point *ori, int marg, SDL_Renderer *renderer);

