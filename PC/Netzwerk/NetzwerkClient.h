/*  Dies soll der Netzwerk-Teil der Bibliothek werden
 * Funktionen die hier erreicht werden sollen:
 * 
 * 	-	Erstellung eines Servers
 * 	-	Erstellung eines Clients 
 * 	-	Herstellen einer Verbindung
 * 	-	Überwachung einer Verbindung
 * 	-	Senden von Daten
 * 	-	Empfangen von Daten
 * 	-	Bereitstellen einer API um Funktionen einfach anzubieten
 * 
 * */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>



#define  BUFSIZ 16384

struct ThreadUebergabe
{
	int fd;
	FILE* VonP;
	FILE* ZuP;
} TCPtoP_Struct,PtoTCP_Struct;

FILE *Log;
volatile sig_atomic_t a;
 int debug; 
int PapaPID;
int ErrorCounter=0;
int flag_sigpipe =0;
pthread_t tid[2];

int ErstelleClient(char *BufferIn,char *BufferOut,int Port, char *IP,int SizeOfIP,FILE *VonP,FILE *ZuP);
void *TCPtoP(void *);
void *PtoTCP(void *);
void extractHeaderFieldValue(char *BufferIn, char *Command, char *HeaderFieldType);
void extractValue(char *BufferIn, char *Value);
void extractValue(char *BufferIn, char *Value);
#ifdef NETZWERK_LIB
int fd_set_blocking(int fd, int blocking);
void sig_handler(int signo);
int main(int argc,char* argv[]);
#endif