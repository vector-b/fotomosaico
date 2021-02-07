#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "mosaic_class.h"

int calcula_tam(char *argv)
{
	int contador = 0;
	DIR *d;
    struct dirent *dir;
    d = opendir("./tiles");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            contador++;
        }
        closedir(d);
    }
    contador -= 2;
    return contador;
}

FILE* P3_type(FILE *f, imagem *img)
{
	return f;
}

FILE* P6_type(FILE *f, imagem *img)
{

	img -> pixels = malloc(img -> height * sizeof(unsigned char*));
	for (int i = 0; i < img -> height; i++)
		img -> pixels[i] = malloc(img -> width * 3 * sizeof(unsigned char*));	

	unsigned char r;
	unsigned char g;
	unsigned char b;
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
			fread(&r, 1, 1, f);
			img -> pixels[i][k] = r;
			fread(&g, 1, 1, f);
			img -> pixels[i][k+1] = g;
			fread(&b, 1, 1, f);
			img -> pixels[i][k+2] = b;

			r_a += (int)r;
			g_a += (int)g;
			b_a += (int)b;
		}
	}
	img -> p_color  = malloc(sizeof(rgb));
	img -> p_color -> r = r_a/img -> max;
	img -> p_color -> g = g_a/img -> max;
	img -> p_color -> b = b_a/img -> max;
	return f;
}

imagem **ler_pastilha(imagem **img)
{

	int count = 0;
	int index = 0;
	DIR *d;
    struct dirent *dir;
    d = opendir("./tiles");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
        	if (count >= 2)
        	{
        		//printf("%s\n", dir->d_name);
        		imagem *im;
        		im = malloc(sizeof(imagem));
        		
        		char path[100] = "tiles/";
        		strcat(path, dir-> d_name);
        		//printf("%s\n",path );
        		//Inicia o file
        		FILE *fp;
        		fp = fopen(path,"r");

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
        		//printf(" Tipo: %s \n Altura: %d \n Largura: %d \n Escala: %d \n Pixels: %d \n ",im -> type,im -> height, im -> width,im -> scale,im -> max);

        		if (strcmp(type,"P3") == 0)
        			fp = P6_type(fp, im);
        		else
        			fp = P6_type(fp, im);

        		fclose(fp);
        		//img[index] = malloc(100*sizeof(im));
        		img[index] = im;
        		//printf("%d\n",index );
        		index++;
        	}
            
            count++;

        }
        closedir(d);
    }

	return img;
}

imagem *ler_img(imagem *img, char file[100])
{
	FILE *fp;
	fp = fopen(file,"r");

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

	strcpy(img -> type , type);
	img -> width = width;
	img -> height = height;
	img -> scale = color_scale;
	img -> max = MAX_PIXELS;

	fp = P6_type(fp, img);
	fclose(fp);

	return img;
}