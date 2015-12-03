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
	FILE *disk =NULL;
	disk = fopen(id.disk_name,"w");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		return rep;
	}
	fseek(disk,(num*1024),SEEK_SET);
	fwrite(b.block_block, sizeof(uint32_t), 1, disk);
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
			printf("ERREUR\n");
		printf("\n");
		printf("APRES read_physical_block\n");
		printf("block_block[0] = %d \n",monblock.block_block[0]);
		printf("block_block[1] = %d \n",monblock.block_block[1]);
		printf("block_block[2] = %d \n",monblock.block_block[2]);
		printf("block_block[3] = %d \n",monblock.block_block[3]);
		check = write_physical_block(mondisque, monblock, 1);
		if(check.error_id != 0)
			printf("ERREUR WRITE\n");
		check = read_physical_block(mondisque, monblock, 1);
		if(check.error_id != 0)
			printf("ERREUR READ\n");
		printf("\n");
		printf("APRES write_physical_block\n");
		printf("block_block[0] = %d \n",monblock.block_block[0]);
		printf("block_block[1] = %d \n",monblock.block_block[1]);
		printf("block_block[2] = %d \n",monblock.block_block[2]);
		printf("block_block[3] = %d \n",monblock.block_block[3]);
	printf("...Execution terminée.\n");
}
