#include "logical_layer.h"

/*
** ./tfs_create -s size [name]
** Permet de créer un disque nommé name de taille size blocks
** Vérifie la disponibilité du nom
** Noms par defaut : disk.tfs, disk_1.tfs, disk_2.tfs, etc..
*/
int main(int argc, char *argv[]){
	if(argc!=3 && argc!=4){
		fprintf(stderr, "Nombre d'arguments incorrect (%d).\n", argc);
		fprintf(stderr, "./tfs_create -s size [name]\n");
		return 1;
	}
	else if(strcmp(argv[1],"-s")!=0){
		fprintf(stderr, "Option invalide : (%s).\n", argv[1]);
		fprintf(stderr, "./tfs_create -s size [name]\n");
		return 2;
	}
	else if(atoi(argv[2])<1){
		fprintf(stderr, "Nombre attendu après \"-s\".\n");
		fprintf(stderr, "./tfs_create -s size [name]\n");
		return 1;
	}
	else if(atoi(argv[2])>4294967295){
		fprintf(stderr, "Taille maximale d'une partition : 4294967295 blocks.\n");
		fprintf(stderr, "./tfs_create -s size [name]\n");
		return 1;
	}
	else if(atoi(argv[2])<10){
		fprintf(stderr, "Taille insufisante : minimum 10 Blocks.\n");
		fprintf(stderr, "./tfs_create -s size [name]\n");
		return 1;
	}
	int size = atoi(argv[2]);
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
	FILE *file = fopen(name, "w+");
	uint8_t *c = NULL;
	c = malloc(1024);
	int i=0;
	for(i=0; i<1024; i++){
		c[i]=0;
	}
	for(i=0; i<size; i++){
		fwrite(c, sizeof(uint8_t), 1024, file);
	}
	write_size_inblock(size,file);
	fclose(file);
	free(c);
	return 0;
}
