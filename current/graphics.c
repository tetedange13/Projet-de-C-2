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

void draw_murs(int width3D, point *middle, SDL_Renderer *renderer3D, int k_tmp,
               int h_tmp, int k, int h)
{
    int pas = width3D / L + 1;
    
    //Droite du mur :
    SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k), 
                                   (int) (middle -> y - h / 2),
                                   (int) (middle -> x + pas * k), 
                                   (int) (middle -> y + h / 2) );
    //Gauche du mur:
    SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k_tmp), 
                                   (int) (middle -> y - h_tmp / 2),
                                   (int) (middle -> x + pas * k_tmp), 
                                   (int) (middle -> y + h_tmp / 2) );   
    //Haut du mur:
    SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k_tmp), 
                                   (int) (middle -> y + h_tmp / 2),
                                   (int) (middle -> x + pas * k), 
                                   (int) (middle -> y + h / 2) );  
    //Bas du mur:
    SDL_RenderDrawLine(renderer3D, (int) (middle -> x + pas * k_tmp), 
                                   (int) (middle -> y - h_tmp / 2),
                                   (int) (middle -> x + pas * k), 
                                   (int) (middle -> y - h / 2) ); 
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

