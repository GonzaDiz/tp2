#define _GNU_SOURCE
#include "hash.h"
#include "lista.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct user{
	char* name;
	size_t* cantidad;
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
	size_t* canTags;
	size_t maxCant = 0;
	hash_t* hash = hash_crear(free);
	caracteres = getline(&line, &bufsize, file);

	while (caracteres != -1 ){
		username = strtok(line,","); //Tomo al user

		if (!hash_pertenece(hash,username)){
			canTags = malloc(sizeof(size_t));
			*canTags = 0;
		}
		else canTags = hash_obtener(hash,username);
		
		while (strtok(NULL,",") != NULL){
			*canTags+=1;
		}

		if (*canTags > maxCant){
			maxCant = *canTags;
		}

		if (!hash_pertenece(hash,username)){
			hash_guardar(hash,username,canTags);
		}
		caracteres = getline(&line, &bufsize, file);

	}
	printf("PITO PALOOOOOOOOOOOOOOOOOOOOOOOOO\n");
	printf("%d\n",maxCant );

	//Para este punto tengo todos los users en un hash y su cant de tags en un cfilter

	lista_t** userTags = malloc(sizeof(lista_t*) * (maxCant+1)); // Creo un vector con listas

	for (int i = 0; i<=maxCant; i++){ // una lista por posicion
		 userTags[i] = lista_crear();
	}

	hash_iter_t* hash_iter = hash_iter_crear(hash); //iterador hash de usuarios

	user_t* user = malloc (sizeof(user_t));
	while (!hash_iter_al_final(hash_iter)){
		user->name = (char*) hash_iter_ver_actual(hash_iter);
		//user->cantidad = cfilter_apariciones(cfilter,user->name);
		user->cantidad = hash_obtener(hash,user->name);
		lista_insertar_primero(userTags[*user->cantidad],user->name);
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
	free(username);
	fclose(file);
	return 0;
}