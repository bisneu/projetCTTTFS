#include "ttfs_api.h"

/*
** Ajoute un bloc à la liste des blocs libres
*/
int add_free_block(disk_id id, uint32_t id_description_block, uint32_t id_block_to_add){
	/* Mise en mémoire du block de description */
  	block description_block;
	b.block_block = malloc(1024);
	if(read_block(id,description_block,id_description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	/* Overwrite de la valeur de TTTFS_VOLUME_FREE_BLOCK_COUNT dans le block de description en mémoire */
	write_inblock(description_block,3,read_inblock(3,description_block)+1);
	/* Overwrite du description_block en mémoire sur le disk */
	write_block(id,description_block,id_description_block);

	/* Récupartion du last_free_block */
	uint32_t id_last_free_block = get_last_free_block(id, id_description_block);
	/* Mise en mémoire du last_free_block */
	block last_free_block;
	last_free_block.block_block = malloc(1024);
	if(read_block(id,last_free_block,id_last_free_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	/* Overwrite de la valeur du next_free_block dans le last_free_block en mémoire */
	write_inblock(last_free_block,255,id_block_to_add);
	/* Overwrite du last_free_block sur le disk */
	write_block(id,last_free_block,id_last_free_block);

	/* Mise en mémoire du block_to_add */
	block block_to_add;
	block_to_add.block_block = malloc(1024);
	if(read_block(id,block_to_add,id_block_to_add).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	/* Overwrite de la valeur du next_free_block dans le block_to_add en mémoire */
	write_inblock(block_to_add,255,id_block_to_add);
	/* Overwrite du block_to_add sur le disk */
	write_block(id,block_to_add,id_block_to_add);
	return 0;
}

/*
** Supprime un bloc de la liste des blocs libres
*/
int remove_free_block(disk_id id, uint32_t id_description_block){
	/* Mise en mémoire du block de description */
  	block description_block;
	b.block_block = malloc(1024);
	if(read_block(id,description_block,id_description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	/* Mise en mémoire de l'id du block à remove */
	uint32_t id_block_to_remove = get_first_free_block(id,id_description_block);
	/* Overwrite de la valeur de TTTFS_VOLUME_FREE_BLOCK_COUNT dans le block de description en mémoire */
	write_inblock(description_block,3,read_inblock(3,description_block)-1);
	/* Overwrite de la valeur de TTTFS_VOLUME_FIRST_FREE_BLOCK dans le block de description en mémoire */
	write_inblock(description_block,4,read_inblock(255,id_block_to_remove));
	/* Overwrite du description_block en mémoire sur le disk */
	write_block(id,description_block,id_description_block);

	/* Mise en mémoire du block_to_remove */
	block block_to_remove;
	block_to_remove.block_block = malloc(1024);
	if(read_block(id,block_to_remove,id_block_to_remove).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	/* Overwrite de la valeur du next_free_block dans le last_free_block en mémoire */
	write_inblock(block_to_add,255,0);
	/* Overwrite du last_free_block sur le disk */
	write_block(id,block_to_remove,id_block_to_remove);
	return 0;
}

/* TODO */

/*
** Ajoute une entrée de répertoire à la liste des entrées libres
*/
int add_free_entry(disk_id id, uint32_t id_description_block){
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
	write_block(id, b, id_description_block);
	return 0;
}

/*
** Supprime une entrée de répertoire de la liste des entrées libres
*/
int remove_free_entry(disk_id id, uint32_t id_description_block){
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
	write_block(id, b, id_description_block);
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

int existence_disk(char* chemin){
	char* str = malloc((strlen(chemin)-7)*sizeof(char));
	bout_chemin_final(chemin,str);
	return verif_rep(bout_chemin(final,0),open("."));	
}
