#include "logical_layer.h"

/*
** ./tfs_analyze [name]
** Permet d'obtenir les informations d'un disque.
** Taille, nombre de partitions, taille de chaque partition...
*/
int main(int argc, char *argv[]){
	if(argc!=1 && argc!=2){
		fprintf(stderr, "Nombre d'arguments incorrect (%d).\n", argc);
		fprintf(stderr, "./tfs_analyze [name]\n");
		return 1;
	}
	disk_id id;
	char *name;
	name = ((argc==2)?argv[1]:"disk.tfs");
	if(start_disk(name, &id).error_id==1){
		fprintf(stderr, "Erreur lors du démarrage du disque.\n");
		return 1;
	}
	printf("--- Démarrage du disque. ---\n");
  	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, 0).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t size = read_inblock(0,b);
	uint32_t nb_part = read_inblock(1,b);
	printf("Lecture des informations.\n");
	printf(" > Nom du disque : %s\n", name);
	printf(" > Nombre de blocks : %d\n", size);
	printf(" > Nombre de Partitions : %d\n", nb_part);
	if(nb_part>0){
		uint32_t i = 0;
		for(i=0; i<nb_part; i++){
			printf(" > Partition %d : %d blocks\n", i, read_inblock(i+2,b));
			block desc_b;
			desc_b.block_block = malloc(1024);
			if(read_block(id, desc_b, get_description_block(b,i+1)).error_id==1){
				fprintf(stderr, "Erreur lors de la lecture d'un block de description.\n");
				return 1;
			}
			if(read_inblock(0,desc_b)>0){
				printf("    > La partition est formatee.\n");
			}
			else{
				printf("    > La partition n'est pas formatee.\n");
			}
		}
	}
	printf("Lecture terminée.\n");
	if(stop_disk(id).error_id==1){
		fprintf(stderr, "Erreur lors de l'arrêt du disque.\n");
		return 1;
	}
	else{
		printf("--- Arrêt du disque. ---\n");
	}
	return 0;
}
