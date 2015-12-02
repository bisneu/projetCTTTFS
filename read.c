#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
	int block_id;
	int block_block[1024];
};

error write_physical_block(disk_id id,block b, uint32_t num){
	error rep;
	FILE *disk =NULL;
	disk = fopen(id.disk_name,"w");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		return rep;
	}
	fseek(disk,(num*1024),SEEK_SET);
	fwrite(b.block_block,1,sizeof(uint32_t),disk);
	return rep;
}

error read_physical_block(disk_id id, block b, uint32_t num){
	error rep;
	FILE *disk = NULL;
	disk = fopen(id.disk_name, "r");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		return rep;
	}
	fseek(disk,num*1024,SEEK_SET);
	fread(b.block_block, 256, sizeof(uint32_t), disk);
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
		block monblock;
		monblock.block_id = 1;
		disk_id mondisque;
		mondisque.disk_name = "tabite.txt";
		read_physical_block(mondisque,monblock,0);
		printf("%d \n",monblock.block_block[0]);
	printf("...Execution terminée.\n");
}
