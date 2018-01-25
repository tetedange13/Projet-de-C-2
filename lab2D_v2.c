#include "lab2D_v2.h"

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

point *coord_pix(int fact, point *ori, double COSINUS, double SINUS, int k)
{
    point *Pk = malloc(sizeof(point));
    Pk->x = fact*COSINUS - k*SINUS + ori->x; 
    Pk->y = fact*SINUS + k*COSINUS + ori->y;
    return Pk;
}

vect *unitaire(point *ori, point *Pk)
{
    vect *vect_unit = malloc(sizeof(vect));
    int norm = dist(ori, Pk);
    vect_unit->x = (Pk->x - ori->x) / (double)norm; 
    vect_unit->y = (Pk->y - ori->y) / (double)norm;
    return vect_unit;
}

int main(int argc, char *argv[]) {
    int width2D = 640, height2D = 640, marg;
    int largeur = 3, hauteur = 3;
    int h = hauteur + 1, l = largeur + 1;
    marg = width2D/(largeur+2);
    tab_nk *tab = init_tab_ouvr(h, l);
    matrice *pm = init_mat(l, h);
    srand(time(NULL));
    gen_laby(tab, pm);
    disp_laby(pm); 
    
    SDL_Window *laby2D = NULL;
    SDL_Renderer *renderer2D = NULL;
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
    draw_laby(pm, marg, renderer2D);
    
    
    
    //TEST:
    point *ori = malloc(sizeof(point)); ori->x = marg+10; ori-> y = marg+10;
    
    int long_U = 20;
    double theta = PI/12;
    
    double COSINUS = cos(theta);
    double SINUS = sin(theta);
    /*double COSINUS_CONE = cos(atan(L/(2.0*D)));
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
    
    
    /*int k;
    for (k = -L/2; k <= L/2; k++) { 
        point *Pk = coord_pix(ori, D, COSINUS, SINUS, k);
    }*/
    
    int i, k = -150;
    point *Pk = coord_pix(D, ori, COSINUS, SINUS, k);
    vect *vect_unit = unitaire(ori, Pk);
    SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255); //Rouge

    for (i = 0; i <= L/2; i++) {
        SDL_RenderDrawPoint(renderer2D, (int)(i*vect_unit->x) + ori->x, 
                                        (int)(i*vect_unit->y) + ori->x);
    }
    printf("cos=%lf sin=%lf\n", COSINUS, SINUS);
    printf("Ori="); disp_coord(ori);
    
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
    
    SDL_SetRenderDrawColor(renderer3D, 255, 255, 255, 0);
    SDL_RenderClear(renderer3D); //fond blanc    
    
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
    
    return 0;
}
