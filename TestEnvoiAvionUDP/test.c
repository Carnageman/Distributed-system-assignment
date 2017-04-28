#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#define TAILLEBUF 20

#ifndef AVION_H
#define AVION_H
#define ALTMAX 20000
#define ALTMIN 0

#define VITMAX 1000
#define VITMIN 200

#define PAUSE 2

struct coordonnees {
  int x;
  int y;
  int altitude;
};

struct deplacement {
  int cap;
  int vitesse;
};
#endif

struct avion {
  char numero_vol[6];
  struct coordonnees coord;
  struct deplacement dep;
};

struct avion avion1;


int creerSocketUDP(int port) {
	// descripteur de la socket locale
	static struct sockaddr_in addr_local;
	int sock;
	// création de la socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
	   perror("erreur création socket");
	   return(-1);
	}
	// liaison de la socket sur le port local 4000
	bzero(&addr_local, sizeof(struct sockaddr_in));
	addr_local.sin_family = AF_INET;
	addr_local.sin_port = htons(port);
	addr_local.sin_addr.s_addr=htonl(INADDR_ANY);
	if( bind(sock, (struct sockaddr*)&addr_local, sizeof(addr_local))== -1 ) {
	      perror("erreur bind");
	      return(-1);
	}
	return sock;
}

#define TAILLEBUF 20
int main(int argc, char *argv[]) {
	// identifiant de la machine serveur
	struct hostent *serveur_host;
	// adresse de la socket coté serveur
	static struct sockaddr_in addr_serveur;
	// taille de l'addresse socket
	socklen_t lg;
	// descripteur de la socket locale
	int sock;
	// chaine  à envoyer
	char *msg = "Coucou Hibou\n";
	// buffer de réception
	char buffer[TAILLEBUF];
	// chaine reçue en réponse
	char *reponse;
	// nombre d'octets lus ou envoyés
	int nb_octets;
	// création d'une socket UDP
  avion1.numero_vol[0] = 'H';
  avion1.numero_vol[1] = 'i';
  avion1.numero_vol[2] = 'b';
  avion1.numero_vol[3] = 'o';
  avion1.numero_vol[4] = 'u';
  avion1.numero_vol[5] = '\0';
  avion1.coord.x = htonl(1);
  avion1.coord.y = htonl(3);
  avion1.coord.altitude = htonl(2000);
  avion1.dep.cap = htonl(50);
  avion1.dep.vitesse = htonl(150);
	sock = creerSocketUDP(0);
	if(sock == -1)
	{
		exit(1);
	}
	// récupération identifiant du serveur
	serveur_host = gethostbyname("localhost");
	if (serveur_host==NULL) {
	perror("erreur adresse serveur");
	exit(1);
	}
	// création adresse socket destinatrice
	bzero(&addr_serveur, sizeof(struct sockaddr_in));
	addr_serveur.sin_family = AF_INET;
	addr_serveur.sin_port = htons(5500);
	memcpy(&addr_serveur.sin_addr.s_addr,
	  serveur_host -> h_addr, serveur_host -> h_length);

	// on envoie le message "bonjour" au serveur
	lg = sizeof(struct sockaddr_in);
	nb_octets = sendto(sock, &avion1, sizeof(avion1), 0,(struct sockaddr*)&addr_serveur, lg);
	if (nb_octets == -1) {
	    perror("erreur envoi message");
	exit(1); }
	printf("paquet envoyé, nb_octets = %d\n",nb_octets);
	// on attend la réponse du serveur
	/*nb_octets = recvfrom(sock, buffer, TAILLEBUF, 0,
	(struct sockaddr*)
	&addr_serveur, &lg);
	if (nb_octets == -1) {
	    perror("erreur réponse serveur");
	exit(1); }
	reponse = (char *)malloc(nb_octets * sizeof(char));
	memcpy(reponse, buffer, nb_octets);
	printf("reponse recue du serveur : %s\n",reponse);*/
	// on ferme la socket
	close(sock);
}
