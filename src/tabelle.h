/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: tabelle.h
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "tabelle.c"

//Converte stringe di tipo dest in binario
char *destBinary(char *destString);

//Converte stringe di tipo comp in binario
char *compBinary(char *compString);

//Converte stringe di tipo jump in binario
char *jumpBinary(char *jumpString);

//Inizializza una tabella per essere modificata
pTable tableInit(void);

//Aggiunge entrate in una tabella contente indice di registro e simbolo associato
void addEntry(pTable obj, char *symbol, int index);

//Verifica se un simbolo esiste gia' in una tabella (1) o meno (0)
int labelExt(pTable obj, char *symbol);

//Prende l'indirizzo di un simbolo associato in una tabella, restituisce -1 se non esiste
int getAddress(pTable thisObject, char *symbol);

//cancella una tabella
void tableDelete(pTable thisObject);

//Stampa la tabelle creata dal file
void tablePrint (pTable obj, FILE* file);

//Riempe una tabella con i registri preallocati per la RAM HACK
void initTable(pTable symbolTable);