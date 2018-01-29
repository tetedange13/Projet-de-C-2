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
{ //Renvoie les coord du point projete à dist fact orthogonalement de k

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

int dist_mur(int ind, int marg, int c)
{ //calcul distance point mur

    return ((ind * marg) - c);
}

short in_laby(point *coeff, int h, int l)
{
    if ((coeff->x < h) && (coeff->y < l)) 
        return 1;
    return 0;
}

point *coli_vert(int k, matrice *pm, point *ori, double COSINUS, double SINUS, 
                  int width, int marg, int h, int l, SDL_Renderer *renderer2D)
{
    point *Pk = coord_pix(D, ori, COSINUS, SINUS, k); 
    vect *vect_unit = unitaire(ori, Pk);
    point *current_pix = malloc(sizeof(point));
    current_pix->x = ori->x; current_pix->y = ori->y;
    point *current_coeff = pix_to_coeff(current_pix, marg);
    int i = 0;
    point *preced_coeff = malloc(sizeof(point));
    while (in_laby(current_coeff, h, l)) {
        current_pix->x = (int)(i*vect_unit->x) + ori->x;
        current_pix-> y = (int)(i*vect_unit->y) + ori->y;
        if ((pm->contenu[current_coeff->x][current_coeff->y] == PD) ||
           (pm->contenu[current_coeff->x][current_coeff->y] == 3)) {
            preced_coeff->x = current_coeff->x;
            preced_coeff->y = current_coeff->y;
            while ((current_coeff->x == preced_coeff->x) && 
                  (current_coeff->y == preced_coeff->y)) {
                current_pix->x = (int)(i*vect_unit->x) + ori->x;
                current_pix-> y = (int)(i*vect_unit->y) + ori->y;
                current_coeff = pix_to_coeff(current_pix, marg);
                i++;
            }
            
            
            if ((current_coeff->x == preced_coeff->x + 1) &&
               (preced_coeff->y == current_coeff->y)) {
               //current_coeff = pix_to_coeff(current_pix, marg);
               printf("%d\n", pm->contenu[preced_coeff->x][preced_coeff->y]);
               printf("Preced: "); disp_coord(preced_coeff);
               disp_coord(current_coeff);
               
               return current_pix;
            } else
                current_coeff->x = 0; current_coeff->y = 0;
               
        }
        current_coeff = pix_to_coeff(current_pix, marg);
        i++;
    }
    //point *bogue = coord_pix(2*width, ori, COSINUS, SINUS, k);
    //current_pix->x = 2 * width; current_pix->y = 2 * width;
    return ori; 
}

/*
printf("cos=%lf sin=%lf\n", COSINUS, SINUS);
    printf("Ori="); disp_coord(ori);*/
    
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
	 100,     
	 100,     
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
    double theta = PI/6;
    
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
    
    
    int k;
    SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255); //Rouge
    for (k = -L/2; k <= L/2; k+=20) { 
        point *I = coli_vert(k, pm, ori, COSINUS, SINUS, width2D, marg, h, l,
                             renderer2D);     
        SDL_RenderDrawLine(renderer2D, ori->x, ori->y, I->x, I->y);
    }
    //SDL_SetRenderDrawColor(renderer2D, 0, 0, 0, 255); //Noir
    //draw_laby(pm, marg, renderer2D);         
    SDL_RenderPresent(renderer2D);
    
    
    
    
    
    
    //2e fenetre:
    
    SDL_Window *laby3D = NULL;
    SDL_Renderer *renderer3D = NULL;
    int width3D = 360, height3D = 640;

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
	 800,     
	 100,     
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
    
    
    SDL_SetRenderDrawColor(renderer3D, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer3D, 0, 180, 640, 180);
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
