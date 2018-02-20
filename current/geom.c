#include "geom.h"

void disp_coord(point *pt) { printf("%lf %lf\n", pt -> x, pt -> y); }

double dist(point *a, point *b)
{ //Calcule la dist euclidienne (en 2D) entre 2 points de coord connues
    
    return sqrt( (a->x - b->x)*(a->x - b->x) +
                 (a->y - b->y)*(a->y - b->y) );
}

point *coeff_to_pix(point *coeff, int scale)
{
    point *pix = malloc(sizeof(point));
    pix -> x = coeff -> x * scale;
    pix -> y = coeff -> y * scale;
    return pix;
}

point *pix_to_coeff(point *pix, int scale)
{
    point *coeff = malloc(sizeof(point));
    coeff -> x = pix -> x / scale;
    coeff -> y = pix -> y / scale;
    return coeff;
}

point *coord_pix(int fact, observer *observ, int k)
{ //Renvoie les coord du point projete à une dist fact orthogonalement de k
    point *Pk = malloc(sizeof(point));
    Pk -> x = (fact * observ -> cosinus) - (k * observ -> sinus)
                                         + observ -> coord -> x; 
    Pk -> y = (fact * observ -> sinus)   + (k * observ -> cosinus)
                                         + observ -> coord -> y;
    return Pk;
}

void avance(observer *obs, int valeur) 
{ //Deplacement de l'observateur en ligne droite (avant seulement)
    obs -> coord -> x += valeur * (obs -> cosinus);
    obs -> coord -> y += valeur * (obs -> sinus);
}
    
void rotate(double theta, observer *obs)
{ //rotation de l'observateur
    obs -> sinus = sin(theta);
    obs -> cosinus = cos(theta);
}
    




















