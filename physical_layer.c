#include "physical_layer.h"

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
	fwrite(b.block_block,sizeof(uint8_t),1024,disk);
	fclose(disk);
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
	fread(b.block_block, sizeof(uint8_t),1024, disk);
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

void little_endian(block b){
	int i=0;
	for(i=0; i<256; i++){
		uint32_t tmp = htole32(b.block_block[i]);
		b.block_block[i] = tmp;
	}
}

