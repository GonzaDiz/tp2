#define _GNU_SOURCE
#include "hash.h"
#include "lib.h"
#include "counting_filter.h"
#include "lista.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANIO 400000

typedef struct user{
	char* name;
	size_t cantidad;
}user_t;

int main(int argc, char const *argv[]){
	FILE* file = fopen(argv[1],"r");
	if (!file){
		return -1;
	}

	char* line = NULL;
	char* username = NULL;
	size_t bufsize = 0;
	size_t caracteres = 0;
	size_t cantUsers = 0;
	int maxCant = 0;
	cfilter_t* cfilter = cfilter_crear(TAMANIO);
	hash_t* hash = hash_crear(NULL);
	caracteres = getline(&line, &bufsize, file);

	while (caracteres != -1 ){
		username = strtok(line,","); //Tomo al user
		while (strtok(NULL,",") != NULL){ // Cuento cantidad de tweets
			cfilter_aumentar(cfilter,username); // Guardo la cantidad en el cfilter
		}

		if (cfilter_apariciones(cfilter,username) > maxCant){
			maxCant = cfilter_apariciones(cfilter,username);
		}

		if (!hash_pertenece(hash,username)){ // Guardo el user en un hash
			hash_guardar(hash,username,username);
			cantUsers++;
		}
		caracteres = getline(&line, &bufsize, file);

	}

	//Para este punto tengo todos los users en un hash y su cant de tags en un cfilter

	lista_t** userTags = malloc(sizeof(lista_t*) * (maxCant+1)); // Creo un vector con listas

	for (int i = 0; i<=maxCant; i++){ // una lista por posicion
		 userTags[i] = lista_crear();
	}

	hash_iter_t* hash_iter = hash_iter_crear(hash); //iterador hash de usuarios

	user_t* user = malloc (sizeof(user_t));
	while (!hash_iter_al_final(hash_iter)){
		user->name = (char*) hash_iter_ver_actual(hash_iter);
		user->cantidad = cfilter_apariciones(cfilter,user->name);
		lista_insertar_primero(userTags[user->cantidad],user->name);
		hash_iter_avanzar(hash_iter);
	}
	hash_iter_destruir(hash_iter);

	for (int i=0; i<=maxCant; i++){
		if (!lista_esta_vacia(userTags[i])){
			lista_iter_t* lista_iter = lista_iter_crear(userTags[i]);
			printf("%i: ",i);
			while(!lista_iter_al_final(lista_iter)){
				printf("%s ", (char*) lista_iter_ver_actual(lista_iter));
				lista_iter_avanzar(lista_iter);
			}	
			printf("\n");		
			lista_iter_destruir(lista_iter);
		}
		lista_destruir(userTags[i],NULL);
	}

	free(userTags);
	free(user);
	hash_destruir(hash);
	cfilter_destruir(cfilter);
	free(username);
	fclose(file);
	return 0;
}