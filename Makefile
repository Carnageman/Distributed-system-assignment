CC = gcc -Wall -g
CJ = javac -cp Console -d classFiles/

all : avion sgca affichage.jar 

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
classFiles/affichage/PacketsAffichage.class : classFiles/common/Avion.class Console/affichage/PacketsAffichage.java
	$(CJ) Console/affichage/PacketsAffichage.java
classFiles/affichage/CommSGCA.class : classFiles/affichage/PacketsAffichage.class classFiles/common/Avion.class Console/affichage/CommSGCA.java classFiles/
	$(CJ) Console/affichage/CommSGCA.java
classFiles/affichage/UpdateVectorAvionThread.class : classFiles/affichage/CommSGCA.class classFiles/common/Avion.class classFiles/affichage/PacketsAffichage.class
	$(CJ) Console/affichage/UpdateVectorAvionThread.java
classFiles/affichage/ConsoleAffichageMain.class : classFiles/affichage/UpdateVectorAvionThread.class
	$(CJ) Console/affichage/ConsoleAffichageMain.java
affichage.jar : classFiles/affichage/ConsoleAffichageMain.class
	jar cfe affichage.jar affichage.ConsoleAffichageMain -C classFiles .

clean:
	rm Libs/TCP_socket.o
	rm Libs/UDP_socket.o	
	rm SGCA/baseDeDonnees.o
	rm sgca
	rm avion
	rm affichage.jar
	rm classFiles/ -R
