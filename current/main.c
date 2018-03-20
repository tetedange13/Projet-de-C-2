#include "main.h"


short in_laby(point *coeff, int h, int l)
{
    if ((coeff->x < h) && (coeff->y < l)) 
        return 1;
    return 0;
}

short est_angle(double coord_x, double coord_y, int scale, matrice *pm)
{
    if ( ( ( (int) (coord_x / scale) == 1 ) && 
         ( (int) (coord_y / scale) == pm -> largeur ) ) //En haut a droite
       || ( ( (int) (coord_x / scale) == pm -> largeur ) && 
         ( (int) (coord_y / scale) == 1 ) ) ) //En haut a droite 
        return 1;
    if ( ( (int) (coord_x / scale) == 0 ) && 
         ( (int) (coord_y / scale) == 1 ) ) //En haut a gauche
        return 1;
    if ( ( (int) (coord_x / scale) == pm -> hauteur ) && 
         ( (int) (coord_y / scale) == pm -> largeur ) ) //En bas a droite
        return 1;
    if ( ( (int) (coord_x / scale) == 0 ) && 
         ( (int) (coord_y / scale) == pm -> hauteur ) ) //En bas a gauche
        return 1;
    return 0;
}

point *cast_vertical(matrice *pm, point *coord, point *pix, int scale)
{
    int j_pix = pix -> x / scale;
    double gamma = pix -> x - coord -> x;
    double delta = pix -> y - coord -> y;
    int j_lim, incr, j_start;
    double GAMMA, DELTA;
    if ( (int) (gamma) == 0) {
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
        if ( est_angle(GAMMA + coord -> x, DELTA + coord -> y, scale, pm) ) {
            point *I = malloc(sizeof(point));
            I -> x = GAMMA + coord -> x;
            I -> y = DELTA + coord -> y;
            return I;
        } if ( (DELTA + coord -> y < 0) || 
           ((int) ((DELTA + coord -> y) / scale) > (pm -> hauteur - 1)) ) {
           //Si le point projete est HORS de l'aire graphique, on arrete  
            return NULL;
        } if (pm->contenu[(int) ((DELTA + coord -> y) / scale)][j - 1] & PD) {
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
    if ( (int) gamma == 0) {
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
	for (int i = i_start; i != i_lim; i += incr) {
	    GAMMA = i * scale - coord -> y;
	    DELTA = GAMMA * delta / gamma;
        if ( est_angle(DELTA + coord -> x, GAMMA + coord -> y, scale, pm) ) {
            point *I = malloc(sizeof(point));
            I -> x = DELTA + coord -> x;
            I -> y = GAMMA + coord -> y;
            return I;
        } if ( (DELTA + coord -> x < 0) || 
           ((int) ((DELTA + coord -> x) / scale) > (pm -> largeur - 1)) ) {
           //Si le point projete est HORS de l'aire graphique, on arrete
            return NULL; 
        } if (pm->contenu[i - 1][(int) ((DELTA + coord -> x) / scale)] & PB) {
            point *I = malloc(sizeof(point));
            I -> x = DELTA + coord -> x;
            I -> y = GAMMA + coord -> y;
            return I;
        }
    }
	return NULL;
}

point *plus_proche(point *coord, point *I_vert, point *I_horiz)
{ //Determine quel point est le plus proche de l'ori
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
               int width3D, int height3D, point *middle, short *test)
{
    double h;
    int k = -L/2, k_tmp, h_tmp;
    while (k < L/2) {
        point *I = point_impact(obs, k, scale, pm);  
        if (I == NULL) {
            //avance(obs, 1);
            SDL_SetRenderDrawColor(renderer2D, 0, 255, 0, 255);//Vert
            point *pix = coord_pix(D, obs, k);
            int vect = pix -> x - obs -> coord -> x;
            int vect2 = pix -> y - obs -> coord -> y;
            pix -> x = pix -> x + scale*vect/5; 
            pix -> y = pix -> y + scale*vect2/5; 
            draw_segment(renderer2D, obs -> coord, pix);
            free(pix);
            k++;
            printf("sprint 666666666666666666666\n");
            *test = 0;
            continue;
        }
        SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);//Rouge
        draw_segment(renderer2D, obs -> coord, I);
        h = 1.5 * height3D * D / dist(obs -> coord, I);
        k_tmp = k; 
        h_tmp = h;
        point *current_I = malloc(sizeof(point));
        current_I -> x = I -> x;
        current_I -> y = I -> y;
        while ( (k < (L/2)) && (meme_case(I, current_I, scale)) ) {
            point *tmp = current_I;
            current_I = point_impact(obs, ++k, scale, pm);
            free(tmp);  
        }
        free(current_I);
        free(I);
        if ( (k - 1) != k_tmp ) {
            current_I = point_impact(obs, k - 1, scale, pm);
            SDL_SetRenderDrawColor(renderer2D, 0, 0, 255, 255);//Bleu
            draw_segment(renderer2D, obs -> coord, current_I);
            h = 1.5 * height3D * D / dist(obs -> coord, current_I);
            free(current_I);
            draw_murs(width3D, middle, renderer3D, k_tmp, h_tmp, k, h);
        } else
            draw_murs(width3D, middle, renderer3D, k_tmp, h_tmp, k, h);
    }
            
}

short pas_dans_mur(point *deb, point *fin, matrice *pm, int scale)
{ //A l'air de marcher
    point *unitaire= malloc(sizeof(point));
    double distance = dist(deb, fin);
    unitaire -> x = ( fin -> x - deb -> x ) / distance;
    unitaire -> y = ( fin -> y - deb -> y ) / distance;
    int i = 0, coeff_i, coeff_j;
    while ( ( (int) (unitaire -> x * i + deb -> x) != (int) (fin -> x) ) && 
            ( (int) (unitaire -> y * i + deb -> y) != (int) (fin -> y) ) ) {
        coeff_i = (int) ( ( unitaire -> x * i + deb -> x ) / scale );
        coeff_j = (int) ( ( unitaire -> y * i + deb -> y ) / scale );
        if ( ( (int) (deb -> x) % scale == 0 ) || 
             ( (int) (fin -> x) % scale == 0 ) ) {
            if (pm->contenu[coeff_i][coeff_j] & PD) {
                return 0;
            }    
        } else if ( ( (int) (deb -> y) % scale == 0 ) || 
                    ( (int) (fin -> y) % scale == 0 ) ) { 
            if (pm->contenu[coeff_i][coeff_j] & PB) {
                return 0;
            }
        }
        i++;
    }
    return 1; //Pas de collision
}

void deplacement(observer *obs, SDL_Event event, double *theta, matrice *pm, \
                 int scale)
{ //Devait empecher de foncer dans les murs, mais marche pas du tout du tout
    observer *obs_copy = malloc(sizeof(observer));
    obs_copy -> sinus = obs -> sinus;
    obs_copy -> cosinus = obs -> cosinus;   
    obs_copy -> coord -> x = obs -> coord -> x;
    obs_copy -> coord -> y = obs -> coord -> y;
    switch (event.key.keysym.sym) {
        case SDLK_UP:
            avance(obs_copy, 5);
            break;
        case SDLK_DOWN:
            avance(obs_copy, -5);
            break;    
        case SDLK_LEFT:
            *theta -= 0.1;
            rotate(*theta, obs_copy);
            break;
        case SDLK_RIGHT:
            *theta += 0.1;
            rotate(*theta, obs_copy);
            break;                
    } 
    point *bord_g = coord_pix(D, obs_copy, L/2);
    point *bord_d = coord_pix(D, obs_copy, -L/2);
    //Si le deplacement est OK, on bouge:
    if ( ( pas_dans_mur(bord_d, bord_g, pm, scale) ) &&
         ( pas_dans_mur(bord_d, obs_copy -> coord, pm, scale) ) &&
         ( pas_dans_mur(obs_copy -> coord, bord_g, pm, scale) ) ) {
        observer *tmp = obs;
        obs = obs_copy;
        free(tmp);
    } else {        
        //Sinon, on free la copy
        free(obs_copy);
    }
    free(bord_d);
    free(bord_g);
}

int main(int argc, char *argv[]) {
    int largeur = 5, hauteur = 5;
    int h = hauteur + 1, l = largeur + 1;
    tab_nk *tab = init_tab_ouvr(h, l);
    matrice *pm = init_mat(l, h);
    srand(time(NULL));
    gen_laby(tab, pm);
    disp_laby(pm);
    //1ere fenetre:
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
    coord -> x = 3 * scale;
    coord -> y = 3 * scale;
    double theta = PI/3;
    observer *obs = malloc(sizeof(observer));
    obs -> coord = coord;
    obs -> sinus = sin(theta);
    obs -> cosinus = cos(theta);
    point *middle = malloc(sizeof(point));
    middle -> x = width3D / 2;
    middle -> y = height3D / 2;
    short test = 1;
    trapez_cast(pm, obs, renderer2D, renderer3D, scale, width3D, height3D, 
                middle, &test);
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
                if (test) {
                    //On commence par effacer les rendus precedents:
                    SDL_SetRenderDrawColor(renderer2D, 255, 255, 255, 0);
                    SDL_RenderClear(renderer2D); //fond blanc
                    SDL_SetRenderDrawColor(renderer3D, 0, 0, 0, 255); //Noir
                    SDL_RenderClear(renderer3D); //fond noir
                    //deplacement(obs, event, &theta, pm, scale);
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
                }
                //Dans tous les cas on cree un nveau rendu et on l'affiche:
                SDL_SetRenderDrawColor(renderer3D, 0, 255, 255, 255); //Cyan
                trapez_cast(pm, obs, renderer2D, renderer3D, scale,  width3D,
                            height3D, middle, &test); 
                SDL_SetRenderDrawColor(renderer2D, 0, 0, 0, 255); //Noir
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
