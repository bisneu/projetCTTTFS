#include "logical_layer.h"

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
** vérifie si le fichier dans le dossier donnée en argument existe
*/
int verif_file(char *str, DIR *cur_dir){
	struct dirent *entry = readdir(cur_dir);	
	do{
		if(strcmp(str,entry->d_name)==0){
			if(((entry->d_type)==DT_UNKNOWN) || (entry->d_type)==DT_REG){	
				return 0;
			}
			fprintf(stderr,"Ce n'est pas un fichier\n");
			return 1;
		}
		entry = readdir(cur_dir);	
	}while(entry!=NULL);
	fprintf(stderr,"Le fichier n'existe pas\n");
	return 1;
}

/*
** vérifie si le dossier dans le dossier donnée en argument existe
*/
int verif_rep(char *str, DIR *cur_dir){
	struct dirent *entry = readdir(cur_dir);	
	do{
		if(strcmp(str,entry->d_name)==0){
			if((entry->d_type)==DT_DIR){	
				return 0;
			}
			fprintf(stderr,"Ce n'est pas un répertoire\n");
			return 1;
		}
		entry = readdir(cur_dir);	
	}while(entry!=NULL);
	fprintf(stderr,"Le répertoire n'existe pas\n");
	return 1;
}


/*
** compte les séprateurs dans \ dans une chaine de caractere donnée  
*/
int  compte_separateur(char* str){	
	int i=0;
	int compteur=0;
	while(*(str+i) != '\0'){
		if( *(str+i) == '/'){
			compteur++;
		}
		i++;
	}
	return compteur;	
}

/*
** renvoie un bout de chemin d'une adresse donnée en argument indice qui commence à 0  
*/
char* bout_chemin(char *str,int del){
	int i=0;
	int j=0;
	int compteur = 0;
	char *str_tmp = malloc(strlen(str)*sizeof(char));
	for(i=0; i<strlen(str); i++){
		*(str_tmp+i) = '\0';
	}
	i=0;
	while(*(str+i) != '\0'){
		if(*(str+i)=='/'){
			compteur ++;
		}
		else if((compteur > del-1)&&(compteur<=del)){
			*(str_tmp+j) = *(str+i);
			j++;
		}
		i++;	
	}
	return str_tmp;		
}

/*
** permet d'avoir le chemin d'un dossier  
*/
void avoir_le_dossier(char* str,char* ret){
	int nombre_slash = compte_separateur(str);
	int i=0;
	for(i=0; i<=nombre_slash-1; i++ ){
		if(i!=0){
			strcat(strcat(ret,"/"),bout_chemin(str,i));	
		}
		else {
			strcat(ret,bout_chemin(str,i));
		}
	}
}

/*
** verifie qu'un fichier est correcte  
*/
int rec_open_file(char *str, int i, DIR *cur_dir){
	char *tmp = bout_chemin(str,i);
	if(i>=compte_separateur(str)){
		char *ret = malloc(sizeof(char)*256);
		avoir_le_dossier(str,ret);
		cur_dir = opendir(ret);
		if(verif_file(tmp,cur_dir)==0){
			free(ret);
			return 0;
		}
		else { 
			free(ret);
			return 1; 
		}	
	}
	else {
		if(verif_rep(tmp,cur_dir) == 0){
			cur_dir = opendir(tmp);	
			return rec_open_file(str,i+1,cur_dir);			
		} 
		else return 1;
	}	
}

/* DEBUT fonction permetant à l écriture dans un block en little endian 32 bit */

/*
** Convertit un int en char pour la conversion en hex
*/
char convert_char(int i){
	if(i==0) return '0';
	else if(i==1) return '1';
	else if(i==2) return '2';
	else if(i==3) return '3';
	else if(i==4) return '4';
	else if(i==5) return '5';
	else if(i==6) return '6';
	else if(i==7) return '7';
	else if(i==8) return '8';
	else if(i==9) return '9';
	else if(i==10) return 'a';
	else if(i==11) return 'b';
	else if(i==12) return 'c';
	else if(i==13) return 'd';
	else if(i==14) return 'e';
	else if(i==15) return 'f';
	else return '0';	
}

/*
** Convertit un int en une chaine de 8 caracteres hex
*/
void convertir_32(int decimal, char* hexa){
	int tmp = decimal;
	int i=0;
	for(i=0; i<8; i++){
		hexa[i] = '0';
	}
	int compteur=7;
	while(tmp != 0){
		int modulo = tmp%16;
		hexa[compteur] = convert_char(modulo);
		compteur--;
		tmp = tmp/16;	
	}		
}

/*
** Convertit un int en une chaine de 2 caracteres hex
*/
void convertir_8(uint8_t decimal, char* hexa){
	hexa[0] = '0';
	hexa[1] = '0';
	hexa[2] = '\0';
	int compteur=1;
	uint8_t tmp = decimal;
	while(tmp != 0){
		int modulo = tmp%16;
		hexa[compteur] = convert_char(modulo);
		compteur--;
		tmp = tmp/16;
	}
}

/*
** Convertit des caracteres hex en int
*/
uint8_t hexa_to_int(char* hex){
	if(strcmp(hex,"a")==0) return 10;
	else if(strcmp(hex,"b")==0) return 11;
	else if(strcmp(hex,"c")==0) return 12;
	else if(strcmp(hex,"d")==0) return 13;
	else if(strcmp(hex,"e")==0) return 14;
	else if(strcmp(hex,"f")==0) return 15;
	return atoi(hex);
}
 

/*
** Concatène deux caractere hex et renvoi le décimal correspondant
*/
uint8_t concat(char c, char c2){
	char tab[2];
	char tab2[2];
	tab[0]=c;
	tab[1]='\0';
	tab2[0]=c2;
	tab2[1]='\0';
	return hexa_to_int(tab)*16+hexa_to_int(tab2);	
}

/*
** Met les couples d'hexa dans un tableau de 4 cases
*/
void convert_to_decimal(char* t, uint8_t* tab){
	int i=0;
	for(i=0;i<4;i++){
		tab[i] = concat(t[6-(2*i)],t[7-(2*i)]);
	}
}

/*
** convertie en little endian un int donnée donnée 
*/
void my_little_endian(uint32_t elem, uint8_t *tab){
	char *hexadecimal = malloc(sizeof(char)*8);
	convertir_32(elem,hexadecimal);
	convert_to_decimal(hexadecimal,tab);
	free(hexadecimal); 
}

/*
** Ecrit dans un block en little endian 32 bit
*/
void write_size_inblock(int size, FILE* file){
	uint8_t *decimal = malloc(sizeof(uint8_t)*4); 
	my_little_endian(size,decimal);
	fseek(file,0,SEEK_SET);
	fwrite(decimal,sizeof(uint8_t),4,file);
	free(decimal);
}
/* FIN */

/**
* écrit dans le block l'element donné par elem à la position donnée par index 
*/
void write_inblock(block b,int index, uint32_t elem){
	int myindex = (index*4);
	uint8_t *tab = malloc(sizeof(uint8_t)*4);
	my_little_endian(elem,tab);
	b.block_block[myindex] = tab[0];
	b.block_block[myindex+1] = tab[1];				
	b.block_block[myindex+2] = tab[2];				
	b.block_block[myindex+3] = tab[3];				
}

/*
** Lit dans un block en little endian 32 bit
*/
uint32_t read_inblock(int indice, block b){
	char *hexa0 = malloc(sizeof(char)*3);
	char *hexa1 = malloc(sizeof(char)*3);
	char *hexa2 = malloc(sizeof(char)*3);
	char *hexa3 = malloc(sizeof(char)*3);
	uint8_t *decimal = malloc(sizeof(uint8_t)*4);
	int i=0;
	for(i=0;i<4;i++){
		decimal[i] = b.block_block[i+(indice*4)];
	}
	convertir_8(decimal[0],hexa0);
	convertir_8(decimal[1],hexa1);
	convertir_8(decimal[2],hexa2);
	convertir_8(decimal[3],hexa3);
	strcat(hexa3, strcat(hexa2, strcat(hexa1,hexa0)));
	uint32_t final;
	sscanf(hexa3, "%x", &final);
	//printf("Hexa : 0x%x | Deci : %d\n", final, final);
	free(hexa0);
	free(hexa1);
	free(hexa2);
	free(hexa3);
	return final;
}

uint32_t total_partition(block b, uint32_t nbr_partition){
	int i=0;
	uint32_t total = 0; 
	for(i=0; i<nbr_partition; i++){
		total = total + read_inblock((2+i),b);				
	}
	return total;	
}

void enleve_le_slash(char* str){
	if(*(str+(strlen(str)-1))=='/'){
		*(str+(strlen(str)-1)) = '\0';
	}	
}

/*
** Permet de vérifier l'existence du fichier nommé name.
*/
int exist_disk(char *name){
	int nombre_separateur = compte_separateur(name);
	enleve_le_slash(name);
	DIR *currentDir=opendir(".");
	if( nombre_separateur >0){
		if(rec_open_file(name,0,currentDir)==0){
			closedir(currentDir);	
			return 0;
		}
		closedir(currentDir);	
		return 1;
	}
	else {
 		struct dirent *entry=readdir(currentDir);
		do{
  			if(strcmp(name,entry->d_name)==0){
				return 0;
			}
			entry=readdir(currentDir);
		}while(entry!=NULL);
		closedir(currentDir); 
		return 1;
	}	
}

/*
** Permet d'écrire un bloc b sur le disque id à l'emplacement num.
*/
error write_block(disk_id id, block b, uint32_t num){
	error rep;
	rep = write_physical_block(id, b, num);
	return rep;
}

/*
** Permet de lire un bloc b sur le disque id à l'emplacement num.
*/
error read_block(disk_id id, block b, uint32_t num){
	error rep;
	rep = read_physical_block(id, b, num);
	return rep;
}

/*
** Permet de manipuler un disque en lui associant un id.
*/
error start_disk(char *name, disk_id *id){
    	error rep;
	rep.error_id=1;
	if(exist_disk(name)==0){
		id->disk_name = name;
		id->disk_id = 0; // a revoir les ids du disque
		FILE *tmp = NULL;
		tmp = fopen(name,"r+");
		if(tmp==NULL){
			rep.error_desc = "Erreur lors de l'ouverture.";
			return rep;
		}
		id->disk_file = tmp;
		rep.error_id=0;
		return rep; 
	}
	rep.error_desc = "Erreur fichier introuvable.";
	return rep;
}

/* 
** Permet de terminer une session de travaille sur un disque.
** Ferme le fichier qui correspond au disque et supprime tout 
** les pointeurs de la mémoire afin de mettre un terme à la session
*/
error stop_disk(disk_id id){
	error rep;
	rep.error_id = 1;
	if(id.disk_file!=NULL){
		rep.error_id = fclose(id.disk_file);
		if(rep.error_id!=0){
			rep.error_desc = "Erreur lors de la fermeture.";
			return rep;
		}
		return rep;
	}
	rep.error_desc = "Erreur fichier introuvable.";
	return rep;
}

/*
** 
*/
error sync_disk(disk_id id){
	error rep;
	return rep;
}

