#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/******************************************************
  *             PRIMITIVAS BASICAS        			  *
  *****************************************************/

//Crea una lista
//Post: devuelve una lista si no puede devuelve NULL.
lista_t* lista_crear(void);

//Pre: la lista fue creada
//Post: devuelve true si la lista esta bacia o false si no lo esta
bool lista_esta_vacia(const lista_t *lista);

//Pre: la lista fue creada
//Post: inserta en la primera posicion de la lista el dato recibido. Devuelve true si lo pudo insertar.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Pre: la lista fue creada
//Post: inserta en la ultima posicion de la lista el dato redibido. Devuelve true si lo pudo insertar.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Pre: la lista fue creada
//Post: borra el primer dato de la lista cambiando la referencia del primer elemento por el segundo, devuelve el valor borrado.
void *lista_borrar_primero(lista_t *lista);

//Pre: la lista fue creada
//Post: devuelve el valor del primer dato de la lista en caso de no estar vacia. Si esta vacia devuelve NULL.
void *lista_ver_primero(const lista_t *lista);

//Pre: la lista fue creada
//Post: devuelve el valor del ultimo dato de la lista en caso de no estar vacia, Si esta vacia devuelve NULL.
void *lista_ver_ultimo(const lista_t* lista);

//Pre: la lista fue creada
//Post: devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

//Pre: la lista fue creada
//Post: los datos de la lista fueron eliminados.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/******************************************************
  *             PRIMITIVAS DE ITERACION      		  *
  *****************************************************/


//Pre: hay una lista creada.
//Post: Devuelve el iterador si lo pudo crear si no devuelve NULL.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Pre: el iterador fue creado.
//Post: si el iterador no esta al final, avanza una posicion y devuelve true, false en caso de estar al final.
bool lista_iter_avanzar(lista_iter_t *iter);

//Pre: el iterador fue creado.
//Post: si esta al final del iterador devuelve NULL, en caso contrario devuelve el valor sobre el que esta posicionado.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Pre: el iterador fue creado.
//Post: devuelve true si el iterador esta al final, false si no lo esta.
bool lista_iter_al_final(const lista_iter_t *iter);

//Pre: el iterador fue creado.
//Post: el iterador fue destruido.
void lista_iter_destruir(lista_iter_t *iter);

//Pre: la lista fue creada y el iterador tambien.
//Post: inserta en la posicion actual del iterador el dato devuelve true, falso si no lo puede insertar.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Pre: el iterador fue creado.
//Post: elimina el elemento de la lista en el que el iterador esta posicionado.
void *lista_iter_borrar(lista_iter_t *iter);

/******************************************************
  *             PRIMITIVAS ITERADOR INTERNO   		  *
  *****************************************************/

  //Pre: la lista fue creada,

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

/*****************************************************/

void pruebas_lista_alumno(void);

#endif