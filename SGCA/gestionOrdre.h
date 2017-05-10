#ifndef GESTIONORDRE_H
#define GESTIONORDRE_H
#include "../Libs/avion.h"

#define TAILLETABORDRE 50
struct BaseOrdre {
  struct Ordre tabOrdre[TAILLETABORDRE];
  int nbOrdre;
};

struct BaseOrdre bddOrdre;

int getNewOrdre(struct Ordre* ordre,char num_vol[]); /*Prend un nouvel ordre dans la base et le retire de la base*/
int putOrdre(struct Ordre ordre);
void retirerOrdres(char num_vol[]);
#endif
