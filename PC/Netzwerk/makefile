
all: NetzwerkClient.out main.out

NetzwerkClient.out: NetzwerkClient.c NetzwerkClient.h
	gcc -o NetzwerkClient.out NetzwerkClient.c -lpthread
	
main.out: main.o ProzessPiClient.o Funktion_zum_Umwandeln.o ProzessPiClient.h
	gcc -o main.out main.o ProzessPiClient.o Funktion_zum_Umwandeln.o  -lpthread

ProzessPiClient.o: main.c ProzessPiClient.c ProzessPiClient.h
	gcc -c ProzessPiClient.c
	
Funktion_zum_Umwandeln.o: Funktion_zum_Umwandeln.c ProzessPiClient.h
	gcc -c Funktion_zum_Umwandeln.c
	
main.o: main.c ProzessPiClient.c ProzessPiClient.h
	gcc -c main.c 
	
clean:
	\rm NetzwerkClient.out
	\rm main.out
	\rm ProzessPiClient.o
	\rm Funktion_zum_Umwandeln.o
	
