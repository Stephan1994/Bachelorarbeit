/*  Dies soll der Netzwerk-Teil der Bibliothek werden
 * Funktionen die hier erreicht werden sollen:
 *
 * 	-	Erstellung eines Clients 
 * 	-	Herstellen einer Verbindung
 * 	-	Überwachung einer Verbindung
 * 	-	Senden von Daten
 * 	-	Empfangen von Daten
 * 
 * */

#include <stdio.h>

#define  BUFFERSIZE 16384

struct ThreadUebergabe
{
	int fd;
	FILE* VonP;
	FILE* ZuP;
} TCPtoP_Struct,PtoTCP_Struct;

FILE *Log;
int debug;
pthread_t tid[2];

int ErstelleClient(char *BufferIn,char *BufferOut,int Port, char *IP,int SizeOfIP,FILE *VonP,FILE *ZuP);
void *TCPtoP(void *);
void *PtoTCP(void *);
void extractHeaderFieldValue(char *BufferIn, char *Command, char *HeaderFieldType);
void extractValue(char *BufferIn, char *Value);

#ifdef NETZWERK_LIB
int fd_set_blocking(int fd, int blocking);
void sig_handler(int signo);
int main(int argc,char* argv[]);
#endif
