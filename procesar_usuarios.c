#define _GNU_SOURCE
#include "hash.h"
#include "lista.h"
#include <stdio.h>
#include <string.h>

typedef struct user{
	char* name;
	size_t* cantidad;
}user_t;

/* *****************************************************************
 *                   	FUNCIONES AUXILIARES                       *
 * *****************************************************************/

void file_a_hash(FILE* file,hash_t* hash,size_t* maxCant){
	char* line = NULL;
	char* username = NULL;
	size_t bufsize = 0;
	size_t caracteres = 0;
	size_t* canTags;
	caracteres = getline(&line, &bufsize, file);
	while (caracteres != -1 ){
		username = strtok(line,","); //Tomo al user

		if (!hash_pertenece(hash,username)){
			canTags = malloc(sizeof(size_t));
			*canTags = 0;
		}
		else canTags = hash_obtener(hash,username);
		
		while (strtok(NULL,",") != NULL)
			*canTags+=1;
		
		if (*canTags > *maxCant)
			*maxCant = *canTags;
		
		if (!hash_pertenece(hash,username))
			hash_guardar(hash,username,canTags);
		
		caracteres = getline(&line, &bufsize, file);
	}
	free(username);
}

void hash_a_vector_listas(lista_t** userTags, hash_t* hash){
	hash_iter_t* hash_iter = hash_iter_crear(hash); //iterador hash de usuarios
	user_t* user = malloc (sizeof(user_t));
	while (!hash_iter_al_final(hash_iter)){
		user->name = (char*) hash_iter_ver_actual(hash_iter);
		user->cantidad = hash_obtener(hash,user->name);
		lista_insertar_primero(userTags[*user->cantidad],user->name);
		hash_iter_avanzar(hash_iter);
	}
	free(user);
	hash_iter_destruir(hash_iter);
}

void imprimir_usuarios(lista_t* lista,size_t i){
	lista_iter_t* lista_iter = lista_iter_crear(lista);
	printf("%i: ",i);
	while(!lista_iter_al_final(lista_iter)){
		printf("%s ", (char*) lista_iter_ver_actual(lista_iter));
		lista_iter_avanzar(lista_iter);
	}	
	printf("\n");		
	lista_iter_destruir(lista_iter);
}

/* *****************************************************************
 *                       PROCESAR USUARIOS                         *
 * *****************************************************************/

int main(int argc, char const *argv[]){
	FILE* file = fopen(argv[1],"r");
	if (!file){
		return -1;
	}
	size_t maxCant = 0;
	hash_t* hash = hash_crear(free);
	if (!hash){
		fclose(file);
		return -1;
	}
	file_a_hash(file,hash,&maxCant); //Guardo cantidad de tweets por usuario en un hash.
	
	lista_t** userTags = malloc(sizeof(lista_t*) * (maxCant+1)); // Creo un vector de listas
	if (!userTags){
		free(hash);
		fclose(file);
		return -1;
	}
	for (int i = 0; i<=maxCant; i++){ // una lista por posicion
		 userTags[i] = lista_crear();
	}
	hash_a_vector_listas(userTags,hash); // Paso la cantidad de tweeets a la posicion del vector y los usuarios a las listas

	for (int i=0; i<=maxCant; i++){
		if (!lista_esta_vacia(userTags[i])){
			imprimir_usuarios(userTags[i],i); //Imprimo por pantalla los users segun i (cantidad);
		}
		lista_destruir(userTags[i],NULL);
	}
	//Libero memoria
	free(userTags);
	hash_destruir(hash);
	fclose(file);
	return 0;
}