#define TTTFS_MAGIC_NUMBER 0x31534654
#define TTTFS_VOLUME_BLOCK_SIZE 1024
#define TFS_REGULAR 0
#define TFS_DIRECTORY 1
#define TFS_PSEUDO 2
#define TFS_DATE 0
#define TFS_DISK 1
#include "logical_layer.h"

void initiate_description_block(block block_zero, block b,int partition,int nbr_fic);

int first_block_partition(block block_zero, int partition); 


