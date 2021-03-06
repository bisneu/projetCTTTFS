#define TTTFS_MAGIC_NUMBER 0x31534654
#define TTTFS_VOLUME_BLOCK_SIZE 1024
#define TFS_REGULAR 0
#define TFS_DIRECTORY 1
#define TFS_PSEUDO 2
#define TFS_DATE 0
#define TFS_DISK 1

#include "logical_layer.h"

typedef struct free_entry free_entry;
struct free_entry{
	uint32_t id;
	uint32_t tfs_size;
	uint32_t tfs_type;
	uint32_t tfs_subtype;
	uint32_t *tfs_direct;
	uint32_t tfs_indirect1;
	uint32_t tfs_indirect2;
	uint32_t tfs_next_free;
};
typedef struct dir_entry dir_entry;

struct dir_entry{
	uint32_t nombre_caractere;	
	uint32_t file_number;
	uint32_t* name;
};

typedef struct directory directory;
struct directory{
	uint32_t nombre_fichier;
	dir_entry* tab; 
};

void initiate_description_block(block block_zero, block b, uint32_t partition,uint32_t nbr_fic);
void initiate_file_table(disk_id id ,block block_zero,int nbr_fic,int partition);
void initiate_block(block b, int n);
void initiate_data_block(disk_id id,uint32_t partition);
uint32_t get_first_free_block(disk_id id, uint32_t id_description_block);
uint32_t get_last_free_block(disk_id id, uint32_t id_description_block);
free_entry get_first_free_file(disk_id id, uint32_t id_description_block);
uint32_t get_last_free_file(disk_id id, uint32_t id_description_block);
uint32_t verif_chemin(char *chemin);
void bout_chemin_final(char *chemin, char* str);
