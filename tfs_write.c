#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]){
	FILE *fichier = fopen(argv[1], "w");
	uint32_t c[512]={5,8,3,6};
	fwrite(c, sizeof(uint32_t), 512, fichier);
	fclose(fichier);
	return 0;
}
