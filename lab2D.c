#include "lab2D.h"
void draw_laby(matrice *pm, int marg, SDL_Renderer *renderer){
    int i, j, x, y;
    for (i = 0; i < (pm -> hauteur); i++) {
        x = marg;
        y = (i+1)*marg;
        for (j = 0; j < (pm -> largeur); j++) {
            switch (pm -> contenu[i][j]) {
            case (PB | PD) : 
            //"_"
            SDL_RenderDrawLine(renderer, x, y, 
                                         x-marg, y);
            //"|"
            SDL_RenderDrawLine(renderer, x, y, 
                                         x, y-marg);
            break;
            case PB :
            //"_ "
            SDL_RenderDrawLine(renderer, x, y, 
                                         x-marg, y);
            break;
            case PD :
            //" |"
            SDL_RenderDrawLine(renderer, x, y, 
                                         x, y-marg);         
            break;
            }
            x+=marg;     
        }
        y+=marg;
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int width = 640, height = 640, marg;
    int largeur = 30, hauteur = 30;
    int h = hauteur + 1, l = largeur + 1;
    marg = width/(largeur+2);
    tab_nk *tab = init_tab_ouvr(h, l);
    matrice *pm = init_mat(l, h);
    srand(time(NULL));
    gen_laby(tab, pm);
    disp_laby(pm); 
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
    draw_laby(pm, marg, renderer);
    do {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
	    switch (e.type) {
	    case SDL_QUIT :
		printf("Exit.\n"); 
		etat = QUIT;
		break;
        }
        }
    } while (etat != QUIT);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
