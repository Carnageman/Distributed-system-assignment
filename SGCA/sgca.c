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
#include "gestionOrdre.h"

#define TAILLEBUF 50
#define MAGICNBR 56841

struct Avion convertAvionToBigEndian(struct Avion a) {
  a.coord.x = htonl(a.coord.x); //Le Java comprend le Big Endian, nous sommes en Litle Endian, conversion...
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
  ((int*)buffer)[0] = htonl(MAGICNBR); //Premier int : MAGICNBR
  ((int*)buffer)[1] = htonl(nb); //Second int : nb passé en paramètre
}

int verificationPaquet(void* buffer, int nbBytes) {
  int premierEntierBuffer;
  if (nbBytes >= 4) { //Pour être sûr que le buffer contient au moins un entier (4 octets)  (sécurité contre le dépassement mémoire)
    premierEntierBuffer = ((int*)buffer)[0];
    return ((nbBytes == 8 || nbBytes == 24) && premierEntierBuffer == htonl(MAGICNBR)); //Un paquet valide est soit de 8 octets (console d'affichage), soit de 24 octets (console de contrôle) et doit comporter comme premier entier un entier spécifique (MAGICNBR)
  }
  else return 1;
}

void convertPaquetToOrdre(void* buffer,struct Ordre* ord) {
  struct Ordre* ptrDebOrd = (struct Ordre*)(&(((int*)buffer)[1]));
  *ord = *ptrDebOrd;
  ord->numero_vol[5] = '\0';
  ord->dep.cap = ntohl(ord->dep.cap); //Le Java a envoyé en Big Endian, conversion en Little Endian...
  ord->dep.vitesse = ntohl(ord->dep.vitesse);
  ord->altitude = ntohl(ord->altitude);
}

void* multicastManager() { /*Gère l'envoi en continu de l'adresse de la socket TCP d'écoute en multicast*/
  struct hostent *serveur_host;
  static struct sockaddr_in addr_serveur; //Adresse de la machine locale
  struct sockaddr_in addr;
  int addrlen, sock, cnt;
  char hostname [30];
  gethostname(hostname, 30); //Récupération du hostname local
  serveur_host = (struct hostent*)gethostbyname((char*)hostname); //Récupération de l'adresse locale à partir du hostname local
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
  addr_serveur.sin_port = htons(1285); //Port de la socket UDP

  addr.sin_addr.s_addr = inet_addr(GROUP_MULTICAST);
  while (1) {
    cnt = sendto(sock, &addr_serveur, sizeof(addr_serveur), 0,(struct sockaddr *) &addr, addrlen); //Envoi de l'adresse du serveur en envoyant directement la structure sockaddr_in
    if (cnt < 0) {
      perror("sendto");
      exit(1);
    }
    sleep(5);
  }
}

void* avionManager(void* sock) { /*Gère la connexion TCP d'un avion*/
  int socket_service = *((int*)sock);
  char num_vol[6]; //Stocke le num_vol de l'avion concerné par le thread
  num_vol[0] = '\0'; //Initialisation du num_vol à zéro
  num_vol[1] = '\0';
  num_vol[2] = '\0';
  num_vol[3] = '\0';
  num_vol[4] = '\0';
  num_vol[5] = '\0';
  struct Avion a;
  struct Ordre o;
  while (1) {
    if (read(socket_service,&a,sizeof(struct Avion)) != (sizeof(struct Avion))) {
      break;
    }
    else {
      if (num_vol[0] == '\0') { //Si le num_vol n'est pas initialisé
        strcpy(num_vol,a.numero_vol);
      }
	    ecrireAvion(a);
      if ((getNewOrdre(&o,a.numero_vol)) == 1) {
        if (write(socket_service,&o,sizeof(struct Ordre)) != sizeof(struct Ordre)) {
          perror("Erreur, impossible d'envoyer l'ordre");
          break;
        }
      }
    }
	}
  supprimerAvion(num_vol); //On supprime l'avion de la base...
  retirerOrdres(num_vol); // ...et les éventuels ordres en attente
	// on ferme la socket
	close(socket_service);
  return 0;
}
void* initialisationConnexionAvionManager() { /*Gère les demandes de connexion TCP des avions*/
	// adresse socket coté client
	static struct sockaddr_in addr_client;
	// longueur adresse
	socklen_t lg_addr;
	// socket d'écoute et de service
	int socket_ecoute, socket_service;
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
	// on attend la connexion d'un client
	lg_addr = sizeof(struct sockaddr_in);
  while (1) {
	  socket_service = accept(socket_ecoute,(struct sockaddr *)&addr_client, &lg_addr);
	  if (socket_service == -1) {
		  perror("erreur accept");
		  exit(1);
	  }
    thread = malloc(sizeof(pthread_t));
    pthread_create(thread,NULL,avionManager,&socket_service); //Création du thread pour traiter l'avion en passant en paramètre la socket de service
  }
	close(socket_ecoute);
  return 0;
}

void* consoleManager() { /*Gère les demandes des consoles d'affichages et de contrôles*/
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
  struct Ordre ord;
  
  sock = creerSocketUDP(5842);
  if(sock == -1) {
    perror("Erreur lors de la création de la socket UDP");
    exit(-1);
  }
  lg = sizeof(struct sockaddr_in);
  while(1) {
    tabAvion = NULL;
    nb_octets = recvfrom(sock,buffer,taillebuffeur,0,(struct sockaddr *)&addr_client,&lg); //En attente d'un paquet des deux types de Consoles
    if(verificationPaquet(buffer,nb_octets)) {
    //Si le premier entier est correct et que le paquet est d'une taille valide (4 ou 8 ou 24)
     switch(nb_octets) {
      case -1: //Si il y a eu une erreur
        perror("Erreur lors de la réception du paquet UDP");
        exit(-1);

      case 8: //Si deux entiers sont recu (Requete pour obtenir les avions en donnant le nbre d'avion attendu (peut être incorrect)) 
        tabAvion = NULL;
        lireAvions(&tabAvion,&nombreAvions);
        nbreAvionPaquet = ((int*)buffer)[1]; //2eme int du paquet
        if (getNbAvion() == 0) { //Si il y a zéro avions en stock, envoyer un paquet même si le nbAvion du paquet est correct pour signifier à la console d'affichage que le SGCA est toujours vivant
          paquetNbAvion(buffer,0);
          nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
          if (nb_octets == -1) {
            perror("Erreur lors de l'envoi d'un paquet UDP");
          }
        }
        else if (nbreAvionPaquet == htonl(nombreAvions)) {
        //Envois des avions à la console, un avion par paquet
          for(i = 0; i < nombreAvions; i++) {
            paquetAvion(buffer,tabAvion[i]);
            nb_octets = sendto(sock,buffer,(sizeof(int)+sizeof(struct Avion)),0,(struct sockaddr*)&addr_client,lg);
            if (nb_octets == -1) {
              perror("Erreur lors de l'envoi d'un paquet UDP");
            }
          }
        }
        else {
        //Si l'entier recu dans la requete n'est pas le nombre d'avion exact, informer la console d'affichage du nombre correct d'avions
          paquetNbAvion(buffer,getNbAvion());
          nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
          if (nb_octets == -1) {
            perror("Erreur lors de l'envoi d'un paquet UDP");
          }
        }
        if (tabAvion != NULL) {
          free(tabAvion); //lireAvions alloue tabAvion avec un malloc, il faut faire un free !
        }
        break;

      case 24: //Si un ordre est reçu (Requete pour prendre en compte un ordre de la console de controle
        convertPaquetToOrdre(buffer,&ord);
        if (checkAvion(ord.numero_vol) != -1) { //Si l'avion existe
          if(putOrdre(ord) == 0) { //Si l'ordre a pu été enregistré dans la base
            paquetNbAvion(buffer,0); //Réponse nominale : ordre pris en compte
            nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
            if (nb_octets == -1) {
              perror("Erreur lors de l'envoi d'un paquet UDP");
            }
          }
          else { //Si l'ordre n'a pas pu être enregistré dans la base
            paquetNbAvion(buffer,2); //Réponse erreur : base surchargée
            nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
            if (nb_octets == -1) {
              perror("Erreur lors de l'envoi d'un paquet UDP");
            }
          }
        }
        else {
          paquetNbAvion(buffer,1); //Réponse erreur : l'avion n'existe pas
          nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
          if (nb_octets == -1) {
            perror("Erreur lors de l'envoi d'un paquet UDP");
          }
        }
        break;
      }
    }
  }
}

int main() {
  pthread_t thread1;
  pthread_t thread2;
  initialiserBase();
  initialiserBaseOrdre();
  pthread_create(&thread1,NULL,consoleManager,NULL);
  pthread_create(&thread2,NULL,initialisationConnexionAvionManager,NULL);
  multicastManager();
  return 0;
}
