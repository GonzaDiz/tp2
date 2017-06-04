#define _GNU_SOURCE
#include "lib.h"
#include "counting_filters.h"
#include "strutil.h"
#include "pila.h"
#include "heap.h"
#include "hash.h"
#include <stdio.h>

#define TAMANIO 400000

/* *****************************************************************
 *                   	FUNCIONES AUXILIARES                       *
 * *****************************************************************/

int cmp (topic_t* a,topic_t* b){
	if (a->apariciones < b->apariciones) return 1;
	if (a->apariciones == b->apariciones) return 0;
	return -1;
}

void hash_a_heap(hash_iter_t* hash_iter, heap_t* heap, size_t k, size_t n, cfilter_t* cfilter){
	for (int i=0; i<k; i++){
		topic_t* topic = malloc(sizeof(topic_t));
		topic->hashtag = strdup((char*) hash_iter_ver_actual(hash_iter));
		topic->apariciones = cfilter_apariciones(cfilter,topic->hashtag);
		heap_encolar(heap,topic);
		if (!hash_iter_avanzar(hash_iter) || hash_iter_al_final(hash_iter)) break;
	}

	size_t contador = k;
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
}

void heap_a_pila(size_t k, heap_t* heap, pila_t* pila){
	for (int i=0; i<k; i++){
		if (heap_ver_max(heap) != NULL)
			pila_apilar(pila,heap_ver_max(heap));
		if (!heap_desencolar(heap)) break;
	}
}

void imprimir_tts(topic_t* aux, size_t k, pila_t* pila){
	for (int i=0; i<k; i++){
		aux = pila_ver_tope(pila);
		if (aux == NULL) break;
		printf("%i. %s \n",i+1,aux->hashtag);
		if (!pila_desapilar(pila) || pila_esta_vacia(pila)) break;
	}
}

/* *****************************************************************
 *                       PROCESAR TWEETS                            *
 * *****************************************************************/
int main(int argc, char const *argv[]){

	size_t n = atoi(argv[1]); // Cantidad de tweets  a leer
	size_t k = atoi(argv[2]); // Cantidad de TTs a imprimir
	if (!n || !k){
		printf("Algo no salio como se esperaba\n");
		return -1;
	}

	char* tweet = NULL;
	size_t bufsize = 0;
	size_t caracteres;
	caracteres = getline(&tweet,&bufsize,stdin);
	tweet[strlen(tweet)-1] = '\0'; //eliminamos el salto de linea que toma getline
	
	int i;
	while (caracteres != -1){
		size_t contador = 0;

		cfilter_t* cfilter = cfilter_crear(TAMANIO);
		hash_t * hash = hash_crear(NULL);

		while (caracteres != -1 && contador < n){ // mientras pueda seguir leyendo tweets
			i = 1; // me salteo el usuario que esta en la pos 0
			char** separado = split(tweet,',');
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
			free_strv(separado);
		}
		//Tengo un hash con los topics sin repetir y un cfilter con las ocurrencias de cada topic
		//Lo paso a un heap
		hash_iter_t* hash_iter = hash_iter_crear(hash);
		heap_t* heap = heap_crear(cmp);
		hash_a_heap(hash_iter,heap,k,n,cfilter);
		cfilter_destruir(cfilter); //Ya no lo necesito

		//Ahora lo paso a una pila para que me quede en orden
		pila_t* pila = pila_crear();
		heap_a_pila(k,heap,pila);

		//Imprimo los tts
		topic_t* aux = malloc(sizeof(topic_t));
		imprimir_tts(aux,k,pila);

		//Libero Memoria
		free(aux);
		pila_destruir(pila);
		heap_destruir(heap,NULL);
		hash_iter_destruir(hash_iter);
		hash_destruir(hash);
		printf("\n");
	}
	return 0;
}