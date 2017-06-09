#include "counting_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/******************************************************
 *                 ESTRUCTURAS 					   	  *
 *****************************************************/

struct cfilter{
	size_t tam;
	int *contadores;
};

/******************************************************
 *              FUNCIONES DE HASHING			   	  *
 *****************************************************/
size_t hashing1 (const char *str, size_t tam) {
	unsigned long hash = 5831;
    unsigned int c;
    while ((c = (*str++)))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    return hash % tam;
}

size_t hashing2 (const char *str, size_t tam) {
    unsigned long hash = 0;
    unsigned int c;

    while ((c = (*str++)))
       hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % tam;
}

size_t hashing3 (const char *str, size_t tam){
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

cfilter_t* cfilter_crear  (size_t tam){
	cfilter_t* cfilter = malloc(sizeof(cfilter_t)); 
	if (!cfilter) return NULL;
	cfilter->tam = tam;
	int* contadores = malloc(sizeof(int) * tam);
	if (!contadores){
		free(cfilter);
		return NULL;
	}
	cfilter->contadores = contadores;
	for (int i=0; i<tam; i++){
		cfilter->contadores[i] = 0;
	}
	return cfilter;
}

void cfilter_aumentar (cfilter_t* cfilter,const char* str){
	cfilter->contadores[hashing1(str,cfilter->tam)]++;
	cfilter->contadores[hashing2(str,cfilter->tam)]++;
	cfilter->contadores[hashing3(str,cfilter->tam)]++;
}

size_t cfilter_apariciones (cfilter_t* cfilter, const char* str){
	size_t apariciones = 0;
	apariciones = cfilter->contadores[hashing1(str,cfilter->tam)];
	if (apariciones > cfilter->contadores[hashing2(str,cfilter->tam)]) apariciones = cfilter->contadores[hashing2(str,cfilter->tam)];
	if (apariciones > cfilter->contadores[hashing3(str,cfilter->tam)]) apariciones = cfilter->contadores[hashing3(str,cfilter->tam)];
	return apariciones;
}

void cfilter_destruir (cfilter_t* cfilter){
	free(cfilter->contadores);
	free(cfilter);
}