#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]){
	FILE *fichier = fopen(argv[1], "r");
	uint32_t c[256];
	fread(c, sizeof(c[0]), 256, fichier);
	fclose(fichier);
	int i=0;
	for(i=0; i<256; i++){
		if(i%4==0 && i!=0){
			printf(" ");
		}
		if(i%32==0 && i!=0)
			printf("\n");
		printf("%d", c[i]);
	}
	printf("\n");
	return 0;
}
