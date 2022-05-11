/* 
 *  Author: Brian Ferri #0001040624
 *  
 *  Nome/Descrizione: stringf.c
 *  
 *  INF-UNIBO 2021-2022
 *  Primo anno del Corso di laurea in Informatica
 */

#include <stdio.h>
#include <stdlib.h>

//Estrae `n` caratteri da una stringa sorgente da un punto di inizio a indice `beg` e lo copia in una stringa di destinazione 
char* substr(char *destination, const char *source, int beg, int n) {
	while (n > 0) {
		*destination = *(source + beg);
		destination++;
		source++;
		n--;
	}
	*destination = '\0'; // termina la stringa con `\0`
	return destination; // ritorna la stringa di destinazione
}

//Trova l'indice dell'ultimo '.' in una stringa
int ext(char* str) {
	int dot;
	for (int i = 0; *str; i++) {
		if (*str == '.') {
			dot = i;
			str++;
		} else 
			str++;
	}
	return dot;
}

//Conta la lunghezza di una stringa
long unsigned int strlen(char *p) {
	long unsigned int count = 0;
	while(*p!='\0') {
		count++;
		p++;
	}
	return count;
}

//Concatena stringhe di caratteri
char* strcat(char* destination, const char* source) {
	// faccio puntare `ptr` alla fine della stringa di destinazione
	char* ptr = destination + strlen(destination);
	// aggiungio i singoli caratteri della stringa `source` a `destinazione`
	while (*source != '\0') {
		*ptr++ = *source++;
	}
	*ptr = '\0';
	return destination;
}

//Copia una stringa di caratteri `source` in un contenitore `destination`
char* strcpy(char* destination, const char* source) {
	// ritorna se non e' stata allocata memoria alla destinazione
	if (destination == NULL) {
		return NULL;
	}
	// puntatore punta all'inizio della stringa di destinazione
	char *ptr = destination;
	// copia la stringa puntata da `source` nell'array
	// puntato da destination
	while (*source != '\0') {
		*destination = *source;
		destination++;
		source++;
	}
	*destination = '\0';
	return ptr;
}

//Confronta due stringe
//ritorna: 
//0 se le stringhe sono identiche 
//>0 se la prima stringa è maggiore della seconda
//<0 altrimenti
int strcmp(const char *s1, const char *s2) {
	const unsigned char *p1 = (const unsigned char *)s1;
	const unsigned char *p2 = (const unsigned char *)s2;
	while (*p1 && (*p1 == *p2)) 
		++p1, ++p2;
	return (*p1 > *p2) - (*p2 > *p1);
}

//Ritorna l'indice della prima occorrenza di un carattere in una stringa, se non esiste ritorna 0
int strchar(char *s, char c) {
	int count = 0; 
	while(*s != c && *s != '\0') {
		s++;
		count++;
	}
	if (*s == '\0')
		return 0;
	return count;
}

//Funzione booleana, ritorna 1 se esiste un carattere in una stringa, 0 se non
int charext(char *s, char c) {
	while (*s != '\0') {
		if (*s == c)
			return 1;
		s++;
	}
	return 0;
}

//Rimuove spazi bianchi e \t, da una stringa
void deblank(char* s) {
	int  i, k=0;
	for(i = 0; s[i]; i++) {
		s[i] = s[i + k];	
		if(s[i] == ' '|| s[i] == '\t') {
			k++;
			i--;
		}
	}
}