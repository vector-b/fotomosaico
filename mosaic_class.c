#include <stdio.h>
#include <string.h>
#include "mosaic_class.h"


void ler_pastilha()
{
	//Inicia o file
	FILE *fp;
	fp = fopen("tiles20/00020FA269E3AAE8538F4B0153DD06D5E799209F.ppm","r");

	//Captura os tipos, tamanhos e color scale
	char type[2];
	fscanf(fp, "%s", type); 
	
	int height;
	int width;
	int color_scale; 
	fscanf(fp, "%d", &width);
	fscanf(fp, "%d", &height); 
	fscanf(fp, "%d", &color_scale); 


	printf(" Tipo: %s \n Altura: %d \n Largura: %d \n Escala: %d \n ",type,height,width,color_scale);

	if (strcmp(type,"P3") == 0)
		P3_type(fp);
	else
		P6_type(fp);

}

FILE* P3_type(FILE *f)
{
	return f;
}
FILE* P6_type(FILE *f)
{
	return f;
}