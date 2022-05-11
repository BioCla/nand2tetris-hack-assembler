/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: control.h
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "control.c"

//cambia l'estensione del nome di un file da .asm a .hack e lo restituisce come stringa (test.asm -> test.hack)
void cambia(char asmfileName[], char *hackfileName);

//Verifica il tipo di istruzione che e' stata letta
instType differentiate (char* bufferString);

//Converte numeri int in binario char
void intToBinChar(int dec, char* dest);