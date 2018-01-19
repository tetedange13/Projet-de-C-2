#include "lab2D.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"


void draw_laby(matrice *pm, int marg, SDL_Renderer *renderer){
    int i, j;
    for (i = 0; i < (pm -> hauteur); i++) {
        for (j = 0; j < (pm -> largeur); j++) {
            switch (pm -> contenu[i][j]) {
            case (B | D) : 
            //"_"
            SDL_RenderDrawLine(renderer, (i+1)*marg, (j+1)*marg, 
                                         (i+2)*marg, (j+1)*marg);
            //"|"
            SDL_RenderDrawLine(renderer, (i+1)*marg, (j+1)*marg, 
                                         (i+1)*marg, j*marg);
            break;
            case B :
            //"_ "
            SDL_RenderDrawLine(renderer, (i+1)*marg, (j+1)*marg, 
                                         (i+2)*marg, (j+1)*marg);
            break;
            case D :
            //" |"
            SDL_RenderDrawLine(renderer, (i+1)*marg, (j+1)*marg, 
                                         (i+1)*marg, j*marg);            
            break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int width = 640, height = 640;
    
    status etat = CONTINUE;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr,
                "\nImpossible d'initialiser SDL :  %s\n",
                SDL_GetError()
		);
        exit(1);
    }
    atexit(SDL_Quit);
    window = SDL_CreateWindow
	 ("Labyrinte",                 
	 SDL_WINDOWPOS_CENTERED,     
	 SDL_WINDOWPOS_CENTERED,     
	 height,                         
	 width,                         
	 0                            
	 );
    if (window == NULL) {
	fprintf(stderr,
		"\nCreation de la fenetre impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
	fprintf(stderr,
		"\nCreation du renderer impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer); //fond blanc
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderDrawLine(renderer, 0, 0, 320, 240);
    SDL_RenderPresent(renderer);
    
    do {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
	    switch (e.type) {
	    case SDL_QUIT :
		printf("fermeture de la fenetre.\n"); 
		etat = QUIT;
		break;
        }
        }
    } while (etat != QUIT);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

