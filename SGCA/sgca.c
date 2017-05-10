#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>

#include "../Libs/avion.h"
#include "../Libs/UDP_socket.h"
#include "../Libs/TCP_socket.h"
#include "baseDeDonnees.h"

#define TAILLEBUF 50
#define MAGICNBR 56841

struct Avion convertAvionToBigEndian(struct Avion a) {
  a.coord.x = htonl(a.coord.x);
  a.coord.y = htonl(a.coord.y);
  a.coord.altitude = htonl(a.coord.altitude);
  a.dep.cap = htonl(a.dep.cap);
  a.dep.vitesse = htonl(a.dep.vitesse);
  return a;
}

void paquetAvion(void* buffer,struct Avion a) {
  struct Avion* debBufferAvion;
  ((int*)buffer)[0] = htonl(MAGICNBR);
  a = convertAvionToBigEndian(a);
  debBufferAvion = (struct Avion*)(&((int*)buffer)[1]); /*debBufferAvion pointe juste après le int magicNbr*/
  *debBufferAvion = a;
}

void paquetNbAvion(void* buffer,int nb) {
  ((int*)buffer)[0] = htonl(MAGICNBR);
  ((int*)buffer)[1] = htonl(nb);
}

int verificationPaquet(void* buffer, int nbBytes) {
  int premierEntierBuffer;
  if (nbBytes >= 4) {
    premierEntierBuffer = ((int*)buffer)[0];
    return ((nbBytes == 4 || nbBytes == 8) && premierEntierBuffer == htonl(MAGICNBR));
  }
  else return 1;
}

void* multicastManager() {
  struct hostent *serveur_host;
  static struct sockaddr_in addr_serveur;
  struct sockaddr_in addr;
  int addrlen, sock, cnt;
  //struct ip_mreq mreq;
  char hostname [30];
  gethostname(hostname, 30);
  serveur_host = (struct hostent*)gethostbyname((char*)hostname);
  if (serveur_host==NULL) {
    perror("erreur adresse serveur");
    exit(1);
  }
  memcpy(&addr_serveur.sin_addr.s_addr,
    serveur_host -> h_addr, serveur_host -> h_length);
  /* set up socket */
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

  addr.sin_addr.s_addr = inet_addr(GROUP_MULTICAST);
  while (1) {
    cnt = sendto(sock, &addr_serveur, sizeof(addr_serveur), 0,(struct sockaddr *) &addr, addrlen);
    if (cnt < 0) {
      perror("sendto");
      exit(1);
    }
    sleep(5);
  }
}

void* avionInfoManager(int* sock) {
  int socket_service = *sock;
	int rang = getNouveauRang();
  int nb_octetsRead;
	char message[TAILLEBUF];
  struct Avion a;
  while (1) {
    nb_octetsRead = read(socket_service,message,TAILLEBUF);		
    printf("nb_octetsRead = %d\n",nb_octetsRead);
    if (nb_octetsRead <= 0) break;
	  a.numero_vol[0] = message[0];
	  a.numero_vol[1] = message[1];
    a.numero_vol[2] = message[2];
    a.numero_vol[3] = message[3];
    a.numero_vol[4] = message[4];
    a.numero_vol[5] = message[5];
	  nb_octetsRead = read(socket_service,message,TAILLEBUF);
    if (nb_octetsRead <= 0) break;
	  memcpy(&a.coord,message,sizeof(message));
	  nb_octetsRead = read(socket_service,message,TAILLEBUF);
    if (nb_octetsRead <= 0) break;

	  memcpy(&a.dep,message,sizeof(message));

	  printf("Avion %s -> localisation : (%d,%d), altitude : %d, vitesse : %d, cap : %d\n",a.numero_vol, a.coord.x, a.coord.y, a.coord.altitude, a.dep.vitesse, a.dep.cap);

	  ecrireAvion(a,rang);
	}
  supprimerAvion(rang);
	// on ferme les sockets
	close(socket_service);
  return 0;
}
void* initialisationConnexionAvionManager() {
	// adresse socket coté client
	static struct sockaddr_in addr_client;
	// adresse socket locale
	static struct sockaddr_in addr_serveur;
	// longueur adresse
	socklen_t lg_addr;
	// socket d'écoute et de service
	int socket_ecoute, socket_service;
	// buffer qui contiendra le message reçu
	char message[TAILLEBUF];
	// chaîne reçue du client
	char *chaine_recue;
	// nombre d'octets reçus ou envoyés
	int nb_octetsRead = TAILLEBUF;
  pthread_t* thread;

	socket_ecoute = creerSocketTCP(1285);
	if(socket_ecoute == -1)
	{
		exit(1);
	}
	// configuration socket écoute : 5 connexions max en attente
	if (listen(socket_ecoute, 5) == -1) {
		perror("erreur listen");
		exit(1);
	}
	// on attend la connexion du client
	lg_addr = sizeof(struct sockaddr_in);
  while (1) {
	  socket_service = accept(socket_ecoute,(struct sockaddr *)&addr_client, &lg_addr);
	  if (socket_service == -1) {
		  perror("erreur accept");
		  exit(1);
	  }
    thread = malloc(sizeof(pthread_t));
    pthread_create(thread,NULL,avionInfoManager,&socket_service);
  }
	close(socket_ecoute);
  return 0;
}

void* consoleAffichageManager() {
  socklen_t lg;
  /*static*/ struct sockaddr_in addr_client;
  int sock;
  size_t taillebuffeur = TAILLEBUF;
  char buffer[TAILLEBUF];
  int nb_octets;
  int nombreAvions;
  int nbreAvionPaquet;
  struct Avion* tabAvion;
  int i;
  
  sock = creerSocketUDP(5842);
  if(sock == -1) {
    perror("Erreur lors de la création de la socket UDP");
    exit(-1);
  }
  lg = sizeof(struct sockaddr_in);
  while(1) {
    tabAvion = NULL;
    nb_octets = recvfrom(sock,buffer,taillebuffeur,0,(struct sockaddr *)&addr_client,&lg);
    if(verificationPaquet(buffer,nb_octets)) {
    //Si le premier entier est correct
     switch(nb_octets) {
      case -1:
      //Si il y a eu une erreur
        perror("Erreur lors de la réception du paquet UDP");
        exit(-1);
      case 4:
      //Si un entier seul est recu (Requete pour obtenir le nbre d'avion)
        paquetNbAvion(buffer,getNbAvion());
        nb_octets = sendto(sock,buffer,8,0,(struct sockaddr*)&addr_client,lg);
        if (nb_octets == -1) {
          perror("Erreur lors de l'envoi d'un paquet UDP");
        }
        break;
    
      case 8:
        lireAvions(&tabAvion,&nombreAvions);
        nbreAvionPaquet = ((int*)buffer)[1];
        //Si deux entiers sont recu (Requete pour obtenir les avions en donnant le nbre d'avion)
        if (nbreAvionPaquet == htonl(nombreAvions)) {
        //Envois des avions à la console
          for(i = 0; i < nombreAvions; i++) {
            paquetAvion(buffer,tabAvion[i]);
            nb_octets = sendto(sock,buffer,(sizeof(int)+sizeof(struct Avion)),0,(struct sockaddr*)&addr_client,lg);
            if (nb_octets == -1) {
              perror("Erreur lors de l'envoi d'un paquet UDP");
            }
          }
<<<<<<< HEAD

          
=======
         free(tabAvion);
>>>>>>> e1067fe13e2bc1fe7f040605eebfb99171311c97
        }
        else {
        //Si l'entier recu dans la requete n'est pas le nombre d'avion
          paquetNbAvion(buffer,getNbAvion());
          nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
          if (nb_octets == -1) {
            perror("Erreur lors de l'envoi d'un paquet UDP");
          }
        }
        if (tabAvion != NULL) {
          free(tabAvion);
        }
        break;
      }
    }
  }
}



void* avionOrdreManager() {

  return 0;
}

void jeuDeTestBase() {
  int rang;
  struct Avion a;

  a.numero_vol[0] = 'H';
  a.numero_vol[1] = 'i';
  a.numero_vol[2] = 'b';
  a.numero_vol[3] = 'o';
  a.numero_vol[4] = 'u';
  a.numero_vol[5] = '\0';
  a.coord.x = 50;
  a.coord.y = 78;
  a.coord.altitude = 589;
  a.dep.cap = 30;
  a.dep.vitesse = 250;
  rang = getNouveauRang();
  ecrireAvion(a,rang);

  a.numero_vol[0] = 'C';
  a.numero_vol[1] = 'h';
  a.numero_vol[2] = 'o';
  a.numero_vol[3] = 'u';
  a.numero_vol[4] = 'e';
  a.numero_vol[5] = '\0';
  a.coord.x = 80;
  a.coord.y = 98;
  a.coord.altitude = 120;
  a.dep.cap = 550;
  a.dep.vitesse = 500;
  rang = getNouveauRang();
  ecrireAvion(a,rang);
}

int main() {
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  initialiserBase();
  jeuDeTestBase();
  pthread_create(&thread1,NULL,multicastManager,NULL);
  pthread_create(&thread2,NULL,consoleAffichageManager,NULL);
  pthread_create(&thread3,NULL,initialisationConnexionAvionManager,NULL);
  while (1);
  //pthread_join(thread1,NULL);
}
