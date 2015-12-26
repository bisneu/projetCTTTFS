#include "ttfs_volume.h"

void initiate_description_block(block block_zero, block b,int partition,int nbr_fic){
	write_inblock(b,0,TTTFS_MAGIC_NUMBER);
	write_inblock(b,1,TTTFS_VOLUME_BLOCK_SIZE);
	uint32_t taille_partition = read_inblock(1+partition,block_zero); 
	write_inblock(b,2,taille_partition);
	uint32_t tmp = (64*nbr_fic)/1024;
	uint32_t libre = taille_partition-(tmp+2);
	write_inblock(b,3,libre);
	write_inblock(b,4,(tmp+3));
	write_inblock(b,5,nbr_fic);
	write_inblock(b,6,nbr_fic);
	// rajouter le numero du premier fichier libre du volume ....
}

int  first_block_partition(block block_zero, int partition){
	uint32_t compteur = 1 ;
	int i = 0; 
	for(i = 1; i<partition; i++){
		compteur = compteur + read_inblock(1+i,block_zero);
	}
	return compteur;
}

