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
	int *block_block;
};

error write_physical_block(disk_id id,block b, uint32_t num){
	error rep;
	rep.error_id = 0;
	FILE *disk =NULL;
	disk = fopen(id.disk_name,"w");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		return rep;
	}
	printf("%ld\n", ftell(disk));
	fseek(disk,(num*1024),SEEK_SET);
	printf("%ld\n", ftell(disk));
	fwrite(b.block_block, sizeof(uint32_t), 256, disk);
	fclose(disk);
	return rep;
}

error read_physical_block(disk_id id, block b, uint32_t num){
	error rep;
	rep.error_id = 0;
	FILE *disk = NULL;
	disk = fopen(id.disk_name, "r");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		printf("ERREUR\n");
		return rep;
	}
	fseek(disk,(num*1024),SEEK_SET);
	fread(b.block_block, sizeof(uint32_t), 256, disk);
	fclose(disk);
	return rep;
}

int display(disk_id id, block b, uint32_t num){
	FILE *disk = NULL;
	disk = fopen(id.disk_name, "r");
	uint32_t c[256];
	fread(c, sizeof(uint32_t), 1024, disk);
	fclose(disk);
	int i=0;
	printf("| ");
	for(i=0; i<256; i++){
		if(i%4 == 0 && i!=0){printf(" | ");}
		if(i%32 == 0 && i!=0){printf("\n| ");}
		printf("%d", c[i]);
	}
	printf(" |\n");
	return 0;
}

int main(){
	printf("Execution en cours...\n");
		block monblock;
		monblock.block_block = malloc(1024);
		monblock.block_id = 1;
		printf("AVANT read_physical_block\n");
		printf("block_block[0] = %d \n",monblock.block_block[0]);
		printf("block_block[1] = %d \n",monblock.block_block[1]);
		printf("block_block[2] = %d \n",monblock.block_block[2]);
		printf("block_block[3] = %d \n",monblock.block_block[3]);
		disk_id mondisque;
		mondisque.disk_name = "tabite.txt";
		error check = read_physical_block(mondisque, monblock, 0);
		if(check.error_id != 0)
			printf("ERREUR READ\n");
		printf("\n");
		printf("APRES read_physical_block\n");
		printf("block_block[0] = %d \n",monblock.block_block[0]);
		printf("block_block[1] = %d \n",monblock.block_block[1]);
		printf("block_block[2] = %d \n",monblock.block_block[2]);
		printf("block_block[3] = %d \n",monblock.block_block[3]);
		display(mondisque, monblock, 0);
		monblock.block_block[4] = 9;
		monblock.block_block[5] = 9;
		monblock.block_block[6] = 9;
		monblock.block_block[7] = 9;
		check = write_physical_block(mondisque, monblock, 0);
		if(check.error_id != 0)
			printf("ERREUR WRITE\n");
		check = read_physical_block(mondisque, monblock, 0);
		if(check.error_id != 0)
			printf("ERREUR READ\n");
		printf("\n");
		printf("APRES write_physical_block\n");
/*
		printf("block_block[0] = %d \n",monblock.block_block[0]);
		printf("block_block[1] = %d \n",monblock.block_block[1]);
		printf("block_block[2] = %d \n",monblock.block_block[2]);
		printf("block_block[3] = %d \n",monblock.block_block[3]);
*/
		display(mondisque, monblock, 0);
	printf("...Execution terminée.\n");
}
