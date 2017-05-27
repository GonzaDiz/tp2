#ifndef COUNTING_FILTERS_H
#define COUNTING_FILTERS_H

typedef struct cFilter cfilter_t;

//Crea un counting filter
//Pre: se le debe pasar un tamanio para crear el array con los contadores
//Post: el counting filter ha sido creado
cfilter_t* cfilter_crear (size_t tam);


#endif