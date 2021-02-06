#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mosaic_class.h"


FILE* P3_type(FILE *f, imagem *img)
{
	return f;
}

FILE* P6_type(FILE *f, imagem *img)
{

	img -> pixels = malloc(img -> height * sizeof(unsigned char*));
	for (int i = 0; i < img -> height; i++)
		img -> pixels[i] = malloc(img -> width * 3 * sizeof(unsigned char*));	

	/*for (int i = 0; i < img -> height; i++)
		for (int k = 0; k < img -> width; k++)
			img -> pixels[i][k] = 'a';	
*/
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int cont = 0;
	int r_a;
	int g_a;
	int b_a;

	r_a = 0;
	g_a = 0;
	b_a = 0;
	
	for (int i = 0; i < img -> height; i++)
	{
		for (int k = 0; k < img -> width; k+=3)
		{		
			printf("Pixel -> %d - %d\n", i,k);
			fread(&r, 1, 1, f);
			img -> pixels[i][k] = r;
			fread(&g, 1, 1, f);
			img -> pixels[i][k+1] = g;
			fread(&b, 1, 1, f);
			img -> pixels[i][k+2] = b;

			r_a += (int)r;
			g_a += (int)g;
			b_a += (int)b;

			printf("%d\n",r_a );
			printf("%d\n",g_a);
			printf("%d\n",b_a);
			printf("\n");
			cont++;
		}
	}
	/*for (int i = 0; i < img -> max; i++)
	{
				
		fread(&r, 1, 1, f);
		fread(&g, 1, 1, f);
		fread(&b, 1, 1, f);

		r_a += (int)r;
		g_a += (int)g;
		b_a += (int)b;

		printf("%d\n",r_a );
		printf("%d\n",g_a);
		printf("%d\n",b_a);
		printf("\n");
		cont++;
	}*/
	printf("%d\n",img->height * img -> width );
	return f;
}

void ler_pastilha()
{

	/*struct imagem{
	char type;
	int width;
	int height;
	int scale;
	int max;
	char** pixels;
	int p_color;
};*/

	imagem *im;
	im = malloc(sizeof(imagem));
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

	int MAX_PIXELS;
	MAX_PIXELS = height * width;

	strcpy(im -> type , type);
	im -> width = width;
	im -> height = height;
	im -> scale = color_scale;
	im -> max = MAX_PIXELS;
	printf(" Tipo: %s \n Altura: %d \n Largura: %d \n Escala: %d \n Pixels: %d \n ",im -> type,im -> height, im -> width,im -> scale,im -> max);

	if (strcmp(type,"P3") == 0)
		fp = P3_type(fp, im);
	else
		fp = P6_type(fp, im);

}
