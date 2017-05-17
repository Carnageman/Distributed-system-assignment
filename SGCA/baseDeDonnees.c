#include "baseDeDonnees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void entreeSC() {
  while (s <= 0); //Attente active
  s--;
}

void sortieSC() {
  s++; //Déblocage de l'attente des autres processus
}

void initialiserBase() {
  bdd.nbAvion = 0; 
  bdd.nbTab = 0;
  bdd.tabAvion = NULL;
  s = 1;
}

/*void supprimerAvion(int rang) {
  entreeSC();
  bdd.tabAvion[rang] = bdd.tabAvion[bdd.nbAvion - 1];
  bdd.nbAvion--;
  sortieSC();
}*/
void supprimerAvion(char num_vol[]) {
  int rang = checkAvion(num_vol);

  if (rang != -1) {
    entreeSC();
    bdd.tabAvion[rang] = bdd.tabAvion[bdd.nbAvion - 1];
    bdd.nbAvion--;
    sortieSC();
  }
}

int getNouveauRang() {
  struct Avion* tabAux = NULL;
  entreeSC();
  if (bdd.nbTab <= bdd.nbAvion) {
    tabAux = malloc(sizeof(struct Avion)*(bdd.nbTab+10));
    if (tabAux == NULL) {
      fprintf(stderr,"getNouveauRang : Erreur : allocation dynamique impossible.\n");
      return -1;
    }
    memcpy(tabAux,bdd.tabAvion,sizeof(struct Avion)*bdd.nbTab);
    if (bdd.tabAvion != NULL) {
      free(bdd.tabAvion);
    }
    bdd.tabAvion = tabAux;
    bdd.nbTab += 10;
  }
  bdd.nbAvion++;
  sortieSC();
  return bdd.nbAvion-1;
}

void ecrireAvion(struct Avion avion) {
  int rang;
  struct Avion* tabAux = NULL;
  entreeSC();
  if (bdd.nbTab <= bdd.nbAvion) {
    tabAux = malloc(sizeof(struct Avion) * (bdd.nbTab + 10));
    if (tabAux == NULL) {
      fprintf(stderr,"getNouveauRang : Erreur : allocation dynamique impossible.\n");
    }
    memcpy(tabAux,bdd.tabAvion,sizeof(struct Avion)*bdd.nbTab);
    if (bdd.tabAvion != NULL) {
      free(bdd.tabAvion);
    }
    bdd.tabAvion = tabAux;
    bdd.nbTab += 10;
  }
  if ((rang = checkAvion(avion.numero_vol)) == -1) {
    printf("Avion non existant, création... %s\n",avion.numero_vol);
    bdd.tabAvion[bdd.nbAvion] = avion;
    bdd.nbAvion++;
  }
  else {
    printf("Avion déjà existant, écriture... %s\n",avion.numero_vol);
    bdd.tabAvion[rang] = avion;
  }
  sortieSC();
}

int lireAvions(struct Avion** tabAvionRes, int* nbAvion) { //PENSER AU MALLOC
  entreeSC();
  *nbAvion = bdd.nbAvion;
  if (*nbAvion != 0) {
  *tabAvionRes = malloc(sizeof(struct Avion)*bdd.nbTab);
  if (tabAvionRes == NULL) {
    fprintf(stderr,"lireAvions : Erreur : allocation dynamique impossible.\n");
    sortieSC();
    return -1;
  }
  memcpy(*tabAvionRes,bdd.tabAvion,bdd.nbAvion*sizeof(struct Avion));
  }
  else *tabAvionRes = NULL;
  sortieSC();
  return 0;
}

int getNbAvion() {
  return bdd.nbAvion;
}

int checkAvion(char num_vol[]) {
  int i = 0;
  int trouve = -1;
  if (bdd.nbAvion != 0) {
    while ((i < bdd.nbAvion) && (trouve == -1)) {
      if (strcmp(num_vol,(bdd.tabAvion[i]).numero_vol) == 0) {
        trouve = i;
      }
      i++;
    }
  }
  return (trouve);
}

/*int main() {
  struct Avion a;
  int nb;
  char un_numero[5] = "mich";
  a.numero_vol[0] = 'C';
  a.numero_vol[1] = 'o';
  a.numero_vol[2] = 'u';
  a.numero_vol[3] = 'c';
  a.numero_vol[4] = 'o';
  a.numero_vol[5] = '\0';
  a.coord.x = 5;
  a.coord.y = 10;
  a.coord.altitude = 4500;
  a.dep.cap = 480;
  a.dep.vitesse = 500;
  initialiserBase();
  printf("%d\n",checkAvion(a.numero_vol));
  nb = getNouveauRang();
  ecrireAvion(a,nb);
  printf("%d\n",checkAvion(a.numero_vol));
  printf("%d\n",checkAvion(un_numero));
}*/
