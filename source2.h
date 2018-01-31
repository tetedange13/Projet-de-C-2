#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define PB 1 //Ferme en bas
#define PD 2 //Ferme a droite
#define PI acos(-1)

#define D 100 //profondeur de champ
#define L 150 //largeur de champ


typedef struct {
    //Coordonnées
    int i;
    int j;
    //Zone
    int z1;
    int z2;
    //Dir
    short dir;
} porte;

typedef struct {
    int largeur;
    int hauteur;
    short **contenu;
} matrice;

typedef struct {
    porte *tab_ouvr;
    int k;
} tab_nk;


int card_portes(int, int);
void disp_portes(tab_nk *);
tab_nk *init_tab_ouvr(int, int);
matrice *init_mat (int, int);
void disp_mat(matrice *);
void disp_laby(matrice *);
void del_porte(tab_nk *, int);
void sort_portes(tab_nk *);
void gen_laby(tab_nk *, matrice *);
