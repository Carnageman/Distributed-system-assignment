#ifndef AVION_H
#define AVION_H
#define ALTMAX 20000
#define ALTMIN 0

#define VITMAX 1000
#define VITMIN 200

#define PAUSE 2

#define PORT_MULTICAST 7659
#define GROUP_MULTICAST "239.0.0.1"


struct coordonnees {
  int x;
  int y;
  int altitude;
};

struct deplacement {
  int cap;
  int vitesse;
};

struct Avion {
  char numero_vol[6];
  struct coordonnees coord;
  struct deplacement dep;
};

struct Ordre {
	char numero_vol[6];
	struct deplacement dep;
	int altitude;
};
#endif
