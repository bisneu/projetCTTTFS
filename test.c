#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fichier;
	fichier = fopen("test.bin", "wb");
	int var[4] = {0};
	fwrite(&var, sizeof(var), 4, fichier);
	printf("%d\n", fread(&var, sizeof(var), 1, fichier));
}
