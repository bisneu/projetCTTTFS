#include "ttfs_volume.h"

/*
** ./tfs_format -p partition -mf file_count [disk]
** Permet de formater une partition du disque
** au format TFS.
*/
int main(int argc, char **argv){
	if((argc<5)||(argc>6)){
		fprintf(stderr, "Nombre d'arguments incorrect (%d).\n", argc);
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(strcmp(argv[1],"-p")!=0){
		fprintf(stderr, "Option invalide : (%s).\n", argv[1]);
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(strcmp(argv[3],"-mf")!=0){
		fprintf(stderr, "Option invalide : (%s).\n", argv[3]);
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(atoi(argv[2])<1){
		fprintf(stderr, "Nombre attendu après \"-p\".\n");
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(atoi(argv[4])<1){
		fprintf(stderr, "Nombre attendu après \"-mf\".\n");
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	char *name;
	name = ((argc==6)?argv[5]:"disk.tfs");
	disk_id id;
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
	if(read_inblock(1,b)<atoi(argv[3])){
		fprintf(stderr, "Erreur, partition %s introuvable.\n", argv[3]);
		return 1;
	}
	printf("Formatage du disque en cours.\n");
/*------------------------------------ DEBUT FORMATAGE ------------------------------------*/
	block b2;
	b2.block_block = malloc(1024);
	error rep = read_block(id,b2,first_block_partition(b,atoi(argv[2])));
	if(rep.error_id == 1){
		fprintf(stderr, "Erreur lors du partitionnage du disque.");
		return 1;
	}
	initiate_description_block(b,b2,atoi(argv[2]),atoi(argv[4]));
	write_block(id,b2,first_block_partition(b,atoi(argv[2])));
	initiate_file_table(id,b,atoi(argv[4]),atoi(argv[2]));
	free(b.block_block);	
	free(b2.block_block);	
/*------------------------------------  FIN FORMATAGE  ------------------------------------*/
	printf("Formatage terminé.\n");
	if(stop_disk(id).error_id==1){
		fprintf(stderr, "Erreur lors de l'arrêt du disque.\n");
		return 1;
	}
	else{
		printf("--- Arrêt du disque. ---\n");
	}
	return 0;
}
