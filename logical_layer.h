#ifndef DEF_LOGICAL_LAYER

#define DEF_LOGICAL_LAYER
#include "physical_layer.h"
#include <dirent.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

error write_block(disk_id id, block b, uint32_t num);
error read_block(disk_id id, block b, uint32_t num);
error start_disk(char *name, disk_id *id);
error stop_disk(disk_id id);
error sync_disk(disk_id id);
int exist_disk(char *name);
#endif
