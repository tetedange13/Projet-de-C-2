#include "source2.h"

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    double x;
    double y;
} vect;

// reduction du nombre de parametres des fonctions
typedef struct {
    point *coord;
    double sinus;
    double cosinus;
} observer;


void disp_coord(point *);
double dist(point *, point *);
double angle(point *, point *, point *);
point *coeff_to_pix(point *, int);
point *pix_to_coeff(point *, int);
point *coord_pix(int, observer *, int);
vect *unitaire(point *, point *);

//point *mult(double, point *);
//double scalar(point *, point *);
