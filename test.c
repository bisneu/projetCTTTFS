#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include <string.h>

typedef struct error error;
struct error{
	char *error_desc;
	uint32_t error_id;
};

typedef struct disk_id disk_id;
struct disk_id{
	uint32_t disk_id;
	char *disk_name;
	FILE *disk_file;
};

typedef struct block block;
struct block{
	uint8_t *block_block;
};

char convert_char(int i);
void convertir_8(uint8_t decimal, char* hexa);
uint32_t read_inblock(int indice, block b);

char convert_char(int i){
	if(i==0) return '0';
	else if(i==1) return '1';
	else if(i==2) return '2';
	else if(i==3) return '3';
	else if(i==4) return '4';
	else if(i==5) return '5';
	else if(i==6) return '6';
	else if(i==7) return '7';
	else if(i==8) return '8';
	else if(i==9) return '9';
	else if(i==10) return 'a';
	else if(i==11) return 'b';
	else if(i==12) return 'c';
	else if(i==13) return 'd';
	else if(i==14) return 'e';
	else if(i==15) return 'f';
	else return '0';	
}

void convertir_8(uint8_t decimal, char* hexa){
	hexa[0] = '0';
	hexa[1] = '0';
	int compteur=1;
	uint8_t tmp = decimal;
	while(tmp != 0){
		int modulo = tmp%16;
		hexa[compteur] = convert_char(modulo);
		compteur--;
		tmp = tmp/16;
	}
}

uint32_t read_inblock(int indice, const block b){
	char *hexa0 = malloc(sizeof(char)*2);
	char *hexa1 = malloc(sizeof(char)*2);
	char *hexa2 = malloc(sizeof(char)*2);
	char *hexa3 = malloc(sizeof(char)*2);
	uint8_t *decimal = malloc(sizeof(uint8_t)*4);
	int i=0;
	for(i=0;i<4;i++){
		decimal[i] = b.block_block[i+(indice*4)];
	}
	convertir_8(decimal[0],hexa0);
	convertir_8(decimal[1],hexa1);
	convertir_8(decimal[2],hexa2);
	convertir_8(decimal[3],hexa3);
	strcat(hexa3, strcat(hexa2, strcat(hexa1,hexa0)));
	uint32_t final;
	sscanf(hexa3, "%x", &final);
	printf("Hexa : 0x%x | Deci : %d\n", final, final);
	return final;
}

int main(int argc, char *argv[]){
	block b;
	int k=8;
	for(k=8; k<1024; k++){
		b.block_block[k]=0;
	}
	b.block_block[0]=71;
	b.block_block[1]=1;
	b.block_block[2]=0;
	b.block_block[3]=23;
	b.block_block[4]=84;
	b.block_block[5]=3;
	b.block_block[6]=0;
	b.block_block[7]=0;
	uint32_t var1 = read_inblock(0, b);
	uint32_t var2 = read_inblock(1, b);
	printf(" > Var1 : %d\n", var1);
	printf(" > Var2 : %d\n", var2);
	return 0;
}
