#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
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

imagem* P6_type(FILE *f, imagem *img)
{

	//img -> pixels = malloc(img -> width * img -> height * sizeof(img -> pixels));
	//fread(img->pixels, 3 * img->width, img->height, f);
	img -> pixels = malloc(img -> height * sizeof(img -> pixels));
	for (int i = 0; i < img -> height; i++)
		img -> pixels[i] = malloc(img -> width * 3 * sizeof(img -> pixels));	


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
		fread(img -> pixels[i], 1, 3 * img -> width, f);

	for (int k = 0; k < img -> height; k++)
		for (int l = 0; l < img -> width-2; l+=3)
		{
			r = img -> pixels[k][l];
			g = img -> pixels[k][l+1];
			b = img -> pixels[k][l+2];	

			r_a += (int)r;
			g_a += (int)g;
			b_a += (int)b;
		}

	img -> p_color  = malloc(sizeof(rgb));
	img -> p_color -> r = r_a/img -> max;
	img -> p_color -> g = g_a/img -> max;
	img -> p_color -> b = b_a/img -> max;
	fclose(f);

	return img;

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
        		im = malloc(100*sizeof(imagem*));
        		
        		char path[100] = "tiles/";
        		strcat(path, dir-> d_name);
        		//printf("%s\n",path );
        		//Inicia o file
        		FILE *fp;
        		if (!(fp = fopen(path,"r")))
        		{
        			printf("erro\n");
        			exit(1);
        		}
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

        		im = P6_type(fp, im);

		//   ARRUMAR ISSO PORA     		fclose(fp);
        		
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
	if (!(fp = fopen(file,"r")))
	{
		printf("deu merda\n");
		exit(1);
	}

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

	img = P6_type(fp, img);

	return img;
}

imagem *busca_pastilha(imagem **pastilhas, int n_pastilhas, rgb *medias)
{
	int n_pas_reco = 0;
	long int r_dif = 0;
	long int g_dif = 0;
	long int b_dif = 0;
	long int total = 0;
	long int menor = 0;
	for (int i = 0; i < n_pastilhas; i++)
	{
		r_dif =  (medias -> r) - pastilhas[i] -> p_color -> r;
		g_dif =  (medias -> g) - pastilhas[i] -> p_color -> g;
		b_dif =  (medias -> b) - pastilhas[i] -> p_color -> b;

		r_dif = r_dif * r_dif;
		g_dif = g_dif * g_dif;
		b_dif = b_dif * b_dif;


		total  = r_dif + g_dif + b_dif;
		total = sqrt(total);
		if (i == 0)
		{
			menor = total;
			n_pas_reco = i;

		}
		else if (total < menor)
		{
			menor = total;
			n_pas_reco = i;
		}
	}
	return pastilhas[n_pas_reco];

}
imagem *input_calc(imagem *img, imagem **pastilhas,int n_pastilhas)
{
	int d_height = pastilhas[0] -> height;
	int d_width = pastilhas[0] -> width;
	int r_m;
	int g_m;
	int b_m;
	printf("Largura %d / Altura %d\n",d_width,d_height );

	for (int i = 0; i < img -> height - d_height-1; i+=d_height)
	{
		for (int k = 0; k < 3*(img -> width) - d_width-1; k+=d_width)
		{
			r_m = 0;
			g_m = 0;
			b_m = 0;
			//Seção quadrados internos
			int cont;
			cont = 0;
			for (int j = i; j < i+d_height-1; j++)
			{
				for (int l = k; l < k + (3*(d_width))-3; l+=3)
				{
					r_m += img -> pixels[j][l];
					g_m += img -> pixels[j][l+1];
					b_m += img -> pixels[j][l+2];
					
					cont++;
				}
			}
			r_m = r_m /cont;
			g_m = g_m /cont;
			b_m = b_m /cont;

			rgb *medias;
			medias = malloc(sizeof(*medias));
			medias -> r = r_m;
			medias -> g = g_m;
			medias -> b = b_m;
			imagem *pastilha_slt;
			pastilha_slt = busca_pastilha(pastilhas,n_pastilhas,medias);
			//substitui pastilha no local
			for (int a = 0; a < d_height; a++)
				for (int b = 0; b < d_width; b+=3)
				{
					img -> pixels[i+a][k+b] = pastilha_slt -> pixels[a][b];
					img -> pixels[i+a][k+b+1] = pastilha_slt -> pixels[a][b+1];
					img -> pixels[i+a][k+b+1] = pastilha_slt -> pixels[a][b+2];
				}
		}
	}

	/*
	for (int i = 0; i < img -> width - d_width; i+= d_width)
		for (int k = 0; k <  3 * (img -> height - d_height); k+=d_height)
			for (int j = i; j < (i + d_width) - 1 ; j++)
				for (int l = k; l < (k + d_height) - 1; l++)
				{
					printf("%d \n",img -> pixels[j][l]);
				}*/
	return img;
}

void escreve_img(imagem *img)
{
	FILE * new;
	new = fopen("result.ppm", "wb");
	fprintf(new, "%s\n", img -> type);
	fprintf(new, "%d %d\n", img -> width, img -> height );
	fprintf(new, "%d",img -> scale);
	
	for (int i = 0; i < img -> height ; i++)
		fwrite(img -> pixels[i], 1, 3*img -> width, new);
	//fwrite(img->pixels, sizeof(img->pixels), 3*img->height*img->width, new);
	//or (int j = 0; j < img -> height; j++) 
	//	fwrite(img->pixels[3*img->width], 1, 3*img->width, new);

	fclose(new);


}
