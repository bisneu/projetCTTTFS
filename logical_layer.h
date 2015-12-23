#ifndef DEF_LOGICAL_LAYER

#define DEF_LOGICAL_LAYER
#include "physical_layer.h"
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <endian.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>

char convert_char(int i);
void convertir_32(int decimal, char* hexa);
void convertir_8(uint8_t decimal, char* hexa);
uint8_t hexa_to_int(char* hex);
uint8_t concat(char c, char c2);
void convert_to_decimal(char* t, uint8_t* tab);
void my_little_endian(uint32_t elem, uint8_t *tab);
void write_size_inblock(int size, FILE* file);
void write_inblock(block b,int index, uint32_t elem);
uint32_t read_inblock(int indice, block b);
uint32_t total_partition(block b,uint32_t nbr_partition);
int exist_disk(char *name);
error write_block(disk_id id, block b, uint32_t num);
error read_block(disk_id id, block b, uint32_t num);
error start_disk(char *name, disk_id *id);
error stop_disk(disk_id id);
error sync_disk(disk_id id);
#endif
