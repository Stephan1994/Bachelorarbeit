/*  Dies soll der Netzwerk-Teil der Bibliothek werden
 * Funktionen die hier erreicht werden sollen:
 * 
 * 	-	Erstellung eines Servers
 * 	-	Herstellen einer Verbindung
 * 	-	Überwachung einer Verbindung
 * 	-	Senden von Daten
 * 	-	Empfangen von Daten
 * 
 * */
#include <stdio.h>

#define BUFFERSIZE 16384

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
int ErrorCounter = 0;
int flag_stop = 0;
int closeNetwork = 0;
pthread_t tid[2];

int ErstelleServer(int Port);
int RunServer(int sock, char *BufferIn,char *BufferOut,FILE *VonP,FILE *ZuP);
int ErstelleClient(char *BufferIn,char *BufferOut,int Port, char *IP,int SizeOfIP,FILE *VonP,FILE *ZuP);
void ReadAndWrite(int fd,char *BufferIn,char *BufferOut,FILE *VonP, FILE *ZuP);
void extractValue(char *BufferIn, char *Value);
void extractHeaderFieldValue(char *BufferIn, char *Command, char *HeaderFieldType);
void str_cut(char *str, int begin , int len);
void *TCPtoP(void *);
void *PtoTCP(void *);
int fd_set_blocking(int fd, int blocking);
