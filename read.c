#include <stdio.h>
#include <stdlib.h>

error read_physical_block(disk_id id, block b, uint32_t num){
		
	return 0;
}

int read(char *ptr_file){
	FILE *file = NULL;
	unsigned char octet = 0;
	file = fopen(ptr_file, "rb");
	if(fichier == NULL)
		fprintf(stderr, "Erreur lors de l'ouverture du fichier...\n");
	int i=0;
	while(fread(&octet, 1, sizeof(octet), file) != 0){
		if(i%4==0 && i!=0)
			printf(" ");
		if(i%32==0 && i!=0)
			printf("\n");
		printf("%02x ", octet);
		i++;
	}
	printf("\n");
	fclose(fichier);
	return 0;
}
