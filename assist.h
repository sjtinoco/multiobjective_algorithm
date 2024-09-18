/*
 * assist.h
 *
 *  Created on: Dec 28, 2015
 *      Author: sandro
 */

#ifndef ASSIST_H_
#define ASSIST_H_

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <math.h>
#include <time.h>
#include <iomanip>

using namespace std;

struct Individual{
	int i;
	//Individual *novo=(Individual*) malloc(sizeof(Individual));
	float accuracy;
	float confidence;
	float nclassifiers;
	int status;
	int level;
	int ngenes;
	int *dominated;
	int *chromosome;
};

void CreatePop(Individual *cromossomo,int popsize,int number_genes)
{
	//cromossomo=(Individual*) malloc(popsize*sizeof(Individual));
	srand(time(NULL));
	int h;
	for(h=0;h<popsize;h++)
	{
		int i;
		cromossomo[h].accuracy=0;
		cromossomo[h].confidence=0;
		cromossomo[h].nclassifiers=number_genes;
		cromossomo[h].status=0;
		cromossomo[h].level=0;
		cromossomo[h].dominated=(int*) malloc(2*number_genes*sizeof(int));
		cromossomo[h].chromosome=(int*) malloc(number_genes*sizeof(int));
		for(i=0;i<number_genes;i++)
		{
			if(rand() % 100 < 50)
				cromossomo[h].chromosome[i]=0;
			else
				cromossomo[h].chromosome[i]=1;
		}
		for(i=0;i<(2*number_genes);i++)
		{
			cromossomo[h].dominated[i]=99;
		}
	}
}

void CreatePopNull(Individual *cromossomo,int popsize,int number_genes)
{
	//cromossomo=(Individual*) malloc(popsize*sizeof(Individual));
	int h;
	for(h=0;h<popsize;h++)
	{
		int i;
		cromossomo[h].accuracy=0;
		cromossomo[h].confidence=0;
		cromossomo[h].nclassifiers=number_genes;
		cromossomo[h].status=0;
		cromossomo[h].level=0;
		cromossomo[h].dominated=(int*) malloc(2*number_genes*sizeof(int));
		cromossomo[h].chromosome=(int*) malloc(number_genes*sizeof(int));
		for(i=0;i<number_genes;i++)
		{
			cromossomo[h].chromosome[i]=0;
		}
		for(i=0;i<(2*number_genes);i++)
		{
			cromossomo[h].dominated[i]=99;
		}
	}
}

void print(Individual *pop,int size,int number_genes)
{
	int i,j;
	for(j=0;j<size;j++)
	{
		for(i=0;i<number_genes;i++)
		{
			printf("%d",pop[j].chromosome[i]);
		}
		printf("\n");
		printf("%.2f %.2f %.2f", pop[j].accuracy,pop[j].confidence,pop[j].nclassifiers);
		printf("\n");
		printf("level: %d", pop[j].level);
		printf("\n");

	}
}

void setAccConf(Individual *pop,int ident,float accuracy,float confidence,float qtde)
{
	pop[ident].accuracy=accuracy;
	pop[ident].confidence=confidence;
	pop[ident].nclassifiers=qtde;
}

void SetStatus(Individual* pop,int index,int status)
{
	pop[index].status = status;
}

int GetStatus(Individual* pop,int index)
{
	return pop[index].status;
}

void SetLevel(Individual* pop,int index,int level)
{
	pop[index].level = level;
}

int GetLevel(Individual* pop,int index)
{
	return pop[index].level;
}

float getAccuracy(Individual *pop,int ident)
{
	return pop[ident].accuracy;
}

float getConfidence(Individual *pop,int ident)
{
	return pop[ident].confidence;
}

float getClassifiers(Individual *pop,int ident)
{
	return pop[ident].nclassifiers;
}


int irandom(int n)//int  irandom(int  n):
{
	int lim = n-1;
	srand (time (NULL)); // Gera uma 'random seed' baseada no retorno da funcao time()
	int num;
	num = rand () % lim; // Retorna um numero aleatorio entre 0 e lim
	return num;
}

void SetInd(Individual* pop,int index,int* ind,int number_genes)
{
	int i;
	for(i=0;i<number_genes;i++)
	{
		pop[index].chromosome[i]=ind[i];
	}
}

int* CopyInd(int* ind1,int number_genes)
{
	int i;
	int* ind2 = (int*) malloc(number_genes*sizeof(int));
	for(i = 0; i < number_genes; i++)
	{
		ind2[i]=ind1[i];
	}
	return ind2;
}

void GetInd(int *ind,Individual* pop,int index,int number_genes)
{
	int i;
	for(i=0;i<number_genes;i++)
	{
		ind[i]=pop[index].chromosome[i];
	}
}

void CreateInd(int* ind,int number_genes)
{
	int i;
		for(i=0;i<number_genes;i++)
	{
		ind[i]=0;
	}
}

void textGenerate(Individual *pop,int popsize,int number_genes)
{

	ofstream acc_outfile, ic_outfile, nc_outfile, co_outfile;
	acc_outfile.open("accuracy.txt");
	if (acc_outfile.is_open()) {
		int a;
		for(a=0; a<popsize; a++)
		{
			if(GetLevel(pop,a)==1)
				acc_outfile << getAccuracy(pop,a) << " ";
		}
		acc_outfile << endl;

		acc_outfile.close();
	} else {
		printf("\n\nERRO ao abrir arquivo: accuracy.txt\n");
	}


	ic_outfile.open("int_confidence.txt");
	if (ic_outfile.is_open()) {
		int a;
		for(a=0; a<popsize; a++)
		{
			if(GetLevel(pop,a)==1)
				ic_outfile << getConfidence(pop,a) << " ";
		}
		ic_outfile << endl;

		ic_outfile.close();
	} else {
		printf("\n\nERRO ao abrir arquivo: int_confidence.txt\n");
	}

	nc_outfile.open("n_classifiers.txt");
	if (nc_outfile.is_open()) {
		int a;
		for(a=0; a<popsize; a++)
		{
			if(GetLevel(pop,a)==1)
				nc_outfile << getClassifiers(pop,a) << " ";
		}
		nc_outfile << endl;

		nc_outfile.close();
	} else {
		printf("\n\nERRO ao abrir arquivo: n_classifiers.txt\n");
	}

	co_outfile.open("cromossomes.txt");
	if (co_outfile.is_open()) {
		int a;
		for(a=0; a<popsize; a++)
		{
			if(GetLevel(pop,a)==1)
				{
				for(int i = 0; i < number_genes; i++)
				{
					co_outfile << pop[a].chromosome[i] << " ";
				}
				co_outfile << endl;
			}
		}
		co_outfile << endl;

		co_outfile.close();
	} else {
		printf("\n\nERRO ao abrir arquivo: cromossomes.txt\n");
	}
}

#endif /* ASSIST_H_ */
