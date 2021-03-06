#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "mosaic_class.h"

//Calcula a quantidade de pastilhas de um dado diretório
int calcula_tam(char *argv)
{
	//Inicia um contador 
	int contador = 0;
	DIR *d;
    struct dirent *dir;
    d = opendir(argv);
    //Percorre toda a pasta de pastilhas, caso não encontre, retorna erro
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            contador++;
        }
        closedir(d);
    }
    contador -= 2;
    if (contador < 0 )
    {
    	fprintf (stderr, "Não foi possível encontrar pastilhas! Tente Novamente... \n");
            exit(1);
    }
    return contador;
}
//Função usada para salvar os dados da imagem e armazená-los
imagem* P_type(FILE *f, imagem *img)
{
	img -> pixels = malloc(img -> height * sizeof(img -> pixels));
	for (int i = 0; i < img -> height; i++)
		img -> pixels[i] = malloc(img -> width * 3 * sizeof(img -> pixels));	

	//Inicia chars 
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int r_a;
	int g_a;
	int b_a;

	r_a = 0;
	g_a = 0;
	b_a = 0;

	//Percorre o File e salva na matriz
	for (int i = 0; i < img -> height; i++)
		fread(img -> pixels[i], 1, 3 * img -> width, f);

	//Calcula a cor media de uma imagem
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

imagem* P3_type(FILE *f, imagem *img)
{
	//Reliza a alocação da matrix utilizada
	img -> pixels = malloc(img -> height * sizeof(img -> pixels));
	for (int i = 0; i < img -> height; i++)
		img -> pixels[i] = malloc(img -> width * 3 * sizeof(img -> pixels));	

	//Inicia chars 
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int r_a;
	int g_a;
	int b_a;

	r_a = 0;
	g_a = 0;
	b_a = 0;
	//Percorre o File e salva na matriz
	for (int i = 0; i < img -> height; i++)
		for (int k = 0; k < 3* img -> width; k++)
		{
			int number;
			fscanf(f,"%d",&number);
			img -> pixels[i][k] = number;
		}
	//Calcula a cor media de uma imagem
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
//Função responsável por percorrer o diretório e procurar pastilhas
imagem **ler_pastilha(imagem **img, char *path_pas)
{

	int count = 0;
	int index = 0;
	DIR *d;
	FILE *fp;
    struct dirent *dir;
    //Percorre todo o diretório procurando pelo nome das pastilhas
    d = opendir(path_pas);
    if (d)
    {
    	//Enquanto a busca por arquivos não for nula
        while ((dir = readdir(d)) != NULL)
        {
        	if (count >= 2)
        	{
        		//Inicia a imagem 
        		imagem *im;
        		im = malloc(100*sizeof(imagem*));
        		
        		char path[100] = "";
        		strcat(path,path_pas);
        		strcat(path, "/");
        		strcat(path, dir-> d_name);
        		//Inicia o file
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
        		//Lê os dados das pastilhas e armazena no tipo imagem para pastilhas
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
        		im = P_type(fp, im);

				
        		img[index] = im;
        		index++;
        	}
            
            count++;

        }
        closedir(d);
    }
    rewind(fp);
    fclose(fp);
	return img;
}
//Função responsável por ler a imagem do file  
imagem *ler_img(imagem *img, char file[100], FILE *std, int val_din)
{
	FILE *fp;
	if (!val_din)
	{
		if (!(fp = fopen(file,"r")))
		{
			fprintf (stderr, "Não foi possível encontrar ou abrir esse arquivo! Tente Novamente... \n");
            exit(1);
		}
	}
	else
	{
		fp = std;
	}
	

	//Captura os tipos, tamanhos e color scale
	char type[2];
	char buffer[100];

	//Captura o tipo P6 ou P3
	fscanf(fp, "%s\n", type);

	if((strcmp(type,"P6") != 0) && (strcmp(type,"P3") != 0))
	{
		fprintf (stderr, "Tipo de arquivo inválido! \n");
		exit(1);
	}
	int len = ftell(fp);
	int pass = 1;
	while (pass)
	{
		fgets(buffer, 100, fp);
		if (buffer[0] != '#')
		{
			pass = 0;
			fseek( fp, len, SEEK_SET );
		}
		else
			len = ftell(fp);
	}
	//Arruma bug em Type
	type[2] = '\0';
	
	int height;
	int width;
	int color_scale; 

	fscanf(fp, "%d", &width);
	fscanf(fp, "%d", &height); 
	fscanf(fp, "%d", &color_scale); 

	int MAX_PIXELS;
	MAX_PIXELS = height * width;

	strcpy(img -> type , type);

		
	//Armazena no tipo arquivo imagem
	img -> width = width;
	img -> height = height;
	img -> scale = color_scale;
	img -> max = MAX_PIXELS;

	if (strcmp(type,"P6") == 0)
	{
		img = P_type(fp, img);
	}
	else
	{
		img = P3_type(fp, img);
	}
	

	return img;
}
//Função responsável por buscar as pastilhas em seus respectivos diretórios e retornar para um bloco NxN da imagem
imagem *busca_pastilha(imagem **pastilhas, int n_pastilhas, rgb *medias)
{
	int n_pas_reco = 0;
	long int r_dif = 0;
	long int g_dif = 0;
	long int b_dif = 0;
	long int total = 0;
	long int menor = 0;
	//Percorre o caminho/numero de todas as pastilhas procurando a pastilha com a media de cor apta para o NXN da imagem grande
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
//Função principal do efeito mosaico, procura em blocos NxN por pastilhas com cor correspondente
imagem *input_calc(imagem *img, imagem **pastilhas,int n_pastilhas)
{
	//Encontra o NxN baseado na primeira pastilha
	int d_height = pastilhas[0] -> height;
	int d_width = pastilhas[0] -> width;
	int r_m;
	int g_m;
	int b_m;
	int ini;
	for (int i = 0; i < img -> height - d_height + 1; i+=d_height)
	{
		//Se for P3 engole a primeira coluna de pixels 
		if ((strcmp(img -> type,"P3") == 0))
			ini = 2;
		else
			ini = 0;
		for (int k = ini; k < 3*(img -> width) - d_width; k+=3*d_width)
		{
			if (!(((i) == 0) && (k) == 0 ))
			{
				r_m = 0;
				g_m = 0;
				b_m = 0;
				//Seção quadrados internos
				int cont;
				cont = 0;
			//Vetor usado caso queira usar mediana invés de média dos quadrados

			//int r_v[d_height * d_width];
			//int g_v[d_height * d_width];
			//int b_v[d_height * d_width];
				for (int j = i; j < i + d_height; j++)
					for (int l = k; l < k + (3*(d_width))-3; l+=3)
					{
						r_m += img -> pixels[j][l] * img -> pixels[j][l];
						g_m += img -> pixels[j][l+1] * img -> pixels[j][l+1];
						b_m += img -> pixels[j][l+2] * img -> pixels[j][l+2];


					//r_v[cont] = r_m;
					//g_v[cont] = g_m;
					//b_v[cont] = b_m;
						cont++;
					}

			//r_m = r_v[d_height*d_height/2 +1];
			//g_m = g_v[d_height*d_height/2 +1];
			//b_m = b_v[d_height*d_height/2 +1];

				//Calcula a raizes das somas 
				r_m = sqrt(r_m/cont);
				g_m = sqrt(g_m/cont);
				b_m = sqrt(b_m/cont);
				//faz as medias
				rgb *medias;
				medias = malloc(sizeof(*medias));
				medias -> r = r_m * 0.25;
				medias -> g = g_m * 0.25;
				medias -> b = b_m * 0.25;
				//Seletor de pastilhas, procura uma pastilha com a cor indicada em médias
				imagem *pastilha_slt;
				pastilha_slt = busca_pastilha(pastilhas,n_pastilhas,medias);
				//Substitui pastilha no local
				for (int a = 0; a < d_height; a++)
					for (int b = 0; b < 3*d_width-3; b+=3)
					{
						img -> pixels[i+a][k+b] = pastilha_slt -> pixels[a][b];
						img -> pixels[i+a][k+b+1] = pastilha_slt -> pixels[a][b+1];
						img -> pixels[i+a][k+b+2] = pastilha_slt -> pixels[a][b+2];

					}
				}
			}
	}

	return img;
}
//Função responsável por imprimir a imagem resultante no arquivo de saída
void escreve_img(imagem *img, char *file_out, FILE *sto, int val_out)
{
	FILE *new;

	if (!val_out)
	{
		if(!(new = fopen(file_out, "wb")))
		{
			fprintf (stderr, "Não foi possível abrir o arquivo! \n");
			exit(1);
		}			
	}
	else{
		rewind(sto);
		new = sto;
	}
	
	//Imprime os tipos e usa fwrite para a matriz
	fprintf(new, "%s\n", img -> type);
	fprintf(new, "%d %d\n", img -> width, img -> height );
	fprintf(new, "%d",img -> scale);
	if (strcmp(img -> type,"P6") == 0)
	{
		//Escrve a imagem tipo P6, percorrendo todas as linhas e usando fwrite
		for (int i = 0; i < img -> height ; i++)
			fwrite(img -> pixels[i], 1, 3*img -> width, new);
	}
	else
	{
		//Escrve a imagem tipo P3, percorrendo toda a matriz e acrescentando \n
		fwrite("\n", sizeof(char), 1, new);
		for (int i = 0; i < img -> height; i++)
			for (int k = 0; k < 3 * img -> width;k++)
			{
				fprintf(new, "%d ",img -> pixels[i][k] );
			}

	}
	

	fclose(new);
}
//Função utilizada para desalocar as estruturas usadas
void desalocador(imagem **pastilhas, imagem *result, imagem *src, int n_pastilhas)
{
	//Desalocador dos objetos criados
	for (int i = 0; i < n_pastilhas; i++)
		free(pastilhas[i]);
	free(pastilhas);
	free(result);
	//free(src);
}
