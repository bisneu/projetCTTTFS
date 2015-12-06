#include "logical_layer.h"
#include <dirent.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

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


error start_disk(char *name, disk_id *id){
    	error rep;
	rep.error_id=1;
	if(exist_disk(name)==0){
		id->disk_name = name;
		id->disk_id = 0;
		FILE *tmp = fopen(name,"+r");
		if(tmp==NULL){
			rep.error_desc = "probleme d'ouverture";	
			return rep;
		}
		id->disk_file = tmp;
		rep.error_id=0;
		return 	 rep; 
	}
	rep.error_desc = "Le fichier n'existe pas";
	return rep;
}


int exist_disk(char *name){
	DIR *myrep=opendir(".");
	struct dirent *cont=readdir(myrep);
	do{
		if(strcmp(name,cont->d_name)==0){
			return 0;
		}
		cont=readdir(myrep);
	}while(cont!=NULL);		
	return 1;
}

/* 
** Ferme le fichier qui correspond au disque et supprime tout 
** les pointeurs de la mémoire afin de mettre un terme à la session
*/
error stop_disk(disk_id id){
	error rep;
	rep.error_id = 1;
	if(id.disk_file!=NULL){
		rep.error_id = fclose(id.disk_file);
		if(rep.error_id!=0){
			rep.error_desc = "Erreur fermeture";
			return rep;
		}
		return rep;
	}
	rep.error_desc = "Erreur fichier null";
	return rep;
}

error sync_disk(disk_id id){
    error rep;
	return rep;
}

