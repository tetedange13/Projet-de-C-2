#include "lab2D.h"

void draw_laby(matrice *pm, int marg, SDL_Renderer *renderer){
    int i, j;
    for (i = 0; i < (pm -> hauteur); i++) {
        for (j = 0; j < (pm -> largeur); j++) {
            switch (pm -> contenu[i][j]) {
            case (PB | PD) : 
            //"_"
            SDL_RenderDrawLine(renderer, (j+1)*marg, (i+1)*marg, 
                                         j*marg, (i+1)*marg);
            //"|"
            SDL_RenderDrawLine(renderer, (j+1)*marg, (i+1)*marg, 
                                         (j+1)*marg, i*marg);
            break;
            case PB :
            //"_ "
            SDL_RenderDrawLine(renderer, (j+1)*marg, (i+1)*marg, 
                                         j*marg, (i+1)*marg);
            break;
            case PD :
            //" |"
            SDL_RenderDrawLine(renderer, (j+1)*marg, (i+1)*marg, 
                                         (j+1)*marg, i*marg);         
            break;
            }
        }
    }
}

/*void segmt_from_angle(double COSINUS,
                      double SINUS, 
                      int long_vect, point *ori, 
                      int marg, SDL_Renderer *renderer)
{
    point *third = malloc(sizeof(point));
    third->x = COSINUS * long_vect + ori->x;
    third->y = SINUS * long_vect + ori->y;  
    disp_coord(third);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);//Vert
    //!\\Cercle trigo à l'envers
    SDL_RenderDrawLine(renderer, ori->x , ori->y,
                                 third->x, third->y); 
} OPTIM POSSIBLE!!*/

void segmt_from_angle(double theta, 
                      int long_vect, point *ori, 
                      int marg, SDL_Renderer *renderer)
{
    point *bout = malloc(sizeof(point));
    bout->x = cos(theta) * long_vect + ori->x;
    bout->y = sin(theta) * long_vect + ori->y;  
    //disp_coord(bout);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);//Vert
    //!\\Cercle trigo à l'envers
    SDL_RenderDrawLine(renderer, ori->x , ori->y,
                                 bout->x, bout->y); 
}

int main(int argc, char *argv[]) {
    SDL_Window *laby2D = NULL;
    SDL_Renderer *renderer2D = NULL;
    int width2D = 640, height2D = 640, marg;
    int largeur = 5, hauteur = 5;
    int h = hauteur + 1, l = largeur + 1;
    marg = width2D/(largeur+2);
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
    laby2D = SDL_CreateWindow
	 ("Labyrinthe",                 
	 SDL_WINDOWPOS_CENTERED,     
	 SDL_WINDOWPOS_CENTERED,     
	 height2D,                         
	 width2D,                         
	 0                            
	 );
    if (laby2D == NULL) {
	fprintf(stderr,
		"\nCreation de la fenetre impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }
    renderer2D = SDL_CreateRenderer(laby2D, -1, 0);
    if (renderer2D == NULL) {
	fprintf(stderr,
		"\nCreation du renderer impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer2D, 255, 255, 255, 0);
    SDL_RenderClear(renderer2D); //fond blanc
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 0, 255);
    //SDL_RenderDrawLine(renderer, 0, 0, 320, 240);
    draw_laby(pm, marg, renderer2D);
    
    
    
    //TEST:
    point *ori = malloc(sizeof(point)); ori->x = marg+10; ori-> y = marg+10;
    
    int long_U = 20;
    double theta = PI/4;
    
    /*double COSINUS = cos(theta);
    double SINUS = sin(theta);
    double COSINUS_CONE = cos(atan(L/(2.0*D)));
    double SINUS_CONE = sin(atan(L/(2.0*D)));
    double SINUS_a-b = SINUS*/
    
    //Tracé de U:                  
    segmt_from_angle(theta, long_U, ori, marg, renderer2D);
    
    //Tracé du cone:
    int long_cone = sqrt( (L/2) * (L/2) + D * D );
    segmt_from_angle(theta - atan(L/(2.0 * D)), 
                     long_cone, ori, marg, renderer2D);
    segmt_from_angle(theta + atan(L/(2.0 * D)), 
                     long_cone, ori, marg, renderer2D);
    
    point *ori_D = malloc(sizeof(point));
    ori_D->x = ori->x + D * cos(theta); ori_D->y = ori->y + D * sin(theta);
    
    segmt_from_angle(theta + PI/2, L/2, ori_D, marg, renderer2D);
    segmt_from_angle(theta - PI/2, L/2, ori_D, marg, renderer2D);
    
    SDL_RenderPresent(renderer2D);
    
    
    
    //2e fenetre:
    
    SDL_Window *laby3D = NULL;
    SDL_Renderer *renderer3D = NULL;
    int width3D = 640, height3D = 640;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr,
                "\nImpossible d'initialiser SDL :  %s\n",
                SDL_GetError()
		);
        exit(1);
    }
    atexit(SDL_Quit);
    laby3D = SDL_CreateWindow
	 ("Vue 3D",                 
	 SDL_WINDOWPOS_CENTERED,     
	 SDL_WINDOWPOS_CENTERED,     
	 height3D,                         
	 width3D,                         
	 0                            
	 );
    if (laby3D == NULL) {
	fprintf(stderr,
		"\nCreation de la fenetre impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }
    renderer3D = SDL_CreateRenderer(laby3D, -1, 0);
    if (renderer3D == NULL) {
	fprintf(stderr,
		"\nCreation du renderer impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }
    
    //FONCTIONS:
    SDL_RenderPresent(renderer3D);
    
    
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
    SDL_DestroyRenderer(renderer2D);
    SDL_DestroyRenderer(renderer3D);
    SDL_DestroyWindow(laby2D);
    SDL_DestroyWindow(laby3D);
    
    /*disp_coord(pix_to_coeff(pix_test, marg));
    point *pix_bis = malloc(sizeof(point)); 
    pix_bis->x = marg+10; pix_bis->y = marg+10;
    printf("marg=%d\n", marg);
    printf("dist=%lf\n", dist(pix_test, pix_bis));
    printf("dist=%lf\n", angle(pix_first, pix_test, pix_bis));*/
    
    /*point *a = malloc(sizeof(point)); a->x = marg+10; a->y = marg+10;
    point *b = malloc(sizeof(point)); b->x = 1; b->y = 0;
    
    theta = angle(a, b,*/ 
    return 0;
}
