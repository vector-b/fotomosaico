//Este software em linguagem C tem como objetivo criar um mosaico a partir de pequenas imagens, formando uma completa de acordo com as cores 
//Desenvolvido por Victor Gabriel em 02/2021

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mosaic_class.h"

int main(int argc, char *argv[])
{

	int n_pastilhas = 0 ;
	char *path = argv[0];
	n_pastilhas = calcula_tam(path);
	
	imagem **pastilhas;	
	pastilhas = malloc(n_pastilhas*sizeof(**pastilhas));
	pastilhas = ler_pastilha(pastilhas);

	printf("%d pastilhas carregadas.\n",n_pastilhas );

	char file[100] = "macaco.ppm";
	imagem *img_crt;
	img_crt = malloc(sizeof(*img_crt));
	
	img_crt = ler_img(img_crt, file);

	printf("Imagem >%s< lida com sucesso.\n",file);

	
	imagem *result;
	result = input_calc(img_crt,pastilhas,n_pastilhas);
	escreve_img(result);

	
	return 0;
}