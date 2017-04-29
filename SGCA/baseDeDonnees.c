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

void supprimerAvion(int rang) {
  entreeSC();
  bdd.tabAvion[rang] = bdd.tabAvion[bdd.nbAvion - 1];
  bdd.nbAvion--;
  sortieSC();
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

void ecrireAvion(struct Avion avion, int rang) {
  entreeSC();
  bdd.tabAvion[rang] = avion;
  sortieSC();
}

int lireAvions(struct Avion** tabAvionRes, int* nbAvion) { //PENSER AU MALLOC
  entreeSC();
  *nbAvion = bdd.nbAvion;
  *tabAvionRes = malloc(sizeof(struct Avion)*bdd.nbTab);
  if (tabAvionRes == NULL) {
    fprintf(stderr,"lireAvions : Erreur : allocation dynamique impossible.\n");
    sortieSC();
    return -1;
  }
  memcpy(*tabAvionRes,bdd.tabAvion,bdd.nbAvion*sizeof(struct Avion));
  sortieSC();
  return 0;
}

int getNbAvion() {
  return bdd.nbAvion;
}
