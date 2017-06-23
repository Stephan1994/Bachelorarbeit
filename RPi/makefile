
all: Netzwerk.out server

Netzwerk.out: Netzwerk.c Netzwerk.h
	gcc -o Netzwerk.out Netzwerk.c -lpthread
	
server: main.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h
	g++ -o server main.o ProzessPi.o Funktion_zum_Umwandeln.o  -lpthread -lbcm2835

ProzessPi.o: main.c ProzessPi.c ProzessPi.h
	gcc -c ProzessPi.c
	
Funktion_zum_Umwandeln.o: Funktion_zum_Umwandeln.c ProzessPi.h
	gcc -c Funktion_zum_Umwandeln.c
	
main.o: main.cpp ProzessPi.c ProzessPi.h
	g++ -c main.cpp 
	
clean:
	\rm Netzwerk.out
	\rm server
	\rm ProzessPi.o
	\rm Funktion_zum_Umwandeln.o
	\rm main.o
	
