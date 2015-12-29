#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h> 
#include <dirent.h> 

int verif_file(char *str, DIR *cur_dir){
	struct dirent *entry = readdir(cur_dir);	
	do{
		if(strcmp(str,entry->d_name)==0){
			if(((entry->d_type)==DT_UNKNOWN) || (entry->d_type)==DT_REG){	
				return 0;
			}
			fprintf(stderr,"Ce n'est pas un fichier");
			return 1;
		}
		entry = readdir(cur_dir);	
	}while(entry!=NULL);
	fprintf(stderr,"Le fichier n'existe pas");
	return 1;
}

int verif_rep(char *str, DIR *cur_dir){
	struct dirent *entry = readdir(cur_dir);	
	do{
		if(strcmp(str,entry->d_name)==0){
			if((entry->d_type)==DT_DIR){	
				return 0;
			}
			fprintf(stderr,"Ce n'est pas un répertoire");
			return 1;
		}
		entry = readdir(cur_dir);	
	}while(entry!=NULL);
	fprintf(stderr,"Le répertoire n'existe pas");
	return 1;
}


int  compte_separateur(char* str){	
	int i=0;
	int compteur=0;
	while(*(str+i) != '\0'){
		if( *(str+i) == '/'){
			compteur++;
		}
		i++;
	}
	return compteur;	
}

char* bout_chemin(char *str,int del){
	int i=0;
	int j=0;
	int compteur = 0;
	char *str_tmp = malloc(strlen(str)*sizeof(char));
	for(i=0; i<strlen(str); i++){
		*(str_tmp+i) = '\0';
	}
	i=0;
	while(*(str+i) != '\0'){
		if(*(str+i)=='/'){
			compteur ++;
		}
		else if((compteur > del-1)&&(compteur<=del)){
			*(str_tmp+j) = *(str+i);
			j++;
		}
		i++;	
	}
	return str_tmp;		
}

int rec_open_file(char *str, int i, DIR *cur_dir, FILE *file){
	char *tmp = bout_chemin(str,i);
	printf("%s\n",tmp);
	if(i>=compte_separateur(str)){
		if(verif_file(tmp,cur_dir)==0){
			file = fopen(bout_chemin(str,i),"r+");
			return 0;
		}
		else { 
			return 1; 
		}	
	}
	else {
		if(verif_rep(tmp,cur_dir) == 0){
			cur_dir = opendir(tmp);	
			rec_open_file(str,i+1,cur_dir,file);			
		} 
		else return 1;
	}	
}


int main(int argc, char **argv){
	char *tata = "../projetCTTTFS/disk.tfs";
	FILE *tmp;
	DIR *cur_dir = opendir(".");
	rec_open_file(tata,0,cur_dir,tmp);
	char *buff = malloc(1024);
	fread(buff, sizeof(uint8_t),1024,tmp);
	return 0;
}
