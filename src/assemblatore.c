/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: assemblatore.c
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "convertf.h"

//Driver code (.\assemblatore.exe input.asm (d) -> converte il file in input.hack (con file di debug.txt))
int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Non abbastanza parametri, esco dal programma.\n%s%s",
			   "=============================================\n",
			   " programma richiede il nome di un file *.asm");
        exit(0);
    }

	char hackFile[strlen(argv[1]) + 1];
	

	if (argc > 2) {
		if (charext(argv[2], 'd')) {
			printf("Debug Mode Attiva, file `debug.txt` verra' creato.\n");
			cambia(argv[1], hackFile);
			crea(argv[1], hackFile, 100); //debug init.
		} else {
			printf("L'unica modalita' implementata e' `d`. Utilizzala per creare il file per il debugging.\n");
		}
		return 0;
	} else {
	printf("Inizializazzione normale, il nuovo file verra' creato.\n");
	cambia(argv[1], hackFile);
	crea(argv[1], hackFile, 0); //init. normale
	return 0;
	}
}