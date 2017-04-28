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

int initialiserBase();
int ecrireAvion(struct Avion avion, int rang);
int lireAvions(struct Avion** tabAvionRes, int* nbAvion);
int supprimerAvion(int rang);
int getNouveauRang();
int getNbAvion();
#endif
