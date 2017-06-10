#ifndef CMSKETCH_H
#define CMSKETCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

struct cmsketch;
typedef struct cmsketch cmsketch_t;

//Crea un counting filter
//Post: el counting filter ha sido creado
cmsketch_t* cmsketch_crear ();

//Al pasarle la cadena a las funciones de hashing etas me devuelven K posiciones
//k dependiendo de cuantas funciones de hashing tenga, luego aumenta los contadores
//que se encuentran en las posiciones devueltas.
//Pre: cmsketch fue creado
//Post: aumenta los contadores del cmsketch.
void cmsketch_aumentar (cmsketch_t* cmsketch, const char* str);

//Pre: cmsketch creado
//Post: devuelve la cantidad de apariciones aproximadas del string en el cmsketch.
size_t cmsketch_apariciones (cmsketch_t* cmsketch, const char* str);

//Pre: cmsketch creado
//Post: destruye el cmsketch
void cmsketch_destruir (cmsketch_t* cmsketch);

#endif