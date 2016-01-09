#include "ttfs_volume.h"

/*
** Initialise le block de description d'une partition 
*/
void initiate_description_block(block block_zero, block b, uint32_t partition,uint32_t nbr_fic){
	write_inblock(b,0,TTTFS_MAGIC_NUMBER); // le magic number 
	write_inblock(b,1,TTTFS_VOLUME_BLOCK_SIZE); // taille d'un block 
	uint32_t taille_partition = read_inblock(1+partition,block_zero); 
	write_inblock(b,2,taille_partition); // taille d'une partition 
	uint32_t tmp = ((64*nbr_fic))/1024; 
	// uint32_t fichier_racine  = (nbr_fic/35)+1; rajouter apres et
	uint32_t libre = taille_partition-(tmp+3); // 1 pour le block de description, tmp pour la table de fichier, 1 pour le fichier systeme 1 pour incrementer tmp
	write_inblock(b,3,libre); // nombre de block libre 
	write_inblock(b,4,(tmp+4)); // le numéro du premier block libre  
	write_inblock(b,5,nbr_fic); // nombre total de fichiers dans ce volume
	write_inblock(b,6,nbr_fic); // nombre de fichiers actuellement libre
	write_inblock(b,7,1); // rajouter le numero du premier fichier libre du volume ....
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
		for(i=0; i<16; i++){
			if(compteur == 0 && i==0){
				write_inblock(b,0+(i*16),0);
				write_inblock(b,1+(i*16),1);
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
				write_inblock(b,15+(i*16),compteur);
			}
			else {
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
				else if(i==15){
					write_block(id,b,block_partition+compteur2);
					compteur2 = compteur2+1;
					initiate_block(b,1024);
				}
			}
		}
	}
	free(b.block_block);	
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
	int block_partition_zero = get_description_block(zero,partition);	
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
free_entry get_first_free_file(disk_id id, uint32_t id_description_block){
	free_entry first_free_entry;
	block b;
	b.block_block = malloc(1024);
	read_block(id, b, id_description_block);
	first_free_entry.tfs_next_free = read_inblock(7,b);
	return first_free_entry;
}


/*
** Renvoie la dernière entrée libre
*/
uint32_t get_last_free_file(disk_id id, uint32_t id_description_block){
	free_entry first_free_file = get_first_free_file(id, id_description_block);
	block file_table;
	file_table.block_block = malloc(1024);
	uint32_t check_file_table = (((first_free_file.tfs_next_free - 1) / 16) + 1);
	if(read_block(id, file_table, id_description_block+check_file_table).error_id==1){
		fprintf(stderr, "Erreur lors de la lecture du block.\n");
		return 1;
	}
	uint32_t next_free_file;
	if(first_free_file.tfs_next_free%16 == 0){
		next_free_file = read_inblock(255,file_table);
	}
	else{
		next_free_file = read_inblock(((first_free_file.tfs_next_free%16)*16),file_table);
	}
	uint32_t last_free_file = first_free_file.tfs_next_free;
	while(next_free_file != last_free_file){
		last_free_file = next_free_file;
		check_file_table = (((last_free_file - 1) / 16) + 1);
		if(read_block(id, file_table, id_description_block+check_file_table).error_id==1){
			fprintf(stderr, "Erreur lors de la lecture du block.\n");
			return 1;
		}
		if(last_free_file%16 == 0){
			next_free_file = read_inblock(255,file_table);
		}
		else{
			next_free_file = read_inblock(((last_free_file%16)*16),file_table);
		}
	}
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
	for(j=2; j<nombre_slash ; j++){
		strcat(str,strcat(bout_chemin(chemin,j),"/"));	
	}
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
				// il faut que je verifie si la partition existe indice 3
				}
				// il faut que je verifie que le chemin existe aussi apres
 				// 	-verifie a la racine si le le name que j aurai a lindice 4 existe 
				// ********* a continuer ******
			}
			return 1;	
		}
		return 1;	
	}
	return 1;
}
void mkdir(char* path){
	
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
