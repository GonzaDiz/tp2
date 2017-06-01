#include "counting_filter.h"
#include "strutil.h"
#include "pila.h"
#include "heap.h"
#include "hash.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAMANIO 500
#define k 10


int int main(int argc, char const *argv[]){
	cfilter_t* cfilter = cfilter_crear(TAMANIO);
	char* tweet = NULL;
	size_t bufsize = 0;
	size_t read;
	
	read = getline(&tweet,&bufsize,stdin);
	char** separado;
	int i;

	while (read != -1){ // mientras pueda seguir leyendo tweets
		i = 1; // me salteo el usuario que esta en la pos 0
		separado = split(tweet,',');
		while (separado[i]!=NULL && separado[i]!='\0'){
			printf("%s\n",separado[i]);
			i++;
		}
		read = getline(&tweet,&bufsize,stdin);
	}
	free_strv(separado);
	free(tweet);
	return 0;
}