
all: Netzwerk.out main.out

Netzwerk.out: Netzwerk.c Netzwerk.h
	gcc -o Netzwerk.out Netzwerk.c -lpthread
	
main.out: main.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h
	gcc -o main.out main.o ProzessPi.o Funktion_zum_Umwandeln.o  -lpthread -lbcm2835

ProzessPi.o: main.c ProzessPi.c ProzessPi.h
	gcc -c ProzessPi.c
	
Funktion_zum_Umwandeln.o: Funktion_zum_Umwandeln.c ProzessPi.h
	gcc -c Funktion_zum_Umwandeln.c
	
main.o: main.c ProzessPi.c ProzessPi.h
	gcc -c main.c 
	
clean:
	\rm Netzwerk.out
	\rm main.out
	\rm ProzessPi.o
	\rm Funktion_zum_Umwandeln.o
	
