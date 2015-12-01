#include <stdio.h>
#include <stdlib.h>

typedef struct error error;
struct error{
	char *error_desc;
	int error_id;
};

typedef struct disk_id disk_id;
struct disk_id{
	char *disk_name;
};

typedef struct block block;
struct block{
	int block_size;
	int block_id;
};

error read_physical_block(disk_id id, block b, int num){
	error rep;
	FILE *disk = NULL;
	disk = fopen(id.disk_name, "r");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		return rep;
	}
	fclose(disk);
	return rep;
}

int read(char *ptr_file){
	FILE *file = NULL;
	file = fopen(ptr_file, "rb");
	unsigned char octet = 0;
	if(file == NULL)
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
	fclose(file);
	return 0;
}

int main(){
	printf("Execution en cours...\n");
	printf("...Execution terminée.\n");
}
