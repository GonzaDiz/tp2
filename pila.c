#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){

	size_t tamanio=30;
	pila_t* pila = malloc(sizeof(pila_t));

	if (pila == NULL){
		return NULL;
	}

	pila->cantidad=0;
	pila->capacidad=tamanio;
	pila->datos = malloc(pila->capacidad * sizeof(void*));

	if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }

    return pila;
}

bool pila_redimensionar(pila_t *pila,size_t tamanio_nuevo) {

    void* datos_nuevo = realloc(pila->datos, tamanio_nuevo * sizeof(void*));

    if (tamanio_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }

    pila->datos = datos_nuevo;
    pila->capacidad = tamanio_nuevo;
    return true;
}
void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){

	if (pila->cantidad > 0) return false;
	return true;
}

bool pila_apilar(pila_t *pila, void* valor){
	if (pila->cantidad >= pila->capacidad) pila_redimensionar(pila,(pila->capacidad*2));

	pila->datos[pila->cantidad]=valor;
	pila->cantidad++;
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)) return NULL;

	return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t *pila){

	if (pila_esta_vacia(pila)) return NULL;

  if (pila->cantidad <= (pila->capacidad/2)){
    pila_redimensionar(pila,pila->capacidad/2);
  }
  pila->cantidad--;
  return pila->datos[pila->cantidad];

}
