/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: control.c
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "tabelle.h"

enum instructionT {A_INST, C_INST, L_INST, COMMENT, SKIP};

typedef enum instructionT instType;

//cambia l'estensione del nome di un file da .asm a .hack e lo restituisce come stringa (test.asm -> test.hack)
void cambia(char asmfileName[], char *hackfileName) {
	char extension[] = ".hack"; 
	int index = ext(asmfileName);
	substr(hackfileName, asmfileName, 0, index);
	strcat(hackfileName, extension);
	strcpy(hackfileName, hackfileName);
}

//Verifica il tipo di istruzione che e' stata letta
instType differentiate (char* bufferString) {
	for (int i = 0; i < strlen(bufferString); i++) {
		if (bufferString[i] == '@')
			return A_INST;
		if (bufferString[i] == '/' && i < strlen(bufferString) - 1) {
			if (bufferString[i + 1] == '/')
				return COMMENT;
		} else if (bufferString[i] == '(')
			return L_INST;
		else if (bufferString[i] != ' ' && bufferString[i] != '\t' && bufferString[i] != '\n')
			return C_INST;
	}
	return SKIP;
}

//Converte numeri int in binario char
void intToBinChar(int dec, char *dest) {
	char bin[16];
	int i = 15;
	while (i > 0) {
		if (dec % 2 == 1)
			bin[i] = '1';
		else
			bin[i] = '0';
		i--;
		dec = dec / 2;
	}
	bin[0] = '0';
	bin[16] = '\0';
	strcpy(dest, bin);
}