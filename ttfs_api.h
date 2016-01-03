#include "ttfs_volume.h"

int add_free_block(disk_id id, uint32_t id_description_block, uint32_t id_block_to_add);
int remove_free_block(disk_id id, uint32_t id_description_block);
int add_free_entry(disk_id id, uint32_t id_description_block);
int remove_free_entry(disk_id id, uint32_t id_description_block);
