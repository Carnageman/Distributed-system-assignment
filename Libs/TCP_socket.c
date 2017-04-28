#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "TCP_socket.h"

int creerSocketTCP(int port) {
	// descripteur de la socket locale
	static struct sockaddr_in addr_local;
	int sock;
	// création de la socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
	   perror("erreur création socket");
	   return(-1);
	}
	// liaison de la socket sur le port local 4000
	bzero(&addr_local, sizeof(struct sockaddr_in));
	addr_local.sin_family = AF_INET;
	addr_local.sin_port = htons(port);
	addr_local.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sock, (struct sockaddr*)&addr_local, sizeof(addr_local))== -1) {
	      perror("erreur bind");
	      return(-1);
	}
	return sock;
}
