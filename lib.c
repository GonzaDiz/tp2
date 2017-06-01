#include "lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *strdup (const char *str) {
    char *copia = malloc (strlen (str) + 1);   
    if (copia == NULL) return NULL;          
    strcpy (copia,str);                      
    return copia;                            
}

int cantidad_subcadenas(const char* str,char sep, size_t largo){
	int contador = 0;
	for (int i=0;i<=largo;i++){
		if (str[i] == sep) contador++;
	}
	return contador+1; 
}

size_t cant_caract(char** strv){
	int i=0;
	size_t cantidadCaract=0;
	while (strv[i]!=NULL){
			cantidadCaract = cantidadCaract + strlen(strv[i])+1;
			i++;
	}
	return cantidadCaract;
}