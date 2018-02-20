#include "graphics.h"

void draw_laby(matrice *pm, int scale, SDL_Renderer *renderer){
    int i, j;
    for (i = 0; i < (pm -> hauteur); i++) {
        for (j = 0; j < (pm -> largeur); j++) {
            switch (pm -> contenu[i][j]) {
            case (PB | PD) : 
		//"_|"
		SDL_RenderDrawLine(renderer, (j+1)*scale, (i+1)*scale, 
				   j*scale, (i+1)*scale);
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

short in_laby(point *coeff, int h, int l)
{
    if ((coeff->x < h) && (coeff->y < l)) 
        return 1;
    return 0;
}

void draw_segment(SDL_Renderer *renderer, point *p, point *q)
{
    SDL_RenderDrawLine
	(renderer, (int) (p -> x), (int) (p -> y), 
	           (int) (q -> x), (int) (q -> y) );
}  

void draw_croix(SDL_Renderer *renderer, point *pt)
{
    SDL_RenderDrawLine(renderer, (int) (pt -> x), (int) (pt -> y + 2), 
                                 (int) (pt -> x), (int) (pt -> y - 2) );
    SDL_RenderDrawLine(renderer, (int) (pt -> x - 2), (int) (pt -> y), 
                                 (int) (pt -> x + 2), (int) (pt -> y) );
}

point *cast_vertical(matrice *pm, point *coord, point *pix, int scale)
{
    int j_pix = pix -> x / scale;
    double gamma = pix -> x - coord -> x;
    double delta = pix -> y - coord -> y;
    int j_lim, incr, j_start;
    double GAMMA, DELTA;
    if ( (int) (gamma) == 0) {
        printf("c'est gamma vertical\n");
        return NULL;
    } if (gamma > 0) {
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
           ((int) ((DELTA + coord -> y) / scale) > (pm -> hauteur - 1)) ) {
           //Si le point projete est HORS de l'aire graphique, on arrete  
            printf("C'est sorti vert\n");
            return NULL;
        } if ( ( (int) (DELTA + coord -> y) % scale == 0 ) ||
               (pm->contenu[(int) ((DELTA + coord -> y) / scale)][j - 1] & PD) )
          {    
            point *I = malloc(sizeof(point));
            I -> x = GAMMA + coord -> x;
            I -> y = DELTA + coord -> y;
            return I;
        }
    }
    printf("C'est la fin vert\n");
	return NULL;
}

point *cast_horizontal(matrice *pm, point *coord, point *pix, int scale)
{
    int i_pix = pix -> y / scale;
    int gamma = pix -> y - coord -> y;
    int delta = pix -> x - coord -> x;
    int i_lim, incr, i_start;
    double GAMMA, DELTA;
    if ( (int) gamma == 0) {
        printf("c'est gamma vertical\n");
        return NULL;
    } if (gamma > 0) {
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
           ((int) ((DELTA + coord -> x) / scale) > (pm -> largeur - 1)) ) {
           //Si le point projete est HORS de l'aire graphique, on arrete
            printf("C'est sorti horiz\n");
            return NULL;
        } if ( ( (int) (DELTA + coord -> x) % scale == 0 ) ||
             (pm->contenu[i - 1][(int) ((DELTA + coord -> x) / scale)] & PB) ) {    
            point *I = malloc(sizeof(point));
            I -> x = DELTA + coord -> x;
            I -> y = GAMMA + coord -> y;
            return I;
        }
    }
    printf("C'est la fin horiz\n");
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
    SDL_SetRenderDrawColor(renderer3D, 0, 0, 0, 255);
    int k;
    for (k = -L/2; k < L/2; k++) {
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
            SDL_RenderDrawLine(renderer3D, middle -> x + pas * k, 
                                           middle -> y - (int) (h / 2),
                                           middle -> x + pas * k, 
                                           middle -> y + (int) (h / 2));
            free(I);
        }
        free(pix); 
    }
}

short meme_case(point *a, point *b, int scale)
{
    if ( (a == NULL) || (b == NULL) )
        return 0;
    if ( ( (int) (a -> x / scale) == (int) (b -> x / scale) ) &&
         ( (int) (a -> y / scale) == (int) (b -> y / scale) ) )
       return 1;
    return 0;   
}

point *point_impact(observer *obs, int k, int scale, matrice *pm)
{
    point *pix = coord_pix(D, obs, k);
    point *I_vert = cast_vertical(pm, obs -> coord, pix, scale);
    point *I_horiz = cast_horizontal(pm, obs -> coord, pix, scale);
    if ( (I_horiz == NULL) && (I_vert == NULL) )
        printf("c'est la merde\n");
    point *I;
    if ((I_vert != NULL) && (I_horiz != NULL)) {
        I = plus_proche(obs -> coord, I_vert, I_horiz);
        if(I == I_vert) 
            free(I_horiz);
        else 
            free(I_vert);
    } else if (I_horiz == NULL) {
        I = I_vert;
    } else if (I_vert == NULL) {
        I = I_horiz;
    }
    free(pix);
    return I;                    
}

void trapez_cast(matrice *pm, observer *obs, SDL_Renderer *renderer2D, 
               SDL_Renderer *renderer3D, int scale, 
               int width3D, int height3D, point *middle)
{
    double h;
    int k = -L/2, pas = width3D / L + 1, k_tmp, h_tmp;
    /*Pointeur vers fonction, qui pointera soit vers cast_vertical, 
    soit vers cast_horizontal: */    
    SDL_SetRenderDrawColor(renderer3D, 0, 255, 255, 0);
    while (k < L/2) {
        point *I = point_impact(obs, k, scale, pm);    
        /*if (I == NULL) {
            avance(obs, 1);
            printf("sprint 666666666666666666666\n");
            continue;
        }*/
        SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);//Rouge
        draw_segment(renderer2D, obs -> coord, I);
        h = 1.5 * height3D * D / dist(obs -> coord, I);
        SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k), 
                                       (int) (middle -> y - h / 2),
                                       (int) (middle -> x + pas * k), 
                                       (int) (middle -> y + h / 2));     
        k_tmp = k;
        h_tmp = h;
        point *current_I = malloc(sizeof(point));
        current_I -> x = I -> x;
        current_I -> y = I -> y;
        while ( (k < (L/2)) && (meme_case(I, current_I, scale)) ) {
            //free(current_I);
            current_I = point_impact(obs, ++k, scale, pm);    
        }
        current_I = point_impact(obs, k - 1, scale, pm);
        if (current_I != NULL) {
            SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
            draw_segment(renderer2D, obs -> coord, current_I);
            h = 1.5 * height3D * D / dist(obs -> coord, current_I);
            
            free(current_I);
            SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k), 
                                           (int) (middle -> y - h / 2),
                                           (int) (middle -> x + pas * k), 
                                           (int) (middle -> y + h / 2) ); 
            SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k_tmp), 
                                           (int) (middle -> y + h_tmp / 2),
                                           (int) (middle -> x + pas * k), 
                                           (int) (middle -> y + h / 2) );  
            SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k_tmp), 
                                           (int) (middle -> y - h_tmp / 2),
                                           (int) (middle -> x + pas * k), 
                                           (int) (middle -> y - h / 2) ); 
        }
    }        
}

void draw_cone(observer *obs, SDL_Renderer *renderer2D)
{
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
    free(orig_ecran);
    free(bord_g);
    free(bord_d);
}

int main(int argc, char *argv[]) {
    int largeur = 5, hauteur = 5;
    int h = hauteur + 1, l = largeur + 1;
    tab_nk *tab = init_tab_ouvr(h, l);
    matrice *pm = init_mat(l, h);
    srand(time(NULL));
    gen_laby(tab, pm);
    disp_laby(pm);
    SDL_Window *laby2D = NULL;
    SDL_Renderer *renderer2D = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr,
                "\nImpossible d'initialiser SDL :  %s\n",
                SDL_GetError()
		);
        exit(1);
    }
    atexit(SDL_Quit);
    int width2D = 640, height2D = 640;
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
    
    int scale = width2D/(largeur+2);
    point *coord = malloc(sizeof(point));
    coord -> x = 3*scale;
    coord -> y = 3*scale;
    double theta = PI/3;
    
    observer *obs = malloc(sizeof(observer));
    obs -> coord = coord;
    obs -> sinus = sin(theta);
    obs -> cosinus = cos(theta);
    
    point *middle = malloc(sizeof(point));
    middle -> x = width3D / 2;
    middle -> y = height3D / 2;
    
    
    //RAY CASTING:
    trapez_cast(pm, obs, renderer2D, renderer3D, scale, width3D, height3D, 
                middle);
     
    SDL_SetRenderDrawColor(renderer2D, 0, 0, 0, 255);
    draw_laby(pm, scale, renderer2D);
    draw_cone(obs, renderer2D);
    
    SDL_RenderPresent(renderer2D);
    SDL_RenderPresent(renderer3D);
       
    status etat = CONTINUE;
    SDL_Event event;
    while (etat != QUIT)
    {
        if (SDL_PollEvent(&event)) {
            if ( (event.type == SDL_WINDOWEVENT) && 
                 (event.window.event == SDL_WINDOWEVENT_CLOSE) ) {    
                printf("EXIT\n");
                etat = QUIT;
            } else {
                //On commence par effacer les rendus precedents:
                SDL_SetRenderDrawColor(renderer2D, 255, 255, 255, 0);
                SDL_RenderClear(renderer2D); //fond blanc
                SDL_SetRenderDrawColor(renderer3D, 0, 0, 0, 255);
                SDL_RenderClear(renderer3D); //fond noir
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        avance(obs, 5);
                        break;
                    case SDLK_DOWN:
                        avance(obs, -5);
                        break;    
                    case SDLK_LEFT:
                        theta -= 0.1;
                        rotate(theta, obs);
                        break;
                    case SDLK_RIGHT:
                        theta += 0.1;
                        rotate(theta, obs);
                        break;                
                }
                //Dans tous les cas on cree un nveau rendu et on l'affiche:
                SDL_SetRenderDrawColor(renderer3D, 0, 0, 0, 255);
                trapez_cast(pm, obs, renderer2D, renderer3D, scale, 
                         width3D, height3D, middle); 
                SDL_SetRenderDrawColor(renderer2D, 0, 0, 0, 255);
                draw_laby(pm, scale, renderer2D);
                draw_cone(obs, renderer2D);                                     
                SDL_RenderPresent(renderer2D);
                SDL_RenderPresent(renderer3D);
            }
        }
    }
    SDL_DestroyRenderer(renderer2D);
    SDL_DestroyRenderer(renderer3D);
    SDL_DestroyWindow(laby2D);
    SDL_DestroyWindow(laby3D);    
    return 0;
}
