#include "maze.h"

#define PI acos(-1)


typedef struct {
    double x;
    double y;
} point;

// reduction du nombre de parametres des fonctions
typedef struct {
    point *coord;
    double sinus;
    double cosinus;
} observer;


void disp_coord(point *);
double dist(point *, point *);
point *coeff_to_pix(point *, int);
point *pix_to_coeff(point *, int);
point *coord_pix(int, observer *, int);
void avance(observer *, int);
void rotate(double, observer *);

