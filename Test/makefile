CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lpthread

OBJ = main.o Robot.o
OBJ_NETWORK = Netzwerk/ProzessPi.o Netzwerk/Funktion_zum_Umwandeln.o

BIN = Robot

all: Network Robot

Robot: $(OBJ)
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

#Robot: main.o Robot.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h Robot.h
#	g++ -o Robot main.o Robot.o ProzessPi.o Funktion_zum_Umwandeln.o -lpthread

#Netzwerk.out: Netzwerk.c Netzwerk.h
#	g++ -o Netzwerk.out Netzwerk.c -lpthread
	
#main.out: main.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h
#	g++ -o main.out main.o ProzessPi.o Funktion_zum_Umwandeln.o  -lpthread

#ProzessPi.o: main.c ProzessPi.c ProzessPi.h
#	g++ -c ProzessPi.c
	
#Funktion_zum_Umwandeln.o: Funktion_zum_Umwandeln.c ProzessPi.h
#	g++ -c Funktion_zum_Umwandeln.c
	
#main.o: main.cpp Robot.o ProzessPi.c ProzessPi.h
#	g++ -c main.cpp 
	
#Robot.o: Robot.cpp Robot.h ProzessPi.c ProzessPi.h
#	g++ -c -std=c++11 Robot.cpp
#clean:
#	\rm Netzwerk.out
#	\rm main.out
#	\rm ProzessPi.o
#	\rm Funktion_zum_Umwandeln.o
	
