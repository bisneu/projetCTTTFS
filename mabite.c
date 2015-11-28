#include <stdio.h>
#include <stdlib.h>

int main(){
	int *p = malloc(sizeof(int));
	*p = 9;
	printf("%.2x %.2x %.2x %.2x\n",*(unsigned char *)p, *((unsigned char *)p+1), *((unsigned char *)p+2), *((unsigned char *)p+3));
	return 0;
}
