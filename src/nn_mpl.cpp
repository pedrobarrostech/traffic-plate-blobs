/*#include <iostream>
#include <math.h>
#include <string>
#include "cv.h"
#include "ml.h"
#include "highgui.h"
#include <sstream>

using namespace std;


int max_amostras = 2;
int max_entradas = 2;

void executarRNA(int num_quadros, int cod, int num)
{
	CvANN_MLP mlp; //RNA
	char arquivo1[100],arquivo2[100],arquivo3[100], buffer[10];
	char buffer2[10];

	FILE *f, *fp;
	int n_amostras = 0; //quantidade de entradas para treinamento
	int i, j; //controlar linhas e colunas dos arquivos
	int col_entrada = max_amostras; //quantidade de colunas com dados de entrada

	int num_entrada,repete; //quantidade de entradas
	if(num_quadros>max_entradas) num_entrada = max_entradas;
	else num_entrada = num_quadros;

	int num_saida = num_entrada; //quantidade de saidas
	int col_saida=1; //quantidade de colunas com dados de saida
	double in[num_entrada][col_entrada], out[num_saida],resultado[num_saida];

	printf(" Executando RNA...\n");

	//Criando as matrizes
	//Dados de entrada. Matriz de ordem (num_entrada x col_entrada)
	CvMat *dados = cvCreateMat(num_entrada, col_entrada, CV_64FC1);
	//Dados de saida. Matriz de ordem (num_saida x col_saida)
	CvMat *respostas = cvCreateMat(num_saida, col_saida, CV_64FC1);
	//Armazenara a Saida produzida pela rede
	CvMat *saida_rede = cvCreateMat(num_saida, col_saida, CV_64FC1);
	//Matriz de representacao da RNA com 4 camadas
	CvMat *camadas_rede = cvCreateMat(4, 1, CV_32SC1);
	//Definindo o numero de neuronios em cada camada da RNA
	/* Camada 1: 1200 neuronios (entradas)
 	 Camada 2: 20 neuronios (camada oculta)
 	 Camada 3: 20 neuronios (camada oculta)
 	 Camada 4: 1 neuronios (1 saida)

	int camadas []= { col_entrada, 20, 20, col_saida } ;

	if(cod==1)
	{
		strcpy(arquivo2,"treinamento/CirculoNet");

		strcpy(arquivo3,"resultados/CirculoNet");
	}
	else if(cod==2)
	{
		strcpy(arquivo2,"treinamento/RetanguloNet");
		strcpy(arquivo3,"resultados/RetanguloNet");
	}
	else
	{
		strcpy(arquivo2,"treinamento/TrianguloNet");
		strcpy(arquivo3,"resultados/TrianguloNet");
	}

	itoa(num,buffer2,10);
	strcat(arquivo3,buffer2);
	strcat(arquivo2,buffer2);
	strcat(arquivo2,".rna");
	strcat(arquivo3,".txt");

	for(i=0;i<num_entrada;i++)
	{
		strcpy(arquivo1,"arquivos/");
		itoa(i,buffer,10); // convertendo inteiro em string
		strcat(arquivo1,buffer);
		strcat(arquivo1,".txt");

		//Lendo um arquivo com dados para treinamento
		if((f=fopen(arquivo1,"r"))==NULL)
		{
			fprintf(stderr," ERRO: arquivo %s nao foi aberto!!!\n",arquivo1);
			return;
		}

		//Obtendo numero de amostras
		fscanf(f, "%d %lf\n", &n_amostras, &out[i]);
		col_entrada = n_amostras;

		//Lendo e Reunindo os dados de treinamento
		printf(" Lendo dados de quadro %d...\n",i);

		for (j=0; j<col_entrada; j++)
			fscanf(f,"%lf\n",&in[i][j]);

		fclose(f); //fechando arquivo
	}

	//inicializando matrizes
	cvInitMatHeader(dados, num_entrada, col_entrada, CV_64FC1, in);

	cvInitMatHeader(respostas, num_saida, col_saida, CV_64FC1, out);
	cvInitMatHeader(camadas_rede, 1, 4, CV_32SC1, camadas);

	//Verifica se o arquivo de treinamento da rede jah existe
	if((fp=fopen(arquivo2,"r"))==NULL)
		mlp.create(camadas_rede); //Criando a RNA
	else
	{
		mlp.load(arquivo2);
		fclose(fp); //fechando arquivo
	}

	do
	{
		printf(" Ciclo %d...\n",i++);
		mlp.train(
				dados, //vetor de entradas
				respostas, //vetor de saidas
				saida_rede, //vetor de pesos
				0, //vetor opicional mostrando colunas de entrada e de saida
				CvANN_MLP_TrainParams( cvTermCriteria( //criterio de finalizacao
						CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, /*CV_TERMCRIT_ITER
						10000, //numero maximo de iteracoes
						0.000001 // precisao necessaria
				),
						CvANN_MLP_TrainParams::BACKPROP,
						/*CvANN_MLP_TrainParams::RPROP
						0.01,
						0.01
				)
		);

		// Salvar classificador para o arquivo, se necessário
		mlp.save( arquivo2 );

		// Testando a rede treinada
		mlp.predict(dados,saida_rede);

		for(j=0, repete=0;j<num_entrada;j++)
		{
			resultado[j] = CV_MAT_ELEM(*saida_rede,double,j,0);
			if(abs(resultado[j]-out[j])>1)
			{
				repete = 1;
				break;
			}
		}
	} while(repete);

	//Lendo um arquivo com dados para treinamento
	if((fp=fopen(arquivo3,"w"))==NULL)
	{
		fprintf(stderr," ERRO: arquivo %s nao foi aberto!!!\n",arquivo3);
		return;
	}
	for(i=0;i<num_entrada;i++)
		fprintf(fp, "%.5lf\n", resultado[i]);

	fclose(fp); //fechando arquivo

	// desalocando recursos
	cvReleaseMat(&dados);
	cvReleaseMat(&respostas);
	cvReleaseMat(&saida_rede);
	cvReleaseMat(&camadas_rede);
	printf(" Fim de execucao da RNA...\n");
}

double loadRNA(char *file_treino)
{
	CvANN_MLP mlp; //RNA
	FILE *f, *fp;
	int n_amostras = 0; //quantidade de amostras para treinamento
	int i, j; //controlar linhas e colunas dos arquivos

	int col_entrada; //quantidade de colunas com dados de entrada
	int num_entrada=1; //quantidade de entradas
	int num_saida=1; //quantidade de saidas
	int col_saida=1; //quantidade de colunas com dados de saida
	double in[max_amostras],resultado;

	printf(" Executando RNA...\n");

	//Lendo um arquivo com dados para treinamento
	if((f=fopen("arquivos/EntradaRede.txt","r"))==NULL)
	{
		fprintf(stderr," ERRO: arquivo 'arquivos/EntradaRede.txt' "
				"nao foi aberto!!!\n");
		return 0;
	}

	//Obtendo numero de amostras
	fscanf(f, "%d\n", &n_amostras);
	col_entrada = n_amostras;

	//Criando as matrizes
	//Dados de entrada. Matriz de ordem (num_entrada x col_entrada)
	CvMat *dados = cvCreateMat(num_entrada, col_entrada, CV_64FC1);
	//Armazenara a Saida produzida pela rede
	CvMat *saida_rede = cvCreateMat(num_saida, col_saida, CV_64FC1);

	//Lendo e Reunindo os dados de treinamento
	for (i=0; i<col_entrada; i++)
		fscanf(f,"%lf\n",&in[i]);

	fclose(f); //fechando arquivo

	//Carregando arquivo de treinamento
	mlp.load(file_treino);

	//Executando a rede treinada
	mlp.predict(dados,saida_rede);

	resultado = CV_MAT_ELEM(*saida_rede,double,0,0);

	// desalocando recursos
	cvReleaseMat(&dados);
	cvReleaseMat(&saida_rede);

	printf(" Fim de execucao da RNA...\n");
	return resultado;
} */


