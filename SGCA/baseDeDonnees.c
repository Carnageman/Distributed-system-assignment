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

void supprimerAvion(char num_vol[]) {
  int rang = checkAvion(num_vol);

  if (rang != -1) { //Si l'avion a supprimer existe
    entreeSC();
    bdd.tabAvion[rang] = bdd.tabAvion[bdd.nbAvion - 1];
    bdd.nbAvion--;
    sortieSC();
  }
}

void ecrireAvion(struct Avion avion) {
  int rang;
  struct Avion* tabAux = NULL;
  entreeSC();
  if (bdd.nbTab <= bdd.nbAvion) { //Si il n'y a plus de place dans le tableau
    tabAux = malloc(sizeof(struct Avion) * (bdd.nbTab + 10)); //Augmenter la taille du tableau de 10
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
  if ((rang = checkAvion(avion.numero_vol)) == -1) { //Si l'avion n'existe pas déjà dans la table, l'ajouter à la fin de la table et augmenter nbAvion
    bdd.tabAvion[bdd.nbAvion] = avion;
    bdd.nbAvion++;
  }
  else { //Si l'avion existe déjà dans la table, remplacer ces valeurs par de nouvelles valeurs
    bdd.tabAvion[rang] = avion;
  }
  sortieSC();
}

int lireAvions(struct Avion** tabAvionRes, int* nbAvion) { //FONCTION QUI ALLOUE tabAvionRes AVEC UN MALLOC, DANGEREUSE, PENSER A FREE tabAvionRes APRES UTILISATION
  entreeSC();
  *nbAvion = bdd.nbAvion;
  if (*nbAvion != 0) { //Si il y a des avions dans la base
    *tabAvionRes = malloc(sizeof(struct Avion)*bdd.nbTab);
    if (tabAvionRes == NULL) {
      fprintf(stderr,"lireAvions : Erreur : allocation dynamique impossible.\n");
      sortieSC();
      return -1;
    }
    memcpy(*tabAvionRes,bdd.tabAvion,bdd.nbAvion*sizeof(struct Avion));
  }
  else *tabAvionRes = NULL; //Renvoyer un pointeur null si jamais il n'y a pas d'avions dans la base
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
