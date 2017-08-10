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
//#include <sys/ipc.h>
//#include <sys/select.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
//#include <sys/msg.h>
#include <stdlib.h>
#include <pthread.h>
#include "Netzwerk.h"

void sig_handler(int signo);

int main(int argc,char* argv[]){
	char BufferIn[BUFFERSIZE];
	char BufferOut[BUFFERSIZE];
	debug = 0;
	
	int vonP, zuP;
	FILE *VonP, *ZuP;

	if(signal(SIGUSR1, sig_handler)== SIG_ERR)
		printf("\n can't catch SIG_USR1");
	if(signal(SIGPIPE, sig_handler)== SIG_ERR)
		printf("\n can't catch SIGPIPE");
	
	if(!strcmp(argv[1],"Server") | !strcmp(argv[1], "server"))
	{
		Log = fopen("NetzwerkServer.log","w");
		
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
			printf("Zu wenig Argumente: \n.	/Netzwerk Server [IP des Servers] [PORT] [Debug 1 / 0] \n");
			fprintf(Log,"Zu wenig Argumente: \n.	/Netzwerk Server [IP des Servers] [PORT] [Debug 1 / 0] \n");
		}
		return 0;
	}
	
	//Server
	if(!strcmp(argv[1],"Server") | !strcmp(argv[1], "server"))
	{		
		printf("Öffne Fifos und Erstelle Server ! \n");
		fprintf(Log,"Öffne Fifos und Erstelle Server ! \n");
		
		//open FIFOs
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

		//run server until closeNetwork is set
		while (closeNetwork == 0)
		{
			RunServer(sock, BufferIn, BufferOut, VonP, ZuP);
			flag_stop = 0;
		}
		
		close(sock);    
		return sock;
		
	}
	
	printf("Keine gültigen Argumente eingegeben! ./Netzwerk.out Server IP PORT \n");
	fprintf(Log,"Keine gültigen Argumente eingegeben! ./Netzwerk.out Server IP PORT \n");
	return 0;
}

//opens socket, bind it to port and prepare network communication
int ErstelleServer(int Port)
{
	printf("Erstelle den Server !\n");
	fprintf(Log,"Erstelle den Server !\n");
	struct sockaddr_in server;
	int sock;
	
	//create socket
	if((sock = socket( PF_INET, SOCK_STREAM, 0))<0)
	{
		printf("Konnte Socket nicht anlegen!\n");
		fprintf(Log,"Konnte Socket nicht anlegen!\n");
	}
	
	//prepare serverdata
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(Port);
	
	int so_reuseaddr = 1;

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
	
	//bind server to socket
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
	//wait for incoming connections
	printf("Warte auf eine Verbindung! \n");
	fprintf(Log,"Warte auf eine Verbindung! \n");
	listen(sock,1);
	
	struct sockaddr_in client;
	unsigned int len;
	len = sizeof(client);
	
	//save client data
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
		
	//run Thread for TCP to Pipe communication
	if((err = pthread_create(&(tid[0]),NULL, &TCPtoP,&TCPtoP_Struct) != 0))
	{
		printf("Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
		fprintf(Log,"Error beim Erstellen des Threads TCP to P !\n Hat keinen Sinn mehr.");
		exit(1);
	}
	
	//run Thread for Pipe to TCP communication
	if((err = pthread_create(&(tid[1]),NULL, &PtoTCP,&PtoTCP_Struct) != 0))
	{
		printf("Error beim Erstellen des Threads P to TCP !\n Hat keinen Sinn mehr.");
		fprintf(Log,"Error beim Erstellen des Threads P to TCP !\n Hat keinen Sinn mehr.");
		exit(1);
	}
	
	//wait for pipe to TCP Thread
	pthread_join( tid[1],NULL);
	printf("Netzwerk.out meldet tid[1] ist fertig! \n");
	fprintf(Log,"Netzwerk.out meldet tid[1] ist fertig! \n");
	
	//wait for TCP to Pipe Thread
	pthread_join( tid[0],NULL);
	printf("Netzwerk.out meldet tid[2] ist fertig! \n");
	fprintf(Log,"Netzwerk.out meldet tid[2] ist fertig! \n");
	
	return sock;
}

//Thread for communication between socket and ProzessPi
void *TCPtoP(void *TCPtoP_Struct)
{

	printf("Tcp To P Thread steht! \n");
    struct ThreadUebergabe *hierhin;
    hierhin = (struct ThreadUebergabe *) TCPtoP_Struct;
    char Buffer[BUFFERSIZE];
    int RecvTemp;

    //getting max writesize for writing in Pipes
    int maxWrite = (int) pathconf("/tmp/TCPtoP", _PC_PIPE_BUF);

    //loop for getting the messages from socket and writing it to Pipe
    while (flag_stop == 0) {
        // receiving messages
        RecvTemp = (int) recv(hierhin->fd, Buffer, (size_t) (BUFFERSIZE - 1), 0);

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
        memset(&Buffer[0], 0, BUFFERSIZE);

    }
    printf("TCPtoP nähert sich dem Ende! \n");
}

//Thread for communication from ProzessPi to socket
void *PtoTCP(void *PtoTCP_Struct)
{	
	printf("P to TCP Thread steht! \n");

	struct ThreadUebergabe *hierhin;
	hierhin = (struct ThreadUebergabe *)PtoTCP_Struct;
	char Buffer[BUFFERSIZE];
	int len,len_send = 0;	
	
	//loop to read messages from PtoTCP-Pipe and write it into socket
	while(flag_stop == 0)
	{	
		usleep(100);

		if(fgets(Buffer,BUFFERSIZE,hierhin->VonP) != NULL)
		{
			len = (int) strlen(Buffer) - 1;
			int tempLen = len;
			while((len_send = send(hierhin->fd, &Buffer[len_send], (size_t) tempLen, 0) < tempLen))
			{ 	
					tempLen -= len_send;
			}
			
			//printf("Aus der Pipe geholt und ans Socket gesendet: %s \n",Buffer);
			
			//waiting for the disconnected-value to end this thread and the TCPtoP-Thread
            //smaller than 400 to avoid large values that could not be disconnect
			if (len < 400){
				char value[len];
				memset(&value[0], 0, sizeof(value));
				
				extractValue(Buffer, value);

				if (strcmp(value, "disconnected") == 0)
				{
					flag_stop = 1;
				}
			}
		} 
	}
	
	printf("PtoTCP nähert sich dem Ende! \n");
}

//extracts Value from message and writes it into Value parameter
void extractValue(char *BufferIn, char *Value)
{
    int startValue = (int)(strstr(BufferIn, "message::") - BufferIn + 9);
    int endValue = (int)(strstr(BufferIn, "::message") - BufferIn);

    strncat(Value, BufferIn + startValue, (size_t)(endValue - startValue));
}

void sig_handler(int signo)
{
	//catching SIGUSR1 ends the while-loop and the process will end
	if(signo == SIGUSR1)
	{
		closeNetwork = 1;
	}
}

