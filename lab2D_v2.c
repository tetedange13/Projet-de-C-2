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

point *cast_vertical(matrice *pm, point *coord, point *pix, int scale)
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
	for(int j = j_start; j != j_lim; j += incr) {
	    GAMMA = j * scale - coord -> x;
	    DELTA = GAMMA * delta / gamma;
        if ( (DELTA + coord -> y < 0) || 
           ((int) ((DELTA + coord -> y) / scale) > (pm -> hauteur - 1)) )
           //Si le point projete est HORS de l'aire graphique, on arrete  
            return NULL; 
        if (pm->contenu[(int) ((DELTA + coord -> y) / scale)][j - 1] & PD) {    
            point *I = malloc(sizeof(point));
            I -> x = GAMMA + coord -> x;
            I -> y = DELTA + coord -> y;
            return I;
        }
    }
	return NULL;
}

point *cast_horizontal(matrice *pm, point *coord, point *pix, int scale)
{
    int i_pix = pix -> y / scale;
    int gamma = pix -> y - coord -> y;
    int delta = pix -> x - coord -> x;
    int i_lim, incr, i_start;
    double GAMMA, DELTA;
    // TO DO : gerer le cas d'un mur vertical vu de profil.
    //(position obs multiple de scale et theta multiple de PI/2)
    if (gamma == 0)
        return NULL;
    if (gamma > 0) {
	    i_start = i_pix + 1;
	    i_lim = pm -> hauteur + 1;
	    incr = 1;
    } else {
	    i_start = i_pix;
	    i_lim = 0;
	    incr = -1;
	}
	for(int i = i_start; i != i_lim; i += incr) {
	    GAMMA = i * scale - coord -> y;
	    DELTA = GAMMA * delta / gamma;
        if ( (DELTA + coord -> x < 0) || 
           ((int) ((DELTA + coord -> x) / scale) > (pm -> largeur - 1)) )
           //Si le point projete est HORS de l'aire graphique, on arrete 
            return NULL; 
        if (pm->contenu[i - 1][(int) ((DELTA + coord -> x) / scale)] & PB) {    
            point *I = malloc(sizeof(point));
            I -> x = DELTA + coord -> x;
            I -> y = GAMMA + coord -> y;
            return I;
        }
    }
	return NULL;
}

point *plus_proche(point *coord, point *I_vert, point *I_horiz)
{ //Détermine quel point est le plus proche de l'ori
    int dist_vert = dist(coord, I_vert);
    int dist_horiz = dist(coord, I_horiz);
    if (dist_vert < dist_horiz)
        return I_vert;
    else
        return I_horiz;
}

void ray_cast(matrice *pm, observer *obs, SDL_Renderer *renderer2D, 
               SDL_Renderer *renderer3D, int scale, 
               int width3D, int height3D, point *middle)
{
    SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);//Rouge
    int k;
    for (k = -L/2; k < L/2; k++) {
        //printf("k=%d\n", k);
        point *pix = coord_pix(D, obs, k);
        point *I_vert = cast_vertical(pm, obs -> coord, pix, scale);
        point *I_horiz = cast_horizontal(pm, obs -> coord, pix, scale);
        point *I;
        if ((I_vert != NULL) && (I_horiz != NULL))
            I = plus_proche(obs -> coord, I_vert, I_horiz);
        else if (I_horiz == NULL) {
            I = I_vert;
            free(I_horiz);
        } else if (I_vert == NULL) {
            I = I_horiz;
            free(I_vert);
        }            
        if(I != NULL){
            
            draw_segment(renderer2D, obs -> coord, I);
            double h = height3D * D / dist(obs -> coord, I);
            int pas = width3D / L;
            //int pas = 2;
            SDL_RenderDrawLine(renderer3D, middle -> x + pas * k, 
                                           middle -> y - (int) (h / 2),
                                           middle -> x + pas * k, 
                                           middle -> y + (int) (h / 2));
            //middle -> x += 8;
            free(I);
        }
        free(pix); 
    }
}

void trapez_cast(matrice *pm, observer *obs, SDL_Renderer *renderer2D, 
               SDL_Renderer *renderer3D, int scale, 
               int width3D, int height3D, point *middle)
{ //BOGUE POSSIBLE ? J'ai eu un CORE DUMPED a un moment..
    point *pix, *I_vert, *I_horiz, *I;
    double h;
    int k = -L/2, pas = width3D / L;
    point * (*functionPtr)(matrice *, point *, point *, int);
    SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);//Rouge
    
    //On trace le 1er trait:
    /*SDL_RenderDrawLine(renderer3D, middle -> x + pas * (-L/2), 
                                           middle -> y - (int) (h / 2),
                                           middle -> x + pas * (-L/2), 
                                           middle -> y + (int) (h / 2));*/
    
    while (k < L/2) {
        printf("salut\n");
        pix = coord_pix(D, obs, k);
        printf("coucou\n");
        I_vert = cast_vertical(pm, obs -> coord, pix, scale);
        I_horiz = cast_horizontal(pm, obs -> coord, pix, scale);
        /*Pointeur vers fonction, qui pointera soit vers cast_vertical, 
        soit vers cast_horizontal: */
        
        if ((I_vert != NULL) && (I_horiz != NULL)) {
            I = plus_proche(obs -> coord, I_vert, I_horiz);
            if ( I == I_vert ) {
                functionPtr = &cast_vertical;
                draw_segment(renderer2D, obs -> coord, I_vert);      
            } else {
                functionPtr = &cast_horizontal;
                draw_segment(renderer2D, obs -> coord, I_horiz); 
            }
        }   
        else if (I_vert != NULL) { //Mur vertical
            functionPtr = &cast_vertical;
            I = I_vert;
            draw_segment(renderer2D, obs -> coord, I_vert);
        } else {
            I = I_horiz;
            functionPtr = &cast_horizontal;
            draw_segment(renderer2D, obs -> coord, I_horiz);
        }
        h = height3D * D / dist(obs -> coord, I);
        
        point *current_I = malloc(sizeof(point));
        current_I -> x = I -> x;
        current_I -> y = I -> y;
        //PAS la bonne condition dans ce while
        while ( ( (current_I -> x / scale) == (I -> x / scale) ) &&
                ( (current_I -> y / scale) == (I -> y / scale) ) ) {
            free(pix);
            free(I);
            I = current_I;
            k++;
            pix = coord_pix(D, obs, k);
            current_I = functionPtr(pm, obs -> coord, pix, scale);
        }
        h = height3D * D / dist(obs -> coord, I);
        draw_segment(renderer2D, obs -> coord, I);
        SDL_RenderDrawLine(renderer3D, middle -> x + pas * (k - 1), 
                                           middle -> y - (int) (h / 2),
                                           middle -> x + pas * (k - 1), 
                                           middle -> y + (int) (h / 2));
        /*free(pix);
        free(current_I);
        free(I);*/
    }        
        /*if ((I_vert != NULL) && (I_horiz != NULL)) {
            I = plus_proche(obs -> coord, I_vert, I_horiz);
        } else if (I_horiz == NULL) {
            I = I_vert;
            free(I_horiz);
        } else if (I_vert == NULL) {
            I = I_horiz;
            free(I_vert);
        }            
        if(I != NULL){
            draw_segment(renderer2D, obs -> coord, I);
            double h = height3D * D / dist(obs -> coord, I);
            int pas = width3D / L;
            //int pas = 2;
            SDL_RenderDrawLine(renderer3D, middle -> x + pas * k, 
                                           middle -> y - (int) (h / 2),
                                           middle -> x + pas * k, 
                                           middle -> y + (int) (h / 2));
            //middle -> x += 8;
            free(I);
        }
        free(pix); 
    }*/
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


       
    
    
    
    
    //2e fenetre:
    
    SDL_Window *laby3D = NULL;
    SDL_Renderer *renderer3D = NULL;
    int width3D = 640, height3D = 320;

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
	 width3D,
	 height3D,                         
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
    
    point *coord = malloc(sizeof(point));
    //coord -> x = scale+10;
    coord -> x = 3*scale;
    //coord -> y = scale+10;
    coord -> y = 3*scale;
    double theta = -PI/3;
    
    observer *obs = malloc(sizeof(observer));
    obs -> coord = coord;
    obs -> sinus = sin(theta);
    obs -> cosinus = cos(theta);
    
    point *middle = malloc(sizeof(point));
    middle -> x = width3D / 2;
    middle -> y = height3D / 2;
    
    
    //RAY CASTING:
    SDL_SetRenderDrawColor(renderer3D, 0, 0, 0, 255);
    //ray_cast(pm, obs, renderer2D, renderer3D, scale, width3D, height3D, middle); 
    trapez_cast(pm, obs, renderer2D, renderer3D, scale, width3D, height3D, middle);

    SDL_SetRenderDrawColor(renderer2D, 0, 0, 0, 255);
    draw_laby(pm, scale, renderer2D);
    point *orig_ecran = coord_pix(D, obs, 0);
    point *bord_g = coord_pix(D, obs, L/2);
    point *bord_d = coord_pix(D, obs, -L/2);
    SDL_SetRenderDrawColor(renderer2D, 0, 255, 0, 255);//Vert
    draw_segment(renderer2D, obs -> coord, coord_pix(D, obs, 0));
    draw_segment(renderer2D, obs -> coord, bord_g);
    draw_segment(renderer2D, obs -> coord, bord_d);
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
    draw_segment(renderer2D, orig_ecran, bord_g);
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
    draw_segment(renderer2D, orig_ecran, bord_d);
    
    SDL_RenderPresent(renderer2D);
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
