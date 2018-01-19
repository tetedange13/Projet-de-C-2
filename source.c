#include "source.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int card_portes(int h, int l)
{
    return (l - 2) * (h - 1) + (l - 1) * (h - 2);
}

void disp_portes(tab_nk *tab)
{
    for (int i = 0; i < tab -> k; i++){
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
    tab -> k = card_portes(h, l);
    tab -> tab_ouvr = malloc(tab -> k * sizeof(porte));
    assert(tab -> tab_ouvr != NULL);
    int k = 0;
    for(int i = 1; i < h; i++){
        for(int j = 1; j < l; j++){
            int centre = (i - 1) * (l - 1) + (j - 1);
            if (i != h-1)
		tab -> tab_ouvr[k++] =
		    (porte) {i, j, centre, centre + (l - 1), B};
            if (j != l-1)
		tab -> tab_ouvr[k++] =
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

void del_porte(tab_nk *tab, int i)
{
  tab -> tab_ouvr[i] = tab -> tab_ouvr[(tab -> k) - 1];
  tab -> k--;
  /// inutile... pourquoi deplacer tab -> tab_ouvr[position]
  /// en fin de tableau ?
  //  porte tmp = tab -> tab_ouvr[position];
  //  tab -> tab_ouvr[position] = tab -> tab_ouvr[(tab -> k) - 1];
  //  tab -> tab_ouvr[(tab -> k) - 1] = tmp;
  //  (tab -> k)--;
}

/* inutile de refaire un parcours, voir ci-dessous... 
 * et il y a un probleme : apres une suppression, il faut 
 * faire i--, pour que le i++ laisse le i sur place 
*/
void sort_portes(tab_nk *tab)
{
    int i;
        for (i = 0; i < tab -> k; i++) {
        printf("%d\n", ((tab -> tab_ouvr[i].z1) == (tab -> tab_ouvr[i].z2)));
	        if ((tab -> tab_ouvr[i].z1) == (tab -> tab_ouvr[i].z2))
	            del_porte(tab, i);
        }
}


/* inutile... la condition pour continuer l'ouverture de 
 * portes est simplement qu'il reste des portes.
 */
/*
short zones_diff(tab_nk *tab, int hauteur, int largeur)
{
    int i, zone = (tab -> tab_ouvr[0]).z1;
    if (zone != (tab -> tab_ouvr[0]).z2)
	return 1;
    for (i = 1; i < card_portes(hauteur, largeur); i++) {
	if (zone != (tab -> tab_ouvr[i]).z1)
	    return 1;
	if (zone != (tab -> tab_ouvr[i]).z2)
	    return 1;
    }
    return 0;
}
*/

// inutile de connaitre h et l
//void gen_laby(tab_nk *tab, int h, int l, matrice *pm)
void gen_laby(tab_nk *tab, matrice *pm)
{
    while (tab -> k)
	{
	  // probleme : sort_portes doit etre
	  // fait immediatement *apres* la jonction
	  // de deux zones.
	  /*	    
		    disp_portes(tab, h, l);
		    sort_portes(tab);
		    disp_portes(tab, h, l);
	  */
	  int i = rand() % (tab -> k);
	  porte p = tab -> tab_ouvr[i];
	  del_porte(tab, i);
	  pm -> contenu[p.i][p.j] = pm -> contenu[p.i][p.j]^p.dir;
	  for(i = 0; i < tab -> k; i++) {
	    if (tab -> tab_ouvr[i].z1 == p.z2)
	      tab -> tab_ouvr[i].z1 = p.z1;
	    if (tab -> tab_ouvr[i].z2 == p.z2)
	      tab -> tab_ouvr[i].z2 = p.z1;
	    if  (tab -> tab_ouvr[i].z1 ==
		 tab -> tab_ouvr[i].z2)
	      {
		del_porte(tab, i);
		i--; // pour rester sur place.
	      }
	  }
	  /* il faudra eliminer les portes qui, apres mise a jour, 
	   * ne separeront plus deux zones distinctes. autant le 
	   * faire directement dans la boucle ci-dessus.
	   */
	  /*
	  for (i = 0; i < card_portes(h, l); i++) {
	    if (tab -> tab_ouvr[i].z1 == z_change) 
	      tab -> tab_ouvr[i].z1 = maPorte.z2;
	    if (tab -> tab_ouvr[i].z2 == z_change) 
	      tab -> tab_ouvr[i].z2 = maPorte.z2;
	  }
	  */

	}
}


int main()
{
    int largeur = 4, hauteur = 4;
    int h = hauteur + 1, l = largeur + 1;
    tab_nk *tab = init_tab_ouvr(h, l);
    matrice *pm = init_mat(l, h);
    srand(time(NULL));
    gen_laby(tab, pm);
    //gen_laby(tab, h, l, pm);
    //disp_portes(tab, h, l);
    disp_laby(pm);
    return 0;
}
