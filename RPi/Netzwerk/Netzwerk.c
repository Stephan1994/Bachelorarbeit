/*BEST OF :
 * 
 * sizeof(argv[2]) anstatt strlen(argv[2]) verwendet -> Gut!
 * Kommandozeilenargumente verschieben sich beim Aufruf von exec um eine Stelle nach links -> gut!
 * Die Verbindung bekommt Probleme wenn in der Server-IP ein Leerzeichen enthalten ist 
 * ---> PARAMETERÜBERGABE ALSO "192..." UND NICHT " 192..." 
 *
 * 
 * fork();
 * exec( ./Netzwerk [Server/Client] [IP] [PORT] )
 * 
 * Dies ist der Netzwerk-Teil der Bibliothek zur MasterArbeit
 * 
 * Server und Client werden in jeweiligen Funktionen geschrieben
 * 
 *
 * 
 * Folgender Aufbau:
 *	 
 *  Prozess <-Pipes-> | Server <---TCP ---> Client | <-Pipes-> Prozess
 * 
 * 
 * Wie eklig ist das denn! Durch exec verschieben sich die Parameter um eine Stelle!
 * 
 * TODO: Abfragen erstellen ob die übergebenen Parameter überhaupt cool so sind
 *
 * Ich möchte gerne diese Pipes haben 
 * Client-Rechner:
 * 	PtoC - Prozess zu Client 
 * 	CtoP - Client zu Prozess
 * Server-Rechner:
 * 	PtoS - Prozess zu Server
 * 	StoP - Server zu Prozess
 * 
 *
 *  
 * letzte Aktualisierungen: 
 * 01.11.16 : 	- Signal Handler implementiert
 * 02.11.16 :	- FIFOs implementiert
 * 				- Funktion für das Umlenken der Daten zwischen Prozess und Stream
 * 03.11.16	:	- erster Test der Verbindung ok -> pipe open non blocking problematisch
 * 				- usleep hinzugefügt um die Prozessorauslastung zu verringern
 * 				- nächster Schritt: dosenoeffner.c schreiben um das Pipe öffnen beim debuggen zu vermeiden
 * 07.11.16 : 	- schreibe umfassendes Debug-System mit Logfile
 * */

#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <fcntl.h>
#include "Netzwerk.h"

void sig_handler(int signo);

int main(int argc,char* argv[]){
	char BufferIn[BUFSIZ];
	char BufferOut[BUFSIZ];
	a=0;
	debug = 0;
	PapaPID = getppid();
	
	int vonP, zuP;
	FILE *VonP, *ZuP;

	if(signal(SIGUSR1, sig_handler) == SIG_ERR)
	printf("\n can't catch SIG 10\n");
	if(signal(SIGUSR2, sig_handler)== SIG_ERR)
	printf("\n can't catch SIG_USR2");
	if(signal(SIGPIPE, sig_handler)== SIG_ERR)
	printf("\n can't catch SIG_USR2");
	
	if(!strcmp(argv[1],"Server") | !strcmp(argv[1], "server"))
	{
		Log = fopen("NetzwerkServer.log","w");
		
	}
	if(!strcmp(argv[1],"Client") | !strcmp(argv[1],"client"))
	{
		Log = fopen("NetzwerkClient.log","w");
			
	}
	
	
	int i;
	for( i = 1; i < argc;i++)
	{
			printf("%d %s \n",i,argv[i]);
			fprintf(Log,"%d %s \n",i,argv[i]);
	}

	if(argc>2 && argc<4 )
	{
		if(debug == 1)
		{
			printf("Zu wenig Argumente: \n.	/Netzwerk [Server/Client] [IP des Servers] [PORT] [Debug 1 / 0] \n");
			fprintf(Log,"Zu wenig Argumente: \n.	/Netzwerk [Server/Client] [IP des Servers] [PORT] [Debug 1 / 0] \n");
		}
		return 0;
	}
	printf(" MEINE PID : %d \n",getpid());
	
	if(!strcmp(argv[1],"Server") | !strcmp(argv[1], "server"))
	{		
		// Flieg und Sieg Server!
		printf("Öffne Fifos und Erstelle Server ! \n");
		fprintf(Log,"Öffne Fifos und Erstelle Server ! \n");
		// öffne FIFOs
		
		if((vonP = open("/tmp/PtoTCP",O_RDONLY | O_CREAT))<0)
		{	
				printf("Netzwerk meldet:	Pipe PtoS open: ");
				fprintf(Log,"Netzwerk meldet:	Pipe PtoS open:  %s \n",strerror(errno));
				perror(NULL);
				printf("\n");
		}
		if((VonP = fdopen(vonP,"r"))<0)
		{	
				printf("Netzwerk meldet:	Pipe PtoS fdopen: ");
				fprintf(Log,"Netzwerk meldet:	Pipe PtoS fdopen:  %s \n",strerror(errno));
				perror(NULL);
				printf("\n");
		}
		
		if((zuP = open("/tmp/TCPtoP",O_WRONLY |O_CREAT))<0)  //  
		{
			printf("\n Pipe StoP open: \n");
			fprintf(Log,"Netzwerk meldet:	Pipe StoP open:  %s \n",strerror(errno));
			perror(NULL);
			printf("\n");	
		}
	
		if((ZuP = fdopen(zuP,"w"))<0)
		{		
			printf("Pipe StoP fdopen: ");
			fprintf(Log,"Netzwerk meldet:	Pipe StoP open:  %s \n",strerror(errno));
			perror(NULL);
			printf("\n");
		}

		int sock = ErstelleServer(atoi(argv[3]));
		//printf("sock is %d and closeNetwork is %d\n", sock, closeNetwork);
		while (closeNetwork == 0)
		{
			RunServer(sock, BufferIn, BufferOut, VonP, ZuP);
			flag_stop = 0;
		}
		
		close(sock);    
		return sock;
		
	}
	
	if(!strcmp(argv[1],"Client"))
	{
	//	Flieg und Sieg Client!	
		printf("Öffne Fifos und Erstelle Client ! \n");
	// öffne FIFOS
		
		if((vonP = open("/tmp/PtoTCP",O_RDONLY | O_NONBLOCK | O_CREAT))<0)
	//	if((vonP = open("/tmp/PtoTCP",O_RDONLY | O_CREAT))<0)
		{
				
			printf("Pipe PtoTCPC open: ");
			fprintf(Log,"Netzwerk meldet:	Pipe PtoS open:  %s \n",strerror(errno));
			perror(NULL);
			printf("\n");
			
		}
		if((VonP = fdopen(vonP,"r"))<0)
		{
			printf("Pipe PtoTCPC fdopen: ");
			fprintf(Log,"Netzwerk meldet:	Pipe PtoS fdopen:  %s \n",strerror(errno));
			perror(NULL);
			printf("\n");
		}
		
		if((zuP = open("/tmp/TCPtoP",O_WRONLY |O_CREAT))<0)  // 
		{
			printf("\n Pipe TCPCtoP open: \n");
			fprintf(Log,"Netzwerk meldet:	Pipe StoP open:  %s \n",strerror(errno));
			perror(NULL);
			printf("\n");	
		}
		
		if((ZuP = fdopen(zuP,"w"))<0)
		{		
			printf("Pipe TCPCtoP fdopen: ");
			fprintf(Log,"Netzwerk meldet:	Pipe StoP open:  %s \n",strerror(errno));
			perror(NULL);
			printf("\n");
		}
	
	
		return ErstelleClient(BufferIn,BufferOut,atoi(argv[3]),argv[2],strlen(argv[2]), VonP,ZuP);	
	 }
	printf("Keine gültigen Argumente eingegeben! ./Netzwerk.out Server/Client IP PORT \n");
	fprintf(Log,"Keine gültigen Argumente eingegeben! ./Netzwerk.out Server/Client IP PORT \n");
	return 0;
}

int ErstelleServer(int Port)
{
	printf("Erstelle den Server !\n");
	fprintf(Log,"Erstelle den Server !\n");
	struct sockaddr_in server;
	int sock;
	
	//erstelle socket
	if((sock = socket( PF_INET, SOCK_STREAM, 0))<0)
	{
		printf("Konnte Socket nicht anlegen!\n");
		fprintf(Log,"Konnte Socket nicht anlegen!\n");
	}
	//speichere Serverdaten
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(Port);
	
	int so_reuseaddr = 1;

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
	
	//weise dem socket den Server zu
	if(bind(sock,(struct sockaddr*)&server, sizeof(server))<0)
	{
		printf("Kann mich nicht an das %d Socket binden! \n",sock);
		fprintf(Log,"Kann mich nicht an das %d Socket binden!  \n %s \n",sock,strerror(errno));
		perror(NULL);
				
		close(sock);
		return 0;
	}
	printf("Server läuft! \n");
	fprintf(Log,"Server läuft! \n");
		
	return sock;
}

int RunServer(int sock, char *BufferIn,char *BufferOut,FILE *VonP,FILE *ZuP)
{
	//warten auf eingehende Verbindungen
	printf("Warte auf eine Verbindung! \n");
	fprintf(Log,"Warte auf eine Verbindung! \n");
	listen(sock,1);
	
	struct sockaddr_in client;
	unsigned int len;
	len = sizeof(client);
	
	//speichern des Clients
    int fd = accept(sock,(struct sockaddr*)&client, &len);
    if (fd < 0)
    {
		printf("Fehler bei accept \n");
		fprintf(Log,"Fehler bei accept \n");
	}
		
	printf("Bearbeite den Client mit der Adresse: %s\n", inet_ntoa(client.sin_addr));
    fprintf(Log,"Bearbeite den Client mit der Adresse: %s\n", inet_ntoa(client.sin_addr));
		
	printf("Lege los!\n");
	fprintf(Log,"Lege los!\n");
		
	int err;
	struct ThreadUebergabe TCPtoP_Struct;
	struct ThreadUebergabe PtoTCP_Struct;
	
	TCPtoP_Struct.fd = fd;
	TCPtoP_Struct.VonP = VonP;
	TCPtoP_Struct.ZuP = ZuP;
	PtoTCP_Struct.fd = fd;
	PtoTCP_Struct.VonP = VonP;
	PtoTCP_Struct.ZuP = ZuP;
		
	//Thread für TCP to Pipe erstellen
	if((err = pthread_create(&(tid[0]),NULL, &TCPtoP,&TCPtoP_Struct) != 0))
	{
		printf("Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
		fprintf(Log,"Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
		exit(1);
	}
	
	//Thread für Pipe to TCP erstellen
	if((err = pthread_create(&(tid[1]),NULL, &PtoTCP,&PtoTCP_Struct) != 0))
	{
		printf("Error beim Erstellen des Threads P to TCP !\n Hat keinen Sinn mehr.");
		fprintf(Log,"Error beim Erstellen des Threads P to TCP !\n Hat keinen Sinn mehr.");
		exit(1);
	}
	
	//warte auf Pipe to TCP Thread
	pthread_join( tid[1],NULL);
	printf("Netzwerk.out meldet tid[1] ist fertig! \n");
	fprintf(Log,"Netzwerk.out meldet tid[1] ist fertig! \n");
	
	//warte auf TCP to Pipe Thread
	pthread_join( tid[0],NULL);
	printf("Netzwerk.out meldet tid[2] ist fertig! \n");
	fprintf(Log,"Netzwerk.out meldet tid[2] ist fertig! \n");
	
	return sock;
}

int ErstelleClient(char *BufferIn,char *BufferOut,int Port, char *IP,int SizeOfIP, FILE *VonP,FILE *ZuP)
{	
	int sock;
	struct sockaddr_in client;
	unsigned long addr;
	
	char ServerIP[16];
	strncpy(ServerIP,IP,15);
	ServerIP[SizeOfIP] = '\0';
	
		if( (sock = socket(AF_INET, SOCK_STREAM, 0))<0)
			{
				printf("Error! Kriege kein Socket!\n");
			}		
			memset( &client,0,sizeof(client));
			addr = inet_addr(ServerIP);
			memcpy( (char*)&client.sin_addr, &addr, sizeof(addr));
			client.sin_family = AF_INET;
			client.sin_port = htons(Port);
		
			printf("Versuche zu verbinden... \n");
			int cncttmp=-1;
			while(cncttmp<0)
			{
			
			if((cncttmp=connect(sock,(struct sockaddr*)&client, sizeof(client)))<0)
			{

			}	
			if(cncttmp > 10000)
			{
				printf("und ich versuche es weiter.... \n");
				cncttmp =0;
			}
			}
			
		int err;
		struct ThreadUebergabe TCPtoP_Struct;
		struct ThreadUebergabe PtoTCP_Struct;
		
		TCPtoP_Struct.fd = sock;
		TCPtoP_Struct.VonP = VonP;
		TCPtoP_Struct.ZuP = ZuP;
		PtoTCP_Struct.fd = sock;
		PtoTCP_Struct.VonP = VonP;
		PtoTCP_Struct.ZuP = ZuP;
		
		
		if((err = pthread_create(&(tid[0]),NULL, &TCPtoP,&TCPtoP_Struct) != 0))
		{
				printf("Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
				fprintf(Log,"Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
				exit(1);
		}
		
		if((err = pthread_create(&(tid[1]),NULL, &PtoTCP,&PtoTCP_Struct) != 0))
		{
				printf("Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
				fprintf(Log,"Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
				exit(1);
		}		
		
		pthread_join( tid[1],NULL);
		printf("Netzwerk.out meldet tid[1] ist fertig! \n");
		pthread_join( tid[0],NULL);
		printf("Netzwerk.out meldet tid[2] ist fertig! \n");						
		return sock;
}
void *TCPtoP(void *TCPtoP_Struct)
{

	printf("Tcp To P Thread steht! \n");
    struct ThreadUebergabe *hierhin;
    hierhin = (struct ThreadUebergabe *) TCPtoP_Struct;
    char Buffer[BUFSIZ];
    int RecvTemp;

    //getting max writesize for writing in Pipes
    int maxWrite = (int) pathconf("/tmp/TCPtoP", _PC_PIPE_BUF);

    //loop for getting the messages from socket and writing it to Pipe
    while (flag_stop == 0) {
        // receiving messages
        RecvTemp = (int) recv(hierhin->fd, Buffer, (size_t) (BUFSIZ - 1), 0);

        //if there is something to read, that's smaller than max writesize
        if (RecvTemp > 0 && RecvTemp < maxWrite) {

            Buffer[RecvTemp] = '\0';

            //printf("Aus dem Socket geholt und in die Pipe gesendet: Length: %d; Value: %s \n", (int)strlen(Buffer), Buffer);

            //TODO change fprintf to write
            //write to TCPtoP-Pipe
            if (fprintf(hierhin->ZuP, "%s", Buffer) < 0)
            {
                perror("fprintf");
                printf("tmp ist NULL? \n");
            }
            fflush(hierhin->ZuP);
            //kill(PapaPID, SIGUSR1);
        }
        //if there is something to read, that's bigger than max writesize, the message have to be splitted
        else if (RecvTemp > maxWrite){
            int written = maxWrite;
            int fd_ZuP = fileno(hierhin->ZuP);
            //write packages with a size of maxWriteSize to Pipe
            while(written < RecvTemp){
                write(fd_ZuP, &Buffer[written-maxWrite], (size_t)maxWrite);
                written += maxWrite;
            }
            written -= maxWrite;
            //rest of message sends here
            write(fd_ZuP, &Buffer[written], (size_t) (RecvTemp-written));
            fflush(hierhin->ZuP);
            //kill(PapaPID, SIGUSR1);

        }

        //reset Buffer
        memset(&Buffer[0], 0, BUFSIZ);

    }
    printf("TCPtoP nähert sich dem Ende! \n");
}

void *PtoTCP(void *PtoTCP_Struct)
{
	
	printf("P to TCP Thread steht! \n");
	// hier kommt fgets() und send()
	// typecast
	struct ThreadUebergabe *hierhin;
	hierhin = (struct ThreadUebergabe *)PtoTCP_Struct;
	char Buffer[BUFSIZ];
	int len,len_send = 0;
	//Ist was in der Pipe zum Senden ans Socket?

	char c;
	int testcounter;
	testcounter = 0;
	
	struct timeval tv = {1, 0};
	fd_set set;
	FD_ZERO(&set);
	int filedesc = fileno(hierhin->VonP);
	FD_SET(filedesc,&set);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//fd_set_blocking(fileno(hierhin->VonP),1);
	while(flag_stop == 0)
	{	
		//printf("ptoTcp while gestartet\n");
		usleep(100);
		fd_set setSelected = set;

		//printf("vor select\n");
		//int ret = select(FD_SETSIZE, &setSelected, NULL, NULL, &tv);
		//if ((ret == -1))
		//{
		//	printf("fail in ptotcp\n");
		//}
		//sleep(10);
		//printf("ISSET: %d\n", FD_ISSET(filedesc, &setSelected));
		if(fgets(Buffer,BUFSIZ,hierhin->VonP) != NULL)
		//if(FD_ISSET(filedesc, &setSelected))
		{
		//	printf("in isset\n");
		//	sleep(10);
		//	printf("jetzt gehts weiter\n");
		//	fgets(Buffer,200,hierhin->VonP);
			//printf("PtoTCP meldet:	%s \n",Buffer);
			len = strlen(Buffer) - 1;
			int tempLen = len;
			while((len_send = send(hierhin->fd, &Buffer[len_send], tempLen, 0) < tempLen))
			{ 	
					tempLen -= len_send;
			}
			//printf("Aus der Pipe geholt und ans Socket gesendet: %s \n",Buffer);
			if (len < 400){
				char value[len];
				memset(&value[0], 0, sizeof(value));
				extractValue(Buffer, value);
				//printf("Value: %s	Length: %d\n", value, len);
				//printf("Should be: %s	Length: %d\n", "disconnected", strlen("disconnected"));
				//printf("Compare: %d\n", strcmp(value, "disconnected"));
				if (strcmp(value, "disconnected") == 0)
				{
					flag_stop = 1;
				}
			}
		} 
	}
	
	printf("PtoTCP nähert sich dem Ende! \n");
	

	// IST NOCH BLOCK
	int tmpfd;
	/*tmpfd = fileno(hierhin->VonP);
	//fread(Buffer,(BUFSIZ-1),1,hierhin->VonP);
	read(tmpfd,Buffer,(BUFSIZ-1));
	
	len = strlen(Buffer);
	if((len_send = send(hierhin->fd, Buffer, len, 0) != len))
	{ 	
		printf("Error beim letzten Senden! \n");
		fprintf(Log,"Error beim letzten Senden! \n");
		perror(NULL);				
	}
		//printf("Aus der Pipe geholt und ans Socket gesendet: %s \n",BufferIn); 
	
	*/
}

/*
//"header::type:(request/answer),command: commandValue[, parted: yes, partnr: 00, parts: 00]::header"
void extractHeaderFieldValue(char *BufferIn, char *Command, char *HeaderFieldType)
{
    int i = 0;
    int startHeader = (int)(strstr(BufferIn, "header::") - BufferIn + 8);
    int endHeader = (int)(strstr(BufferIn, "::header") - BufferIn);
    char fullHeaderFieldType[strlen(HeaderFieldType) + 2];
    strcpy(fullHeaderFieldType, HeaderFieldType);
    strcat(fullHeaderFieldType, ": ");
    char headerField[40];
    while(startHeader < endHeader){
        i = startHeader;
        while (BufferIn[i] != ',' && i < endHeader){i++;};
        memset(&headerField[0], 0, sizeof(headerField));
        strncat(headerField, BufferIn + startHeader,(size_t) (i - startHeader));
        if (strstr(headerField, fullHeaderFieldType) != NULL){
            strncat(Command, headerField + strlen(fullHeaderFieldType) , (size_t)(i - startHeader - strlen(fullHeaderFieldType)));
            return;
        }
        startHeader = i+1;
    }
}
*/
void extractValue(char *BufferIn, char *Value)
{
    int startValue = (int)(strstr(BufferIn, "message::") - BufferIn + 9);
    int endValue = (int)(strstr(BufferIn, "::message") - BufferIn);

    strncat(Value, BufferIn + startValue, (size_t)(endValue - startValue));
}

void ReadAndWrite(int fd,char *BufferIn,char *BufferOut, FILE *VonP, FILE *ZuP)
{
	//usleep(10);
//initialisieren von Temporären Variablen
	int SendTemp;
	int RecvTemp;
	SendTemp = 0;
	RecvTemp = 0;
	int len;
	int len_send;
	
//Ist was in der Pipe zum Senden ans Socket?

	//if( (RecvTemp = fscanf(VonP,"%s",BufferIn)) < 0)
	if(fgets(BufferIn,BUFSIZ,VonP) !=NULL) //
	{
			len = strlen(BufferIn);
			if((len_send = send(fd, BufferIn, len, 0) != len))
			{ 	
				printf("Switche zu blocking.. ");
				fd_set_blocking(fd,1);
				send(fd, BufferIn, len, 0);
				fd_set_blocking(fd,0);
				printf("und wieder zurück..!\n");
				
		/*		if(len_send != -1)
				{
					ErrorCounter=(len - len_send) + ErrorCounter;
					printf("Fehler beim Senden! Fehlerhaft: %d 	von insgesamt:	%d Bits\n",(len-len_send),(ErrorCounter));
					
					usleep(10);	
				}
				else{
					ErrorCounter=strlen(BufferIn) + ErrorCounter;
					printf("Send hat total versagt und %d geschmissen\n",strlen(BufferIn));
				}
				*/
				
				
			}
		//printf("Aus der Pipe geholt und ans Socket gesendet: %s \n",BufferIn);
	} 
// Ist was im Socket zum senden an die Pipe?
	if((RecvTemp = recv(fd, BufferOut, BUFSIZ,0)) == -1 )
	{
//	perror(NULL);
	} 
	if(RecvTemp > 0)
	{
		int tmp=strlen(BufferOut);
		//printf("Recv Temp ist so groß: %d \n",RecvTemp);
		
		BufferOut[RecvTemp] = '\0';
		//printf("Aus dem Socket geholt und in die Pipe gesendet: %s \n", BufferOut);
		
		if((tmp = fprintf(ZuP,"%s",BufferOut))<0)	//write(fileno(ZuP),BufferOut,10))== 0)
		{
			perror("fprintf");
			printf("tmp ist NULL? \n");
		}
		fflush(ZuP);
		kill(PapaPID,SIGUSR1);
	//	printf("Soviele Zeichen habe ich in die Pipe gesendet: %d \n",tmp);	
	} 
	BufferIn[0] = '\0';
	BufferOut[0] = '\0';
}
void sig_handler(int signo)
{
  if(signo == SIGUSR1)
	{
		//exit(1);
		closeNetwork = 1;
	}
	if(signo == SIGUSR2)
	{
		printf("Client meldet sich ab.\n");
		fprintf(Log,"Client meldet sich ab. \n");
		//usleep(100);
		//flag_sigpipe = 1;
	}
	
	//if(signo == SIGINT)
	//{
		
//	}
	
 if(signo == SIGPIPE)
 {
	
	FILE *letzte_Nachricht;
	letzte_Nachricht = fopen("/tmp/TCPtoP","w");
	fprintf(letzte_Nachricht,"BEENDEN, Hier kommt die Erlösung aus EmpfangeKommando()!;\n");
	fflush(letzte_Nachricht);
	fclose(letzte_Nachricht);
	printf("Netzwerk.out -> habe SIGPIPE erhalten\n");
	sleep(1);
	exit(1);
 }
 
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

