#include "logical_layer.h"

/*
** ./tfs_partition -p size [size] ... [name]
** Permet de partitionner l'espace physique du disque
** en paquets de blocs.
*/
int main(int argc, char *argv[]){
	if(argc<3){
		printf("Nombre d'arguments incorrect (%d).\n", argc);
		printf("./tfs_partition -p size [size] ... [name]\n");
		return 1;
	}
	else if(strcmp(argv[1],"-p")!=0){
		printf("Option invalide : (%s).\n", argv[1]);
		printf("./tfs_partition -p size [size] ... [name]\n");
		return 2;
	}

	return 0;
}
