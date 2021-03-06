//Este software em linguagem C tem como objetivo criar um mosaico a partir de pequenas imagens, formando uma completa de acordo com as cores 
//Desenvolvido por Victor Gabriel em 02/2021

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include "mosaic_class.h"

int main(int argc, char *argv[])
{

	
	//Variáveis de verificação stdin/stdout
	int std = 1;
	int sto = 1;

	//Inicialização de strings
	char *file;
	char *file_out;
	char *path_past;
	path_past = "./tiles";
	
	//Estrutura de recebimento de parâmetros
	int opt; 
	while((opt = getopt(argc, argv, "i:o:p:h")) != -1)  
    {  
        switch(opt)  
        {  
            case 'i':
            	file = optarg;
            	std = 0;
            	break;
            case 'o':
                file_out = optarg;
                sto = 0;
            	break;
            case 'p':
                path_past = optarg;
            	break;
            case 'h':
            {
            	fprintf (stderr, "Este programa realiza mosaicos!\nUse -i (fonte) para indicar onde o programa deve procurar a imagem de origem\nUse -o (fonte) para indicar onde o progrmaa deve direcionar a imagem de saida\nUse -p (fonte) para indicar onde o programa deve procurar as pastilhas\n ");
            	exit(1);
            	break;
            }
        }  
    }  
	
    //Calcula o número de pastilhas na pasta indicada
	int n_pastilhas = 0 ;
	n_pastilhas = calcula_tam(path_past);

	printf("Reading tiles from %s\n",path_past );

	//Lê todas as pastilhas na pasta e armazenas em uma vetor de imagens
	imagem **pastilhas;	
	pastilhas = malloc(n_pastilhas*sizeof(**pastilhas));
	pastilhas = ler_pastilha(pastilhas,path_past);

	//Log
	printf("%d tiles read.\n",n_pastilhas );
	printf("Tile size is %dx%d\n", pastilhas[0] -> height , pastilhas[0] -> width);
	printf("Calculating tiles' average colors\n");

	//Alocação da imagem
	imagem *img_crt;
	img_crt = malloc(sizeof(*img_crt));


	int val_din = 0;
	int num;
	//Verifica a existência de stdin e recebe o arquivo
	fseek (stdin, 0, SEEK_END);
	num = ftell (stdin);
	if ((std == 1) && (num > 0))
	{
		printf("Input by STDIN\n");
		val_din = 1;
		rewind(stdin);
	}
	else if (std == 0)
	{
		printf("Input by -i\n");
	}
	
	printf("Reading input image\n");
	//Leitura de imagem 
	img_crt = ler_img(img_crt, file, stdin, val_din);

	printf("Input image is PPM %s, %dx%d pixels\n",img_crt -> type,img_crt -> height,img_crt -> width  );
	
	//Constroi o mosaico e retorna em result
	printf("Building mosaic image\n");
	imagem *result;
	//Imagem resultante após fazer o mosaico
	result = input_calc(img_crt,pastilhas,n_pastilhas);

	//Escreve o arquivo em uma saida
	printf("Writing output file\n");
	escreve_img(result,file_out, stdout, sto);
	
	//Desaloca
	desalocador(pastilhas,result,img_crt,n_pastilhas);
	
	return 0;
}