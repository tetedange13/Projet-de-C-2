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

void draw_segment(SDL_Renderer *renderer, point *p, point *q) {
    SDL_RenderDrawLine
	(renderer, p -> x , p -> y, q -> x, q -> y);
}

/*void segmt_from_angle(double COSINUS,
  double SINUS, 
  int long_vect, point *obs, 
  int marg, SDL_Renderer *renderer)
  {
  point *third = malloc(sizeof(point));
  third->x = COSINUS * long_vect + obs->x;
  third->y = SINUS * long_vect + obs->y;  
  disp_coord(third);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);//Vert
  //!\\Cercle trigo à l'envers
  SDL_RenderDrawLine(renderer, obs->x , obs->y,
  third->x, third->y); 
  } OPTIM POSSIBLE!!*/

void segmt_from_angle(double theta, 
                      int long_vect, point *obs, 
                      int marg, SDL_Renderer *renderer)
{
    point *bout = malloc(sizeof(point));
    bout->x = cos(theta) * long_vect + obs->x;
    bout->y = sin(theta) * long_vect + obs->y;  
    //disp_coord(bout);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);//Vert
    //!\\Cercle trigo à l'envers
    SDL_RenderDrawLine(renderer, obs->x , obs->y,
		       bout->x, bout->y); 
}

point *coord_pix(int fact, point *obs, double COSINUS, double SINUS, int k)
{ //Renvoie les coord du point projete à dist fact orthogonalement de k
    point *Pk = malloc(sizeof(point));
    Pk -> x = fact * COSINUS - k * SINUS   + obs -> x; 
    Pk -> y = fact * SINUS   + k * COSINUS + obs -> y;
    return Pk;
}

vect *unitaire(point *obs, point *Pk)
{
    vect *vect_unit = malloc(sizeof(vect));
    int norm = dist(obs, Pk);
    vect_unit->x = (Pk->x - obs->x) / (double)norm; 
    vect_unit->y = (Pk->y - obs->y) / (double)norm;
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

void draw_croix(SDL_Renderer *renderer, int x, int y)
{
    SDL_RenderDrawLine(renderer, x, y + 2, x, y - 2);
    SDL_RenderDrawLine(renderer, x - 2, y, x + 2, y);
}

point *cast_vertical(matrice *pm, point *obs, point *pix, int scale, 
                     SDL_Renderer *renderer)
{
    int j_pix = pix -> x / scale;
    int gamma = pix -> x - obs -> x;
    int delta = pix -> y - obs -> y;
    int j_lim, incr, GAMMA, j_start;
    double DELTA;
    point *I = malloc(sizeof(point));
    // TO DO : gerer le cas d'un mur vertical vu de profil.
    //(position obs multiple de marg et theta multiple de PI/2)
    if (gamma == 0)
        return NULL;
    if (gamma > 0) {
	    j_start = j_pix + 1;
	    j_lim = pm -> largeur + 1;
	    incr = 1;
    } else {
	    j_start = j_pix;
	    j_lim = 0;
	    incr = -1;
	}
	for(int j = j_start; j != j_lim; j += incr) {
	    GAMMA = j * scale - obs -> x;
	    DELTA = GAMMA * delta / gamma;
	    I -> x = GAMMA + obs -> x;
        I -> y = (int) DELTA + obs -> y;
        //printf("Mat=%d\n", pm->contenu[coeff_DELTA -> x - 1][coeff_DELTA -> y]);
        printf("salut %d %d\n", j, (int) ((obs -> y + DELTA) / scale));
        if (( (int)((obs -> y + DELTA) / scale) >= 0) &&
        (pm->contenu[(int) ((obs -> y + DELTA) / scale)][j - 1] & PD)) {
	        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//Rouge
	        draw_croix(renderer, I -> x, I -> y);
            return I;
        }              
	}
	
	return NULL;
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
    point *obs = malloc(sizeof(point));
    obs -> x = 3*marg;
    obs -> y = 3*marg;
    double theta = -5*PI/6;
    double COSINUS = cos(theta);
    double SINUS = sin(theta);
    
    point *orig_ecran =
	coord_pix(D, obs, COSINUS, SINUS, 0);
    point *bord_g =
	coord_pix(D, obs, COSINUS, SINUS, L/2);
    point *bord_d =
	coord_pix(D, obs, COSINUS, SINUS, -L/2);
    SDL_SetRenderDrawColor(renderer2D, 0, 255, 0, 255);//Vert
    draw_segment(renderer2D, obs, orig_ecran);
    SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);//Rouge
    draw_segment(renderer2D, orig_ecran, bord_g);
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
    draw_segment(renderer2D, orig_ecran, bord_d);
    
      
    int long_U = 20;
    
    /*double COSINUS_CONE = cos(atan(L/(2.0*D)));
      double SINUS_CONE = sin(atan(L/(2.0*D)));
      double SINUS_a-b = SINUS*/
    
    
    
    
    //Tracé de U:                  
    segmt_from_angle(theta, long_U, obs, marg, renderer2D);
    
    //Tracé du cone:
    int long_cone = sqrt( (L/2) * (L/2) + D * D );
    segmt_from_angle(theta - atan(L/(2.0 * D)), 
                     long_cone, obs, marg, renderer2D);
    segmt_from_angle(theta + atan(L/(2.0 * D)), 
                     long_cone, obs, marg, renderer2D);
    
    point *obs_D = malloc(sizeof(point));
    obs_D->x = obs->x + D * cos(theta); obs_D->y = obs->y + D * sin(theta);
    
    segmt_from_angle(theta + PI/2, L/2, obs_D, marg, renderer2D);
    segmt_from_angle(theta - PI/2, L/2, obs_D, marg, renderer2D);

    int k;
    for (k = -L/2; k < L/2; k += 1) {
        point *pix = coord_pix(D, obs, COSINUS, SINUS, k);
        point *I = cast_vertical(pm, obs, pix, marg, renderer2D);
        
        if(I != NULL){
            draw_segment(renderer2D, obs, I);
            printf("k=%d\n", k);
            free(I);
        }
        free(pix); 
    }
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
