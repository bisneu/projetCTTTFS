#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>

typedef struct error error;
struct error{
	char *error_desc;
	uint32_t error_id;
};

typedef struct disk_id disk_id;
struct disk_id{
	uint32_t disk_id;
	char *disk_name;
	FILE *disk_file;
};

typedef struct block block;
struct block{
	uint32_t *block_block;
};

void little_endian(block b){
	int i=0;
	for(i=0; i<256; i++){
		uint32_t tmp = htole32(b.block_block[i]);
		b.block_block[i] = tmp;
	}
}

error write_physical_block(disk_id id,block b, uint32_t num){
	error rep;
	rep.error_id = 0;
	FILE *disk = fopen(id.disk_name,"r+");
	if(disk==NULL){
		rep.error_desc = "Disk introuvable";
		rep.error_id = 1;
		return rep;
	}
	fseek(disk,(num*1024),SEEK_SET);
	little_endian(b);
	fwrite(b.block_block, sizeof(uint32_t), 256, disk);
	fclose(disk);
	return rep;
}

/* Ferme le fichier qui correspond au disque et supprime tout les pointeurs de la mémoire afin de mettre un terme à la session*/
error stop_disk(disk_id d){
	error rep;
	rep.error_id = 1;
	if(d.disk_file!=NULL){
		rep.error_id = fclose(d.disk_file);
		if(rep.error_id!=0){
			rep.error_desc = "erreur fermeture";
			return rep;
		}
		return rep;
	}
	rep.error_desc = "erreur fichier null";
	return rep;
}

error read_physical_block(disk_id id, block b, uint32_t num){
	error rep;
	rep.error_id = 0;
	FILE *disk = fopen(id.disk_name, "r");
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

int display(disk_id id, uint32_t nb_block){
	FILE *disk = fopen(id.disk_name, "r");
	int nb_elem = nb_block*256;
	uint32_t *c = NULL;
	c = malloc(nb_block*256*4);
	fread(c, sizeof(uint32_t), nb_elem, disk);
	fclose(disk);
	int i=0;
	printf("\n| ");
	for(i=0; i<nb_elem; i++){
		if(i%4 == 0 && i!=0){printf(" | ");}
		if(i%256 == 0 && i!=0){printf("\n");}
		if(i%32 == 0 && i!=0){printf("\n| ");}
		printf("%d", c[i]);
	}
	printf(" |\n\n");
	return 0;
}

int main(int argc, char *argv[]){
	printf("Execution en cours...\n\n");
	block my_block; my_block.block_block = malloc(1024);
	disk_id my_disk; my_disk.disk_name = "tabite.txt";
	printf("AVANT read_physical_block(my_disk, my_block, 0) : block_block[0,1,2,3] = {%d,%d,%d,%d} \n", my_block.block_block[0], my_block.block_block[1], my_block.block_block[2], my_block.block_block[3]);
	error check = read_physical_block(my_disk, my_block, 0);
	if(check.error_id != 0)
		printf("--> ERREUR READ <--\n");
	printf("\nAPRES read_physical_block(my_disk, my_block, 0) : block_block[0,1,2,3] = {%d,%d,%d,%d} \n", my_block.block_block[0], my_block.block_block[1], my_block.block_block[2], my_block.block_block[3]);
	display(my_disk, 2);
	printf("block_block[0,1,6,7] = {0,0,9,9}\n");
	my_block.block_block[0] = 0; my_block.block_block[1] = 0;
	my_block.block_block[6] = 9; my_block.block_block[7] = 9;
	check = write_physical_block(my_disk, my_block, 1);
	if(check.error_id != 0)
		printf("--> ERREUR WRITE <--\n");
	printf("\nAVANT read_physical_block(my_disk, my_block, 1) : block_block[2,3,6,7] = {%d,%d,%d,%d} \n", my_block.block_block[2], my_block.block_block[3], my_block.block_block[6], my_block.block_block[7]);
	check = read_physical_block(my_disk, my_block, 1);
	if(check.error_id != 0)
		printf("--> ERREUR READ <--\n");
	printf("\nAPRES read_physical_block(my_disk, my_block, 1) : block_block[2,3,6,7] = {%d,%d,%d,%d} \n", my_block.block_block[2], my_block.block_block[3], my_block.block_block[6], my_block.block_block[7]);
	printf("\nAPRES write_physical_block(my_disk, my_block, 1)\n");
	display(my_disk, 2);	
	printf("...Execution terminée.\n");




}
