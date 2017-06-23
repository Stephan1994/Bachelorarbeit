#include "ProzessPi.h"

uint64_t Delay_SPI=0;
uint64_t Delay_I2C=0;

pthread_t tid[4]; // Threads des RPi


pthread_t thread_SPI=0;
pthread_t thread_I2C=0;
pthread_t thread_Speichern=0;
pthread_t thread_Senden=0;

pid_t fork(void);
pthread_mutex_t I2C_Konflock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t SPI_Konflock=PTHREAD_MUTEX_INITIALIZER;
void sig_handler(int signo);
volatile int Flag_Start=0;
volatile int Flag_Stop =0;
volatile int Flag_Start_Messung=0;
volatile int Flag_ZuP=0;
volatile int Flag_RPi=0;
volatile int Flag_Speichern=0;
volatile int Flag_Done = 0;
volatile int Pipe_Flag=1;

char File_Name[200]; // Hier kommt der Dateiname rein. Wird von Threads verwendet.
char Pfad[200];	// Problematisch, PATH_MAX konnte nicht eingebunden werden -> Fehlerquelle
char Kommando[21]; // Buffer für Kommandos
char WerteBuffer[100000];
int Werte[10000];
char KonfBuffer[20];
uint8_t testmosi[8][3];
volatile sig_atomic_t GlobalFlag;
int bib_pid;
int status;
FILE *fd_senden;
FILE *fd_empfangen;

struct SPI_Wandler WandlerA,WandlerB;
struct I2C_Liste *I2C_Liste_Anfang;


void sig_handler(int signo)
{
  if(signo == SIGUSR1)
	{
		Flag_ZuP=1;
		return;
	}
	if(signo == SIGUSR2)
	{
		if(Flag_Start == 0)
		{
	//		Flag_Start=1;
//			Flag_Stop=0;
		return;
		}
	//		Flag_Start=0;
//			Flag_Stop=1;
		return;
	}
		
 if(signo == SIGPIPE)
 {
	
	//exit(1);
	if(Flag_Done == 0)
	{
		printf("\n Client tot? Davon erhole ich mich nicht mehr! Versuche einen Neustart! \n");
		done();
		Flag_Done = 1;
	//	printf("Flag_Done wurde 1 gesetzt!\n");
	}
	

//	exit(1);
 }
 
}

int init(char *IP,char *PORT){
	
	if(signal(SIGUSR1, sig_handler) == SIG_ERR)
	{
		printf("\n can't catch SIG 10\n");
	}
	if(signal(SIGUSR2, sig_handler)== SIG_ERR){
		printf("\n can't catch SIG_USR2");
	}
	if(signal(SIGPIPE, sig_handler)== SIG_ERR)
	{
		printf("\n can't catch SIG_USR2");
	}
	unlink("/tmp/TCPtoP");
	unlink("/tmp/PtoTCP");
	
	//if(mknod("/tmp/PtoS", S_IFIFO | 0666,0) < 0 )
	if(mkfifo("/tmp/PtoTCP",0666))
	{
		perror(NULL);
		return 0;
	}
	//if(mknod("/tmp/StoP", S_IFIFO | 0666,0) < 0 )
	if(mkfifo("/tmp/TCPtoP",0666))
	{
		perror(NULL);
		return 0;
	}
	
	char Tempbuf[14];
	char wd[300];
	
	FILE *fd;
	fd=fopen("/etc/issue","r");
	char tbuf[9];
	int t;
	for(t=0;t<9;t++)
	{
		tbuf[t]=fgetc(fd);
	}
			if(strncmp(tbuf,"Raspbian",3)==0)
			{
				Flag_RPi = 1;
				strcpy(Tempbuf,"Server");
				Tempbuf[6] = '\0';
			}
			else
			{
				strcpy(Tempbuf,"Client");
				Tempbuf[6] = '\0';
			}
	fclose(fd);
	switch(bib_pid=fork()){
	case -1: 
		
			printf("\n \n Error beim Erstellen des Prozesses! \n \n"); return 0;
	
	case 0:
			
            getcwd(wd,sizeof(wd));   

            strcat(wd,"/Netzwerk.out");
          //  sprintf(wd,"/home/theo/Schreibtisch/MasterArbeit/Programmierung/Laptop/ProzessStation");
            printf("Das Verzeichnis ist -->%s<--",wd);


            execlp(wd,"noob", Tempbuf,IP, PORT,NULL);
			perror("execlp");	
	
			printf("\n\n\n\n\nHier könnte Ihre Werbung stehen!\n\n\n\n\n\n");
	
	break;	  
	default: 
			printf("	Testprogramm gestartet \n");
            if((fd_senden = fopen("/tmp/PtoTCP","w"))==0)
			{
				printf("Testprogramm meldet für PtoSTCP fopen: ");
				perror(NULL);
				printf("\n");
			}

			
			
			fprintf(fd_senden,"Pfeifentest!\n");	// Testet die Pipe 
			fflush(fd_senden);
            //kill(bib_pid,SIGUSR1);
	
            if((fd_empfangen = fopen("/tmp/TCPtoP","r"))==0)
			{
				printf("Testprogramm meldet für TCPStoP fopen: ");
				perror(NULL);
				printf("\n");
			}
			
		
			
	}
	printf("	Hier ist der Anfang vom Ende \n");
	
	fgets(Tempbuf,14,fd_empfangen);
	
	//read(fileno(fd_empfangen),Tempbuf,13);
	printf("		und hier das Ende! \n");
	printf("\n %s \n",Tempbuf);
	
	
	
			fd_set_blocking(fileno(fd_senden),0);
			
			
	return 1;
}
void done()
{
	
	kill(bib_pid,SIGUSR2);
	unlink("/tmp/TCPtoP");
	unlink("/tmp/PtoTCP");
	printf("done()!\n");
//	printf("Habe Netzwerk ein Signal zum Beenden geschickt! \n");
	//wait(&status);	
//	exit(1);	
}

int SendeKommando(char *Kommando, char* Wert)
{
		if(fprintf(fd_senden,"%s,%s;",Kommando,Wert) == 0)
		{
			return 0;
		}
		fflush(fd_senden);
		
		return 1;
}

void ErstelleListe_I2C( )
{
	I2C_Liste_Anfang = (struct I2C_Liste*)malloc(sizeof(struct I2C_Liste));	
	I2C_Liste_Anfang->next=NULL;
}

int I2C_neuerWandler(int Read,int SlaveAdress,int AnzahlWoerter,uint8_t *SendeBuffer, int FrequenzModus,int FunktionsNummer)
{
	struct I2C_Liste *Zeiger, *Anfang;
	
	if(I2C_Liste_Anfang == NULL)
	{
		ErstelleListe_I2C();
	}
	
	Anfang = I2C_Liste_Anfang;
	Zeiger = I2C_Liste_Anfang;
	int Nummer = 0;
	while(Zeiger->next != NULL)
	{
		Nummer++;
		Zeiger=Zeiger->next;
	}
	if((Zeiger->next =
     (struct I2C_Liste*) malloc(sizeof(struct I2C_Liste))) == NULL) {
          fprintf(stderr,"Kein Speicherplatz fuer das "
                         "letzte Element\n"); return 0;}
     Zeiger=Zeiger->next;
     Zeiger->Nummer= Nummer;
     Zeiger->Aktiv = 0;
     Zeiger->Read = Read;
     
     if(Read == 1)
     {
		 Anfang->Nummer++;
	 }
	 
     Zeiger->len = AnzahlWoerter;
     Zeiger-> SlaveAdress = SlaveAdress;
     
//     printf("SendeBuffer[0]--------->%d	---> %d\n",SendeBuffer[0],SendeBuffer[0]);
   
     Zeiger->SendeBuffer[0] = SendeBuffer[0];
     
		if(AnzahlWoerter > 1)
		{
			Zeiger->SendeBuffer[1] = SendeBuffer[1];
		}
		if(AnzahlWoerter > 1)
		{
			Zeiger->SendeBuffer[2] = SendeBuffer[2];	
		}	
		
     Zeiger->FrequenzModus = FrequenzModus;
     Zeiger->FunktionZumWandeln = FunktionsNummer;
	printf("Zeiger->FunktionZumWandeln = %d \n",Zeiger->FunktionZumWandeln);
	
	Funktion_zum_Umwandeln_I2C(Zeiger,FunktionsNummer);
     
     printf("Habe einen neuen Wandler erstellt! Nr---->%d \n",Nummer);
	Zeiger->next = NULL;
	return 1;
}
int NeueWerteSenden_I2C(int AnzahlWerte,int *WandlerNummer,int *WandlerWerte)
{
	// WandlerNummer[2] = 3
	// WandlerWerte[4] =...
	// WandlerWerte[5] =...
	
	if(AnzahlWerte < 1)
	{
		return 0;
	}
	char toSend[AnzahlWerte*3*sizeof(int)+10];
	char tmp[sizeof(int)*3+5];
	int i;
	toSend[0] = '\0';
	tmp[0] = '\0';
	//Ich sortiere mir hier die Nummern schon aufsteigend
	// Damit spare ich mir ja dann Zeit auf dem Pi 
	// Wenn ich weniger in der Liste spielen muss

	// erste Fehlerquelle falls Werte falsch ankommen!
	int tmp_int;
	int tmp_int2;
	int j;
	for(j=0;j<AnzahlWerte;j++)
	{
		for(i=0;i<AnzahlWerte-1;i++)
		{
			if(WandlerNummer[i] > WandlerNummer[i+1])
			{
				tmp_int = WandlerNummer[i+1];
				WandlerNummer[i+1] = WandlerNummer[i];
				WandlerNummer[i] = tmp_int;

				tmp_int 	= WandlerWerte[(i+1)*2];
				tmp_int2 	= WandlerWerte[((i+1)*2)+1];
				WandlerWerte[(i+1)*2] = WandlerWerte[i*2];
				WandlerWerte[(i+1)*2+1] = WandlerWerte[i*2+1];
				WandlerWerte[i*2] = tmp_int;
				WandlerWerte[i*2+1]=  tmp_int2;
			}
		}
	}
	
	sprintf(tmp,"%d,",AnzahlWerte);
	strcat(toSend,tmp);
	printf("toSend Zwischenschritt: %s\n",toSend);
	
	for(i = 0; i < AnzahlWerte;i++)
	{
		sprintf(tmp,"%d,%d,%d,",WandlerNummer[i],WandlerWerte[i*2],WandlerWerte[(i*2)+1]);
		strcat(toSend,tmp);
		//printf("toSend Zwischenschritt: %s\n",toSend);
	}
	printf("I2CS,%s\n",toSend);
	SendeKommando("I2CS",toSend);
	return 1;
}
int NeueWerte_I2C(int AnzahlWerte,int *WandlerNummer,int *Position,int *Wert)
{
	char Buffer[3*5*AnzahlWerte + 2*AnzahlWerte]; // 5 Stellen pro Wert / Position / Wandlernummer -> muss reichen
	char Buffer_Temp[3*10];
	Buffer[0]='\0';
	int i = 0;
	sprintf(Buffer,"%d,",AnzahlWerte);
	
	do{
		Buffer_Temp[0]='\0';
		sprintf(Buffer_Temp,"%d,%d,%d,",WandlerNummer[i],Position[i],Wert[i]);
		strncat(Buffer,Buffer_Temp,strlen(Buffer_Temp));
		
	}while((i++) != AnzahlWerte);
	
	SendeKommando("I2CS",Buffer);
	Buffer[0]='\0';
	return 0;
} 
int I2C_sendeEinzelwert(int FrequenzModus,int read,uint8_t SlaveAdress,int AnzahlWoerter,uint8_t *Woerter)
{
	//int Einzelwert( FrequenzModus,read, Slaveadresse, anzahl Bufs, buf[]);
	
	char Temp_Buf[30];
	char SendeBuffer[500];
	Temp_Buf[0]='\0';
	SendeBuffer[0]='\0';

	sprintf(Temp_Buf,"%d,",FrequenzModus);
	strcat(SendeBuffer,Temp_Buf);
	Temp_Buf[0]='\0';
	sprintf(Temp_Buf,"%d,",read);
	strcat(SendeBuffer,Temp_Buf);
	Temp_Buf[0]='\0';
	sprintf(Temp_Buf,"%d,",SlaveAdress);
	strcat(SendeBuffer,Temp_Buf);
	Temp_Buf[0]='\0';
	sprintf(Temp_Buf,"%d,",AnzahlWoerter);
	strcat(SendeBuffer,Temp_Buf);
	Temp_Buf[0]='\0';
	int i;
	for(i = 0; i < AnzahlWoerter;i++)
	{
		sprintf(Temp_Buf,"%d,",Woerter[i]);
		strcat(SendeBuffer,Temp_Buf);
		Temp_Buf[0]='\0';	
		
		if(strlen(SendeBuffer) > 450)
		{
			return 0;
		}
	}
	printf("Ich sende: %s \n",SendeBuffer);
	SendeKommando(I2C_EINZELWERT,SendeBuffer);
    return 1;
}	
void I2C_syncListe()
{
	struct I2C_Liste *Zeiger;
	Zeiger = I2C_Liste_Anfang;
	Zeiger=Zeiger->next;
	char tmp[100];
	while(Zeiger->next != NULL)
	{
		
	sprintf(tmp,"%d,%d,%d,%d,%d,%d,%d,%d",(int)Zeiger->len,
								Zeiger->Read,
								Zeiger->SlaveAdress,
								(int)Zeiger->SendeBuffer[0],
								(int)Zeiger->SendeBuffer[1],
								(int)Zeiger->SendeBuffer[2],
								Zeiger->FrequenzModus,
								Zeiger->FunktionZumWandeln);
								
     SendeKommando("I2C_NEUERWANDLER",tmp);
     printf("Habe gesendet: I2CNeu ---> %s \n",tmp); 
		Zeiger=Zeiger->next;	
	}
	sprintf(tmp,"%d,%d,%d,%d,%d,%d,%d,%d",(int)Zeiger->len,
								Zeiger->Read,
								Zeiger->SlaveAdress,
								(int)Zeiger->SendeBuffer[0],
								(int)Zeiger->SendeBuffer[1],
								(int)Zeiger->SendeBuffer[2],
								Zeiger->FrequenzModus,
								Zeiger->FunktionZumWandeln);
								
     SendeKommando("I2C_NEUERWANDLER",tmp);
     printf("Habe gesendet: I2CNeu ---> %s \n",tmp); 
}
void I2C_printListe()
{
	if(I2C_Liste_Anfang != NULL)
	{
	struct I2C_Liste *Zeiger;
	Zeiger = I2C_Liste_Anfang;
	Zeiger=Zeiger->next;
	while(Zeiger->next != NULL)
	{
		printf("\n-I2C-Wandler------- \n");
		printf("Nummer:	%d \n",Zeiger->Nummer);
		printf("Read: %d \n",Zeiger->Read);
		printf("len:	%d \n",Zeiger->len);
		printf("SlaveAdresse Dez :	%d \n",Zeiger-> SlaveAdress);
		printf("SendeBuffer[0]:	%d \n",Zeiger-> SendeBuffer[0]);
		printf("SendeBuffer[1]:	%d	\n",Zeiger->SendeBuffer[1]);
		printf("SendeBuffer[2]:	%d	\n",Zeiger->SendeBuffer[2]);
		printf("FrequenzModus:	%d	\n",Zeiger->FrequenzModus);
		Zeiger=Zeiger->next;
	}
		printf("\n-I2C-Wandler------- \n");
		printf("Nummer:	%d \n",Zeiger->Nummer);
		printf("Read: %d \n",Zeiger->Read);
		printf("len:	%d \n",Zeiger->len);
		printf("SlaveAdresse Dez :	%d \n",Zeiger-> SlaveAdress);
		printf("SendeBuffer[0]:	%d \n",Zeiger-> SendeBuffer[0]);
		printf("SendeBuffer[1]:	%d	\n",Zeiger->SendeBuffer[1]);
		printf("SendeBuffer[2]:	%d	\n",Zeiger->SendeBuffer[2]);
		printf("FrequenzModus:	%d	\n",Zeiger->FrequenzModus);
	}
	
}

int EmpfangeRobotKommando(char* value)
{
	int i = 0;
	int filePosition = ftell(fd_empfangen);
	while((Kommando[i++]=fgetc(fd_empfangen)) != ',');
	if (strcmp(Kommando,"Robot"))
	{
		char buffer[10000]; //seems to be size of pipe
		i = 0;
		while((buffer[i++]=fgetc(fd_empfangen)) != ';');
		buffer[i]='\0';
		strncpy(value, buffer, sizeof(buffer));
		return 16;
	}
	else
	{
		fseek(fd_empfangen, filePosition, SEEK_SET);
		return EmpfangeKommando();
	}
}

int EmpfangeKommando()
{	
	char delimiter[] = " ,;:";	
	char *ptr;
    char ZwischenBuffer[10000]; // Pipe-Size
	
	
	ZwischenBuffer[0]='\0';
	WerteBuffer[0]='\0';

	int i=0;
    while((Kommando[i++]=fgetc(fd_empfangen)) != ',');
    Kommando[i-1] = '\0';
    i=0;
  //  printf("Kommando:	--->%s<-- \n ",Kommando);
	
  //  while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
 //  ZwischenBuffer[i-1] = '\0';
 //   printf("Zwischenbuffer: \n %s",ZwischenBuffer);
//return 0;

    if(strcmp(Kommando,BEENDEN) == 0)
	{
		Flag_Done = 1;
		printf("Habe Beenden empfangen und Flag auf 1 gesetzt!\n");
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		return 0;
	}
		
	if(strncmp(Kommando,START,8)==0)
	{
		Flag_Start=1;
		
		if(pthread_mutex_trylock(&I2C_Konflock) == 0)
		{
			pthread_mutex_unlock(&I2C_Konflock); // Unlock zeigt ohne lock undefined behaviour
			printf("MainProzess:	I2C gelockt und unlocked!\n");
		}
	
		if(pthread_mutex_trylock(&SPI_Konflock) == 0)
		{
			pthread_mutex_unlock(&SPI_Konflock); // Unlock zeigt ohne lock undefined behaviour
			printf("MainProzess:	SPI gelockt und unlocked!\n");
		}
		
		
	pthread_mutex_unlock(&I2C_Konflock);
	pthread_mutex_unlock(&SPI_Konflock);
	
	
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i]='\0';
		printf("MainProzess:---> gestartet! \n");
		return 1;
	}	
	if(strcmp(Kommando,STOP)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i]='\0';
		pthread_mutex_lock(&I2C_Konflock);
		pthread_mutex_lock(&SPI_Konflock);


		printf("MainProzess:--->Gestoppt! \n");
		return 0;
	}
	
	if(strncmp(Kommando,SPEICHERN_STOPPEN,17)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
			Flag_Speichern=0;
			
			printf("Speicherflag wurde auf %d gesetzt! \n",Flag_Speichern);

			return 4;
	}
	if(strncmp(Kommando,SPEICHERN,8)==0)
	{
			while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
			 time_t t;
			struct tm *ts;
			time(&t);
			ts = localtime(&t);
       
			// Es wird der Dateipfad /media/usb/aktuelles_Datum/Uhrzeit.txt 	verwendet zum Erstellen des Pfades
			
			sprintf(Pfad,"/media/usb/%d%d%d/",ts->tm_mday,(ts->tm_mon+1),(ts->tm_year)-100);
			mkdir(Pfad,0777);
			sprintf(File_Name,"%d:%dUhr",ts->tm_hour,ts->tm_min);
			strcat(Pfad,File_Name);	
			printf("Der Pfad in EmpfangeKommando wird: %s \n",Pfad);
			Flag_Speichern = 1;
			printf("Speicherflag wurde auf %d gesetzt! \n",Flag_Speichern);
			return 3;
	}
	
	/*if(strncmp(Kommando,MESSUNG_FERTIG,8)==0)
	{
		printf("Done!\n");	
		SendeKommando(BEENDEN,"Beende mich!");	
		printf("Kommando habe ich gesendet?\n");
		sleep(1);
		done();
		exit(1);		
	}
	*/
	
	if(strcmp(Kommando,NEUE_WERTE_A)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';	
	//	printf("Zwischenbuffer nach EmpfangeKommando:	%s \n",ZwischenBuffer);
		 SPI_syncWerte(&WandlerA,ZwischenBuffer);		
		return 5;
	}
	if(strcmp(Kommando,NEUE_WERTE_B)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';	
		SPI_syncWerte(&WandlerB,ZwischenBuffer);		
		return 6;
	}
	if(strcmp(Kommando,NEUE_WERTE_I2C) == 0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';	
       // printf(" Habe neue Werte für I2C erhalten!\n %s \n",ZwischenBuffer);
		int Tmp_Nr;
		int Wert;
			
		struct I2C_Liste *Zeiger;
		Zeiger = I2C_Liste_Anfang;
		Zeiger=Zeiger->next;
		if(Zeiger->next == 0)
		{
			return 0;
		}
		
		ptr = strtok(ZwischenBuffer,delimiter);
		
		while(ptr != NULL)
		{
            Tmp_Nr = 0;
			Tmp_Nr = atoi(ptr);
			ptr=strtok(NULL,delimiter);


                if( ptr!=NULL)
                {
                    Wert = atoi(ptr);
                }
            ptr=strtok(NULL,delimiter);
			
			while(Zeiger->Nummer != Tmp_Nr)
			{
                if((Zeiger=Zeiger->next)!=NULL)
                {}
                else{
                    return 0;
                }

			}
			
			Zeiger->AktuellerWert = Wert;
	//		printf("Zeiger->Nummer: %d 	Zeiger->Wert: %d\n",Zeiger->Nummer,Zeiger->AktuellerWert);
			Zeiger = I2C_Liste_Anfang;
			
		}
		
		
		return 7;
	}
	if(strncmp(Kommando,SPI_SYNC,8)==0)
	{

		char x;
		i=0;
		
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i] = '\0';
		printf("ZwischenBuffer nach SYNC:	%s \n",ZwischenBuffer);	
		printf("ZwBuf[0] %d	",ZwischenBuffer[0]);
		printf("ZwBuf[1] %d	",ZwischenBuffer[1]);
		printf("ZwBuf[2] %d	",ZwischenBuffer[2]);
	
	
        if((ZwischenBuffer[0] != 65) | (ZwischenBuffer[0] != 66) | (ZwischenBuffer[0] == 67) | (ZwischenBuffer[0] != 68) )
		{
			x=ZwischenBuffer[0];
		}
		else
		{			
			x=ZwischenBuffer[1];		
		}
		
		
		printf("ZwischenBuffer[0] ist genau das hier: -> %c \n",x);
		switch(x)
		{
			case 'A': printf(" Ich schreibe das in Wandler A! \n"); SPI_kleinerSync(&WandlerA,ZwischenBuffer); break;
			case 'B': printf(" Ich schreibe das in Wandler B! \n");	SPI_kleinerSync(&WandlerB,ZwischenBuffer); break;
		}
		
		
		
		return 8;
	}
	if(strcmp(Kommando,I2C_NEUERWANDLER)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';
	//	printf("ZwischenBuffer ist : %s \n",ZwischenBuffer);
		if(I2C_Liste_Anfang == NULL)
		{
			ErstelleListe_I2C();
		}
		ptr = strtok(ZwischenBuffer, delimiter);
		char SendeBuf[3];
	
			int len= atoi(ptr);
		ptr = strtok(NULL,delimiter);
			int Read= atoi(ptr);
		ptr = strtok(NULL,delimiter);
			int SlaveAdress=atoi(ptr);
		ptr = strtok(NULL,delimiter);
			SendeBuf[0]=atoi(ptr);
		ptr = strtok(NULL,delimiter);
		
			SendeBuf[1]=atoi(ptr);
			ptr = strtok(NULL,delimiter);
			SendeBuf[2]=atoi(ptr);
			ptr = strtok(NULL,delimiter);

		int FrequenzModus=atoi(ptr);
		ptr = strtok(NULL,delimiter);
		int FunktionsNummer = atoi(ptr);
		
		I2C_neuerWandler(Read,SlaveAdress,len,SendeBuf,FrequenzModus,FunktionsNummer);
		return 9;		
	}
	if(strcmp(Kommando,I2C_NEUEWERTE)==0)
	{
		
		// Es werden neue Werte entgegengenommen
		// -> Die Werte werden in Arrays zwischengespeichert
		//---> i2c_lock
		// -> Es werden die neuen Werte entsprechend in die Liste geschrieben
		//---> i2c_unlock
		
		//printf("----Alte Liste----- \n");
		//	PrintListe_I2C();
		
		int AnzahlWerte;
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';
		
		printf("Ich habe das hier erhalten: %s \n",ZwischenBuffer);
		struct I2C_Liste *Zeiger;
		Zeiger = I2C_Liste_Anfang;
		Zeiger=Zeiger->next;
		if(Zeiger->next == 0)
		{
			return 0;
		}
		
		ptr = strtok(ZwischenBuffer, delimiter);
		//ptr = strtok(NULL,delimiter);
		AnzahlWerte =  atoi(ptr);
		ptr = strtok(NULL,delimiter);
		
		int WandlerNummer[AnzahlWerte];
		int Position[AnzahlWerte];
		int Wert[AnzahlWerte];
		
		int i;
		
		for(i = 0; i < AnzahlWerte; i++)
		{
			
				WandlerNummer[i]=atoi(ptr);
		//		printf("WandlerNummer: %d\n",WandlerNummer[i]);
				ptr = strtok(NULL,delimiter);
				Position[i]=atoi(ptr);
			//	printf("	Position: %d\n",Position[i]);

				ptr = strtok(NULL,delimiter);
				Wert[i]=atoi(ptr);
				printf("		Wert: %d\n",Wert[i]);

				ptr = strtok(NULL,delimiter);
		}
		
		pthread_mutex_lock(&I2C_Konflock);
			
		do{
			for(i=0;i < AnzahlWerte;i++)
			{
				if(WandlerNummer[i] == Zeiger->Nummer)
				{
			//		printf("		WandlerNummer ist gleich der Zeiger->Nummer! \n");
					switch(Position[i])
					{
						case 0: Zeiger->SendeBuffer[0] = Wert[i]; break;
						case 1: Zeiger->SendeBuffer[1] = Wert[i]; break;
						case 2: Zeiger->SendeBuffer[2] = Wert[i]; break;
					}
				
				}
			}
		Zeiger = Zeiger->next;
		}while(Zeiger != 0);




		pthread_mutex_unlock(&I2C_Konflock);
		
	//	printf("\n----Neue Liste----- \n");
	//	PrintListe_I2C();
		
			return 10;
	}
	
	if(strcmp(Kommando,SPI_DELAY)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';
		ptr = strtok(ZwischenBuffer,delimiter);
		Delay_SPI = (uint64_t)atoi(ptr);
		return 11;
	}
	if(strcmp(Kommando,I2C_DELAY)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';
		ptr = strtok(ZwischenBuffer,delimiter);
		Delay_I2C = (uint64_t)atoi(ptr);
		
		return 12;
	}
	if(strcmp(Kommando,I2C_EINZELWERT)==0)
	{
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';

#ifdef __arm__
		BCM2835_I2C_Einzelwert(ZwischenBuffer);
#endif		
		return 13;
	}
	if(strcmp(Kommando,I2C_BLOCK)==0)
	{
		pthread_mutex_lock(&I2C_Konflock);
		printf("Habe I2C gesperrt! \n");
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';
		return 14;
	}
	if(strcmp(Kommando,SPI_BLOCK)==0)
	{
		pthread_mutex_lock(&SPI_Konflock);
		printf("Habe SPI gesperrt! \n");
		while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';
		return 15;

	}	

    i=0;
    while(((ZwischenBuffer[i]=fgetc(fd_empfangen)) != ';' ) | (i>100));
	{ 
        i++;
	}
    printf("Fehlerhafte Nachricht empfangen!\n");
	return 0;
}
int SPI_syncWerte(struct SPI_Wandler *DieserWandler, char *ZwischenBuffer)
{
		int i,AnzahlEmpfangenerWerte;
		i=0;
		AnzahlEmpfangenerWerte = 0;
		char delimiter[] = " ,;:";	
		
		char *ptr;
	/*	while((ZwischenBuffer[i++]=fgetc(fd_empfangen)) != ';');
		ZwischenBuffer[i-1] = '\0';	
	*/	
	
     //  printf("\n Der Zwischenbuffer: %s \n",ZwischenBuffer);
       // fflush(stdout);

		if((ZwischenBuffer[0] == 'A') | (ZwischenBuffer[0] == 'B'))
		{
			return 0;
		}
		
		
		for(i = 0; i < DieserWandler->AnzahlKanaele; i++)
		{
			if(DieserWandler->Kanal[i]>0)
			{
				DieserWandler->Kanal[i] = 1;
			}
		}	
	
		ptr = strtok(ZwischenBuffer, delimiter);
		i=0;
	
		int KanalNummer;
		while(ptr != NULL)	
		{
			KanalNummer = atoi(ptr);
			ptr = strtok(NULL,delimiter);
         //   printf("%s\n",ptr);
          //  printf("Iteration: %d    ->	KanalNummer:	%d  mit Wert %s \n",i++,KanalNummer,ptr);

             if(ptr == NULL)
             {
              //   printf("error in EmpfangeKommando()! PTR ist Null wo er es nicht sein sollte.\n");
                 return 0;
             }
        switch( KanalNummer)
        {          // Wert von Kanal:0 mit der WertNr: Kanal[0]-1
			case 0: 	DieserWandler->Werte[0][DieserWandler->Kanal[0]-1] = atoi(ptr); DieserWandler->Kanal[0]++; AnzahlEmpfangenerWerte++;	break;
			case 1:		DieserWandler->Werte[1][DieserWandler->Kanal[1]-1] = atoi(ptr); DieserWandler->Kanal[1]++; AnzahlEmpfangenerWerte++;	break;
			case 2:		DieserWandler->Werte[2][DieserWandler->Kanal[2]-1] = atoi(ptr); DieserWandler->Kanal[2]++; AnzahlEmpfangenerWerte++;	break;
			case 3:		DieserWandler->Werte[3][DieserWandler->Kanal[3]-1] = atoi(ptr); DieserWandler->Kanal[3]++; AnzahlEmpfangenerWerte++;	break;
			case 4:		DieserWandler->Werte[4][DieserWandler->Kanal[4]-1] = atoi(ptr); DieserWandler->Kanal[4]++; AnzahlEmpfangenerWerte++;	break;
			case 5:		DieserWandler->Werte[5][DieserWandler->Kanal[5]-1] = atoi(ptr); DieserWandler->Kanal[5]++; AnzahlEmpfangenerWerte++;	break;
			case 6:		DieserWandler->Werte[6][DieserWandler->Kanal[6]-1] = atoi(ptr); DieserWandler->Kanal[6]++; AnzahlEmpfangenerWerte++;	break;
			case 7:		DieserWandler->Werte[7][DieserWandler->Kanal[7]-1] = atoi(ptr); DieserWandler->Kanal[7]++; AnzahlEmpfangenerWerte++;	break;
			case 8: 	DieserWandler->Werte[8][DieserWandler->Kanal[8]-1] = atoi(ptr); DieserWandler->Kanal[8]++; AnzahlEmpfangenerWerte++;	break;
			case 9:		DieserWandler->Werte[9][DieserWandler->Kanal[9]-1] = atoi(ptr); DieserWandler->Kanal[9]++; AnzahlEmpfangenerWerte++;	break;
			case 10:	DieserWandler->Werte[10][DieserWandler->Kanal[10]-1] = atoi(ptr); DieserWandler->Kanal[10]++; AnzahlEmpfangenerWerte++;	break;
			case 11:	DieserWandler->Werte[11][DieserWandler->Kanal[11]-1] = atoi(ptr); DieserWandler->Kanal[11]++; AnzahlEmpfangenerWerte++;	break;
			case 12:	DieserWandler->Werte[12][DieserWandler->Kanal[12]-1] = atoi(ptr); DieserWandler->Kanal[12]++; AnzahlEmpfangenerWerte++;	break;
			case 13:	DieserWandler->Werte[13][DieserWandler->Kanal[13]-1] = atoi(ptr); DieserWandler->Kanal[13]++; AnzahlEmpfangenerWerte++;	break;
			case 14:	DieserWandler->Werte[14][DieserWandler->Kanal[14]-1] = atoi(ptr); DieserWandler->Kanal[14]++; AnzahlEmpfangenerWerte++;	break;
			case 15:	DieserWandler->Werte[15][DieserWandler->Kanal[15]-1] = atoi(ptr); DieserWandler->Kanal[15]++; AnzahlEmpfangenerWerte++;	break;
			default: break;
		}


        // printf("KanalNummer %d 	Der Wert:	%d  und i:	%d\n",KanalNummer,DieserWandler->Werte[0][i-1],i);
			ptr = strtok(NULL,delimiter);	
			
			
		}
			return AnzahlEmpfangenerWerte;
}

void SPI_kleinerSync(struct SPI_Wandler* DieserWandler,char *ZwischenBuffer)
{
	DieserWandler->Aktiv = 1;
	char delimiter[] = " ,;:";	
	char *ptr;

	ptr = strtok(ZwischenBuffer, delimiter);

	ptr = strtok(NULL,delimiter);	
	
	//printf("AD? -------->%s\n",ptr);
	DieserWandler->AD = atoi(ptr);
	
	
	ptr = strtok(NULL,delimiter);	
	
	DieserWandler->AnzahlMesswerte=atoi(ptr);
	//printf("Anzahl Messwerte? -------->%s\n",ptr);
	ptr = strtok(NULL,delimiter);	

	DieserWandler->BitOrder=atoi(ptr);
	ptr = strtok(NULL,delimiter);	

	DieserWandler->Frequenz=atoi(ptr);
	ptr = strtok(NULL,delimiter);	

	DieserWandler->Polaritaet=atoi(ptr);
	ptr = strtok(NULL,delimiter);	
	
	DieserWandler->DataMode=atoi(ptr);
	ptr = strtok(NULL,delimiter);	
	
	DieserWandler->AnzahlKanaele =atoi(ptr);
	ptr = strtok(NULL,delimiter);	
	
	DieserWandler->Wortlaenge=atoi(ptr);
	ptr = strtok(NULL,delimiter);
	
	DieserWandler->FunktionZumWandeln=atoi(ptr);
	ptr = strtok(NULL,delimiter);
	Funktion_zum_Umwandeln_SPI(DieserWandler,DieserWandler->FunktionZumWandeln);
	
		int i = 0;
		int j = 0;
		
		for(i=0;i<DieserWandler->AnzahlKanaele;i++)
		{
			if( atoi(ptr) == 1)
			{
				DieserWandler->Kanal[i] = 1;
			}
			ptr = strtok(NULL,delimiter);
			
		}
		
		for(j = 0; j < DieserWandler->AnzahlKanaele;j++)
		{
			
			if(DieserWandler->Kanal[j] == 1)
			{
				for( i = 0; i < DieserWandler->Wortlaenge; i++)
				{
					DieserWandler->mosi[j][i] = (uint8_t)atoi(ptr);
					//printf(" DieserWandler->mosi[j][i]	%s \n",ptr);
					ptr = strtok(NULL,delimiter);
					
				}
			}
		}	
		
		
		
		
		
		SPI_printWandler(DieserWandler);
}
	
void TerminalOszi(int x)
{
	
	//printf("\n");
	//printf("%c[2K",27);
	//printf("\033[2J");
	printf("\n");
	int i,j;
	j=0;
	for(i=0;i<1023;i++)
	{
		j++;
		if(j == 10)
		{
			printf("-");
			j=0;
		}
		if(i == x)
		{
			printf("|");
		}
	}
	
}
int SPI_erstelleWandler(struct SPI_Wandler* DieserWandler,int AD,int AnzahlMesswerte,int BitOrder,int Frequenz,int Polaritaet,int DataMode,int AnzahlKanaele,int *AktiveKanaele,int Wortlaenge,int FunktionsNummer)//uint8_t *mosi[])
{
    if((AD == 0) | (AD==1))
	{
		DieserWandler->AD = AD;
	}
	else{
		printf("Wandler wurde nicht auf AD oder DA festgelegt. Setze ihn auf DA Wandler, da geht nichts kaputt.... hoffentlich!\n");
		DieserWandler->AD = 0;
	}
	
	if(AnzahlMesswerte > 1)
	{
			DieserWandler->AnzahlMesswerte = AnzahlMesswerte;
	}
	else{
        DieserWandler->AnzahlMesswerte = 1;
        printf("Kein gültiger Wert für die Anzahl Messwerte. Setze sie auf 1 fest. \n");
	}
    if((BitOrder < 0) | (BitOrder >4))
	{
		printf("Bitorder muss zwischen 0 und 4 liegen. Ich kann mir da nichts ausdenken. Ich kann so nicht arbeiten. \n");
		return 0;
	}
	else{
		DieserWandler->BitOrder = BitOrder;
	}
    if((Frequenz < 0) | (Frequenz > 32000000))
	{
		printf("Frequenz ist mir zu hoch oder zu klein. Gehe nur bis 32kk Hz, danach bringt es der Pi nicht mehr.\n");
		return 0;
	}
	else{
		DieserWandler->Frequenz = Frequenz;
	}
    if((Polaritaet == 0) | (Polaritaet == 1))
	{
			DieserWandler->Polaritaet = Polaritaet;
	}
	else{
		printf("Polarität wurde falsch angegeben.Muss 1 oder 0 sein. Kann mir da nichts ausdenken. Tschüß! \n");
		return 0;
	}
    if((DataMode > 4) | (DataMode < 0))
	{
		printf("DataMode wurde falsch angegeben. Muss zwischen 0 und 4 sein. Kann mir da nichts ausdenken. Tschüß! \n");
	}
	else{
		DieserWandler->DataMode = DataMode;
	}
	if(Wortlaenge < 0)
	{
		printf("Die Wortlänge ist kleiner 0. Ich glaube, das ist so nicht korrekt? Tschüß! \n");
		return 0;
	}
	else{
		DieserWandler->Wortlaenge = Wortlaenge;
	}
	if(AnzahlKanaele < 0)
	{
			printf("Die Anzahl der Kanaele ist kleiner als 0. Soll das so sein? Tschüß!\n");
			return 0;
	}
	else{
		DieserWandler->AnzahlKanaele = AnzahlKanaele;

        int i = 0;
		int j = 0;

        for(j = 0; j < AnzahlKanaele;j++)
		{
			if(AktiveKanaele[j] == 1)
			{
				for( i = 0; i < Wortlaenge; i++)
				{
						DieserWandler->Kanal[j] = 1;
                //		DieserWandler->mosi[j][i] = testmosi[j][i];
				}
			}
		}
	}



    printf("Wandler wurde erfolgreich erstellt!\n");
    printf("Ich hoffe Du hast an mosi[][] gedacht?\n");
	
	DieserWandler->FunktionZumWandeln = FunktionsNummer;
	DieserWandler->Aktiv = 1;
	return 1;	
}

int SPI_syncWandler(struct SPI_Wandler* DieserWandler,char x)
{
	if((x == 'A')| (x=='B') )
	{	
		// Hier wird der Befehl zum SYNC des SPI_Wandlers X zusammen mit der übertragenen Größe des SPI_Wandlers X in die Pipe geschrieben
		char ArrayString[400];
		ArrayString[0] = '\0';
		char tmp[100];
		int i = 0;
		int j = 0;
		int y;
		
		tmp[0] = x;
		tmp[1] = ',';
		tmp[2] = '\0';
		DieserWandler->Aktiv = 1;

		strcat(ArrayString,tmp); 
		
		printf(" x und Name des Wandlers	->%s\n",ArrayString);
		sprintf(tmp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,",DieserWandler->AD,DieserWandler->AnzahlMesswerte,DieserWandler->BitOrder,DieserWandler->Frequenz,DieserWandler->Polaritaet,
		DieserWandler->DataMode,DieserWandler->AnzahlKanaele,DieserWandler->Wortlaenge,DieserWandler->FunktionZumWandeln);
		strcat(ArrayString,tmp);
		
		printf("Die Nachricht ist nach dem massen-sprintf: 	--->%s \n",ArrayString);
		
		for(y = 0;y < DieserWandler->AnzahlKanaele;y++)
		{
			if(DieserWandler->Kanal[y] == 1)
			{
				strcat(ArrayString,"1,");
			}
			else
			{
				strcat(ArrayString,"0,");
			}
			
		}
	
		
		for(j = 0; j < DieserWandler->AnzahlKanaele;j++)
		{
			if(DieserWandler->Kanal[j] == 1)
			{
				for( i = 0; i < DieserWandler->Wortlaenge; i++)
				{
						sprintf(tmp,"%d,",DieserWandler->mosi[j][i]);
						printf(" tmp nach %d ---> %s \n",i,tmp);
						strcat(ArrayString,tmp);
						tmp[0]='\0';
				}
			}
		}
		
        //printf("Die Nachricht ist: 	%s \n",ArrayString);
		SendeKommando(SPI_SYNC,ArrayString);	
		
		
	}

	return 1;
}

void SPI_printWandler(struct SPI_Wandler *DieserWandler)
{
		printf("\n");
		printf("DieserWandler->aktiv: %d\n",DieserWandler->Aktiv);
		printf("DieserWandler->AD: %d\n",DieserWandler->AD);	
		printf("DieserWandler->AnzahlKanaele: %d \n",DieserWandler->AnzahlKanaele);
		printf("DieserWandler->AnzahlMesswerte: %d\n",DieserWandler->AnzahlMesswerte);
		printf("DieserWandler->Polaritaet: %d\n",DieserWandler->Polaritaet);
		printf("DieserWandler->BitOrder: %d\n",DieserWandler->BitOrder);
		printf("DieserWandler->Frequenz: %d\n",DieserWandler->Frequenz);
		printf("DieserWandler->DataMode: %d\n",DieserWandler->DataMode);
		printf("DieserWandler->FunktionsNummer: %d \n",DieserWandler->FunktionZumWandeln);
		
		int i;
		for(i = 0;i < DieserWandler->AnzahlKanaele;i++)
		{
			printf("DieserWandler->Kanal[%d]: %d\n",i,DieserWandler->Kanal[i]);
		}
		
		
		printf("\n");
}

int fd_set_blocking(int fd, int blocking) {
    /* Save the current flags */
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return 0;

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags) != -1;
}


#ifdef __arm__
void *BCM2835_SPI(void *ADW)
{
	struct SPI_Wandler *DieserWandler;
	FILE *fp_schreiben;
	FILE *speicherPipe;
	
		speicherPipe = fdopen(fd[1],"w");
	
		if((fp_schreiben = fdopen(fd[1],"w")) == NULL)
		{
			printf("SPI-Thread meldet: Probleme beim Öffnen von Pipe fd[1] ");
			perror(NULL);
		}
		
	printf("SPI-Thread:-->gehe jetzt in die While! \n");
	while(1)
	{	
		
			if(Delay_SPI > 0 )
			{
				bcm2835_delayMicroseconds(Delay_SPI);
			}	
			pthread_mutex_lock(&SPI_Konflock);
				
				bcm2835_spi_begin();
				if(WandlerA.Aktiv)
				{
					BCM2835_SPI_Konfiguriation(&WandlerA);
					BCM2835_SPI_Lesen(&WandlerA,speicherPipe,0);
				}
				bcm2835_spi_end();	// beim Testen ging sonst die zweite Konfiguration nicht
				
				bcm2835_spi_begin();	
				if(WandlerB.Aktiv)
				{
					BCM2835_SPI_Konfiguriation(&WandlerB);
					BCM2835_SPI_Lesen(&WandlerB,speicherPipe,1);	
				}
				bcm2835_spi_end();
			pthread_mutex_unlock(&SPI_Konflock);
		
	}
		fclose(fp_schreiben);
	    printf("BCM2835_SPI ist fertig!\n");
}

int BCM2835_SPI_Konfiguriation(struct SPI_Wandler* DieserWandler)
{
	
	if(DieserWandler->Frequenz == 0)	//Konfiguration wird entsprechend des synchronisierten SPI_Wandlers vorgenommen
	{
		printf("begin geschafft\n");
		bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
		printf("Bitorder geschafft \n");
		bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
		printf("Data Mode geschafft \n");
		bcm2835_spi_setClockDivider(32);
		printf("Clock Divider geschafft\n");
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		printf("Chip-Select geschafft \n");
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
		printf("Polarity geschafft \n");
	}
	else{	
		switch(DieserWandler->BitOrder)
		{
		case 0:	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); break;
		case 1: bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST); break;
		default: printf("BitOrder wurde nicht gesetzt -> Exit! \n"); return 0;
		}
		switch(DieserWandler->DataMode)
		{
			case 0: bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); break;
			case 1: bcm2835_spi_setDataMode(BCM2835_SPI_MODE1); break;
			case 2: bcm2835_spi_setDataMode(BCM2835_SPI_MODE2); break;
			case 3: bcm2835_spi_setDataMode(BCM2835_SPI_MODE3); break;
			default: printf("DataMode wurde nicht gesetzt\n Exit! \n"); return 0;
 		}
		
		
		if(DieserWandler->Frequenz > 0 | DieserWandler->Frequenz < 120000000)
		{
			bcm2835_spi_setClockDivider((400000000) / DieserWandler->Frequenz );	

		}
		else
		{
			printf("Frequenz ist 0 oder größer als 120000000 \n");
		}
		
				switch(DieserWandler->ChipSelect)
		{
			
			case 0:	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);	switch(DieserWandler->Polaritaet)
																{
																case 0:	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW); break;
																case 1: bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH); break;
																default: printf("Polarität wurde nicht richtig angegeben! \n Exit! \n"); return 0;
																} break;
			case 1:	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
																switch(DieserWandler->Polaritaet)
																{
																case 0:	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW); break;
																case 1: bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, HIGH); break;
																default: printf("Polarität wurde nicht richtig angegeben! \n Exit! \n"); return 0;
																} break;
			default: printf("Chip Select wurde nicht gesetzt! \n Exit!\n"); return 0;
			
		}

	}
	
	return 1;
}
int BCM2835_SPI_Lesen(struct SPI_Wandler* DieserWandler,FILE *Pipe,int Ident)
{

	int i;
	int j;
	
		
	/* Hier findet die Messung statt. Es werden AnzahlMesswerte durchläufe genommen -> Bei 8 Kanäle sind es 
	 * insgesamt 8*AnzahlMesswerte an eigentlichen Messwerten. Diese werden in die Pipe zum Speicherthread geschrieben. */
	  				
	 // 	printf("Bin im ADC Thread nach der Konfiguration von WandlerA\n");

		for(i=0; i <DieserWandler->AnzahlMesswerte; i++)
        {
		
			for(j=0; j < DieserWandler->AnzahlKanaele;j++)
			{
			
				switch(DieserWandler->Kanal[j])
				{
				case 1	:	
							bcm2835_spi_transfernb(DieserWandler->mosi[j],DieserWandler->miso+1,DieserWandler->Wortlaenge); 
	
							if(DieserWandler->AD==0)
							{		
								
								uint8_t tmp;
		
								switch(Ident)
								{
									case 0: 	DieserWandler->miso[0] = 0xA0 + (uint8_t)j; break;
									case 1:		DieserWandler->miso[0] = 0xB0 + (uint8_t)j; break;
					
								}
							
								fwrite(&DieserWandler->miso,1,DieserWandler->Wortlaenge+1,Pipe);
				
							}
							break;
				default: break;
				}
			
			}
        }
 		
	
}

int BCM2835_I2C_Einzelwert(char *ZwischenBuffer)
{
	char delimiter[] = " ,;:";	
	char *ptr;
	char buf[10],data;
	int i=0;
	int read;
	int buf_laenge;
	int FrequenzModus;
	uint8_t SlaveAdress;
	uint8_t SlaveRegister;
	
	// Reihenfolge: int Einzelwert( FrequenzModus,read, Slaveadresse, anzahl Bufs, buf[]);
	// Hier geht der Spaß wieder los
	
	ptr = strtok(ZwischenBuffer,delimiter);	
	FrequenzModus=atoi(ptr);
	ptr = strtok(NULL,delimiter);
	read = atoi(ptr);
	ptr = strtok(NULL,delimiter);
	SlaveAdress = atoi(ptr);
	ptr = strtok(NULL,delimiter);
	buf_laenge = atoi(ptr);
	
	i=0;
	while(i < buf_laenge)
	{
		ptr = strtok(NULL,delimiter);
		buf[i] = atoi(ptr);
		i++;
	}
	
	pthread_mutex_lock(&I2C_Konflock);
	bcm2835_i2c_begin();
	
		bcm2835_i2c_setSlaveAddress(SlaveAdress);
		switch(FrequenzModus)
		{
				case 0:bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500); break;//bcm2835_i2c_set_baudrate(100000); break;
				case 1:bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626 ); break;// bcm2835_i2c_set_baudrate(3993610); break;
				case 2:bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_150 ); break;//bcm2835_i2c_set_baudrate(1666000000); break;
			default: return 0; 
		}
		
	
	//	printf("I2C Debug:	buf\n");
		
		if(read == 0)
		{
			data = bcm2835_i2c_write(buf,buf_laenge);
			printf("Ich habe in I2C geschrieben: Nummer: %d		buf[0]: %d  buf[1]: %d\n",buf_laenge,buf[0],buf[1]);
		}
		else{
				data = bcm2835_i2c_read(buf,buf_laenge);
				int i;
				for(i=0;i<buf_laenge;i++)
				{
		//			Messwerte[i+1] = buf[0];
				}		
	
		//		write(fd[1],&Messwerte,Zeiger->len+1);
			}
			
	bcm2835_i2c_end();
	pthread_mutex_unlock(&I2C_Konflock);
	
	return 1;
}
void *BCM2835_I2C()
{
	printf("BCM2835_I2C() startet... \n");
	
	int i,k;	
	uint8_t data;
	uint8_t Messwerte[10];
	struct I2C_Liste *Zeiger;
	char buf[12];
	FILE *fp_schreiben;
	if((fp_schreiben = fdopen(fd[1],"w")) == NULL)
	{
		printf("I2C-Thread meldet: ");
		perror(NULL);
	}
	
	//printf("I2C Debug i2c_begin geschafft \n");
	printf("I2C-Thread:-->gehe jetzt in die While! \n");
	pthread_mutex_unlock(&I2C_Konflock);
while(1)
{	
	
	if(Delay_I2C > 0 )
	{
		bcm2835_delayMicroseconds(Delay_I2C);
	}	
	pthread_mutex_lock(&I2C_Konflock);
	bcm2835_i2c_begin();
	
	
	if(I2C_Liste_Anfang != NULL)
	{	
		Zeiger = I2C_Liste_Anfang;
		Zeiger=Zeiger->next;
	}
	else
	{
		
		return;
	}
	do
	{
		Messwerte[0] = 0xC0 + (uint8_t)Zeiger->Nummer;
		
	 if(!Zeiger->Aktiv)
	 {
		bcm2835_i2c_setSlaveAddress(Zeiger->SlaveAdress);
		switch(Zeiger->FrequenzModus)
		{
				case 0:bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500); break;//bcm2835_i2c_set_baudrate(100000); break;
				case 1:bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626 ); break;// bcm2835_i2c_set_baudrate(3993610); break;
				case 2:bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_150 ); break;//bcm2835_i2c_set_baudrate(1666000000); break;
			default: bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);  break;//bcm2835_i2c_set_baudrate(100000); break; 
		}
		
		buf[0] = Zeiger->SendeBuffer[0];
		buf[1] = Zeiger->SendeBuffer[1];
		buf[2] = Zeiger->SendeBuffer[2];
	//	printf("I2C Debug:	buf\n");
		
		if(Zeiger->Read == 0)
		{
			data = bcm2835_i2c_write(buf,Zeiger->len);	
		//	printf("Ich habe in I2C geschrieben: Nummer: %d Länge: %d	buf[0]: %d  buf[1]: %d\n",Zeiger->len,Zeiger->Nummer,buf[0],buf[1]);
		}
		else{
				data = bcm2835_i2c_read(buf,Zeiger->len);
						
				int i;
				for(i=0;i<Zeiger->len;i++)
				{
					Messwerte[i+1] = buf[i];
				}				
				
	//		printf("I2C Debug: 	Messwerte[0]: %d\n",Messwerte[0]);
	//		printf("I2C Debug:	Messwerte[1]: %d\n",Messwerte[1]);
	//		printf("I2C Debug:	Messwerte[2]: %d\n",Messwerte[2]);
	//		printf("I2C_Debug: 	Zeiger->len+1: %d\n",Zeiger->len+1);
		
		
				if(data == BCM2835_I2C_REASON_OK)
				{
					 write(fd[1],&Messwerte,Zeiger->len+1);
				}

		//	printf("Wandler mit der Nummer %d liest mit der Länge %d folgendes Ergebnis:	%d %d\n",Zeiger->Nummer,Zeiger->len,buf[0],buf[1]);
			}
	
	//		printf("Ich habe in I2C gelesen... Nummer: %d 	Wert: %d \n",Zeiger->Nummer,data);		
	}
	Zeiger = Zeiger->next;
	
	}while(Zeiger != 0);

	bcm2835_i2c_end();
	pthread_mutex_unlock(&I2C_Konflock);
	
}


	printf(" I2C Thread Done \n");
}
void *LesenUndSenden()
{
	/* Ablauf des Threads
	 * 	- 	initialisieren
	 * 	-	Es wird gewartet bis Pipe geöffnet wird
	 * 
	 * 	-	Pipe wird gelesen 	->	Falls 'N' werden die Dateipfade geöffnet
	 * 	-						-> 	Falls '0' (oder EOF) werden die Datein gelesen und entsprechend der Buffer geöffnet
	 * 									-> Pipe gibt EOF zurück wenn es kein Write für diese Pipe gibt 
	 * 									-> muss also im Speicher-Thread geschlossen werden
	 * 							-> Falls 'A', 'B', 'I' werden die Daten entsprechend in deren Buffer geschrieben.
	 * 							wenn dieser mit einer bestimmten Anzahl Werten beschrieben ist, wird gesendet und Buffer geleert
	 * */
	
	uint8_t BuchstabeUndKanal[1];
	uint8_t Buchstabe;
	uint8_t Messwert_Kanal;
	struct I2C_Liste *Anfang;
	long int Nummer_Zeile;
	char c;
	int Temp_Buchstabe;
	int i=0;
	int Buf_Cnt_A, Buf_Cnt_B,Buf_Cnt_I;
	int Buf_Cnt_AA,Buf_Cnt_BB,Buf_Cnt_II;
	int Zeilencounter_A=0;
	int Zeilencounter_B=0;
	int Zeilencounter_I=0;
	Pipe_Flag = 1;
	Buf_Cnt_A = 0;
	Buf_Cnt_B = 0;
	Buf_Cnt_I = 0;
	Buf_Cnt_AA = 0;
	Buf_Cnt_BB = 0;
	Buf_Cnt_II = 0;
	
	// Diese Buffer sind kritisch : 	Falls nach dem ersten Start eine andere Wortlänge o.ä. genommen wird, kann das zum Absturz führen
	char BufferA[AnzahlZeilenZumSenden_A*20]; // Platz für Rohdaten (schlimmstenfalls)-> viel Overhead
	char BufferB[AnzahlZeilenZumSenden_B*20]; // Platz für Rohdaten (schlimmstenfalls)-> viel Overhead
	char BufferI2C[256*10]; // Irgendwie muss man hier die Anzahl an I2C Geräten festlegen
	BufferA[0] = '\0';
	BufferB[0] = '\0';
	BufferI2C[0] = '\0';
	
	char Lese_Buffer[210]; // Hier werden einzelne Werte entgegen genommen
	Nummer_Zeile = 0;
	FILE *fp_datei_spia;
	FILE *fp_datei_spib;
	FILE *fp_datei_i2c;
	FILE *Pipe_Lesen;
	FILE *fp;
	fpos_t pos;
	
		//Pipe wird gelesen und auf Kommandos gewartet
		
	while((Pipe_Lesen = fopen("/tmp/PipeStattFile","r")) == NULL); // Diese Pipe wird anstelle von Files verwendet wenn nicht gespeichert werden soll
	printf("Lese-Thread:	->Bin bis hinter die Pipe gekommen! \n");
	while(1)
	{
		
		while(Pipe_Flag == 1)
		{	
				Lese_Buffer[0] = fgetc(Pipe_Lesen);
				//printf("LesenUndSenden:	Read war erfolgreich: %c \n",Lese_Buffer[0]);
				
						switch(Lese_Buffer[0])
						{	
								default: break;								
								case 'C': 
										printf("Lesen Und Schreiben versucht Datein zu schließen!\n");
										Pipe_Flag = 1;
										fclose(fp_datei_spia);
										fclose(fp_datei_spib);
										fclose(fp_datei_i2c);
										printf("Lesen Und Schreiben hat die Datein geschlossen! \n");
								break;
										
								// N heißt neue Pfade für die Datein
								case 'N':	Pipe_Flag = 0;
											if(fp_datei_spia != NULL)
											{
											//	fclose(fp_datei_spia);
											//	fclose(fp_datei_spib);
											//	fclose(fp_datei_i2c);
											}
											i=0;
											while((Lese_Buffer[i++]=fgetc(Pipe_Lesen)) != ';');
											Lese_Buffer[i-1] = '\0';	
											
											printf("Der Lese-Dateiname SPIA: %s\n",Lese_Buffer);
											if(((fp_datei_spia = fopen(Lese_Buffer,"r")) == NULL))	// Es wird eine Datei mit dem aktuellen Datum und Uhrzeit _SPIA erstellt
											{
												printf("LesenUndSenden: Fehler beim Öffnen von Datei _SPIA ");
												perror("Error");
											}
											i=0;
											while((Lese_Buffer[i++]=fgetc(Pipe_Lesen)) != ';');
											Lese_Buffer[i-1] = '\0';	
											
											printf("Der Lese-Dateiname SPIB: %s\n",Lese_Buffer);
											if(((fp_datei_spib = fopen(Lese_Buffer,"r")) == NULL))	// Es wird eine Datei mit dem aktuellen Datum und Uhrzeit _SPIA erstellt
											{
												printf("LesenUndSenden: Fehler beim Öffnen von Datei _SPIB ");
												perror("Error");
											}
											
											i=0;
											while((Lese_Buffer[i++]=fgetc(Pipe_Lesen)) != ';');
											Lese_Buffer[i-1] = '\0';	
											printf("Der Lese-Dateiname I2C: %s\n",Lese_Buffer);
											if(((fp_datei_i2c = fopen(Lese_Buffer,"r")) == NULL))	// Es wird eine Datei mit dem aktuellen Datum und Uhrzeit _SPIA erstellt
											{
												printf("LesenUndSenden: Fehler beim Öffnen von Datei _I2C ");
												perror("Error");
											}
											
											printf("LesenUndSenden: Bin mit den Datein fertig! \n");
											
								break;
											
								
								case 'A':	LesenUndSenden_Pipe('A',Pipe_Lesen,BufferA,&Buf_Cnt_A,&Buf_Cnt_AA,AnzahlZeilenZumSenden_A);		break;
								case 'B':	LesenUndSenden_Pipe('B',Pipe_Lesen,BufferB,&Buf_Cnt_B,&Buf_Cnt_BB,AnzahlZeilenZumSenden_B);		break;
								case 'I':	LesenUndSenden_Pipe('I',Pipe_Lesen,BufferI2C,&Buf_Cnt_I,&Buf_Cnt_II,AnzahlZeilenZumSenden_I);		break;
						}
	//	}
	}
	// Pipe hat nichts interessantes neues
		
		while(Pipe_Flag == 0)
		{
		//	printf("LesenUndSenden: bin hier! \n");
			
			
			if(!(LesenUndSenden_File('A',AnzahlZeilenZumSenden_A,fp_datei_spia))) 
			{
				Pipe_Flag = 1;
				printf("Lesen und Senden:	Pipe_Flag auf 1 gesetzt!\n");
			}
		    if(!(LesenUndSenden_File('B',AnzahlZeilenZumSenden_B,fp_datei_spib))) 
		    {
				Pipe_Flag = 1;
				printf("Lesen und Senden:	Pipe_Flag auf 1 gesetzt!\n");
			}
			if(!(LesenUndSenden_File('I',AnzahlZeilenZumSenden_I,fp_datei_i2c))) 
			{
				Pipe_Flag = 1;
				printf("Lesen und Senden:	Pipe_Flag auf 1 gesetzt!\n");
			}
	
		}
			
	}

}
int LesenUndSenden_File(char Buchstabe,int AnzahlZeilen,FILE* fp_datei)
{
	char Buffer[AnzahlZeilen*30];
	int Buf_Cnt;
	int Temp_Buchstabe;
	Buf_Cnt=0;
	Buffer[0]='\0';
	
	int Zeilencounter=0;
	//printf("Bin für %c da!\n",Buchstabe);						// hier war -1
	while(((Temp_Buchstabe = fgetc(fp_datei)) != EOF) && (Buffer[Buf_Cnt-1] != '\n'))
	{
		
		
					if(!((Temp_Buchstabe == ',') && (Buffer[Buf_Cnt] == ',')))
					{
						Buffer[Buf_Cnt++]=Temp_Buchstabe;
					}
					
					
				//	printf("%c",Temp_Buchstabe);
					if(Buf_Cnt > AnzahlZeilen*30)
					{
						Buf_Cnt = 0;
					}
					
						if(Buffer[Buf_Cnt-1] == 'x')
						{
							return 2;
							printf("Pipe_Flag wurde auf 1 gesetzt!\n");
						}		
					
				
						
					if(Buffer[Buf_Cnt-1] == '\n')
					{
						Buffer[Buf_Cnt-1] = ',';
					//	printf("LesenUndSenden: Zeile! %d\n",Zeilencounter);
						Zeilencounter++;	
					}
			
				
					if(Zeilencounter == AnzahlZeilen)
					{
					//	printf(" %s \n",Buffer);
					
						Buffer[Buf_Cnt] = '\0';
						switch(Buchstabe)
						{
							case 'A': SendeKommando(NEUE_WERTE_A,Buffer); break;
							case 'B': SendeKommando(NEUE_WERTE_B,Buffer); break;
							case 'I': SendeKommando(NEUE_WERTE_I2C,Buffer); break;
						 }	
				
						Buffer[0] = '\0';
					//	printf("LesenUndSenden: --->Gesendet! \n");
						Buf_Cnt=0;
						Zeilencounter=0;
					}			
	}
		return 1;
}
int LesenUndSenden_Pipe(char Buchstabe,FILE *Pipe,char *Buffer,int *Buffer_Zaehler,int *Zeilen_Zaehler,int AnzahlZeilenZumSenden)
{

	while((Buffer[(*Buffer_Zaehler)++]=fgetc(Pipe)) != ';');

		Buffer[(*Buffer_Zaehler)-1] = ',';

	
	
	//printf(" Gelesen:	%s %d\n",Lese_Buffer,i);
	
	if(( (*Zeilen_Zaehler)) < AnzahlZeilenZumSenden+1)
	{ 
	//strcat(Buffer,Lese_Buffer);
	//	printf("%s \n",Buffer);
		(*Zeilen_Zaehler)++;
	}
	else
	{
		Buffer[*Buffer_Zaehler] = '\0';
		switch(Buchstabe)
		{		
			case 'A':	SendeKommando(NEUE_WERTE_A,Buffer);	break;
			case 'B':	SendeKommando(NEUE_WERTE_B,Buffer);	break;
			case 'I':	SendeKommando(NEUE_WERTE_I2C,Buffer); break;
		
//		printf("%s \n",BufferA);	
		}
		*Buffer_Zaehler= 0;
		*Zeilen_Zaehler=0;
		Buffer[0] = '\0';
	}
	return 1;
	
}

void *SpeichernDerWerte()
{
	// Dieser Thread nimmt die Datenwerte der Sammel-Threads auf und speichert sie in 3 Datein
	// ...bestenfalls auf einem USB Stick
	
	// Hier wäre der richtige Ort um die Umwandlung der Werte einzufügen
	// Die Rohdaten kosten nämlich ziemlich viel Rechenleistung beim Verschieben
	
	/*	Dieser Thread könnte eine Beeinträchtigung der Leistung sein. 
	 *  Um einen Messwert zu fetchen müssen 2 read befehle ausgeführt werden
	 *  -> Zeitverschwendung?
	 *  Außerdem muss die Länge des I2C-Wandlers gefetched werden, für den ein Messwert angekommen ist
	 * */
	 
	struct I2C_Liste *Zeiger;
	int (*WertUmwandeln)(uint8_t *miso, int AnzahlWoerter);
	int i,j,q;
	int Nummer_Wert=0;
	int Messwert_gewandelt;
	char Pipe_Buchstabe;
	int Kanal_Nummer;
	int Flag_File=0;
	
	FILE *fp_datei_spia = NULL;
	FILE *fp_datei_spib = NULL;
	FILE *fp_datei_i2c = NULL;
	FILE *fp_temp;
	FILE *fp_lesen;
	FILE *Pipe_Schreiben;
	
	unlink("/tmp/PipeStattFile");
	if(mkfifo("/tmp/PipeStattFile",0666))
	{
		perror(NULL);
	}
	//printf("Speicher-Thread:Bin bis hinter die Pipe gekommen!\n");
	uint8_t Messwert[10];
	uint8_t BuchstabeUndKanal[1];
	uint8_t Buchstabe;
	uint8_t Messwert_Kanal;
	uint8_t Messwert_Laenge;
	char buf[12];
	char Messwert_Buffer[30];
	Messwert_Buffer[0] = '\0';
	
	char Pfad_SPIA[strlen(Pfad)+40];	// Hier wird _SPI
	char Pfad_SPIB[strlen(Pfad)+40];	// Hier wird _SPI
	char Pfad_I2C[strlen(Pfad)+40]; // und hier _I2C angehangen
	
	if((fp_lesen = fdopen(fd[0],"r")) == NULL)	//Dieser Pointer liest Daten aus der Werte-Pipe
	{
		printf("Speichern der Werte meldet: ");
		perror(NULL);
	}

	if((Pipe_Schreiben = fopen("/tmp/PipeStattFile","w")) == NULL) // Diese Pipe wird anstelle von Files verwendet wenn nicht gespeichert werden soll
	{
		printf("Speichern der Werte meldet: ");
		perror(NULL);
	}
	
	//printf(" SpeichernUndSenden: Flag_Speichern = %d\n",Flag_Speichern);
	
while(1)	// Speicher Hauptschleife
{		
	
	if((fp_datei_spia == NULL)	&& (Flag_Speichern==1)) // Die Datei wird nur erstellt, falls der Dateipointer auf 0 zeigt und Speicherflag == 1
	{
		usleep(10000);
		Flag_File =1;
		printf("SchreibenUndSenden:	 Speicherflag ist bei mir angekommen! \n");
	//	Pfad_SPIA[0]= '\0';
	//	Pfad_SPIB[0]= '\0';
	//	Pfad_I2C[0] ='\0';
	
		printf("Bereite Pfade vor! \n");
		printf(" Der Pfad ist: %s \n",Pfad);
		
		strcpy(Pfad_SPIA,Pfad);
		strcpy(Pfad_SPIB,Pfad);
		strcpy(Pfad_I2C,Pfad);
		
		printf("A Pfad nach strcpy: %s \n",Pfad_SPIA);
		strcat(Pfad_SPIA,"_SPIA");
		printf("A Pfad nach strcat: %s \n",Pfad_SPIA);
		strcpy(Pfad_SPIB,Pfad);
		printf("B Pfad nach strcpy: %s \n",Pfad_SPIB);
		strcat(Pfad_SPIB,"_SPIB");
		printf("B Pfad nach strcat: %s \n",Pfad_SPIB);
		strcpy(Pfad_I2C,Pfad);
		printf("I Pfad nach strcpy: %s \n",Pfad_I2C);
		strcat(Pfad_I2C,"_I2C");
		printf("I Pfad nach strcat: %s \n",Pfad_I2C);
	
	//	printf("Pfade fertig! \n");
		
		
		
		if(((fp_datei_spia = fopen(Pfad_SPIA,"a+")) == NULL))	// Es wird eine Datei mit dem aktuellen Datum und Uhrzeit _SPIA erstellt
		{
				printf("SpeichernDerWerte: ");
				perror("Error");
		}
		if(((fp_datei_spib = fopen(Pfad_SPIB,"a+")) == NULL))	// Es wird eine Datei mit dem aktuellen Datum und Uhrzeit _SPIB erstellt
		{
				printf("SpeichernDerWerte: ");
				perror("Error");
		}
		if(((fp_datei_i2c = fopen(Pfad_I2C,"a+")) == NULL))	// Es wird eine Datei mit dem aktuellen Datum und Uhrzeit _I2C erstellt
		{
				printf("SpeichernDerWerte: ");
				perror("Error");
		}

		// Hier übertrage ich die Dateinamen für den LeseUndSende-Thread
		putc('N',Pipe_Schreiben); //-> Neue Dateinamen
	//	printf("SchreibenUndSenden:	 N in die Pipe geschrieben! \n");
		fprintf(Pipe_Schreiben,"%s;",Pfad_SPIA);
		fprintf(Pipe_Schreiben,"%s;",Pfad_SPIB);
		fprintf(Pipe_Schreiben,"%s;",Pfad_I2C);
		fflush(Pipe_Schreiben); // An der Pipe-Spülung ziehen
	}


// Dateien werden geschlossen wenn Flag_Speichern wieder auf 0 gesetzt wurde
if((fp_datei_spia != NULL) && (Flag_Speichern == 0))
{
	Flag_File =0;
//	printf("SchreibenUndSenden: Bin im Datei-Schließer! \n");
	fprintf(fp_datei_spia,"x");
	fprintf(fp_datei_spib,"x");
	fprintf(fp_datei_i2c,"x");
	fflush(fp_datei_spia);
	fflush(fp_datei_spib);
	fflush(fp_datei_i2c);
	Pfad_SPIA[0]= '\0';
	Pfad_SPIB[0]= '\0';
	Pfad_I2C[0] ='\0';

	fputc('C',Pipe_Schreiben);
	Pipe_Flag=1;
	//printf("SchreibenUndSenden: 'C' in die Pipe geschrieben \n");
	
	fclose(fp_datei_spia);
	fclose(fp_datei_spib);
	fclose(fp_datei_i2c);
	fp_datei_spia = NULL;
	fp_datei_spib = NULL;
	fp_datei_i2c = NULL;
//	printf("SchreibenUndSenden: Datein geschlossen	\n");
}

	//printf("SchreibenUndSenden:---------->lese\n");
	read(fd[0],&BuchstabeUndKanal,1); // Erstmal 1 Byte um den Buchstaben herauszukriegen
	Buchstabe = 0b11110000 & BuchstabeUndKanal[0];	// Wandler habe ich
	Messwert_Kanal = 0b00001111 & BuchstabeUndKanal[0];	// Kanal-Info habe ich
	//printf("\n SpeichernUndSenden ------->hat diesen Buchstaben erhalten: %x \n",Buchstabe);
	
	switch(Buchstabe)
	{
		case 0xA0: fp_temp=fp_datei_spia; Pipe_Buchstabe = 'A';	Messwert_Laenge=WandlerA.Wortlaenge; WertUmwandeln = WandlerA.WertUmwandeln;	break;	// Jetzt wird eigentlicher Messwert
		case 0xB0: fp_temp=fp_datei_spib; Pipe_Buchstabe = 'B';	Messwert_Laenge=WandlerB.Wortlaenge; WertUmwandeln = WandlerB.WertUmwandeln;	break;	// nachgeholt
		case 0xC0: fp_temp=fp_datei_i2c;		
			//	printf("\n SpeichernUndSenden hat etwas für I2C erhalten! \n");
				Pipe_Buchstabe = 'I';
				Zeiger = I2C_Liste_Anfang;
				Zeiger=Zeiger->next;
				int i;
				for(i=0;i<Messwert_Kanal;i++)
				{
						Zeiger=Zeiger->next;		
				}
				Messwert_Laenge=Zeiger->len;
				WertUmwandeln = Zeiger->WertUmwandeln;		
		
				break;
		
		
		default:	fp_temp=NULL; 
					WertUmwandeln = WandlerA.WertUmwandeln;
					Messwert_Laenge = 0;
		
					break; // wenn ich nichts identifiziere gehts eben ab ins Nirvana
	}

	
	//printf("SchreibenUndSenden: lese 2\n");
	read(fd[0],&Messwert,Messwert_Laenge);
	int i;
	
	//printf(" Messwert[0]: %d Messwert[1]: %d Messwert[2]: %d Messwert[3]: %d\n",Messwert[0],Messwert[1],Messwert[2],Messwert[3]);
	Messwert_gewandelt = WertUmwandeln(Messwert,Messwert_Laenge);
	
	//printf("SchreibenUndSenden: Buchstabe: %c gewandelt zu: %d! \n",Pipe_Buchstabe,Messwert_gewandelt);
	
	if(Flag_File == 1)
	{
			//printf(".");
		//	printf("Speichern schreibt das in File:	%c%d:%d \n",Pipe_Buchstabe,Messwert_Kanal,Messwert_gewandelt);
			if(fprintf(fp_temp,"%d:%d\n",Messwert_Kanal,Messwert_gewandelt)<1)
			{
				printf("SpeichernUndSenden hat Fehler beim Schreiben in den File für: %c\n",Pipe_Buchstabe);
			}	
		//	fflush(fp_temp);																	 //	0:Messwert\n
	}else
	{
     // Hier wird der Messwert einfach in die Pipe gedrückt
	
	//	printf("Speichern schreibt das in Pipe:	%c%d:%d;\n",Pipe_Buchstabe,Messwert_Kanal,Messwert_gewandelt);
	//	fputc(Pipe_Buchstabe,Pipe_Schreiben);
		fprintf(Pipe_Schreiben,"%c%d:%d;",Pipe_Buchstabe,Messwert_Kanal,Messwert_gewandelt); // A0:Messwert;
	//	fflush(Pipe_Schreiben);
	}
	
	
}	

	fclose(fp_lesen);
	fclose(Pipe_Schreiben);
	fclose(fp_datei_i2c);
	fclose(fp_datei_spia);
	fclose(fp_datei_spib);
	return;
}

#endif


