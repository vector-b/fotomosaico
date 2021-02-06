//Este software em linguagem C tem como objetivo criar um mosaico a partir de pequenas imagens, formando uma completa de acordo com as cores 
//Desenvolvido por Victor Gabriel em 02/2021

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mosaic_class.h"

int main(int argc, char const *argv[])
{
	int n_pastilhas = 0 ;
	n_pastilhas = calcula_tam();
	
	imagem **pastilhas;	
	pastilhas = malloc(sizeof(pastilhas));
	printf("pastilhas -> %d\n",n_pastilhas );
	pastilhas = ler_pastilha(pastilhas);

	return 0;
}