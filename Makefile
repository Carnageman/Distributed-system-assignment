CC = gcc -Wall -g
CJ = javac -cp Console -d classFiles/

all : avion sgca affichage.jar controle.jar

avion : Avion/avion.c Libs/avion.h Libs/TCP_socket.o Libs/TCP_socket.h
	$(CC) -o avion Avion/avion.c -lm Libs/TCP_socket.o
Libs/TCP_socket.o : Libs/TCP_socket.c Libs/TCP_socket.h
	$(CC) -o Libs/TCP_socket.o -c Libs/TCP_socket.c
Libs/UDP_socket.o : Libs/UDP_socket.c Libs/UDP_socket.h
	$(CC) -o Libs/UDP_socket.o -c Libs/UDP_socket.c

SGCA/baseDeDonnees.o : SGCA/baseDeDonnees.h SGCA/baseDeDonnees.c
	$(CC) -o SGCA/baseDeDonnees.o -c SGCA/baseDeDonnees.c
SGCA/gestionOrdre.o : SGCA/gestionOrdre.h SGCA/gestionOrdre.c
	$(CC) -o SGCA/gestionOrdre.o -c SGCA/gestionOrdre.c
sgca : Libs/avion.h SGCA/sgca.c Libs/UDP_socket.o SGCA/baseDeDonnees.o Libs/TCP_socket.o SGCA/gestionOrdre.o
	$(CC) -o sgca SGCA/sgca.c Libs/UDP_socket.o SGCA/baseDeDonnees.o Libs/TCP_socket.o SGCA/gestionOrdre.o -lpthread

classFiles/ :
	mkdir classFiles
classFiles/common/Avion.class : Console/common/Avion.java classFiles/
	$(CJ) Console/common/Avion.java
classFiles/common/OrdreDeplacement.class : Console/common/OrdreDeplacement.java
	$(CJ) Console/common/OrdreDeplacement.java
classFiles/common/SGCATimeOutException.class : Console/common/SGCATimeOutException.java
	$(CJ) Console/common/SGCATimeOutException.java

classFiles/affichage/PacketsAffichage.class : classFiles/common/Avion.class Console/affichage/PacketsAffichage.java
	$(CJ) Console/affichage/PacketsAffichage.java
classFiles/affichage/CommSGCA.class : classFiles/affichage/PacketsAffichage.class classFiles/common/Avion.class Console/affichage/CommSGCA.java classFiles/
	$(CJ) Console/affichage/CommSGCA.java
classFiles/affichage/UpdateVectorAvionThread.class : classFiles/affichage/CommSGCA.class classFiles/common/Avion.class classFiles/affichage/PacketsAffichage.class
	$(CJ) Console/affichage/UpdateVectorAvionThread.java
classFiles/affichage/ConsoleAffichageMain.class : classFiles/affichage/UpdateVectorAvionThread.class classFiles/common/SGCATimeOutException.class
	$(CJ) Console/affichage/ConsoleAffichageMain.java
	
classFiles/controle/AvionNotFoundException.class : Console/controle/AvionNotFoundException.java
	$(CJ) Console/controle/AvionNotFoundException.java
classFiles/controle/CommunicationSGCA.class : Console/controle/CommunicationSGCA.java
	$(CJ) Console/controle/CommunicationSGCA.java
classFiles/controle/NoFreeSpaceOrderException.class : Console/controle/NoFreeSpaceOrderException.java
	$(CJ) Console/controle/NoFreeSpaceOrderException.java
classFiles/controle/ControleMain.class : Console/controle/ControleMain.java classFiles/controle/AvionNotFoundException.class classFiles/controle/CommunicationSGCA.class
	$(CJ) Console/controle/ControleMain.java
	
affichage.jar : classFiles/affichage/ConsoleAffichageMain.class
	jar cfe affichage.jar affichage.ConsoleAffichageMain -C classFiles common/ -C classFiles affichage/
controle.jar : classFiles/controle/ControleMain.class classFiles/controle/AvionNotFoundException.class classFiles/controle/CommunicationSGCA.class classFiles/controle/NoFreeSpaceOrderException.class classFiles/common/SGCATimeOutException.class classFiles/common/OrdreDeplacement.class
	jar cfe controle.jar controle.ControleMain -C classFiles controle/ -C classFiles common/

clean:
	rm Libs/TCP_socket.o
	rm Libs/UDP_socket.o	
	rm SGCA/baseDeDonnees.o
	rm sgca
	rm avion
	rm affichage.jar
	rm classFiles/ -R
