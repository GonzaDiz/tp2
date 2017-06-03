#define _GNU_SOURCE
#include "lib.h"
#include "counting_filters.h"
#include "strutil.h"
#include "pila.h"
#include "heap.h"
#include "hash.h"
#include <stdio.h>
#include <stdbool.h>

#define TAMANIO 400000

/* *****************************************************************
 *                   	FUNCIONES AUXILIARES                       *
 * *****************************************************************/

int cmp (topic_t* a,topic_t* b){
	if (a->apariciones < b->apariciones) return 1;
	if (a->apariciones == b->apariciones) return 0;
	return -1;
}

/* *****************************************************************
 *                       PROCESAR TWEETS                            *
 * *****************************************************************/
int main(int argc, char const *argv[]){

	size_t n = atoi(argv[1]); // Cantidad de tweets  a leer
	size_t k = atoi(argv[2]); // Cantidad de TTs a imprimir

	char* tweet = NULL;
	size_t bufsize = 0;
	size_t caracteres;
	
	caracteres = getline(&tweet,&bufsize,stdin);
	tweet[strlen(tweet)-1] = '\0'; //haciendo esto eliminamos el salto de linea que toma getline

	char** separado;
	int i;

	while (caracteres != -1){


		size_t contador = 0;

		cfilter_t* cfilter = cfilter_crear(TAMANIO);
		hash_t * hash = hash_crear(NULL);

		/*Aca se leen todos los tweets y se calculan la cantidad de apariciones aproximadas,
		se guardan los distintos tweets en un hash*/
		while (caracteres != -1 && contador < n){ // mientras pueda seguir leyendo tweets
			i = 1; // me salteo el usuario que esta en la pos 0
			separado = split(tweet,',');
			while (separado[i]!=NULL && separado[i]!='\0'){
				cfilter_aumentar (cfilter,separado[i]);
				if (!hash_pertenece(hash,separado[i])){
					hash_guardar(hash,separado[i],separado[i]);
				}
				i++;
			}
			caracteres = getline(&tweet,&bufsize,stdin);
			tweet[strlen(tweet)-1] = '\0';
			contador++;
		}


		//En este punto tengo un hash con todos los hashtags sin repetir y un cfilter con las apariciones.
		hash_iter_t* hash_iter = hash_iter_crear(hash);
		heap_t* heap = heap_crear(cmp);


		for (int i=0; i<k; i++){
			topic_t* topic = malloc(sizeof(topic_t));
			topic->hashtag = strdup((char*) hash_iter_ver_actual(hash_iter));
			topic->apariciones = cfilter_apariciones(cfilter,topic->hashtag);
			heap_encolar(heap,topic);
			if (!hash_iter_avanzar(hash_iter) || hash_iter_al_final(hash_iter)) break;
		}


		contador = k;
		while (!hash_iter_al_final(hash_iter) && contador < n ){
			topic_t* topic = malloc(sizeof(topic_t));
			topic->hashtag = strdup((char*) hash_iter_ver_actual(hash_iter));
			topic->apariciones = cfilter_apariciones(cfilter,topic->hashtag);
			topic_t* aux = malloc(sizeof(topic_t));
			aux = heap_ver_max(heap);

			if (topic->apariciones > aux->apariciones){
				heap_desencolar(heap);
				heap_encolar(heap,topic);
			}
			hash_iter_avanzar(hash_iter);
			contador++;
		}


		pila_t* pila = pila_crear();
		for (int i=0; i<k; i++){
			if (heap_ver_max(heap) != NULL)
				pila_apilar(pila,heap_ver_max(heap));
			if (!heap_desencolar(heap)) break;
		}


		topic_t* aux = malloc(sizeof(topic_t));
		for (int i=0; i<k; i++){
			aux = pila_ver_tope(pila);
			if (aux == NULL) break;
			printf("%i. %s Ocurrencias: %i\n",i+1,aux->hashtag,aux->apariciones);
			if (!pila_desapilar(pila) || pila_esta_vacia(pila)) break;
		}
		printf("\n");
	}
	return 0;
}