#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fichier;
	fichier = fopen("test.bin", "wb");
	int var = 15;
	fwrite(&var, sizeof(int), 1, fichier);
}
