#include "lab2D_v2.h"

void draw_laby(matrice *pm, int scale, SDL_Renderer *renderer){
    int i, j;
    for (i = 0; i < (pm -> hauteur); i++) {
        for (j = 0; j < (pm -> largeur); j++) {
            switch (pm -> contenu[i][j]) {
            case (PB | PD) : 
		//"_"
		SDL_RenderDrawLine(renderer, (j+1)*scale, (i+1)*scale, 
				   j*scale, (i+1)*scale);
		//"|"
		SDL_RenderDrawLine(renderer, (j+1)*scale, (i+1)*scale, 
				   (j+1)*scale, i*scale);
		break;
            case PB :
		//"_ "
		SDL_RenderDrawLine(renderer, (j+1)*scale, (i+1)*scale, 
				   j*scale, (i+1)*scale);
		break;
            case PD :
		//" |"
		SDL_RenderDrawLine(renderer, (j+1)*scale, (i+1)*scale, 
				   (j+1)*scale, i*scale);         
		break;
            }
        }
    }
}

/*void segmt_from_angle(double COSINUS,
  double SINUS, 
  int long_vect, point *obs, 
  int scale, SDL_Renderer *renderer)
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
                      int scale, SDL_Renderer *renderer)
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

int dist_mur(int ind, int scale, int c)
{ //calcul distance point mur
    return ((ind * scale) - c);
}

short in_laby(point *coeff, int h, int l)
{
    if ((coeff->x < h) && (coeff->y < l)) 
        return 1;
    return 0;
}

void draw_segment(SDL_Renderer *renderer, point *p, point *q)
{
    SDL_RenderDrawLine
	(renderer, p -> x , p -> y, q -> x, q -> y);
}  

void draw_croix(SDL_Renderer *renderer, int x, int y)
{
    SDL_RenderDrawLine(renderer, x, y + 2, x, y - 2);
    SDL_RenderDrawLine(renderer, x - 2, y, x + 2, y);
}

point *cast_vertical(matrice *pm, point *coord, point *pix, int scale, 
                     SDL_Renderer *renderer)
{
    int j_pix = pix -> x / scale;
    int gamma = pix -> x - coord -> x;
    int delta = pix -> y - coord -> y;
    int j_lim, incr, j_start;
    double GAMMA, DELTA;
    // TO DO : gerer le cas d'un mur vertical vu de profil.
    //(position obs multiple de scale et theta multiple de PI/2)
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
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//Rouge
	for(int j = j_start; j != j_lim; j += incr) {
	    GAMMA = j * scale - coord -> x;
	    DELTA = GAMMA * delta / gamma;
        if ( (DELTA + coord -> y < 0) || 
           ((int) ((coord -> y + DELTA) / scale) > (pm -> hauteur - 1)) ) 
            return NULL; //Si le point projete est HORS de l'aire graphique, on arrete  
        //printf("salut %lf %d\n", DELTA, coord -> y);
        //printf("Coord= %lf %lf\n", GAMMA + coord -> x, DELTA + coord -> y);
        //if (( (int)((coord -> y + DELTA) / scale) >= 0) &&
        //(pm->contenu[(int) ((coord -> y + DELTA) / scale)][j - 1] & PD)) {
        if (pm->contenu[(int) ((coord -> y + DELTA) / scale)][j - 1] & PD) {    
            point *I = malloc(sizeof(point));
            I -> x = GAMMA + coord -> x;
            I -> y = DELTA + coord -> y;
	        draw_croix(renderer, I -> x, I -> y);
            return I;
        }
        //if (coord -> y > DELTA) {
        /*if ( coord -> y + DELTA <= -scale ) {
            printf("pas bon %lf %lf %lf %d %d\n", DELTA + coord -> y, DELTA, GAMMA, gamma, delta);
            draw_croix(renderer, GAMMA + coord -> x, DELTA + coord -> y);
            draw_segment(renderer, coord, pix);
        }*/
    }
	return NULL;
}

point *cast_horizontal(matrice *pm, point *coord, point *pix, int scale, 
                     SDL_Renderer *renderer)
{
    int j_pix = pix -> x / scale;
    int gamma = pix -> x - coord -> x;
    int delta = pix -> y - coord -> y;
    int j_lim, incr, j_start;
    double GAMMA, DELTA;
    // TO DO : gerer le cas d'un mur vertical vu de profil.
    //(position obs multiple de scale et theta multiple de PI/2)
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
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//Rouge
	for(int j = j_start; j != j_lim; j += incr) {
	    DELTA = j * scale - coord -> y;
	    GAMMA  =  delta / (gamma*DELTA);
        if ( (DELTA + coord -> y < 0) || 
           ((int) ((coord -> y + DELTA) / scale) > (pm -> hauteur - 1)) ) 
            return NULL; //Si le point projete est HORS de l'aire graphique, on arrete  
        //printf("salut %lf %d\n", DELTA, coord -> y);
        //printf("Coord= %lf %lf\n", GAMMA + coord -> x, DELTA + coord -> y);
        //if (( (int)((coord -> y + DELTA) / scale) >= 0) &&
        //(pm->contenu[(int) ((coord -> y + DELTA) / scale)][j - 1] & PD)) {
        if (pm->contenu[(int) ((coord -> y + GAMMA) / scale)][j - 1] & PB) {    
            point *I = malloc(sizeof(point));
            I -> x = GAMMA + coord -> x;
            I -> y = DELTA + coord -> y;
	        draw_croix(renderer, I -> x, I -> y);
            return I;
        }
        //if (coord -> y > DELTA) {
        /*if ( coord -> y + DELTA <= -scale ) {
            printf("pas bon %lf %lf %lf %d %d\n", DELTA + coord -> y, DELTA, GAMMA, gamma, delta);
            draw_croix(renderer, GAMMA + coord -> x, DELTA + coord -> y);
            draw_segment(renderer, coord, pix);
        }*/
    }
	return NULL;
}


int main(int argc, char *argv[]) {
    int width2D = 640, height2D = 640, scale;
    int largeur = 5, hauteur = 5;
    int h = hauteur + 1, l = largeur + 1;
    scale = width2D/(largeur+2);
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
    draw_laby(pm, scale, renderer2D);
    
    
    
    //TEST:
    point *coord = malloc(sizeof(point));
    coord -> x = 2*scale;
    coord -> y = 3*scale;
    double theta = 5*PI/6;
    //double theta = -PI/2;
    
    observer *obs = malloc(sizeof(observer));
    obs -> coord = coord;
    obs -> sinus = sin(theta);
    obs -> cosinus = cos(theta);
    
    point *orig_ecran = coord_pix(D, obs, 0);
    point *bord_g = coord_pix(D, obs, L/2);
    point *bord_d = coord_pix(D, obs, -L/2);
    SDL_SetRenderDrawColor(renderer2D, 0, 255, 0, 255);//Vert
    draw_segment(renderer2D, obs -> coord, coord_pix(D, obs, 0));
    draw_segment(renderer2D, obs -> coord, bord_g);
    draw_segment(renderer2D, obs -> coord, bord_d);
    SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);//Rouge
    draw_segment(renderer2D, orig_ecran, bord_g);
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
    draw_segment(renderer2D, orig_ecran, bord_d);

    int k;
    for (k = -L/2; k < L/2; k += 1) {
        //printf("k=%d\n", k);
        point *pix = coord_pix(D, obs, k);
        point *I = cast_vertical(pm, coord, pix, scale, renderer2D);
        if(I != NULL){
            draw_segment(renderer2D, coord, I); 
            free(I);
        }
        free(pix); 
    }
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
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
		case SDL_KEYDOWN:
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
