#ifndef __MOSAIC__
#define __MOSAIC__

//Struct rgb, a qual foi utilizada para definir cor média e mais frequente 
struct rgb
{
	int r;
	int g;
	int b;	
};
typedef struct rgb rgb;

//Struct principal do programa, a qual compõe tudo no que se refere a ler e a escrever as imagens, armazena todos os pixels em **pixels
//Também comporta os tipos e detalhes adicionais
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

//Definição das funções
imagem **ler_pastilha(imagem **img, char *path_pas);
imagem* P_type(FILE *f, imagem *img);
imagem *ler_img(imagem *img, char file[100], FILE *std, int val_din);
imagem *input_calc(imagem *img, imagem **pastilhas,int n_pastilhas);
imagem *busca_pastilha(imagem **pastilhas,int n_pastilhas, rgb *medias);
void escreve_img(imagem *img, char *file_out, FILE *sto, int val_out);
void desalocador(imagem **pastilhas, imagem *result, imagem *src, int n_pastilhas);
int calcula_tam(char *argv);
#endif