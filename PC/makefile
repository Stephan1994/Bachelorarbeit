CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lpthread

OBJ = main.o Robot.o MessageListener.o
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
