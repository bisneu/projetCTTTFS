#include "ttfs_volume.h"

/*
** ./tfs_format -p partition -mf file_count [disk]
** Permet de formater une partition du disque
** au format TFS.
*/
int main(int argc, char **argv){
	if((argc<5)||(argc>6)){
		fprintf(stderr, "Nombre d'arguments incorrect (%d).\n", argc);
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(strcmp(argv[1],"-p")!=0){
		fprintf(stderr, "Option invalide : (%s).\n", argv[1]);
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(strcmp(argv[3],"-mf")!=0){
		fprintf(stderr, "Option invalide : (%s).\n", argv[3]);
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	else if(atoi(argv[2])<0){
		fprintf(stderr, "Nombre attendu après \"-p\".\n");
		fprintf(stderr, "./tfs_format -p partition -mf file_count [disk]\n");
		return 1;
	}
	char *name;
	name = ((argc==6)?argv[5]:"disk.tfs");

}
