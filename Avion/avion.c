#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include "../Libs/avion.h"
#include "../Libs/TCP_socket.h"

// caract�ristiques du d�placement de l'avion
struct deplacement dep;

// coordonn�es spatiales de l'avion
struct coordonnees coord;

// num�ro de vol de l'avion : code sur 5 caract�res
char numero_vol[6];

// numéro de socket
int sock;

void changer_vitesse(int vitesse);
void changer_cap(int cap);
void changer_altitude(int altitude);
/********************************
 ***  3 fonctions � impl�menter
 ********************************/

int ouvrir_communication()
{
  // fonction � impl�menter qui permet d'entrer en communication via TCP
  // avec le gestionnaire de vols
  struct sockaddr_in addr;
  int sock, cnt;
  socklen_t addrlen; 
  struct ip_mreq mreq;
  static struct sockaddr_in addr_serveur;
  /*CREATION DE LA SOCKET UDP MULTICAST*/
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
	    perror("socket");
	    exit(1);
	  }
	  bzero((char *)&addr, sizeof(addr));
	  addr.sin_family = AF_INET;
	  addr.sin_addr.s_addr = htonl(INADDR_ANY);
	  addr.sin_port = htons(PORT_MULTICAST);
	  addrlen = sizeof(addr);
	  if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
	    perror("bind");
	    exit(1);
	  }
	  mreq.imr_multiaddr.s_addr = inet_addr(GROUP_MULTICAST);
	  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq)) < 0) {
	    perror("setsockopt mreq");
	    exit(1);
	  }

	  cnt = recvfrom(sock, &addr_serveur, sizeof(addr_serveur), 0,(struct sockaddr *) &addr, &addrlen); //Attente sur la socket UDP multicast pour l'adresse de la socket TCP d'écoute du SGCA
		if (cnt < 0) {
	    perror("recvfrom");
		  exit(1);
		}
    close(sock);
	  sock = creerSocketTCP(0);
	  if (sock == -1) {
      perror("Erreur création de socket TCP !");
      return -1; 
    }
	  addr_serveur.sin_family = AF_INET;
		if (connect(sock,(struct sockaddr *)&addr_serveur,sizeof(struct sockaddr_in)) == -1) { //Connexion sur l'adresse reçu de la socket UDP multicast
      perror("TCP : ");
      printf("Connexion TCP échouée !\n");
			return 0; 
		}
    fcntl(sock, F_SETFL, O_NONBLOCK); //Passage en lecture non bloquante pour la socket TCP
	  return sock;
}

	void fermer_communication()
	{
	  // fonction � impl�menter qui permet de fermer la communication
	  // avec le gestionnaire de vols
	  close(sock);
	}

	void envoyer_caracteristiques(int sock)
	{
	  // fonction � impl�menter qui envoie l'ensemble des caract�ristiques
	  // courantes de l'avion au gestionnaire de vols
    struct Ordre o;
    struct Avion a;
    strcpy(a.numero_vol,numero_vol);
    a.coord = coord;
    a.dep = dep;
    if (write(sock,&a,sizeof(struct Avion)) != sizeof(struct Avion)) { //Ecriture des données de l'avion dans la socket TCP
      perror("Erreur envoi de données de l'avion");
      exit(4);
    }
    if (read(sock,&o,sizeof(struct Ordre)) == sizeof(struct Ordre)) { //Lecture d'ordre éventuel en mode lecture non bloquante
      //Si un des attributs est négatifs, il n'est pas pris en compte
      if (o.dep.vitesse >= 0) {
        changer_vitesse(o.dep.vitesse);
      }
      if (o.dep.cap >= 0) {
        changer_cap(o.dep.cap);
      }
      if (o.altitude >= 0) {
        changer_altitude(o.altitude);
      }
    }
    sleep(PAUSE);
  }

	/********************************
	 ***  Fonctions g�rant le d�placement de l'avion : ne pas modifier
	 ********************************/

	// initialise al�atoirement les param�tres initiaux de l'avion
	void initialiser_avion()
	{
	  // initialisation al�atoire du compteur al�atoire
	  time_t seed;
	  time(&seed);
	  srandom(seed);

	  // intialisation des param�tres de l'avion
	  coord.x = 1000 + random() % 1000;
	  coord.y = 1000 + random() % 1000;
	  coord.altitude = 900 + random() % 100;

	  dep.cap = random() % 360;
	  dep.vitesse = 600 + random() % 200;

	  // initialisation du numero de l'avion : chaine de 5 caract�res
	  // form�e de 2 lettres puis 3 chiffres
	  numero_vol[0] = (random() % 26) + 'A';
	  numero_vol[1] = (random() % 26) + 'A';
	  sprintf (&(numero_vol)[2], "%3ld", (random() % 999) + 1);
	  numero_vol[5] = 0;
	}

// modifie la valeur de l'avion avec la valeur pass�e en param�tre
void changer_vitesse(int vitesse)
{
  if (vitesse < 0)
    dep.vitesse = 0;
  else if (vitesse > VITMAX)
    dep.vitesse = VITMAX;
  else dep.vitesse = vitesse;
}

// modifie le cap de l'avion avec la valeur pass�e en param�tre
void changer_cap(int cap)
{
  if ((cap >= 0) && (cap < 360))
    dep.cap = cap;
}

// modifie l'altitude de l'avion avec la valeur pass�e en param�tre
void changer_altitude(int alt)
{
  if (alt < 0)
    coord.altitude = 0;
  else if (alt > ALTMAX)
    coord.altitude = ALTMAX;
  else coord.altitude = alt;
}

// affiche les caract�ristiques courantes de l'avion
void afficher_donnees()
{
  printf("Avion %s -> localisation : (%d,%d), altitude : %d, vitesse : %d, cap : %d\n",
	 numero_vol, coord.x, coord.y, coord.altitude, dep.vitesse, dep.cap);
}

// recalcule la localisation de l'avion en fonction de sa vitesse et de son cap
void calcul_deplacement()
{
  float dep_x, dep_y;

  if (dep.vitesse < VITMIN)
    {
      printf("Vitesse trop faible : crash de l'avion\n");
      fermer_communication();
      exit(2);
    }
  if (coord.altitude == 0)
    {
      printf("L'avion s'est ecrase au sol\n");
      fermer_communication();
      exit(3);
    }


  dep_x = cos(dep.cap * 2 * M_PI / 360) * dep.vitesse * 10 / VITMIN;
  dep_y = sin(dep.cap * 2 * M_PI / 360) * dep.vitesse * 10 / VITMIN;

  // on se d�place d'au moins une case quels que soient le cap et la vitesse
  // sauf si cap est un des angles droit
  if ((dep_x > 0) && (dep_x < 1)) dep_x = 1;
  if ((dep_x < 0) && (dep_x > -1)) dep_x = -1;

  if ((dep_y > 0) && (dep_y < 1)) dep_y = 1;
  if ((dep_y < 0) && (dep_y > -1)) dep_y = -1;


  coord.x = coord.x + (int)dep_x;
  coord.y = coord.y + (int)dep_y;

  afficher_donnees();
}

// fonction principale : g�re l'ex�cution de l'avion au fil du temps
void se_deplacer(int sock)
{
  while(1)
    {
      sleep(PAUSE);
      calcul_deplacement();
      envoyer_caracteristiques(sock);
    }
}

int main()
{
  int err;
  // on initialise l'avion
  initialiser_avion();
  afficher_donnees();
  // on quitte si on arrive � pas contacter le gestionnaire de vols
  err = ouvrir_communication();
  if (!err) {
      printf("Impossible de contacter le gestionnaire de vols\n");
      exit(1);
    }

  // on se d�place une fois toutes les initialisations faites
  se_deplacer(err);
  return 0;
}
