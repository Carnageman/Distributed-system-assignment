CC = gcc -Wall -g

all : avion example.jar sgca

avion : Avion/avion.c Libs/avion.h Libs/TCP_socket.o Libs/TCP_socket.h
	$(CC) -o avion Avion/avion.c -lm Libs/TCP_socket.o
Libs/TCP_socket.o : Libs/TCP_socket.c Libs/TCP_socket.h
	$(CC) -o Libs/TCP_socket.o -c Libs/TCP_socket.c
Libs/UDP_socket.o : Libs/UDP_socket.c Libs/UDP_socket.h
	$(CC) -o Libs/UDP_socket.o -c Libs/UDP_socket.c

SGCA/baseDeDonnees.o : SGCA/baseDeDonnees.h SGCA/baseDeDonnees.c
	$(CC) -o SGCA/baseDeDonnees.o -c SGCA/baseDeDonnees.c
sgca : Libs/avion.h SGCA/sgca.c Libs/UDP_socket.o SGCA/baseDeDonnees.o
	$(CC) -o sgca SGCA/sgca.c Libs/UDP_socket.o SGCA/baseDeDonnees.o -lpthread

Console/package2/ExampleIO.class : Console/package2/ExampleIO.java
	javac -cp Console Console/package2/ExampleIO.java
Console/package1/ExampleProgram.class : Console/package1/ExampleProgram.java
	javac -cp Console Console/package1/ExampleProgram.java
example.jar : Console/package1/ExampleProgram.class Console/package2/ExampleIO.class
	jar cfe example.jar package1.ExampleProgram -C Console package1/ExampleProgram.class -C Console package2/ExampleIO.class

clean:
	rm Libs/TCP_socket.o
	rm Libs/UDP_socket.o	
	rm SGCA/baseDeDonnees.o
	rm Console/package1/ExampleProgram.class
	rm Console/package2/ExampleIO.class
	rm sgca
	rm avion
