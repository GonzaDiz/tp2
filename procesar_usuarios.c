#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "lista.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//falta ver como printer y arreglar strtok.

char* tkncount(char* str, char tkn){
	size_t counter = 0;
	size_t ndigitos = 1;
	for(size_t i=0; str[i] != '\0'; i++){
		if(str[i] == tkn){ 
			counter++;
			if(counter > 9){
				ndigitos++;
			}
		} 
	}

	char* buffer = calloc(ndigitos + 1,1);
	snprintf(buffer,ndigitos + 1,"%zu",counter);
	return buffer;
}

int main(int argc, char const *argv[]){

	// validar que se le pasa solamente un argumento al binario
	if (argc != 2) {
		fprintf(stderr, "cantidad de argumentos argumentos incorrectos");
		exit(EXIT_FAILURE);
		}

	// abrir y validar que existe el archivo
	FILE *stream;   
	stream = fopen(argv[1], "r");
	if (!stream) {
		perror("fopen");
		exit(EXIT_FAILURE);
		}



	char *line = NULL;
	char *nombreUsuario = NULL;
	char *cantidadHashtags = NULL;
	hash_t* hash = hash_crear(free);
	lista_t* lista = NULL;
	lista_t* list = NULL; 
	size_t len = 0;


	// guardar datos
	while(getline(&line, &len, stream) != -1){
		cantidadHashtags = tkncount(line,',');
		nombreUsuario = strtok(line,',');
		if(!hash_pertenece(hash,cantidadHashtags)){
			lista = lista_crear();
			lista_insertar_ultimo(lista,nombreUsuario);
			hash_guardar(hash,cantidadHashtags,lista);
		}
		else{
			list = hash_obtener(hash,cantidadHashtags);
			lista_insertar_ultimo(list,nombreUsuario);
		}
		free(nombreUsuario);
	}

	hash_iter_t* hash_iter = hash_iter_crear(hash);
	lista_iter_t* lista_iter = NULL;

	while(hash_iter_avanzar(hash_iter)){
		cantidadHashtags = hash_iter_ver_actual(hash_iter);
		printf("%s:",cantidadHashtags);
		list = hash_obtener(hash,cantidadHashtags);
		lista_iter = lista_iter_crear(list);
		while(lista_iter_avanzar(lista_iter)){
			printf(" %s,", lista_iter_ver_actual(lista_iter));
		}
		lista_iter_destruir(lista_iter);
		lista_destruir(list,free);
	}
	hash_iter_destruir(hash_iter);
	hash_destruir(hash);
	free (line);
	fclose(stream);
	exit(EXIT_SUCCESS);
}


