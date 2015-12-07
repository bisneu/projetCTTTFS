#include "logical_layer.h"

/*
** ./tfs_analyze [name]
** Permet d'obtenir les informations d'un disque.
** Taille, nombre de partitions, taille de chaque partition...
*/
int main(int argc, char *argv[]){
	if(argc!=1 && 2){
		printf("Nombre d'arguments incorrect (%d).\n", argc);
		printf("./tfs_analyze [name]\n");
		return 1;
	}

	return 0;
}
