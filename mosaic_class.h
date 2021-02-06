#ifndef __MOSAIC__
#define __MOSAIC__


struct rgb
{
	int r;
	int g;
	int b;	
};
typedef struct rgb rgb;

struct imagem{
	char type[2];
	int width;
	int height;
	int scale;
	int max;
	unsigned char** pixels;
	rgb* p_color;
};

typedef struct imagem imagem;

void ler_pastilha();
FILE* P3_type(FILE *f, imagem *img);
FILE* P6_type(FILE *f, imagem *img);


#endif