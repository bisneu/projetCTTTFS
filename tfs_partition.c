#include "logical_layer.h"
#include "endian.h"

/**
* écrit dans le block l'element donné par elem à la position donnée par index 
*/
void write_in_block(block b,int index, uint32_t elem){
	int myindex = (index*4);
	uint8_t *tab = malloc(sizeof(uint8_t)*4);
	my_little_endian(elem,tab);
	b.block_block[myindex] = tab[0];
	b.block_block[myindex+1] = tab[1];				
	b.block_block[myindex+2] = tab[2];				
	b.block_block[myindex+3] = tab[3];				
}

uint32_t total_partition(block b,uint32_t nbr_partition){
	int i=0;
	uint32_t total = 0; 
	for(i=0; i<nbr_partition; i++){
		total = total + read_inblock((2+i),b);				
	}
	printf("%d\n",total);
	return total;	
}


/*
** ./tfs_partition -p size [size] ... [name]
** Permet de partitionner l'espace physique du disque
** en paquets de blocs.
** read_size_inblock()
*/
int main(int argc, char **argv){
	/*********************************************************************************************
						Vérification
	**********************************************************************************************/
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
	int i=1;
	uint32_t compteur = 0; // compteur de partition
	uint32_t mes_partitions[argc]; // tableau qui contiendra les partiions	
	disk_id disk;
	if(exist_disk(argv[argc-1])!=0){
		fprintf(stderr,"Ton fichier n'existe pas ....\n");
		return 1;				
	}
	error rep = start_disk(argv[argc-1],&disk);
	if (rep.error_id ==1 ){
		fprintf(stderr,"Erreur lors du démarrage du disque\n");
		return 1;
	}
	block first;
	first.block_block = malloc(sizeof(uint8_t)*1024);
	read_block(disk,first,0);
	uint32_t taille_disque = read_inblock(0,first);	
	uint32_t nbr_partitions = read_inblock(1,first);	
	printf("mon nombre de parititon %d\n",nbr_partitions);
	for(i=0; i<argc; i++){
		mes_partitions[i]=0;				
	}			
	for(i=1; i< (argc-1); i++){
		if(strcmp(argv[i],"-p")!=0){
			fprintf(stderr,"L'option est incorrecte\n");
			return 1;
		}
		else {
			if((i+1) >= (argc-1)){
				fprintf(stderr,"La commande a mal été utilisé aprés -p obligatoirement ce met un nombre\n");
				return 1;
			}
			else {
				if((atoi(argv[i+1]) <= 0) || (atoi(argv[i+1])>= taille_disque || (atoi(argv[i+1])<3) )){
					fprintf(stderr,"Cette partition ne pourra pas être créer regarde ta stupidité -p %s \n",argv[i+1]);
					return 1;
				}				
				else {
					mes_partitions[compteur]=atoi(argv[i+1]);	
					compteur++;	
					i=i+1;	
				}
			}
		}		
	}
	/*******************************************************************************************************************
						Deuxième étapes écriture dans le fichier
	*******************************************************************************************************************/ 
	if(taille_disque <=(total_partition(first,(nbr_partitions))+compteur)){
		fprintf(stderr,"La partition demandé ne peut pas être effectué elle dépasse le nombre de block possible\n");
		return 1;	
	}
	else{
		int j=0;
		for(j=0; j<compteur;j++){
			write_in_block(first,(2+nbr_partitions+j),mes_partitions[j]);		
		}
			write_in_block(first,1,(nbr_partitions+compteur));
			write_block(disk,first,0);
						
	}						
	/*******************************************************************************************************************
							 en cours 
	*******************************************************************************************************************/
			// vérifier qu'une partition n'est pas trops grande pour le disque
			// matché le name pour voir si le disque existe 
			// tabeau à taille de argc qui correspondra au nombre de partition
			// faut il que le disque soit vide ou bien cherche les block vide pour faires les partitions? 
	stop_disk(disk);	 
	return 0;
}

