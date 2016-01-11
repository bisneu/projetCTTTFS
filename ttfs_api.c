#include "ttfs_api.h"
/*
** Ajoute un bloc à la liste des blocs libres
*/
int add_free_block(disk_id id, uint32_t id_description_block, uint32_t id_block_to_add){
	/* Mise en mémoire du block de description */
  	block description_block;
	description_block.block_block = malloc(1024);
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
	description_block.block_block = malloc(1024);
	if(read_block(id,description_block,id_description_block).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	/* Mise en mémoire de l'id du block à remove */
	uint32_t id_block_to_remove = get_first_free_block(id,id_description_block);
	/* Overwrite de la valeur de TTTFS_VOLUME_FREE_BLOCK_COUNT dans le block de description en mémoire */
	write_inblock(description_block,3,read_inblock(3,description_block)-1);
	/* Overwrite de la valeur de TTTFS_VOLUME_FIRST_FREE_BLOCK dans le block de description en mémoire */
	block remove_block;
	remove_block.block_block = malloc(1024);
	read_block(id,remove_block,id_description_block+id_block_to_remove);
	write_inblock(description_block,4,read_inblock(255,remove_block));
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
	write_inblock(block_to_remove,255,0);
	/* Overwrite du last_free_block sur le disk */
	write_block(id,block_to_remove,id_block_to_remove);
	return 0;
}

/* TODO */

/*
** Ajoute une entrée de répertoire à la liste des entrées libres
*/
int add_free_entry(disk_id id, uint32_t id_description_block){
	return 0;
}

/*
** Supprime une entrée de répertoire de la liste des entrées libres
*/
int remove_free_entry(disk_id id, uint32_t id_description_block){
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



uint32_t verif_chemin(char *chemin){
	if(strcmp(bout_chemin(chemin,0),"FILE:") != 0){
		fprintf(stderr,"Une erreur dans le chemin, il doit commencer par FILE:\n");		
		return 1;	
	}
	if(strcmp(bout_chemin(chemin,1),"") != 0){
		fprintf(stderr,"Une erreur dans le chemin\n");		
		return 1;	
	}
	return 0;
}

void bout_chemin_final(char *chemin, char* str){
	uint32_t nombre_slash = compte_separateur(chemin);
	int i=0;
	// initialisation du string 
	for(i=0; i<strlen(chemin-7)*sizeof(char); i++){
		*(str+i) = '\0';
	}
	int j=0;
	for(j=2; j<=nombre_slash ; j++){
		if(j!=nombre_slash){	
			strcat(str,strcat(bout_chemin(chemin,j),"/"));	
		}
		else{
			strcat(str,bout_chemin(chemin,j));	
		}
	}
}

int existence_disk(char* chemin){
	char* str = malloc((strlen(chemin)-7)*sizeof(char));
	bout_chemin_final(chemin,str);
	return verif_rep(bout_chemin(str,0),opendir("."));	
}

void transforme_ascii(char* str, uint32_t* tab){
	int i=0;
	for(i=0; i<strlen(str); i++){
		*(tab+i) = *(str+i);
	}
}

/*
** fonction qui pour un block donnee renvoie la position de la premiere entree libre 
** renvoie 0 en cas d erreur 
*/
uint32_t recherche_place_dossier(block dir){
	int test =0;
	int i=0;
	for(i=2; i<35; i++){
		if(read_inblock((i*29),dir)==0){
			int j=0;
			for(j=0; j<28; j++){
				if(read_inblock((i*29)+j,dir)!=0){
					test = 1;
			   		break;	
				}
			}
			if(test==0){
				return i*29;
			}
			else { test = 0;}	
		}
	}
	return 0;
}

/*
** fonction qui cree une entree si tout est correcte 
*/
uint32_t create_entree(dir_entry new_entry ,char* name, uint32_t file_number){
	if(strlen(name)<=28){
		new_entry.file_number = file_number;
		uint32_t* tab = malloc(sizeof(uint32_t)*strlen(name));
		new_entry.nombre_caractere = strlen(name);
		transforme_ascii(name,tab);
		int i=0;
		for(i=0; i<strlen(name); i++){
			if(*(tab+i)<0 || *(tab+i)>127){
				fprintf(stderr, "Verifier le nom probleme de codage en ascii-0\n");		
				return 1;
			}	 	
		}
		new_entry.name = tab;
		return 0;
	}	
		fprintf(stderr, "nombre de caractere trops eleve\n");		
		return 1;
}

/*
** fonction qui transforme un tabeau d'ascii en string 
*/
void ascii_to_string(dir_entry entry, char* str){
	int i=0;
	for(i=0; i<entry.nombre_caractere; i++){
		*(str+i) = entry.name[i];	
	}

}

/*
** fonction qui rajoute une entree dans un block donnee 
*/
int add_file_dir(block dir, uint32_t file_number, char* name){
	dir_entry entry;
	if(create_entree(entry,name,file_number) == 1){
		fprintf(stderr,"Le fichier n'a pas ete cree \n");		
		return 1;	
	}
	uint32_t place=recherche_place_dossier(dir);
	if(place == 0){
		return 1;	
	}
	write_inblock(dir,place,entry.file_number);
	int i=0;
	for(i=1; i<strlen(name); i++){
		write_inblock(dir,place+i,entry.name[i]);
	}
	return 0;
}

/*
** fonction pour un numero de fichier donne recupere dans un block l entree de fichier 
*/
void get_file_entry(block table,uint32_t file_number, free_entry entry){
	int i=0;
	for(i=0; i<15; i++){
		// 4-13
		if(file_number == read_inblock((i*16)+15,table)){
			entry.tfs_size = read_inblock((i*16)+0,table);	
			entry.tfs_type = read_inblock((i*16)+1,table);	
			entry.tfs_subtype = read_inblock((i*16)+3,table);	
			entry.tfs_direct = malloc(sizeof(uint32_t)*10);
			int j=0;
			for(j=0; j<10; j++){
				*(entry.tfs_direct+j) = read_inblock((i*16)+4+j,table);	
			}
			entry.tfs_indirect1 = read_inblock((i*16)+14,table);	
			entry.tfs_indirect2 = read_inblock((i*16)+15,table);	
			entry.tfs_next_free = read_inblock((i*16)+16,table);	
		}
	}
}

uint32_t get_free_block(block zero){
	return read_inblock(3,zero);
}

uint32_t first_free_file(block zero){
	return read_inblock(7,zero);
}

uint32_t get_free_file(block zero){
	return read_inblock(6,zero);
}

uint32_t first_free_block(block zero){
	return read_inblock(4,zero);
}

void  set_first_free_block(block zero,uint32_t elem){
	write_inblock(zero,4,elem);
}

void  set_first_free_file(block zero,uint32_t elem){
	write_inblock(zero,7,elem);
}

void  set_free_block(block zero,uint32_t elem){
	write_inblock(zero,3,elem);
}

void  set_free_file(block zero,uint32_t elem){
	write_inblock(zero,6,elem);
}

uint32_t nombre_partition(block zero){
	return read_inblock(1,zero);
}

uint32_t get_racine(block description){
	uint32_t nombre_fic = read_inblock(5,description);	
	uint32_t tmp =  (nombre_fic*64)/1024;
	return tmp+2;
}

int verif_path(char* path){
	if(verif_chemin(path) == 0){
		// l'indice 0 correspond au FILE 
		// l'indice 1 correspond au vide
		// l'indice 2 correspond au disk
		char* name = bout_chemin(path,2);
		disk_id disk; 
		if(exist_disk(name)==0){
			error rep = start_disk(name,&disk);
			if(rep.error_id==0){
				uint32_t numero_partition = atoi(bout_chemin(path,3))+1;
				if(numero_partition<0){
					fprintf(stderr,"Le numero de la partition ne peut pas etre negatif");
					return 1;
				} 
				block zero; 
				zero.block_block = malloc(1024);
				read_block(disk,zero,0);
				if(numero_partition <= nombre_partition(zero)){
					block description;
					description.block_block = malloc(1024);
					read_block(disk,description,get_description_block(zero,numero_partition));			
					char* str = malloc(strlen(path)-7);
					bout_chemin_final(path,str);
					uint32_t racine = get_description_block(zero,numero_partition)+get_racine(description);	
					block racine_block; 	
					racine_block.block_block = malloc(1024);
					read_block(disk,racine_block,racine);	
					//directory dir;  		
					
					
					
				}
		
				// disk.tfs/0/baba  compte_nombre_slash -2 l'indice du bout de chemin commence a 2 jusqua nombre de slash
				// si l'indice de la boucle est egale au nombre de slash verifier si le nom du fichier existe dans le fic courant 
				// structure directory creer une entree avec un tableau de d'entree le nombre d'entree
				// -verifie a la racine si le le name que j aurai a lindice 4 existe 
				// ********* a continuer ******
			}
			return 1;	
		}
		return 1;	
	}
	return 1;
}

/*
** Permet de créer un dossier
*/
int mkdir(char* path){
	
	return 0;
}

/*
int verif_direct(block table , uint32_t file_number){
	return 0;	
}

void add_in_table(){

} 

int tfs_rename(char* old, char *new){	
	return 0;		
}*/

