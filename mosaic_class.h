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

imagem **ler_pastilha(imagem **img, char *path_pas);
FILE* P3_type(FILE *f, imagem *img);
imagem* P6_type(FILE *f, imagem *img);
int calcula_tam(char *argv);
imagem *ler_img(imagem *img, char file[100], FILE *std, int val_din);
imagem *input_calc(imagem *img, imagem **pastilhas,int n_pastilhas);
imagem *busca_pastilha(imagem **pastilhas,int n_pastilhas, rgb *medias);
void escreve_img(imagem *img, char *file_out, FILE *sto, int val_out);
void desalocador(imagem **pastilhas, imagem *result, imagem *src, int n_pastilhas);

#endif