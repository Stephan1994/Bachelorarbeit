#include "ProzessPi.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#define IP "127.0.0.1"
#define IP2 "192.168.178.57"
#define PORT "5001"

//pthread_t tid[2];

int main()
{
while(1)
{
	char testbuf[5];
	Flag_Done = 0;
	Flag_Start = 0;
	Flag_ZuP = 0;
//	printf("Vor init()... ");
	init(IP,PORT);
//	printf("ist nacht init()!\n");
	int err;


	if(pthread_mutex_trylock(&I2C_Konflock) == 0)
	{
		pthread_mutex_unlock(&I2C_Konflock); // Unlock zeigt ohne lock undefined behaviour
		printf("I2C gelockt und unlocked!\n");
	}
	
	if(pthread_mutex_trylock(&SPI_Konflock) == 0)
	{
		pthread_mutex_unlock(&SPI_Konflock); // Unlock zeigt ohne lock undefined behaviour
		printf("SPI gelockt und unlocked!\n");
	}

	err=pipe(fd);
	if(err < 0)
	{
		printf("Probleme mit der Pipe!");
		exit(3);
	}
	err=pipe(PipeStattFile);
	if(err < 0)
	{
		printf("Probleme mit der Pipe!");
		exit(3);
	}

	//bcm2835_set_debug (1);
	bcm2835_init();

	WandlerA.ChipSelect=0;
	WandlerB.ChipSelect=1;


	Flag_Start = 0;
	
	do{
//		printf("Warte auf go?\n");
		EmpfangeKommando();
	}while(Flag_Start==0);

	
		if((err = pthread_create(&thread_Speichern, NULL, &SpeichernDerWerte,NULL)) != 0)
		{
			printf("Error beim Erstellen des Threads!\n");
			perror(NULL);
		}

		if((err = pthread_create(&thread_Senden, NULL, &LesenUndSenden,NULL)) != 0)
		{
				printf("Error beim Erstellen des Threads!\n");
				perror(NULL);
		}
	
	
	if((err = pthread_create(&thread_I2C, NULL, &BCM2835_I2C,NULL)) != 0)
	{
			printf("Error beim Erstellen des Threads!\n");
			perror(NULL);
	}
	if((err = pthread_create(&thread_SPI, NULL, &BCM2835_SPI,NULL)) != 0)
	{
		printf("Error beim Erstellen des Threads!\n");
		perror(NULL);
	}


	Flag_ZuP = 0;
	int counter=0;
	do
	{

			EmpfangeKommando();
			Flag_ZuP = 0;
	
	}while(Flag_Done == 0);
	
		printf("Hauptprozess: Rausgehüpft! \n");
		pthread_cancel(thread_SPI);
		pthread_cancel(thread_I2C);
		pthread_cancel(thread_Speichern);
		pthread_cancel(thread_Senden);


		printf("Hauptprozess: Threads gekillt! \n");
		bcm2835_spi_end();
		bcm2835_close();  

		printf(" Bin fertig \n");
		sleep(2);
		//done();
		char wd[255];
		getcwd(wd,sizeof(wd));
		strcat(wd,"/main.out");
		execlp(wd,NULL);

}

	return 0;
}

// SIGUSR1 zeigt an, dass etwas an den Prozess gesendet wurde

