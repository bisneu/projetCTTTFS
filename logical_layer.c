#include "logical_layer.h"

/*
** Permet d'écrire un bloc b sur le disque id à l'emplacement num.
*/
error write_block(disk_id id, block b, uint32_t num){
	error rep;
	rep = write_physical_block(id, b, num);
	return rep;
}

/*
** Permet de lire un bloc b sur le disque id à l'emplacement num.
*/
error read_block(disk_id id, block b, uint32_t num){
	error rep;
	rep = read_physical_block(id, b, num);
	return rep;
}

/*
** Permet de manipuler un disque en lui associant un id.
*/
error start_disk(char *name, disk_id *id){
    	error rep;
	rep.error_id=1;
	if(exist_disk(name)==0){
		id->disk_name = name;
		id->disk_id = 0;
		FILE *tmp = fopen(name,"+r");
		if(tmp==NULL){
			rep.error_desc = "Erreur lors de l'ouverture.";	
			return rep;
		}
		id->disk_file = tmp;
		rep.error_id=0;
		return rep; 
	}
	rep.error_desc = "Erreur fichier introuvable.";
	return rep;
}

/*
** Permet de vérifier l'existence du fichier nommé name.
*/
int exist_disk(char *name){
	DIR *currentDir=opendir(".");
	struct dirent *entry=readdir(currentDir);
	do{
		if(strcmp(name,entry->d_name)==0){
			return 0;
		}
		entry=readdir(currentDir);
	}while(entry!=NULL);
	closedir(currentDir);	
	return 1;
}

/* 
** Permet de terminer une session de travaille sur un disque.
** Ferme le fichier qui correspond au disque et supprime tout 
** les pointeurs de la mémoire afin de mettre un terme à la session
*/
error stop_disk(disk_id id){
	error rep;
	rep.error_id = 1;
	if(id.disk_file!=NULL){
		rep.error_id = fclose(id.disk_file);
		if(rep.error_id!=0){
			rep.error_desc = "Erreur lors de la fermeture.";
			return rep;
		}
		return rep;
	}
	rep.error_desc = "Erreur fichier introuvable.";
	return rep;
}

/*
** 
*/
error sync_disk(disk_id id){
	error rep;
	return rep;
}

