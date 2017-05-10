#include <string.h>
#include <stdio.h>
#include "gestionOrdre.h"


int retirerOrdre(int rang) {
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
  if (trouve != -1) {
    *ordre = bddOrdre.tabOrdre[trouve];
    retirerOrdre(trouve);
    return 1;
  }
  else return 0;
}

int putOrdre(struct Ordre ordre) {
  if (bddOrdre.nbOrdre < 50) {
    bddOrdre.tabOrdre[bddOrdre.nbOrdre] = ordre;
    bddOrdre.nbOrdre++;
    return 0;
  }
  else return -1;
}

void retirerOrdres(char num_vol[]) {
  int i;
  for (i = 0; i < bddOrdre.nbOrdre; i++) {
    if (strcmp(num_vol,(bddOrdre.tabOrdre[i]).numero_vol) == 0) {
      retirerOrdre(i);
    }
  }
}
/*PROGRAMME DE TEST*/
int main() {
  struct Ordre ordre;
  ordre.numero_vol[0] = 'C';
  ordre.numero_vol[1] = 'o';
  ordre.numero_vol[2] = 'u';
  ordre.numero_vol[3] = 'c';
  ordre.numero_vol[4] = '\0';
  ordre.dep.cap = 152;
  ordre.dep.vitesse = 500;
  ordre.altitude = 5000;
  putOrdre(ordre);
  ordre.dep.cap = 500;
  retirerOrdres(ordre.numero_vol);
  printf("%d\n",ordre.dep.cap);
  printf("%d\n",getNewOrdre(&ordre,ordre.numero_vol));
  printf("%d\n",ordre.dep.cap);
  printf("%d\n",getNewOrdre(&ordre,ordre.numero_vol));
}
