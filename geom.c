#include "geom.h"

void disp_coord(point *pt) { printf("%d %d\n", pt -> x, pt -> y); }

double dist(point *a, point *b)
{ //Calcule la dist euclidienne (en 2D) entre 2 points de coord connues
    
    return sqrt( (a->x - b->x)*(a->x - b->x) +
                 (a->y - b->y)*(a->y - b->y) );
}

double angle(point *a, point *b, point *c)
{ //Calcule l'angle ABC (en rad) formé par 3 atomes de coord connues 

    //Calcul des coord des 2 vect AB et BC:    
    int xAB = a->x - b->x, yAB = a->y - b->y;
    int xBC= c->x - b->x, yBC = c->y - b->y;
    
    double prod_scal = (xAB * xBC) + (yAB * yBC);
    double norm_AB = dist(a, b);
    double norm_BC = dist(b, c);

    return acos(prod_scal/(norm_AB * norm_BC));
    
    /*if -1 <= prod_scal/(norm_u * norm_v) <= 1: 
       
    else:
        return round(prod_scal/(norm_u * norm_v) *500)*/
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

vect *unitaire(point *coord, point *Pk)
{ //Transforme un vecteur en vecteur unitaire (a partir du bout et de l'ori)
    vect *vect_unit = malloc(sizeof(vect));
    int norm = dist(coord, Pk);
    vect_unit->x = (Pk -> x - coord -> x) / (double)norm; 
    vect_unit->y = (Pk->y - coord -> y) / (double)norm;
    return vect_unit;
}

void avance(observer *obs){
    //avancée de l'observateur en ligne droite.
    obs -> coord -> x += 2*(obs -> cosinus);
    obs -> coord -> y += 2*(obs -> sinus);
}
    
void rotate(double theta, observer *obs){
    //rotation de l'obersateur
    theta += 0.1;
    obs -> sinus = sin(theta);
    obs -> cosinus = cos(theta);
}
    




















