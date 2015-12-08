#include "logical_layer.h"
#include "endian.h"
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
	int i=1;
	int compteur = 0; // compteur de partition
	int tab [argc] ; // tableau qui contiendra les partiions	
	for(j=0; j<argc; i++){
		tab[j] = -1;
	}
	/*******************************************************************************************************************
							 en cours 
	*******************************************************************************************************************/
	for(i=1; i<argc; i++){
		if(strcmp(argv[i],"-p")==0){
			tab[compteur]=atoi(argv[i+1]);
			compteur++;
			// vérifier qu'une partition n'est pas trops grande pour le disque
			// regardé qu'après le nombre d'élèments est compréhensible 
			// matché le name pour voir si le disque existe 
			// tabeau à taille de argc qui correspondra au nombre de partition
			// faut il que le disque soit vide ou bien cherche les block vide pour faires les partitions?  
			if(atoi(argv[i+1])){
			
			}		
		}
	}
	
	return 0;
}

/**
 * écrit dans le block l'element donné par elem à la position donnée par index 
*/
void write_in_block(block b,int index, uint32_t elem ){
	if(b.block_block != NULL){
		b.block_block[index] = htole32(elem);	
	}
}
