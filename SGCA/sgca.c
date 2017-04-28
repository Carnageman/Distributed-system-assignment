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
#include "baseDeDonnees.h"

#define TAILLEBUF 50 

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

void* initialisationConnexionAvionManager() {

}

void* consoleAffichageManager() {
  socklen_t lg;
  socklen_t lgEnvoi;
  /*static*/ struct sockaddr_in addr_client;
  struct sockaddr_in addr_client2;
  int sock;
  size_t taillebuffeur = TAILLEBUF;
  char buffer[TAILLEBUF];
  char bufferEnvoi[TAILLEBUF];
  int nb_octets, nb_octets2;
  int nombreAvions;
  struct Avion* tabAvion;
  int i;
  struct Avion* debBufferAvion;
  int sockEnvoi;
  
  sock = creerSocketUDP(5842);
  sockEnvoi = creerSocketUDP(0);
  if(sock == -1) {
    fprintf(stderr,"Erreur, ouverture de socket UDP impossible !\n");
    exit(-1);
  }
  lg = sizeof(struct sockaddr_in);
  while(1) {
    lg = sizeof(struct sockaddr_in);
    printf("J'attend un paquet... %d\n",sock);
    nb_octets = recvfrom(sock,buffer,taillebuffeur,0,(struct sockaddr *)&addr_client,&lg);
    addr_client2 = addr_client;
    memcpy(bufferEnvoi,buffer,TAILLEBUF);
    printf("J'ai recu un paquet de %d octets capitaine, je le traite... \n",nb_octets);
    switch(nb_octets) {
    case -1:
    //Si il y a eu une erreur
      perror("Erreur lors de la réception du packet");
      exit(-1);
    case 4:
    //Si un entier seul est recu (Requete pour obtenir le nbre d'avion)
      //((int*)bufferEnvoi)[1] = htonl(getNbAvion()); //Peut etre ajouter un to big endien
      printf("Demande de nbAvion recu !\n");
      lgEnvoi = sizeof(struct sockaddr_in);
      nb_octets2 = sendto(sockEnvoi,buffer,8,0,(struct sockaddr*)&addr_client2,lgEnvoi);
      printf("nb_octets2 : %d\n",nb_octets2);
      printf("sizeof : %d\n",sizeof(int)*2);
      break;
    
    case 8:
      lireAvions(&tabAvion,&nombreAvions);
      //Si deux entiers sont recu (Requete pour obtenir les avions en donnant le nbre d'avion)
      if (((int*)buffer)[1] == htonl(nombreAvions)) {
      //Envois des avions à la console
        printf("Demande d'avions recu !\n");
        debBufferAvion = (struct Avion*)(&(((int*)buffer)[1]));
        for(i = 0; i < nombreAvions; i++) {
          (*debBufferAvion) = tabAvion[i];
          nb_octets = sendto(sock,buffer,(sizeof(int) + sizeof(struct Avion)),0,(struct sockaddr*)&addr_client,lg);
        }
      }
      else {
      //Si l'entier recu dans la requete n'est pas le nombre d'avion
        ((int*)buffer)[1] = nombreAvions; 
        nb_octets = sendto(sock,buffer,(sizeof(int)*2),0,(struct sockaddr*)&addr_client,lg);
      }
      break;
    }
  }
  printf("Je me barre.\n");
}

void* avionInfoManager() {

}

void* avionOrdreManager() {

}

int main() {
  pthread_t thread1;
  pthread_t thread2;
  initialiserBase();
  printf("Hello world.\n");
  pthread_create(&thread1,NULL,multicastManager,NULL);
  pthread_create(&thread2,NULL,consoleAffichageManager,NULL);
  while (1);
  //pthread_join(thread1,NULL);
}
