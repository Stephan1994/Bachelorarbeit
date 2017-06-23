
all:
	echo Compiling $(CLION_EXE_DIR)/$@ ...
	PC Netzwerk.out

PC: main.o Robot.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h Robot.h
	g++ -o PC main.o Robot.o ProzessPi.o Funktion_zum_Umwandeln.o -lpthread

Netzwerk.out: Netzwerk.c Netzwerk.h
	g++ -o $(CLION_EXE_DIR)/Netzwerk.out Netzwerk.c -lpthread
	
#main.out: main.o ProzessPi.o Funktion_zum_Umwandeln.o ProzessPi.h
#	g++ -o $(CLION_EXE_DIR)/main.out main.o ProzessPi.o Funktion_zum_Umwandeln.o  -lpthread

ProzessPi.o: main.c ProzessPi.c ProzessPi.h
	gcc -c ProzessPi.c
	
Funktion_zum_Umwandeln.o: Funktion_zum_Umwandeln.c ProzessPi.h
	g++ -c Funktion_zum_Umwandeln.c
	
main.o: main.cpp Robot.o ProzessPi.c ProzessPi.h
	g++ -c main.cpp 
	
Robot.o: Robot.cpp Robot.h ProzessPi.c ProzessPi.h
	g++ -c -std=c++11 Robot.cpp

#clean:
#	\rm Netzwerk.out
#	\rm main.out
#	\rm ProzessPi.o
#	\rm Funktion_zum_Umwandeln.o
	
