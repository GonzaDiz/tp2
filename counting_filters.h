#ifndef COUNTING_FILTERS_H
#define COUNTING_FILTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

struct cfilter;
typedef struct cfilter cfilter_t;

//Crea un counting filter
//Pre: se le debe pasar un tamanio para crear el array con los contadores
//Post: el counting filter ha sido creado
cfilter_t* cfilter_crear (size_t tam);

//Al pasarle la cadena a las funciones de hashing etas me devuelven K posiciones
//k dependiendo de cuantas funciones de hashing tenga, luego aumenta los contadores
//que se encuentran en las posiciones devueltas.
//Pre: cfilter fue creado
//Post: aumenta los contadores del cfilter.
bool cfilter_aumentar (cfilter_t* cfilter, const char* str);


#endif