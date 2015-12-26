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
	uint32_t tfs_size;
	uint32_t tfs_type;
	uint32_t tfs_subtype;
	uint32_t *tfs_direct;
	uint32_t tfs_indirect1;
	uint32_t tfs_indirect2;
	uint32_t tfs_next_free;
}


void initiate_description_block(block block_zero, block b,int partition,int nbr_fic);

int first_block_partition(block block_zero, int partition); 


