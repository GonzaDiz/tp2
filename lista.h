#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* lista enlazada */
struct lista;
typedef struct lista lista_t;

/* iterador externo */
struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un elemento al principio de la lista, devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se inserta el dato al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void* dato);

// Inserta un elemento al final de la lista, devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se inserta el dato al final de la lista
bool lista_insertar_ultimo(lista_t *lista, void* dato);

// Elimina el primero elemento de lista y devuelve su valor
// Pre: la lista fue creada y tiene al menos un elemento
// Post: se elimino y devolvio el primero elemento, 
// en caso de no tener ningun elemento devuelve NULL
void* lista_borrar_primero(lista_t* lista);

// Retorna el primero elemento de la lista
// Pre: la lista fue creada y tiene al menos un elemento
// Post: devuelve el primero elemento, en caso de estar vacia devuelve NULL
void* lista_ver_primero(const lista_t* lista);

// Retorna el ultimo elemento de la lista
// Pre: la lista fue creada y tiene al menos un elemento
// Post: devuelve el ultimo elemento, en caso de estar vacia devuelve NULL
void* lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad total de elementos en la lista
// Pre: la lista fue creada 
size_t lista_largo(const lista_t* lista);

// Destruye todos los nodos de la lista
// Pre: la lista fue creada
// Post: libera todos los nodos de memoria
void lista_destruir(lista_t* lista, void (*destruir_dato)(void*));

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea el iterador externo de la lista
// Post: se creo el iterador 
lista_iter_t* lista_iter_crear(lista_t* lista);

// Mueve en 1 posicion al iterador, devuelve false en caso de error 
// y true en caso que la operacion se realizo de forma correcta
// Pre: el iterador fue creado
// Post: el iterador se movio 1 posicion en la lista
bool lista_iter_avanzar(lista_iter_t *iter);

// Muestra el elemento del nodo en el que el iterador esta posicionado
// Pre: el iterador fue creado
void* lista_iter_ver_actual(const lista_iter_t *iter);

// Lleva el iterador hasta el ultimo elemento
// Pre: el iterador fue creado
// Post: el iterador esta posicionado sobre el ultimo elemento
bool lista_iter_al_final(const lista_iter_t *iter);
// Libera la memoria ocupada por el iterador

// Pre: el iterador fue creado
// Post: el iterador es destruido
void lista_iter_destruir(lista_iter_t *iter);

// inserta un elemento en la lista
// Pre: la lista y el iterador fueron creados
// Post: la lista tiene un elemento mas
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// borra un elemento de la lista
// Pre: la lista y el iterador fueron creados
// Post: la lista tiene un elemento menos
void* lista_iter_borrar(lista_iter_t *iter);

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en cola.h
// (esto es, las pruebas no pueden acceder a los miembros del struct cola).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);






#endif // LISTA_H


