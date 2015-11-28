#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc!=2){
		fprintf(stderr, "Erreur: %s prend un argument.\n", argv[0]);
		return 1;
	}
	FILE *f = fopen(argv[1], "rb");
	if(!f){
		fprintf(stderr, "Erreur d'ouverture de fichier(s).\n");
		return 1;
	}
	fread(&var, sizeof(var), 1, f);
	fclose(f);
	return 0;
}
