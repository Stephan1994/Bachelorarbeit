/*****************
 * 
 * 
 * 
 * gcc testmain.c -lbcm2835 -pthread
 * 
 * */
#ifdef __arm__
#include <bcm2835.h>
int fd[2]; // Pipe für ADC Thread & Speicher-Thread
int PipeStattFile[2]; // Pipe für Senden ohne Datei zu erzeugen
#endif


#ifndef ProzessPi_H
#define ProzessPi_H



#define AnzahlZeilenZumSenden_A 200
#define AnzahlZeilenZumSenden_B 200
#define AnzahlZeilenZumSenden_I 3


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <linux/limits.h>
#include <pthread.h>





#define START "START" // Setzt Startflag 1
#define STOP "STOP" //  Setzt Startflag 0
#define SPEICHERN "SPEICHERN"
#define SPEICHERN_STOPPEN "SPEICHERN_STOPPEN"
#define BEENDEN "BEENDEN"

#define SPI_SYNC "SPI_SYNC"
#define SPI_BLOCK "SBL"
#define SPI_DELAY "Delay_SPI"
#define NEUE_WERTE_A "A"
#define NEUE_WERTE_B "B"


#define I2C_NEUERWANDLER "I2C_NEUERWANDLER"
#define I2C_NEUEWERTE "I2C_NEUEWERTE"
#define I2C_EINZELWERT "ISV"
#define I2C_BLOCK "IBL"
#define I2C_DELAY "Delay_I2C"
#define NEUE_WERTE_I2C "I"


extern pthread_mutex_t I2C_Konflock;
extern pthread_mutex_t SPI_Konflock;

extern uint64_t Delay_SPI;
extern uint64_t Delay_I2C;



extern volatile int Flag_Start;
extern volatile int Flag_Stop;
extern volatile int Flag_Start_Messung;
extern volatile int Flag_ZuP;
extern volatile int Flag_RPi;
extern volatile int Flag_Speichern;
extern volatile int Flag_Done;
extern volatile int Pipe_Flag;

extern pthread_t tid[4]; // Threads des RPi

extern pthread_t thread_SPI;
extern pthread_t thread_I2C;
extern pthread_t thread_Speichern;
extern pthread_t thread_Senden;


extern pid_t fork(void);


extern char File_Name[200]; // Hier kommt der Dateiname rein. Wird von Threads verwendet.
extern char Pfad[200];	// Problematisch, PATH_MAX konnte nicht eingebunden werden -> Fehlerquelle
extern char Kommando[21]; // Buffer für Kommandos
extern char WerteBuffer[100000];
extern int Werte[10000];
extern char KonfBuffer[20];
extern uint8_t testmosi[8][3];
extern volatile sig_atomic_t GlobalFlag;
extern int bib_pid;
extern int status;
extern FILE *fd_senden;
extern FILE *fd_empfangen;


extern struct SPI_Wandler{
	
		int AD; //0 - AD / 	1 - DA
		int Aktiv; // 0 aus, 1 an
		int Wortlaenge; // Wieviele Übertragungen werden gebraucht pro Wert?
		int AnzahlKanaele;
		int AnzahlMesswerte;
		int BitOrder;
		int Frequenz; // Mit welcher Frequenz taktet der IC ?
		int Polaritaet;
		int DataMode;
		int ChipSelect;
		int Kanal[16];
		int FunktionZumWandeln;
		
		/* 
		*	Kanal[0] = 1; -> Kanal aktiv
		*	Kanal[1] = 0; -> Kanal inaktiv
		*   ...
		*   Kanal[AnzahlKanaele] = 1 -> Kanal aktiv 
		*
		*/
		
		uint8_t mosi[16][10];
		uint8_t miso[20];
		char WerteBuffer[100000];
		int Werte[16][10000];

		int (*WertUmwandeln)(uint8_t *Woerter,int AnzahlWoerter);
	
} WandlerA,WandlerB;

extern struct I2C_Wandler_Werte{	
	int Werte[40][5];
}I2C_Werte;

extern struct I2C_Liste{
	
		int Aktiv; // 0 aus, 1 an
		int AktuellerWert;
		int Nummer;
		int len;
		int Read; // Read 0 heißt nur schreiben
		int SlaveAdress;			// I2C_Slaveadresse, zB 0x48 
		uint8_t SendeBuffer[5];		// Für 8 Bit Werte zB 0 - 255, 16 bit entsprechend 0 - .. 
		char EmpfangsBuffer[5];		// Hier werden Werte gespeichert			
		int FrequenzModus;
		int FunktionZumWandeln;
		int (*WertUmwandeln)(uint8_t *Woerter,int AnzahlWoerter);
		
		struct I2C_Liste *next;
}*I2C_Liste_Anfang;

//extern struct I2C_Liste *I2C_Liste_Anfang;

#ifdef __cplusplus
extern "C"{
#endif

//Allgemein
#ifdef PROZESSPI_LIB
extern void sig_handler(int signo);
extern int fd_set_blocking(int fd, int blocking);
#endif
extern void TerminalOszi(int);


// Kommunikation
extern int SendeKommando(char *Kommando, char* Wert);
extern int EmpfangeKommando();
extern int init(char *IP,char *PORT);
extern void done();
extern void *SpeichernDerWerte(); 	// Thread RPi
extern void *LesenUndSenden(); 	  	// Thread RPi
extern int LesenUndSenden_File(char Buchstabe,int AnzahlZeilen,FILE*);
extern int LesenUndSenden_Pipe(char Buchstabe,FILE *Pipe,char *Buffer,int *Buffer_Zaehler,int *Zeilen_Zaehler,int AnzahlZeilenZumSenden);


// I2C- Funktionen
extern void I2C_printListe();
extern void I2C_syncListe();
extern void ErstelleListe_I2C();
extern int I2C_neuerWandler(int Read,int SlaveAdress,int AnzahlWoerter, uint8_t *SendeBuf, int FrequenzModus,int FunktionsNummer);
extern int NeueWerte_I2C_8(int AnzahlWerte,int *WandlerNummer,int *WandlerWerte);
extern int NeueWerte_I2C(int AnzahlWerte,int *WandlerNummer,int *Position,int *Wert);
extern int I2C_sendeEinzelwert(int FrequenzModus,int read,uint8_t SlaveAdress,int AnzahlWoerter,uint8_t *Woerter);
extern void *BCM2835_I2C(); 		// Thread RPi
extern int BCM2835_I2C_Einzelwert(char *);



// SPI-Funktionen
extern int  SPI_syncWandler(struct SPI_Wandler*,char);
extern void SPI_kleinerSync(struct SPI_Wandler*,char *);
extern int SPI_syncWerte(struct SPI_Wandler *DieserWandler, char *ZwischenBuffer);
extern void SPI_printWandler(struct SPI_Wandler *DieserWandler);
extern int SPI_erstelleWandler(struct SPI_Wandler* DieserWandler,int AD,int AnzahlMesswerte,int BitOrder,int Frequenz,int Polaritaet,int DataMode,int AnzahlKanaele,int *AktiveKanaele,int Wortlaenge,int FunktionsNummer);
extern void *BCM2835_SPI(void *ADW);	// Thread RPi
extern int BCM2835_SPI_Konfiguriation(struct SPI_Wandler* DieserWandler); 	//Thread-Funktionen
extern int BCM2835_SPI_Lesen(struct SPI_Wandler* DieserWandler,FILE *,int);	//Thread Funktionen


// Funktionen zum Umwandeln
// -> sind in "Funktionen_zum_Umwandeln" definiert
extern int Funktion_zum_Umwandeln_SPI(struct SPI_Wandler *DieserWandler, int FunktionsNummer);
extern int Funktion_zum_Umwandeln_I2C(struct I2C_Liste *DieserWandler,int FunktionsNummer);
extern int mcp3008(uint8_t *Woerter,int AnzahlWoerter);
extern int mcp3208(uint8_t *Woerter,int AnzahlWoerter);
extern int mcp3551(uint8_t *Woerter,int AnzahlWoerter);
extern int I2C_8Bit(uint8_t *Woerter,int AnzahlWoerter);


#ifdef __cplusplus
}
#endif

#endif
