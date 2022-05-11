/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: convertf.c
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "control.h"

#define MAX_STRING_LEN 100

char bufferSpace[MAX_STRING_LEN]; // Buffer per ogni scopo all'interno del file


//Crea il nuovo file con estensione .hack con all'interno la conversione del file .asm in bit 
//(e crea il file di debug, se richiesto)
void crea(char source[], char destination[], const int mode) {
	FILE *asmfile, *hackfile, *debug, *labelCreator;
	
	// Modalita' debug: Genera un nuovo file di nome `debug.txt` con dentro le informazioni su ogni istruzione letta
	if (mode == 100) {
		debug = fopen("debug.txt", "r");
		if(debug != NULL) {
			printf("Un file per il debugging esiste gia', lo sovrascrivo.\n");
			fputs("\n", debug);
		}
		debug = fopen("debug.txt", "w");
		if(debug == NULL) {
			perror("Errore in apertura del file per il debugging.");
			exit(1);
		}
	}

	// verifica che il file da leggere esiste
	asmfile = fopen(source, "r");
	if(asmfile == NULL) {
		printf("Il file da convertire non esiste.\n");
		exit(1);
	}

	// il file di output esiste?
	// se il file di output esiste gia' allora lo preparo per aggiornarlo
	hackfile = fopen(destination, "r");
	if(hackfile != NULL) {
		printf("Il file convertito esiste gia', lo aggiorno.\n");
		fputs("\n", hackfile);
	}

	// se il file di output non esiste devo crearlo apre il file da sul quale scrivere
	hackfile = fopen(destination, "w");
	if(hackfile == NULL) {
		perror("Errore in apertura del file nel quale scrivere");
		exit(1);
	}

	//Prima lettura del file: Acquisizione dei (label)
	unsigned int counter = 0; // Contatore utilizzato per indicizzare la linea dei (label)
	labelCreator = fopen(source, "r");
	if(asmfile == NULL) {
		printf("Il file da convertire non esiste.\n");
		exit(1);
	}
	
	//Prima passata per prendere i (label)
	pTable table = tableInit();
	initTable(table);
	while (fgets(bufferSpace, MAX_STRING_LEN, labelCreator)) {
		//Preparazione della stringa: rimozione di spazi binachi e commenti
		deblank(bufferSpace);
		for (int i = 0; i < strlen(bufferSpace); i++) {
			if (bufferSpace[i] == '/' && i < strlen(bufferSpace) - 1) {
				if (bufferSpace[i + 1] == '/')
					bufferSpace[i] = '\0';
			}
		}

		// Se alla fine della istruzione un carattere fa andare a capo, rimuoverlo
		if (bufferSpace[strlen(bufferSpace) - 1] == '\n')
			bufferSpace[strlen(bufferSpace) - 1] = '\0';
		if (bufferSpace[strlen(bufferSpace) - 1] == '\r')
			bufferSpace[strlen(bufferSpace) - 1] = '\0';
		switch (differentiate(bufferSpace)) {
			case L_INST:
			{
				char label[strlen(bufferSpace - 2)];
				substr(label, bufferSpace, strchar(bufferSpace, '(') + 1, strchar(bufferSpace, ')') - 1);
				if (!labelExt(table, label))	
					addEntry(table, label, counter);
				break;
			}
			case C_INST:
			{
				counter += 1;
				break;
			}
			case A_INST:
			{
				counter += 1;
				break;
			}
			default:
			{
				break;
			}
		}
	}
	fclose(labelCreator);

	unsigned int line = 0; // Conta a quale linea viene letta l'istruzione
	unsigned int varcounter = 16;
	unsigned int debugcounter = 1;
	while (fgets(bufferSpace, MAX_STRING_LEN, asmfile)) {
		line += 1;

		// Se alla fine della istruzione un carattere fa andare a capo, rimuoverlo
		if (bufferSpace[strlen(bufferSpace) - 1] == '\n')
			bufferSpace[strlen(bufferSpace) - 1] = '\0';
		if (bufferSpace[strlen(bufferSpace) - 1] == '\r')
			bufferSpace[strlen(bufferSpace) - 1] = '\0';
		switch (differentiate(bufferSpace)) {
			case A_INST: 
			{
				//Preparazione della stringa: rimozione di spazi binachi e commenti
				deblank(bufferSpace);
				for (int i = 0; i < strlen(bufferSpace); i++) {
					if (bufferSpace[i] == '/' && i < strlen(bufferSpace) - 1) {
						if (bufferSpace[i + 1] == '/')
							bufferSpace[i] = '\0';
					}
				}

				//@num -> bin
				substr(bufferSpace, bufferSpace, strchar(bufferSpace, '@') + 1, strlen(bufferSpace) - 1);
				if (bufferSpace[0] >= '0' && bufferSpace[0] <= '9') {
					//senza Label				
					int num = atoi(bufferSpace); //converte una stringa numerica direttamente in formato `int`
					char bin[16];
					intToBinChar(num, bin);
					
					//Debugging string
					if (mode == 100) {
						fprintf(debug, "%d | A-Instruction: %s%s%s%s%d\n", line, bin, " <- ", bufferSpace,  " <- | ", debugcounter);
						debugcounter += 1;
					}
					fprintf(hackfile, "%s\n", bin);
				} else {
					//Con Label
					char label[16];

					if (!labelExt(table, bufferSpace)) {
						addEntry(table, bufferSpace, varcounter);
						varcounter += 1;
					}
					int index = getAddress(table, bufferSpace);
					intToBinChar(index, label);

					//Debugging string
					if (mode == 100) {
						fprintf(debug, "%d | A-Instruction (Label): %s%s%s%s%d%s%d\n", line, label, " <- ", bufferSpace, " <- | ", debugcounter,"\n\tIndice di istruzione allocata: ", index);
						debugcounter += 1;
					}
					fprintf(hackfile, "%s\n", label);
				}
				break;
			}
			case C_INST:
			{
				//Preparazione della stringa: rimozione di spazi binachi e commenti
				deblank(bufferSpace);
				for (int i = 0; i < strlen(bufferSpace); i++) {
					if (bufferSpace[i] == '/' && i < strlen(bufferSpace) - 1) {
						if (bufferSpace[i + 1] == '/')
							bufferSpace[i] = '\0';
					}
				}

				// formato letterale: dest=comp;jump
				// formato binario: 111c cccc ccdd djjj
				char dest[5], comp[5], jump[5];

				// Inizializzazione delle parti di stringa
				strcpy(dest, "\0\0\0\0"); strcpy(comp, "\0\0\0\0"); strcpy(jump, "\0\0\0\0");

				// Inizializzazione segmenti della stringa
				unsigned int 
				eqIndex = strchar(bufferSpace, '='), //Indice del `=` se esiste
				scIndex = strchar(bufferSpace, ';'); //Indice del `;` se esiste

				// Discernimento struttura istruzione -> Comp esiste sempre
				//dest && jump
				//Se esistono sia un `=` che un `;` nella istruzione allora esistono simultaneamente `dest`, `comp` e `jump`
				if (charext(bufferSpace, ';') && charext(bufferSpace, '=')) {
					printf("\t|\n%d\t| %s\n\t|\n%s\n", line, bufferSpace,
					"WARNING:\nQuesta istruzione potrebbe essere errata, sicuro che sia corretta?");
					substr(dest, bufferSpace, 0, eqIndex);
					int jumpLen = 3; //Lunghezza fissa per qualsiasi `jump`
					substr(jump, bufferSpace, scIndex + 1, jumpLen);
					int compLen = scIndex - eqIndex - 1;
					substr(comp, bufferSpace, eqIndex + 1, compLen);
				} else 

				//solo dest
				//Se esiste solo un `=` la istruzione contiene solo `dest` e `comp`
				if (charext(bufferSpace, '=')) {
					substr(dest, bufferSpace, 0, eqIndex);
					int compLen = strlen(bufferSpace) - eqIndex - 1;
					substr(comp, bufferSpace, eqIndex + 1, compLen);
				} else 

				//solo jump 
				//Se esiste solo un `;` la istruzione contiene solo `comp` e `dest`
				if (charext(bufferSpace, ';')) {
					int jumpLen = 3; //Lunghezza fissa per qualsiasi `jump`
					substr(jump, bufferSpace, scIndex + 1, jumpLen);
					int compLen = scIndex - eqIndex;
					substr(comp, bufferSpace, eqIndex, compLen);
				}
				
				//Conversioni:
				char destBin[strlen(destBinary(dest))];
				strcpy(destBin, destBinary(dest));
				char compBin[strlen(compBinary(comp))];
				strcpy(compBin, compBinary(comp));
				char jumpBin[strlen(jumpBinary(jump))];
				strcpy(jumpBin, jumpBinary(jump));

				//Debugging front: Se e' stata selezionata la modalita debug il file `debug.txt` conterra' queste
				//Informazioni relative alle C-Instruction lette nel file .asm e poi convertite nel file .hack
				if (mode == 100) {
					//Informazioni generali:
					fprintf(debug,"%d | C-Instruction: %s", line, bufferSpace);
					fprintf(debug,"\n\t'=' Index: %d", eqIndex);
					fprintf(debug,"\n\t';' Index: %d", scIndex);
					//Forme letterali:
					fprintf(debug,"\n\tDest: %s", dest);
					fprintf(debug,"\n\tComp: %s", comp);
					fprintf(debug,"\n\tJump: %s", jump);
					//Forme binarie:
					fprintf(debug,"\n\tDest in Binary: %s", destBin);
					fprintf(debug,"\n\tComp in Binary: %s", compBin);
					fprintf(debug,"\n\tJump in Binary: %s", jumpBin);
					fprintf(debug,"\n\t111%s%s%s%s%d\n", compBin, destBin, jumpBin, " <- | ", debugcounter);
					debugcounter += 1;
				}

				//Error Handling: Errore 404: Lettura di mnemonica non trovata nelle tabelle di mnemoniche ammesse.
				if (!strcmp(destBin, "404")) {
					printf("\t|\n%d\t| %s\n\t|\n%s", line, bufferSpace, "Questa istruzione non e' valida.\n");
					perror("Non e' possibile che una istruzione esista senza componenete `dest` valida");
					exit(404);
				} else 
				if (!strcmp(compBin, "404")) {
					printf("\t|\n%d\t| %s\n\t|\n%s", line, bufferSpace, "Questa istruzione non e' valida.\n");
					perror("Non e' possibile che una istruzione esista senza componenete `comp` valida");
					exit(404);
				} else 
				if (!strcmp(jumpBin, "404")) {
					printf("\t|\n%d\t| %s\n\t|\n%s", line, bufferSpace, "Questa istruzione non e' valida.\n");
					perror("Non e' possibile che una istruzione esista senza componenete `jump` valida");
					exit(404);
				}
				
				// Stampa nel file la C-Instruction assemblata in binario
				fprintf(hackfile, "111%s%s%s\n", compBin, destBin, jumpBin);
				break;
			}
			case L_INST:
			{
				//Debugging string
				if (mode == 100) {
					fprintf(debug, "%d | L-Instruction: %s\n", line, bufferSpace);
				}
				break;
			}
			case COMMENT:
			{
				//Debugging string
				if (mode == 100) {
					fprintf(debug, "%d | Comment: %s\n", line, bufferSpace);
				}
				break;
			}
			case SKIP:
			{
				//Debugging string
				if (mode == 100) {
					fprintf(debug, "%d | Empty Line, Skip.\n", line);
				}
				break;
			}
			default:
				//Error Handling: Out Of Bounds, Istruzione ritornata in qualche modo differisce da quelle predisposte
				printf("Errore, OOB: linea %d", line);
				perror("Istruzione non riconosiuta, la differenziazione non e' riuscita?\nVerifica che la istruzione sia scritta correttamente.");
				exit(500);
				break;
		}
	}
	if (mode == 100) {
		tablePrint(table, debug);	
		fclose(debug);
	}
	tableDelete(table);
	fclose(asmfile);
	fclose(hackfile);
}