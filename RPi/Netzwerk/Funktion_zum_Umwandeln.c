/*
 *
 * 
 * Hier stehen Funktionen um die Wörter entsprechend der Wandler umzuwandeln
 * 
 * Beispiel: WandlerA ist ein MCP3008 (oder muss identisch umgewandelt werden)
 * 
 * 	----> WandlerA.WertUmwandeln = mcp3008;<-------
 * 	----> WandlerX.WertUmwandeln = FunktionX;
 * 
 * 
*/
#include <stdint.h>
#include "ProzessPi.h"


int Funktion_zum_Umwandeln_SPI(struct SPI_Wandler(* DieserWandler), int FunktionsNummer)
{
	switch(FunktionsNummer)
	{
        case 1: DieserWandler->WertUmwandeln = mcp3008; printf("DieserWandler->mcp3008 gesetzt!\n"); return 1;
		case 2: DieserWandler->WertUmwandeln = mcp3208; printf("DieserWandler->mcp3208 gesetzt!\n"); return 2;
		case 3: DieserWandler->WertUmwandeln = mcp3551; printf("DieserWandler->mcp3551 gesetzt!\n"); return 3;
	//	case 3: DieserWandler->WertUmwandeln = Funktion_Deiner_Wahl;
		
		default :return 0;
	}
}

int Funktion_zum_Umwandeln_I2C(struct I2C_Liste(* DieserWandler), int FunktionsNummer)
{
	switch(FunktionsNummer)
	{
        case 1: DieserWandler->WertUmwandeln = I2C_8Bit; return 1;
	
		default :return 0;
	}
}




//int Funktion_Deiner_Wahl(uint8_t *Woerter,int AnzahlWoerter)
//{
//	Abhängig der Kodierung der binären Wörter muss hier gewandelt werden	
//	
//	
//}


int mcp3008(uint8_t *Woerter,int AnzahlWoerter)
{
	int Zahl;
	if(AnzahlWoerter < 1)
	{
		return 0;
	}
	Zahl = (Woerter[1] & 0x0F)<<8;
	Zahl = Zahl | Woerter[2];
	return	Zahl;
}
int mcp3208(uint8_t *Woerter,int AnzahlWoerter)
{
	int Zahl;
	if(AnzahlWoerter < 1)
	{
		return 0;
	}
	Zahl = (Woerter[1] & 0xFF)<<8;
	Zahl = Zahl | Woerter[2];
	return	Zahl;
}

int mcp3551(uint8_t *Woerter,int AnzahlWoerter)
{
	if(AnzahlWoerter < 1)
	{
		return 0;
	}
	
	int eins;
	int zwei;
	int drei;
	eins = 0;
	zwei = 0;
	drei = 0;
	
	eins = eins | Woerter[0];
	eins = eins << 17;
	zwei =  zwei | Woerter[1];
	zwei = zwei << 9;
	drei = drei | Woerter[2];
	drei = drei << 1;
	
	return (eins | zwei | drei);
}

int I2C_8Bit(uint8_t *Woerter,int AnzahlWoerter)
{
	
	// Kein Plan wie man das organisieren könnte
	
	return Woerter[AnzahlWoerter-1];
}
