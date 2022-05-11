/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: tabelle.c
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "stringf.h"

typedef struct sym *pSym;
typedef struct sym Sym;
struct sym {
	char symbol[256];
	int reg;
	pSym next;
};

typedef struct table *pTable;
typedef struct table Table;
struct table {
	pSym table;
};

//Converte stringe di tipo dest in binario
char *destBinary(char *destString) {
	char *destBit;
	if (!strcmp(destString, "M")) {
		destBit = "001";return destBit;}
	else if (!strcmp(destString, "D")) {
		destBit = "010";return destBit;}
	else if (!strcmp(destString, "MD")) {
		destBit = "011";return destBit;}
	else if (!strcmp(destString, "A")) {
		destBit = "100";return destBit;}
	else if (!strcmp(destString, "AM")) {
		destBit = "101";return destBit;}
	else if (!strcmp(destString, "AD")) {
		destBit = "110";return destBit;}
	else if (!strcmp(destString, "AMD")) {
		destBit = "111";return destBit;}
	else if (!strcmp(destString, "\0")) {
		destBit = "000";return destBit;}
	else {
		return "404"; //Mnemonic not found
	}
}

//Converte stringe di tipo comp in binario
char *compBinary(char *compString) {
	char *compBit;
	if (!strcmp(compString, "0")) {
		compBit = "0101010";return compBit;}
	else if (!strcmp(compString, "1")) {
		compBit = "0111111";return compBit;}
	else if (!strcmp(compString, "-1")) {
		compBit = "0111010";return compBit;}
	else if (!strcmp(compString, "D")) {
		compBit = "0001100";return compBit;}
	else if (!strcmp(compString, "A")) {
		compBit = "0110000";return compBit;}
	else if (!strcmp(compString, "M")) {
		compBit = "1110000";return compBit;}
	else if (!strcmp(compString, "!D")) {
		compBit = "0001101";return compBit;}
	else if (!strcmp(compString, "!A")) {
		compBit = "0110001";return compBit;}
	else if (!strcmp(compString, "!M")) {
		compBit = "1110001";return compBit;}
	else if (!strcmp(compString, "-D")) {
		compBit = "0001111";return compBit;}
	else if (!strcmp(compString, "-A")) {
		compBit = "0110011";return compBit;}
	else if (!strcmp(compString, "-M")) {
		compBit = "1110011";return compBit;}
	else if (!strcmp(compString, "D+1")) {
		compBit = "0011111";return compBit;}
	else if (!strcmp(compString, "A+1")) {
		compBit = "0110111";return compBit;}
	else if (!strcmp(compString, "M+1")) {
		compBit = "1110111";return compBit;}
	else if (!strcmp(compString, "D-1")) {
		compBit = "0001110";return compBit;}
	else if (!strcmp(compString, "A-1")) {
		compBit = "0110010";return compBit;}
	else if (!strcmp(compString, "M-1")) {
		compBit = "1110010";return compBit;}
	else if (!strcmp(compString, "D+A")) {
		compBit = "0000010";return compBit;}
	else if (!strcmp(compString, "D+M")) {
		compBit = "1000010";return compBit;}
	else if (!strcmp(compString, "D-A")) {
		compBit = "0010011";return compBit;}
	else if (!strcmp(compString, "D-M")) {
		compBit = "1010011";return compBit;}
	else if (!strcmp(compString, "A-D")) {
		compBit = "0000111";return compBit;}
	else if (!strcmp(compString, "M-D")) {
		compBit = "1000111";return compBit;}
	else if (!strcmp(compString, "D&A")) {
		compBit = "0000000";return compBit;}
	else if (!strcmp(compString, "D&M")) {
		compBit = "1000000";return compBit;}
	else if (!strcmp(compString, "D|A")) {
		compBit = "0010101";return compBit;}
	else if (!strcmp(compString, "D|M")) {
		compBit = "1010101";return compBit;}
	else {
		return "404"; //Mnemonic not found
	}
}

//Converte stringe di tipo jmup in binario
char *jumpBinary(char *jumpString) {
	char *jumpBit;
	if (!strcmp(jumpString, "JGT")) {
		jumpBit = "001";return jumpBit;}
	else if (!strcmp(jumpString, "JEQ")) {
		jumpBit = "010";return jumpBit;}
	else if (!strcmp(jumpString, "JGE")) {
		jumpBit = "011";return jumpBit;}
	else if (!strcmp(jumpString, "JLT")) {
		jumpBit = "100";return jumpBit;}
	else if (!strcmp(jumpString, "JNE")) {
		jumpBit = "101";return jumpBit;}
	else if (!strcmp(jumpString, "JLE")) {
		jumpBit = "110";return jumpBit;}
	else if (!strcmp(jumpString, "JMP")) {
		jumpBit = "111";return jumpBit;}
	else if (!strcmp(jumpString, "\0")) {
		jumpBit = "000";return jumpBit;}
	else {
		return "404"; //Mnemonic not found
	}
}

//Inizializza una tabella per essere modificata
pTable tableInit(void) {
    static struct table obj;
    obj.table = NULL;
    return &obj;
}

//Aggiunge entrate in una tabella contente indice di registro e simbolo associato
void addEntry(pTable obj, char *symbol, int index) {
    pSym entry = (pSym)malloc(sizeof(Sym));
    strcpy(entry->symbol, symbol);
    entry->reg = index;
    entry->next = obj->table;
    obj->table = entry;
}

//Verifica se un simbolo esiste gia' in una tabella (1) o meno (0)
int labelExt(pTable obj, char *symbol) {
    pSym entry = obj->table;
    while (entry != NULL) {
        if (strcmp(entry->symbol, symbol) == 0) {
            return 1;
        }
        entry = entry->next;
    }
    return 0;
}

//Prende l'indirizzo di un simbolo associato in una tabella, restituisce -1 se non esiste
int getAddress(pTable obj, char *symbol) {
    pSym entry = obj->table;
    while (entry != NULL) {
        if (strcmp(entry->symbol, symbol) == 0) {
            return entry->reg;
        }
        entry = entry->next;
    }
    return -1;
}

//cancella una tabella
void tableDelete(pTable obj) {
    pSym entry = obj->table;
    while (entry != NULL) {
        pSym nextEntry = entry->next;
        free(entry);
        entry = nextEntry;
    }
}

//Stampa la tabelle creata dal file
void tablePrint (pTable obj, FILE* file) {
	pSym entry = obj->table;
	fprintf(file, "\n\n%s\n===============================\n", "Symbol Table: ");
	while (entry != NULL) {
		fprintf(file, "%s%s%d\n",	entry->symbol, " <-> ", entry->reg);
		entry = entry->next;
	}
}

//Riempe una tabella con i registri preallocati per la RAM HACK
void initTable(pTable symbolTable) {
	// Label, Indirizzo RAM
    addEntry(symbolTable, "SP", 0);
    addEntry(symbolTable, "LCL", 1);
    addEntry(symbolTable, "ARG", 2);
    addEntry(symbolTable, "THIS", 3);
    addEntry(symbolTable, "THAT", 4);
    addEntry(symbolTable, "R0", 0);
    addEntry(symbolTable, "R1", 1);
    addEntry(symbolTable, "R2", 2);
    addEntry(symbolTable, "R3", 3);
    addEntry(symbolTable, "R4", 4);
    addEntry(symbolTable, "R5", 5);
    addEntry(symbolTable, "R6", 6);
    addEntry(symbolTable, "R7", 7);
    addEntry(symbolTable, "R8", 8);
    addEntry(symbolTable, "R9", 9);
    addEntry(symbolTable, "R10", 10);
    addEntry(symbolTable, "R11", 11);
    addEntry(symbolTable, "R12", 12);
    addEntry(symbolTable, "R13", 13);
    addEntry(symbolTable, "R14", 14);
    addEntry(symbolTable, "R15", 15);
    addEntry(symbolTable, "SCREEN", 16384);
    addEntry(symbolTable, "KBD", 24576);
}