#include "ttfs_volume.h"

int add_free_block(disk_id id, uint32_t id_description_block, uint32_t id_block_to_add);
int remove_free_block(disk_id id, uint32_t id_description_block);
int add_free_entry(disk_id id, uint32_t id_description_block, uint32_t entry_to_add);
int remove_free_entry(disk_id id, uint32_t id_description_block);
int add_file_block();
int remove_file_block();
uint32_t verif_chemin(char *chemin);
void bout_chemin_final(char *chemin, char* str);
int existence_disk(char* chemin);
void transforme_ascii(char* str, uint32_t* tab);
uint32_t recherche_place_dossier(block dir);
uint32_t create_entree(dir_entry new_entry ,char* name, uint32_t file_number);
void ascii_to_string(dir_entry entry, char* str);
int add_file_dir(block dir, uint32_t file_number, char* name);
void get_file_entry(block table,uint32_t file_number, free_entry entry);
uint32_t get_free_block(block zero);
uint32_t first_free_file(block zero);
uint32_t get_free_file(block zero);
uint32_t first_free_block(block zero);
void  set_first_free_block(block zero,uint32_t elem);
void  set_first_free_file(block zero,uint32_t elem);
void  set_free_block(block zero,uint32_t elem);
void  set_free_file(block zero,uint32_t elem);
uint32_t nombre_partition(block zero);
uint32_t get_racine(block description);
int verif_path(char* path);
int mkdir(char* path);
