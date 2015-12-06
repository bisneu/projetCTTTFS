#include "tfs_create.h"

int main(int argc, char *argv[]){ // tfs_create -s size [name]
	FILE *file = fopen(argv[3], "w");
	int nb_elem = atoi(argv[2])*256;
	uint32_t *c = NULL;
	c = malloc(nb_elem*4);
	int i=0;
	for(i=0; i<nb_elem; i++){
		c[i]=0;
	}
	fwrite(c, sizeof(uint32_t), nb_elem, file);
	fclose(file);
	return 0;
}
