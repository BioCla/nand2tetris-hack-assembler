/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: stringf.h
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include "stringf.c"

//Estrae `n` caratteri da una stringa sorgente da un punto di inizio a indice `beg` e lo copia in una stringa di destinazione 
char* substr(char *destination, const char *source, int beg, int n);

//Trova l'indice dell'ultimo '.' in una stringa
int ext(char* str);

//Conta la lunghezza di una stringa
long unsigned int strlen(char *p);

//Concatena stringhe di caratteri
char* strcat(char* destination, const char* source);

//Copia una stringa di caratteri `source` in un contenitore `destination`
char* strcpy(char* destination, const char* source);

//Confronta due stringe
//ritorna: 
//0 se le stringhe sono identiche 
//>0 se la prima stringa è maggiore della seconda
//<0 altrimenti
int strcmp(const char *s1, const char *s2);

//Ritorna l'indice della prima occorrenza di un carattere in una stringa, se non esiste ritorna 0
int strchar(char *s, char c);

//Funzione booleana, ritorna 1 se esiste un carattere in una stringa, 0 se non
int charext(char *s, char c);

//Rimuove spazi bianchi, \r, \n, \t, da una stringa
void deblank(char* s);