#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define PB 1 //Ferme en bas
#define PD 2 //Ferme a droite
#define PI acos(-1)


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


int card_portes(int h, int l);

void disp_portes(tab_nk *tab);

tab_nk *init_tab_ouvr(int h, int l);

matrice *init_mat (int largeur, int hauteur);

void disp_mat(matrice *pm);

void disp_laby(matrice *pm);

void del_porte(tab_nk *tab, int i);

void sort_portes(tab_nk *tab);

void gen_laby(tab_nk *tab, matrice *pm);

