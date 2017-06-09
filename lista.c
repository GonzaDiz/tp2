#include "lista.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct nodo {
	void* dato;
	void* siguiente;
} nodo_t;

struct lista{
	void* primerNodo;
	void* ultimoNodo;
	size_t tamanio;
};

struct lista_iter{
	void* nodoActual;
	void* nodoAnterior;
	lista_t* lista;
};

/***************************************
 * 		  PRIMITIVAS DE LA LISTA       *
 ***************************************/

lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	lista->primerNodo=NULL;
	lista->ultimoNodo=NULL;
	lista->tamanio=0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	if (lista->tamanio == 0) return true;
	return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return false;
	nodo->dato = dato;	
	if (lista_esta_vacia(lista)) {
		nodo->siguiente=NULL;
		lista->ultimoNodo = nodo;
	}
	else nodo->siguiente = lista->primerNodo;
	lista->primerNodo = nodo;
	lista->tamanio++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* anteultimo_nodo = lista->ultimoNodo;
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return false;
	nodo->dato = dato;
	nodo->siguiente = NULL;	
	if (lista_esta_vacia(lista)) lista->primerNodo = nodo;
	else anteultimo_nodo->siguiente = nodo;
	lista->ultimoNodo = nodo;
	lista->tamanio++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	nodo_t* nodo = lista->primerNodo;
	void* valor = nodo->dato;
	lista->primerNodo = nodo->siguiente;
	lista->tamanio--;
	free(nodo);
	return valor;
}

void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	nodo_t* nodo = lista->primerNodo;
	return nodo->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)) return NULL;
	nodo_t* nodo = lista->ultimoNodo;
	return nodo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->tamanio;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){

	while(!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if ((destruir_dato)) destruir_dato(dato);
	}
	free(lista);
}

/***************************************
 * 		  PRIMITIVAS DE ITERACION      *
 ***************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;
	if (lista_esta_vacia(lista)){
		iter->nodoActual = lista->ultimoNodo;
	}
	else iter->nodoActual = lista->primerNodo;
	
	iter->nodoAnterior = NULL;
	iter->lista = lista;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (lista_iter_al_final(iter)) return false;
		nodo_t* nodo = iter->nodoActual;
		iter->nodoAnterior = nodo;
		iter->nodoActual = nodo->siguiente;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (lista_iter_al_final(iter) || lista_esta_vacia(iter->lista)) return NULL;
	nodo_t* nodo = iter->nodoActual;
	void* valor = nodo->dato;
	return valor;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->nodoActual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if (lista_iter_al_final(iter)){
		if (lista_insertar_ultimo(iter->lista,dato)){
			iter->nodoAnterior = iter->nodoActual;
			iter->nodoActual = iter->lista->ultimoNodo;
			return true;
		}
	}
	else if (iter->nodoActual == iter->lista->primerNodo){
			if (lista_insertar_primero(iter->lista,dato)){
				iter->nodoActual = iter->lista->primerNodo;
				iter->nodoAnterior = NULL;
				return true;
			}
	}
	
	nodo_t* nodo = malloc (sizeof(nodo_t));
	if (nodo == NULL) return false;
	nodo_t* nodoAnterior = iter->nodoAnterior;
	nodo->dato = dato;
	nodo->siguiente = iter->nodoActual;
	nodoAnterior->siguiente = nodo;
	iter->nodoActual = nodo;
	iter->lista->tamanio++;
	return true;	
}

void *lista_iter_borrar(lista_iter_t *iter){
	if (lista_esta_vacia(iter->lista)){
		return NULL;
	}
	else if (lista_iter_al_final(iter)){
		return NULL;
	}
	else if (iter->nodoActual == iter->lista->primerNodo){
		void* dato = lista_iter_ver_actual(iter);
		lista_borrar_primero(iter->lista);
		iter->nodoActual = iter->lista->primerNodo;
		return dato;;
	}
	else if (iter->nodoActual == iter->lista->ultimoNodo){
		nodo_t* nodo = iter->nodoAnterior;
		void* dato = lista_iter_ver_actual(iter);
		nodo->siguiente = NULL;
		iter->lista->ultimoNodo = nodo;
		iter->lista->tamanio--;
		free(iter->nodoActual);
		iter->nodoActual = nodo->siguiente;
		return dato;
	}
	else {
		nodo_t* nodo = iter->nodoActual;
		nodo_t* nodoAnterior = iter->nodoAnterior;
		void* dato = lista_iter_ver_actual(iter);
		nodoAnterior->siguiente = nodo->siguiente; // El siguiente del nodo anterior es el siguiente del nodo actual.
		iter->nodoActual = nodo->siguiente;//El nodo actual es el nodo siguiente.
		iter->lista->tamanio--;
		free(nodo);
		return dato;
	}
	return NULL;
}

/***************************************
 * 		 PRIMITIVAS ITERADOR INTERNO   *
 ***************************************/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra){
	nodo_t* nodoActual = lista->primerNodo;
	while (nodoActual!=NULL){
		if (!visitar(nodoActual->dato,extra)) return;
		nodoActual = nodoActual->siguiente;
	}
}

