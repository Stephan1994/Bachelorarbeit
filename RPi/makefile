CC = g++
CFLAGS = -Wall -std=c++11 `pkg-config --cflags opencv`
LDFLAGS = -lpthread -lbcm2835 `pkg-config --libs opencv`

OBJ = server.o RobotHandler.o GPIOLibrary.o ProtocolLibrary.o MessageWriter.o Camera.o
OBJ_NETWORK = Netzwerk/ProzessPi.o Netzwerk/Funktion_zum_Umwandeln.o

BIN = Server

all: Network Server

Server: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(OBJ_NETWORK) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

Network:
	make -C Netzwerk

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)

.PHONY: clean_all
clean_all:
	rm -rf $(BIN) $(OBJ)
	make clean -C Netzwerk




#all: Netzwerk.out server

#Netzwerk.out: Netzwerk.c Netzwerk.h
#	gcc -o Netzwerk.out Netzwerk.c -lpthread
	
#server: main.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h
#	g++ -o server main.o ProzessPi.o Funktion_zum_Umwandeln.o  -lpthread -lbcm2835

#ProzessPi.o: main.c ProzessPi.c ProzessPi.h
#	gcc -c ProzessPi.c
	
#Funktion_zum_Umwandeln.o: Funktion_zum_Umwandeln.c ProzessPi.h
#	gcc -c Funktion_zum_Umwandeln.c
	
#main.o: main.cpp ProzessPi.c ProzessPi.h
#	g++ -c main.cpp 
	
#clean:
#	\rm Netzwerk.out
#	\rm server
#	\rm ProzessPi.o
#	\rm Funktion_zum_Umwandeln.o
#	\rm main.o
	
