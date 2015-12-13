#include "logical_layer.h"
#include <stdio.h>
#include <endian.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// convertie int en char pour la conversion en hexadécimal
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

// convertie un int en une chaine de caractere hexadecimal 
void convertir_32(int i, char* tab){
	int tmp = i;
	tab[0] = '0';
	tab[1] = '0';
	tab[2] = '0';
	tab[3] = '0';
	tab[4] = '0';
	tab[5] = '0';
	tab[6] = '0';
	tab[7] = '0';
	int compteur=7;
	while(tmp != 0){
		int modulo = tmp%16;
		tab[compteur] = convert_char(modulo);
		compteur--;
		tmp = tmp/16;	
	}		
}

// convertie des caracteres hexadecimal en int 
uint8_t hexa_to_int(char* hex){
	if(strcmp(hex,"a")==0) return 10;
	else if(strcmp(hex,"b")==0) return 11;
	else if(strcmp(hex,"c")==0) return 12;
	else if(strcmp(hex,"d")==0) return 13;
	else if(strcmp(hex,"e")==0) return 14;
	else if(strcmp(hex,"f")==0) return 15;
	return atoi(hex);
}
 
void affiche_tab(char *tab){
	printf("%s\n",tab);
}

// concatene deux caractere hexadecimal et renvoi le decimal correspondant
uint8_t concat(char c, char c2){
	char tab[2];
	char tab2[2];
	tab[0]=c;
	tab[1]='\0';
	tab2[0]=c2;
	tab2[1]='\0';
	return hexa_to_int(tab)*16+hexa_to_int(tab2);	
}

// met les couple d'hexa dans un tableau de 4 cases 
void convert_to_decimal(char* t, uint8_t* tab){
	tab[0] = concat(t[6],t[7]);	
	tab[1] = concat(t[4],t[5]);	
	tab[2] = concat(t[2],t[3]);	
	tab[3] = concat(t[0],t[1]);	
}

void write_size_inblock(int size, FILE* file){
	char *hexadecimal = malloc(sizeof(char)*8); 
	uint8_t *decimal = malloc(sizeof(uint8_t)*4); 
	convertir_32(size,hexadecimal);
	convert_to_decimal(hexadecimal,decimal);
	fseek(file,0,SEEK_SET);
	fwrite(decimal,sizeof(uint8_t),4,file);
	
}

/*
** ./tfs_create -s size [name]
** Permet de créer un disque nommé name de taille size blocks
** Vérifie la disponibilité du nom
** Noms par defaut : disk.tfs, disk_1.tfs, disk_2.tfs, etc..
*/
int main(int argc, char *argv[]){
	if(argc!=3 && argc!=4){
		printf("Nombre d'arguments incorrect (%d).\n", argc);
		printf("./tfs_create -s size [name]\n");
		return 1;
	}
	else if(strcmp(argv[1],"-s")!=0){
		printf("Option invalide : (%s).\n", argv[1]);
		printf("./tfs_create -s size [name]\n");
		return 2;
	}
	printf("Taille 1 : %s\n", argv[2]);
	int size = atoi(argv[2]);
	printf("Taille 2 : %d\n", size);
	DIR *currentDir=opendir(".");
	struct dirent *entry=readdir(currentDir);
	char *name;
	name = (argc==4)?argv[3]:"disk.tfs";
	int k=1;
	do{
		if(strcmp(name,entry->d_name)==0){
		// Si un fihcier du même nom existe déjà dans le répertoire
			char name_tmp[100] = "";	
			if(argc==4){
				char name_tmp[100] = "";	
				printf("Le nom %s est déjà utilisé dans ce répertoire.\n", argv[3]);
				printf("Veuillez saisir un autre nom :");
				fgets(name_tmp, sizeof(name_tmp), stdin);
				name = name_tmp;
				rewinddir(currentDir);
			}
			// Alors on invite l'utilisateur à choisir un autre nom
			else if(argc==3){
				sprintf(name_tmp, "disk_%d.tfs", k);
				name = name_tmp;
				k++;
				rewinddir(currentDir);
			}
			// Ou on lui attribut un nom par défaut : disk_k.tfs
		}
		else{
			entry=readdir(currentDir);
		}
	}while(entry!=NULL);
	closedir(currentDir);
	printf("Nom attribué : %s\n", name);
	FILE *file = fopen(name, "a+");
	uint32_t *c = NULL;
	c = malloc(1024);
	int i=0;
	for(i=0; i<256; i++){
		c[i]=0;
	}
	for(i=0; i<size; i++){
		fwrite(c, sizeof(uint32_t), 256, file);
	}
	write_size_inblock(size,file);
	fclose(file);
	free(c);
	return 0;
}
