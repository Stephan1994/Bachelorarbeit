#include <stdio.h>
#include "ProzessPi.h"


int main()
{
    printf("initalize..\n");
    init("192.168.2.105", "5001");
    printf("sending command..\n");
    SendeKommando("Kommando", "Wert");
    printf("done\n");
    done();
	/*printf("Öffne PtoTCP..");
	fd_senden = fopen("/tmp/PtoTCP","r");
	printf("geöffnet!");
    int tmpfd;
    tmpfd = fileno(fd_senden);
    printf("fileno: %d", tmpfd);
    char a[100];
    read(tmpfd,a,(100-1));
    printf("gelesen: %s",a);*/
	//fprintf(fd_senden,"Pfeifentest2!\n");

	//fflush(fd_senden);
    //fclose(fd_senden);
	//printf("Pfeifentest! gesendet.");

	/*printf("öffne TCPtoP..");
	fd_empfangen = fopen("/tmp/TCPtoP","r");
	printf("geöffnet");
	char a[100];
	printf("lese pipe..");
	int readedSigns = 0;
	while (readedSigns < 1){
		readedSigns = fscanf(fd_empfangen,"%[^\n]\n", a);
	}
	printf("gelesen: %s",a);*/
	return 0;
}
