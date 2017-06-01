#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAM_INICIAL 199

// Definicion de heap
struct heap{
	cmp_func_t comparar;
	void** datos;
	size_t cantidad;
	size_t tamanio;
};

/* *****************************************************************
 *                    AUXILIARES DEL HEAP                          *
 * *****************************************************************/

bool heap_redimensionar(heap_t* heap, size_t tamanio_nuevo) {
    void* datos_nuevo = realloc(heap->datos, tamanio_nuevo * sizeof(void*));
    if (tamanio_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }
    heap->datos = datos_nuevo;
    heap->tamanio = tamanio_nuevo;
    return true;
}

void swap (void** dato1, void** dato2){
	void* auxiliar = *dato1;
	*dato1 = *dato2;
	*dato2 = auxiliar;
	return;
}

void upheap (void *arreglo[], size_t pos, cmp_func_t comparar){
	if (pos == 0) return;
	size_t padre = (pos-1)/2;
	if (comparar(arreglo[padre],arreglo[pos])<0){
		swap(&arreglo[padre],&arreglo[pos]);
		upheap(arreglo,padre,comparar);
	}
}

void downheap (void *arreglo[], size_t cantidad, size_t pos, cmp_func_t comparar){
	if (pos>=cantidad) return;
	size_t max = pos;
	size_t hijo_izq = (2*pos) +1;
	size_t hijo_der = (2*pos) +2;
	if (hijo_izq <cantidad && comparar(arreglo[hijo_izq],arreglo[max])>0) max = hijo_izq;
	if (hijo_der <cantidad && comparar(arreglo[hijo_der],arreglo[max])>0) max = hijo_der;
	if (max!= pos){
		swap(&arreglo[max],&arreglo[pos]);
		downheap(arreglo,cantidad,max,comparar);
	}
}

void heapify(void *arreglo[], size_t cantidad, cmp_func_t comparar){
	for (int i = (int) cantidad; i>=0; i--){
		downheap(arreglo,cantidad,i,comparar);
	}
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP                          *
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;
	void* datos = malloc (sizeof(void*) * TAM_INICIAL);
	if (!datos){
		free(heap);
		return NULL;
	}
	heap->comparar = cmp;
	heap->datos = datos;
	heap->cantidad = 0;
	heap->tamanio = TAM_INICIAL;
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = heap_crear(cmp);
	if (!heap) return NULL;
	if (n >= (heap->tamanio - (heap->tamanio/2))) heap_redimensionar(heap,heap->tamanio *2);
	for (int i = 0; i<n; i++){
		heap->datos[i] = arreglo [i];
		heap->cantidad++;
	}
	heapify(heap->datos,heap->cantidad,heap->comparar); 
	return heap;                                                                                                                                  
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	for (int i=0; i<heap->cantidad; i++){
		if (destruir_elemento && (heap->datos[i]!=NULL)) destruir_elemento(heap->datos[i]);
	}
	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	if (heap_cantidad(heap) == 0) return true;
	return false;
}

bool heap_encolar(heap_t *heap, void *elem){
	if (elem == NULL || !heap) return false;
	if (heap->cantidad >= (heap->tamanio-(heap->tamanio/2))) heap_redimensionar(heap,heap->tamanio *2);
	heap->datos[heap->cantidad] = elem;
	upheap(heap->datos,heap->cantidad,heap->comparar);
	heap->cantidad++;
	return true;
}

void *heap_ver_max(const heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
	if (heap_esta_vacio(heap) || !heap) return NULL;
	void* max = heap->datos[0];
	heap->datos[0] = heap->datos[heap->cantidad -1];
	heap->datos[heap->cantidad-1] = NULL;
	heap->cantidad--;
	downheap(heap->datos,heap->cantidad,0,heap->comparar);
	return max;
}

/* *****************************************************************
 *                    		HEAP SORT                              *
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos,cant,cmp);
	while (cant > 0){
		cant--;
		swap(&elementos[0],&elementos[cant]);
		downheap(elementos,cant,0,cmp);
	}
}