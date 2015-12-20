#include "logical_layer.h"

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
	printf("Hexa : 0x%x | Deci : %d\n", final, final);
	free(hexa0);
	free(hexa1);
	free(hexa2);
	free(hexa3);
	return final;
}

/*
** Permet de vérifier l'existence du fichier nommé name.
*/
int exist_disk(char *name){
	DIR *currentDir=opendir(".");
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
		id->disk_id = 0;
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

