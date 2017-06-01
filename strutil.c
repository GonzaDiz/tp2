#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strutil.h"
#include "lib.h"

char** split (const char* str,char sep){
	size_t largo  = strlen(str);
	int cantidadSubcadenas = cantidad_subcadenas(str,sep,largo);
	char** strv = malloc(sizeof(char*) * (cantidadSubcadenas+1));
	if (strv == NULL) return NULL;
	char* subStrv = malloc (sizeof(char) *largo+2);
	if (subStrv==NULL) return NULL;
	int j=0,k=0,i=0;
	while (i<=cantidadSubcadenas){
		while(str[j]!=sep && j<largo){
			subStrv[k]=str[j];
			j++;
			k++;
		}
		if (i<cantidadSubcadenas){
			subStrv[k]='\0';
			strv[i]=strdup(subStrv);
		}
		else strv[i]=NULL;
		j++;
		k=0;
		i++;
		}
	free(subStrv);
	return strv;
}

char* join (char** strv, char sep){
	char* resultado = malloc(sizeof(char) * (cant_caract(strv)+1));
	if (resultado == NULL) return NULL;
	int i=0,j=0,k=0;
	while (strv[i]!=NULL){
		while (strv[i][j]!='\0'){
			resultado[k]=strv[i][j];
			k++;
			j++;
		}
		resultado[k]=sep;
		k++;
		j=0;
		i++;
	}
	if (k>=1) resultado[k-1]='\0';
	else resultado[k]='\0';
	return resultado;
}

void free_strv(char *strv[]){
	int i=0;
	while (strv[i]!=NULL){
		free(strv[i]);
		i++;
	}
	free(strv[i]);
	free(strv);
}
