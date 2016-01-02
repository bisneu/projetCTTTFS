#include "ttfs_volume.h"

/*
** Initialise le block de description d'une partition 
*/
void initiate_description_block(block block_zero, block b, uint32_t partition,uint32_t nbr_fic){
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
	write_inblock(b,7,1); // rajouter le numero du premier fichier libre du volume ....
}

/*
** Renvoie le premier block d'une partition 
*/
uint32_t get_description_block(block block_zero, uint32_t partition){
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
	uint32_t block_partition = get_description_block(block_zero,partition);
	block b;
	uint32_t compteur=0;
	uint32_t compteur2=1;
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
			compteur = compteur+1;
			if(compteur < nbr_fic){
				write_inblock(b,15+(i*16),compteur+1); // nombre de fichier 
			}
			if(compteur == nbr_fic){ 
				write_inblock(b,15+(i*16),compteur); // nombre de fichier 
				write_block(id,b,block_partition+compteur2);
				compteur2 = compteur2+1;
				break;
			}
			else if(i==14){
				write_block(id,b,block_partition+compteur2);
				compteur2 = compteur2+1;
				initiate_block(b,1024);
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

/*
** fonction qui initialise les data_block d'une partition 
*/
void initiate_data_block(disk_id id,uint32_t partition){
	block zero;
	block description; 
	block tmp; 
	zero.block_block = malloc(1024);
	description.block_block = malloc(1024);
	tmp.block_block = malloc(1024);
	read_block(id,zero,0);
	int block_partition_zero = first_block_partition(zero,partition);	
	read_block(id,description,block_partition_zero);
	int first_free_block = read_inblock(4,description);
	int total_free_block = read_inblock(3,description);
	int i = 0;
	for(i=0; i<total_free_block; i++){
		read_block(id,tmp,(block_partition_zero+first_free_block-1)+i);
		if(i!=(total_free_block-1)){
			write_inblock(tmp,255,i+1);
		}
		else {
			write_inblock(tmp,255,i);
		}
		write_block(id,tmp,(block_partition_zero+first_free_block-1)+i);				
	}
}

/*
** Renvoie l'id du premier block libre de la partition
*/
uint32_t get_first_free_block(disk_id id, uint32_t id_description_block){
	uint32_t retour_valeur = 0;
	block description_block;
	description_block.block_block = malloc(1024);
	read_block(id,description_block,id_description_block);
	retour_valeur = id_description_block +read_inblock(4,description_block);
	free(description_block.block_block);
	return retour_valeur;
}

/*
** Renvoie l'id du dernier block libre de la partition
*/
uint32_t get_last_free_block(disk_id id, uint32_t id_description_block){
	uint32_t prev_free_block = get_first_free_block(id,id_description_block);
	uint32_t next_free_block = 0;
	block description_block;
	block tmp_block;
	description_block.block_block = malloc(1024);
	tmp_block.block_block = malloc(1024);
	read_block(id,description_block,id_description_block);
	read_block(id,tmp_block,prev_free_block);
	next_free_block = read_inblock(1023,tmp_block)+ id_description_block;
	while(next_free_block != prev_free_block){
		prev_free_block = next_free_block;
		read_block(id,tmp_block,prev_free_block);
		next_free_block = read_inblock(1023,tmp_block)+ id_description_block;
	}
	free(description_block.block_block);
	free(tmp_block.block_block);
	return next_free_block;
}

/*
** Renvoie la première entrée libre
*/
free_entry get_first_free_file(disk_id id, uint32_t description_block){
	free_entry first_free_entry;
	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	first_free_entry.tfs_next_free = get_next_free_file();
	read_inblock(7,b) = ;
	return first_free_entry;
}


/** Pas fini ! **/
/*
** Renvoie la dernière entrée libre
*/
uint32_t get_last_free_file(disk_id id, uint32_t description_block){
	uint32_t first_free_file = get_first_free_file(id, description_block);
	block b;
	b.block_block = malloc(1024);
	if(read_block(id, b, description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t last_free_file = read_inblock(7,b);
	return 0;
}

/** Pas fini ! **/
/*
** Renvoie l'entrée libre suivante
*/
uint32_t get_next_free_file(uint32_t file){
	
	return 0;
}

