#include "logical_layer.h"

/*
** ./tfs_analyze [name]
** Permet d'obtenir les informations d'un disque.
** Taille, nombre de partitions, taille de chaque partition...
*/
int main(int argc, char *argv[]){
	if(argc!=1 && argc!=2){
		printf("Nombre d'arguments incorrect (%d).\n", argc);
		printf("./tfs_analyze [name]\n");
		return 1;
	}
	disk_id id;
	char *name;
	name = (argc==4)?argv[3]:"disk.tfs";
	if(start_disk(*name, *id)==1){
		fprintf(stderr, "Erreur lors du démarrage du disque.\n");
	}
	else{
		printf("Démarrage du disque.\n");
	  	block b;
		uint32_t num = 0;
		if(read_block(id, b, num)==1){
			fprintf(stderr, "Erreur lors de la lecture du block.\n");
		}
		else{	  
			printf("Lecture du block.\n");
			printf("	Nom du disque : %s\n", name);
			printf("	Nombre de blocks : %d\n", );

			if(stop_disk(*id)==1){
				fprintf(stderr, "Erreur lors de l'arrêt du disque.\n");
			}
			else{
				printf("Arrêt du disque.\n");
			}
		}
	}
	return 0;
}
