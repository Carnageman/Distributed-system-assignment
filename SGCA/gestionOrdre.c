#include <string.h>
#include <stdio.h>
#include "gestionOrdre.h"

void initialiserBaseOrdre() {
  bddOrdre.nbOrdre = 0;
}

void retirerOrdre(int rang) {
  bddOrdre.tabOrdre[rang] = bddOrdre.tabOrdre[bddOrdre.nbOrdre - 1];
  bddOrdre.nbOrdre--;
}

int getNewOrdre(struct Ordre* ordre,char num_vol[]) {
  int i = 0;
  int trouve = -1;
  while ((i < bddOrdre.nbOrdre) && (trouve == -1)) {
    if (strcmp(num_vol,(bddOrdre.tabOrdre[i]).numero_vol) == 0) {
      trouve = i;
    }
    i++;
  }
  if (trouve != -1) { //Si un ordre associé au num_vol a été trouvé dans la base
    *ordre = bddOrdre.tabOrdre[trouve];
    retirerOrdre(trouve);
    return 1;
  }
  else return 0; //Si aucun ordre associé au num_vol n'a été trouvé
}

int putOrdre(struct Ordre ordre) {
  if (bddOrdre.nbOrdre < TAILLETABORDRE) { //Si il reste de la place dans le tableau
    bddOrdre.tabOrdre[bddOrdre.nbOrdre] = ordre;
    bddOrdre.nbOrdre++;
    return 0;
  }
  else return -1; //Si il ne reste plus de place dans le tableau
}

void retirerOrdres(char num_vol[]) {
  int i;
  for (i = 0; i < bddOrdre.nbOrdre; i++) {
    if (strcmp(num_vol,(bddOrdre.tabOrdre[i]).numero_vol) == 0) {
      retirerOrdre(i);
    }
  }
}
