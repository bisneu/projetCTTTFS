#ifndef DEF_PHYSICAL_LAYER

#define DEF_PHYSICAL_LAYER
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
	uint8_t *block_block;
};

error write_physical_block(disk_id id,block b, uint32_t num);
error read_physical_block(disk_id id, block b, uint32_t num);
int display(disk_id id, uint32_t nb_block);
void little_endian(block b);

#endif
