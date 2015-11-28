#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]){
	FILE *fichier = fopen(argv[1], "w");
	uint32_t c[256]={0};
	fwrite(c, sizeof(uint32_t), 256, fichier);
	fclose(fichier);
	return 0;
}
