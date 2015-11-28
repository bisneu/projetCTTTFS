#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]){
	FILE *fichier = fopen(argv[1], "r");
	uint32_t c[256];
	fread(c, sizeof(uint32_t), 256, fichier);
	fclose(fichier);
	int i=0;
	unsigned bit = 0;
	unsigned mask = 1;
	for(i=0; i<256; i++){
/*
		if(i%4==0 && i!=0)
			printf(" ");
		if(i%32==0 && i!=0)
			printf("\n");
		printf("%d", c[i]);
*/
		unsigned bit = 0;
		unsigned mask = 1;
		int j;
		for(j=0; j<32; j++){
			if(j%4==0 && j!=0)
				printf(" ");
			bit = (c[i] & mask) >> j;
			printf("%d", bit);
			mask <<= 1;
		}
		printf("  ");
		if(i%2==0 && i!=0)
			printf("\n");
	}
	printf("\n");
	return 0;
}
