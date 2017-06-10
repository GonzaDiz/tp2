#include "cmsketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#define TAMANIO 14000

/******************************************************
 *                 ESTRUCTURAS 					   	  *
 *****************************************************/

struct cmsketch{
	size_t tam;
	int *contadores;
	int *contadores1;
	int *contadores2;
};

/******************************************************
 *              FUNCIONES DE HASHING			   	  *
 *****************************************************/
static size_t hashing (const char *str, size_t tam) {
	unsigned long hash = 5831;
    unsigned int c;
    while ((c = (*str++)))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    return hash % tam;
}

static size_t hashing1 (const char *str, size_t tam) {
    unsigned long hash = 0;
    unsigned int c;

    while ((c = (*str++)))
       hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % tam;
}

static size_t hashing2 (const char *str, size_t tam){
	unsigned long hash = 0;
	unsigned int c;

	while ((c = (*str++))){
	    hash += c;
	}
	return hash % tam;
}

/******************************************************
 *             	  COUNTING FILTER    			   	  *
 *****************************************************/

cmsketch_t* cmsketch_crear  (){
	cmsketch_t* cmsketch = malloc(sizeof(cmsketch_t)); 
	if (!cmsketch) return NULL;
	cmsketch->tam = TAMANIO;
	int* contadores = malloc(sizeof(int) * TAMANIO);
	if (!contadores){
		free(cmsketch);
		return NULL;
	}
	int* contadores1 = malloc(sizeof(int) * TAMANIO);
	if (!contadores1){
		free(contadores);
		free(cmsketch);
		return NULL;
	}
	int* contadores2 = malloc(sizeof(int) * TAMANIO);
	if (!contadores2){
		free(contadores1);
		free(contadores);
		free(cmsketch);
		return NULL;
	}
	cmsketch->contadores = contadores;
	cmsketch->contadores1 = contadores1;
	cmsketch->contadores2 = contadores2;
	for (int i=0; i<TAMANIO; i++){
		cmsketch->contadores[i] = 0;
		cmsketch->contadores1[i] = 0;
		cmsketch->contadores2[i] = 0;
	}
	return cmsketch;
}

void cmsketch_aumentar (cmsketch_t* cmsketch,const char* str){
	cmsketch->contadores[hashing(str,cmsketch->tam)]++;
	cmsketch->contadores1[hashing1(str,cmsketch->tam)]++;
	cmsketch->contadores2[hashing2(str,cmsketch->tam)]++;
}

size_t cmsketch_apariciones (cmsketch_t* cmsketch, const char* str){
	size_t apariciones = 0;
	apariciones = cmsketch->contadores[hashing(str,cmsketch->tam)];
	if (apariciones > cmsketch->contadores1[hashing1(str,cmsketch->tam)]) apariciones = cmsketch->contadores1[hashing1(str,cmsketch->tam)];
	if (apariciones > cmsketch->contadores2[hashing2(str,cmsketch->tam)]) apariciones = cmsketch->contadores2[hashing2(str,cmsketch->tam)];
	return apariciones;
}

void cmsketch_destruir (cmsketch_t* cmsketch){
	free(cmsketch->contadores2);
	free(cmsketch->contadores1);
	free(cmsketch->contadores);
	free(cmsketch);
}