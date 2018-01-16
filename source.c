#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define B 1 //Ferme en bas
#define D 2 //Ferme a droite

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

int card_portes(int h, int l)
{
    return (l - 2) * (h - 1) + (l - 1) * (h - 2);
}

void disp_portes(tab_nk *tab, int h, int l)
{
    int cst = card_portes(h, l);
    for (int i = 0; i < cst; i++){
	printf("Coord: ");
	printf("%d ", tab -> tab_ouvr[i].i);
	printf("%d ", tab -> tab_ouvr[i].j);
	printf("Zones: ");
	printf("%d ", tab -> tab_ouvr[i].z1);
	printf("%d ", tab -> tab_ouvr[i].z2);
	printf("Dir: ");
	printf("%d ", tab -> tab_ouvr[i].dir);
	printf("\n");
    }
}

tab_nk *init_tab_ouvr(int h, int l)
{
    assert(h > 0 && l > 0);
    tab_nk *tab = malloc(sizeof(tab_nk));
    assert(tab != NULL);
    tab -> tab_ouvr = malloc(sizeof(porte)*card_portes(h, l));
    assert(tab -> tab_ouvr != NULL);
    tab -> k = 0;
    for(int i = 1; i < h; i++){
        for(int j = 1; j < l; j++){
            int centre = (i - 1) * (l - 1) + (j - 1);
            if (i != h-1)
            tab -> tab_ouvr[(tab -> k)++] =
                (porte) {i, j, centre, centre + (l - 1), B};
            if (j != l-1)
            tab -> tab_ouvr[(tab -> k)++] =
                (porte) {i, j, centre, centre + 1, D};
        }
    }
    return tab;	    
}

matrice *init_mat (int largeur, int hauteur)
{
    matrice *pm;
    int i, j;
    pm = malloc(sizeof(matrice));
    pm -> largeur = largeur;
    pm -> hauteur = hauteur;
    pm -> contenu = malloc(hauteur * sizeof(short *));
    for (i = 0; i < hauteur; i++) { 
        pm -> contenu[i] = malloc(largeur * sizeof(short));
        if (i == 0) pm -> contenu[i][0] = 0;
        else pm -> contenu[i][0] = D;
        for (j = 1; j < largeur; j++)
            if (i == 0) pm -> contenu[i][j] = B;
            else pm -> contenu[i][j] = (B | D);
    }
    return pm;
}

void disp_mat(matrice *pm)
{
    int i, j;
    for (i = 0; i < (pm -> hauteur); i++) {
        for (j = 0; j < (pm -> largeur); j++) {
            printf("%d ", pm -> contenu[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void disp_laby(matrice *pm)
{
    int i, j;
    printf("\n");
    for (i = 0; i < (pm -> hauteur); i++) {
        for (j = 0; j < (pm -> largeur); j++) {
            switch (pm -> contenu[i][j]) {
            case (B | D) : printf("_|"); break;
            case B : printf("_ "); break;
            case D : printf(" |"); break;
            default :  printf("  "); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

void del_porte(tab_nk *tab, int position)
{
    porte tmp = tab -> tab_ouvr[position];
    tab -> tab_ouvr[position] = tab -> tab_ouvr[(tab -> k) - 1];
    tab -> tab_ouvr[(tab -> k) - 1] = tmp;
    (tab -> k)--;
}

int main()
{
    int largeur = 3, hauteur = 3;
    int h = hauteur + 1, l = largeur + 1;

    tab_nk *tab = init_tab_ouvr(hauteur, largeur);
    disp_portes(tab, hauteur, largeur);

    matrice *pm = init_mat(l, h);
    disp_laby(pm);
    
    int nbAleat;
    srand(time(NULL));
    nbAleat = rand() % (tab -> k);
    printf("Ind de la porte selectionnee: %d\n", nbAleat);
    
    porte maPorte = tab -> tab_ouvr[nbAleat];
    printf("%d %d\n", maPorte.i, maPorte.j);
    pm -> contenu[maPorte.i + 1][maPorte.j + 1] = 
            ((pm -> contenu[maPorte.i][maPorte.j])^(maPorte.dir));
    disp_laby(pm);
    
    del_porte(tab, nbAleat);
    disp_portes(tab, hauteur, largeur);

    
    return 0;
}
	 
    
