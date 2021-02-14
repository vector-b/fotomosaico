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

	

	int std = 1;
	int sto = 1;

	char *file;
	char *file_out;
	char *path_past;
	path_past = "./tiles20";
	
	int opt; 
	while((opt = getopt(argc, argv, "i:o:p:h")) != -1)  
    {  
        switch(opt)  
        {  
            case 'i':
            	//printf("filename: %s\n", optarg);
            	file = optarg;
            	std = 0;
            	break;
            case 'o':
                //printf("filename: %s\n", optarg);
                file_out = optarg;
                sto = 0;
            	break;
            case 'p':
                path_past = optarg;
            	break;
            case 'h':
            {
            	fprintf (stderr, "Este programa realiza mosaicos! \n");
            	exit(1);
            	break;
            }
        }  
    }  
	

	int n_pastilhas = 0 ;
	n_pastilhas = calcula_tam(path_past);

	imagem **pastilhas;	
	pastilhas = malloc(n_pastilhas*sizeof(**pastilhas));
	pastilhas = ler_pastilha(pastilhas,path_past);

	printf("%d partilhas carregadas.\n",n_pastilhas );


	imagem *img_crt;
	img_crt = malloc(sizeof(*img_crt));

	int val_din = 0;
	int num;
	fseek (stdin, 0, SEEK_END);
	num = ftell (stdin);
	if ((std == 1) && (num > 0))
	{
		printf("Entrada recebida por STDIN\n");
		val_din = 1;
		rewind(stdin);
	}
	else if (std == 0)
	{
		printf("Entrada recebida por Parâmetro\n");
	}
	
	//Leitura de imagem 
	img_crt = ler_img(img_crt, file, stdin, val_din);

	
	
	imagem *result;
	result = input_calc(img_crt,pastilhas,n_pastilhas);


	
	escreve_img(result,file_out, stdout, sto);

	desalocador(pastilhas,result,img_crt,n_pastilhas);
	
	return 0;
}