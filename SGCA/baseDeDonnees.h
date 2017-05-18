#ifndef BASEDEDONNEES_H
#define BASEDEDONNEES_H
#include "../Libs/avion.h"

struct baseDeDonnees {
  int nbAvion;
  int nbTab;
  struct Avion* tabAvion;
};

struct baseDeDonnees bdd;

int s;

void initialiserBase();
void ecrireAvion(struct Avion avion);
int lireAvions(struct Avion** tabAvionRes, int* nbAvion);
void supprimerAvion(char num_vol[]);
int getNbAvion();
int checkAvion(char num_vol[]);
#endif
