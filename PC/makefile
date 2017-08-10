CC = g++
CFLAGS = -Wall -std=c++11 `pkg-config --cflags opencv`
LDFLAGS = -lpthread `pkg-config --libs opencv`

OBJ = main.o Robot.o MessageListener.o ProtocolLibrary.o
OBJ_NETWORK = Netzwerk/ProzessPiClient.o Netzwerk/Funktion_zum_Umwandeln.o

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
