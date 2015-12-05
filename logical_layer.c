#include "logical_layer.h"

error write_block(disk_id id, block b, uint32_t num){
    error rep;
    rep = write_physical_block(id, b, num);
	return rep;
}

error read_block(disk_id id, block b, uint32_t num){
    error rep;
    rep = read_physical_block(id, b, num);
	return rep;
}

error start_disk(char *name, disk_id *id,){
    error rep;
	return rep;
}

/* 
** Ferme le fichier qui correspond au disque et supprime tout 
** les pointeurs de la mémoire afin de mettre un terme à la session
*/
error stop_disk(disk_id d){
	error rep;
	rep.error_id = 1;
	if(d.disk_file!=NULL){
		rep.error_id = fclose(d.disk_file);
		if(rep.error_id!=0){
			rep.error_desc = "Erreur fermeture";
			return rep;
		}
		return rep;
	}
	rep.error_desc = "Erreur fichier null";
	return rep;
}

error sync_disk(disk_id *id,){
    error rep;
	return rep;
}

