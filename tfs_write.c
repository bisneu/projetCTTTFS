#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]){
	FILE *fichier = fopen(argv[1], "w");
	uint32_t c[256]={1,2,3,4};
	fwrite(c, sizeof(uint32_t), 256, fichier);
	uint32_t cb[256]={1};
	fwrite(cb, sizeof(uint32_t), 256, fichier);
	fclose(fichier);
	return 0;
}
