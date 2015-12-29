#include "logical_layer.h"

/*
** ./tfs_partition -p size [-p size] ... [name]
** Permet de partitionner l'espace physique du disque
** en paquets de blocs.
*/
int main(int argc, char **argv){
/*********************************************************************************************
					Vérification
**********************************************************************************************/
	if(argc<3){
		fprintf(stderr, "Nombre d'arguments incorrect (%d).\n", argc);
		fprintf(stderr, "./tfs_partition -p size [-p size] ... [name]\n");
		return 1;
	}
	uint32_t mes_partitions[argc]; 	// tableau qui contiendra les partiions
	uint32_t compteur = 0; 			// compteur de partition
	int i=0;
	for(i=0; i<argc; i++){
		mes_partitions[i]=0;				
	}
	for(i=1; i<(argc-1); i++){
		if(strcmp(argv[i],"-p")!=0){
			fprintf(stderr, "Option invalide : (%s).\n", argv[i]);
			fprintf(stderr, "./tfs_partition -p size [-p size] ... [name]\n");
			return 1;
		}
		else {
			if((i+1) >= argc){
				fprintf(stderr, "Nombre attendu après \"-p\".\n");
				fprintf(stderr, "./tfs_partition -p size [-p size] ... [name]\n");
				return 1;
			}
			else {
				if(atoi(argv[i+1])<9){
					fprintf(stderr, "Taille minimale d'une partition : 9 blocks.\n");
					return 1;
				}
				else {
					mes_partitions[compteur]=atoi(argv[i+1]);	
					compteur++;	
					i++;	
				}
			}
		}
	}
	disk_id id;
	if(start_disk(((strcmp(argv[argc-2], "-p")>0)?argv[argc-1]:"disk.tfs"), &id).error_id==1){
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
	printf("Partionnage du disque en cours.\n");
	uint32_t taille_disque = read_inblock(0,b);
	uint32_t nbr_partitions = read_inblock(1,b);
	uint32_t check_space = 0;
	for(i=1; i<argc; i++){
		check_space += mes_partitions[i];
	}
	if(check_space >= taille_disque){
		fprintf(stderr,"Le disque n'a que %d blocks.\n", taille_disque);
		return 1;
	}
	
	/*******************************************************************************************************************
						Deuxième étapes écriture dans le fichier
	*******************************************************************************************************************/ 

	if(taille_disque <=(total_partition(b,(nbr_partitions))+compteur)){
		fprintf(stderr,"Partitionnage impossible : nombre de blocks disponibles insufisant.\n");
		return 1;
		/* Rajouter une option d'overwrite ! */
	}
	else{
		int j=0;
		for(j=0; j<compteur;j++){
			write_inblock(b,(2+nbr_partitions+j),mes_partitions[j]);		
		}
		write_inblock(b,1,(nbr_partitions+compteur));
		write_block(id,b,0);
	}		
				
	/*******************************************************************************************************************
							 EN COURS 
			// vérifier qu'une partition n'est pas trops grande pour le disque
			// matché le name pour voir si le disque existe 
			// tabeau à taille de argc qui correspondra au nombre de partition
			// faut il que le disque soit vide ou bien cherche les block vide pour faires les partitions?
	*******************************************************************************************************************/

	printf("Partitionnage terminé.\n");
	if(stop_disk(id).error_id==1){
		fprintf(stderr, "Erreur lors de l'arrêt du disque.\n");
		return 1;
	}
	else{
		printf("--- Arrêt du disque. ---\n");
	}
	return 0;
}

