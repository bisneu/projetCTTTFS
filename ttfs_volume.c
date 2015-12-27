#include "ttfs_volume.h"

/*
** Initialise le block de description d'une partition 
*/
void initiate_description_block(block block_zero, block b,int partition,int nbr_fic){
	write_inblock(b,0,TTTFS_MAGIC_NUMBER); // le magic number 
	write_inblock(b,1,TTTFS_VOLUME_BLOCK_SIZE); // taille d'un block 
	uint32_t taille_partition = read_inblock(1+partition,block_zero); 
	write_inblock(b,2,taille_partition); // taille d'une partition 
	uint32_t tmp = (64*nbr_fic)/1024; 
	uint32_t libre = taille_partition-(tmp+2);
	write_inblock(b,3,libre); // nombre de block libre 
	write_inblock(b,4,(tmp+3)); // le numéro du premier block libre  
	write_inblock(b,5,nbr_fic); // nombre total de fichiers dans ce volume
	write_inblock(b,6,nbr_fic); // nombre de fichiers actuellement libre
	// rajouter le numero du premier fichier libre du volume ....
}

/*
** Renvoie le premier block d'une partition 
*/
int  first_block_partition(block block_zero, int partition){
	uint32_t compteur = 1 ;
	int i = 0; 
	for(i = 1; i<partition; i++){
		compteur = compteur + read_inblock(1+i,block_zero);
	}
	return compteur;
}

/*
** Initialise à 0 un block donnée 
*/
void initiate_block(block b,int n){
	int i = 0;
	for(i=0; i<n ; i++){
		b.block_block[i] = 0;
	}
}

/*
** Initialise la table des fichiers d'une partition
*/
void initiate_file_table(disk_id id,block block_zero,int nbr_fic,int partition){
	int block_partition = first_block_partition(block_zero,partition);
	block b;
	int compteur=0;
	int compteur2=1;
	b.block_block = malloc(1024);
	initiate_block(b,1024);
	int i = 0 ;
	while(compteur<nbr_fic){
		for(i=0; i<15; i++){
			write_inblock(b,0+(i*16),0);
			write_inblock(b,1+(i*16),0);
			write_inblock(b,2+(i*16),0);
			write_inblock(b,3+(i*16),0);
			write_inblock(b,4+(i*16),0);
			write_inblock(b,5+(i*16),0);
			write_inblock(b,6+(i*16),0);
			write_inblock(b,7+(i*16),0);
			write_inblock(b,8+(i*16),0);
			write_inblock(b,9+(i*16),0);
			write_inblock(b,10+(i*16),0);
			write_inblock(b,11+(i*16),0);
			write_inblock(b,12+(i*16),0);
			write_inblock(b,13+(i*16),0);
			write_inblock(b,14+(i*16),0);
			write_inblock(b,15+(i*16),i+1); // nombre de fichier  
			compteur = compteur+1;
			if(compteur == nbr_fic){ 
				write_block(id,b,block_partition+compteur2);
				compteur2 = compteur2+1;
				break;
			}
			else if(i==15){
				write_block(id,b,block_partition+compteur2);
				compteur2 = compteur2+1;
			}	
		}
	}
	free(b.block_block);	
}

/*
** Ajoute un bloc à la liste des blocs libres
*/
int add_free_block(disk_id id, int description_block){
  	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t i = read_inblock(3,b)+1;
	uint32_t j = read_inblock(4,b)-1;
	write_inblock(b,3,i); // i = TTTFS_VOLUME_FREE_BLOCK_COUNT ++
	write_inblock(b,4,j); // j = TTTFS_VOLUME_FIRST_FREE_BLOCK --
	write_block(id, b, description_block);
	return 0;
}

/*
** Supprime un bloc de la liste des blocs libres
*/
int remove_free_block(disk_id id, int description_block){
  	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t i = read_inblock(3,b)-1;
	uint32_t j = read_inblock(4,b)+1;
	write_inblock(b,3,i); // i = TTTFS_VOLUME_FREE_BLOCK_COUNT --
	write_inblock(b,4,j); // j = TTTFS_VOLUME_FIRST_FREE_BLOCK ++
	write_block(id, b, description_block);
	return 0;
}

/*
** Ajoute une entrée de répertoire à la liste des entrées libres
*/
int add_free_entry(disk_id id, int description_block){
  	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t i = read_inblock(6,b)+1;
	uint32_t j = read_inblock(7,b)-1;
	write_inblock(b,6,i); // i = TTTFS_VOLUME_FREE_FILE_COUNT ++
	write_inblock(b,7,j); // j = TTTFS_VOLUME_FIRST_FREE_FILE --
	write_block(id, b, description_block);
	return 0;
}

/*
** Supprime une entrée de répertoire de la liste des entrées libres
*/
int remove_free_entry(disk_id id, int description_block){
  	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t i = read_inblock(6,b)-1;
	uint32_t j = read_inblock(7,b)+1;
	write_inblock(b,6,i); // i = TTTFS_VOLUME_FREE_FILE_COUNT --
	write_inblock(b,7,j); // j = TTTFS_VOLUME_FIRST_FREE_FILE ++
	write_block(id, b, description_block);
	return 0;
}

/*
** Ajoute un bloc à la liste des blocs d’un fichier
*/
int add_file_block(){
	return 0;
}

/*
** Supprime un bloc de la liste des blocs d’un fichier
*/
int remove_file_block(){
	return 0;
}
