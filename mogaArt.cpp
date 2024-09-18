//============================================================================
// Name        : mogaArt.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//C++ default library
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
//

//User library
#include "assist.h"
#include "loadData.h"
//

#define NONE  -1
#define FALSE  0
#define TRUE   1

using namespace std;


int dominates(Individual *pop,int i1,int i2)
{
	/*
	 * Verifica se o indivíduo i1 domina, é dominado ou não domina e nem é dominado por i2
	 * */

	float size_ind1 = getClassifiers(pop,i1);
	float size_ind2 = getClassifiers(pop,i2);
	float accuracy_ind1 = getAccuracy(pop,i1);
	float accuracy_ind2 = getAccuracy(pop,i2);

	int indA_domina = -1; //  = -1, se ind1 nao domina e não é dominado por ind2;
					      //  =  0, se ind1 é dominado por ind2;
						  //  =  1, se ind1 domina ind2

	if( ( (size_ind1 <= size_ind2)&&(accuracy_ind1 >  accuracy_ind2) ) ||
		( (size_ind1 <  size_ind2)&&(accuracy_ind1 >= accuracy_ind2) )	)
	{
		indA_domina = 1; // domina
	}
	else if( ( (size_ind1 >= size_ind2)&&(accuracy_ind1 <  accuracy_ind2) ) ||
			 ( (size_ind1 >  size_ind2)&&(accuracy_ind1 <= accuracy_ind2) )  )
	{
		indA_domina = 0; // é dominado
	}

	return indA_domina;
}


void markDominance(Individual *pop,int popsize)
{
	/*
	 * Distribui os individuos em seus níveis de dominância
	 * sendo que no nível 1 estao os indivíduos que dominam e/ou não são dominados
	 * */
	int counter=0;
	int level = 1;
	int np,k;

	while(counter<popsize)// para garantir que todos os indivíduos sejam analisados
	{
		for(int i=0;i<popsize;i++)//para indivíduo 1: este indivíduo não pode ter sido atribuído a nenhum nível de dominância
		{
			np=0;// para contar quantos indíviduos j dominam o indivíduo 1
			if((GetLevel(pop,i) == 0))//verifica se não foi atribuído um nível ao indivíduo 1,
			{                         //se o indivíduo já estiver em um nível ele não pode ser analisado como indivíduo 1, que domina
				for(int j=0;j<popsize;j++)//para indivíduo 2
				{
					k=0;// para contar quantos indivíduos j o indivíduo 1 domina
					if((GetLevel(pop,j) == 0)||(GetLevel(pop,j) == level))//verifica se não foi atribuído um nível ao indivíduo j
					{                                                     //ou se o índivíduo j pertence ao nível atual

						if(dominates(pop,i,j)==TRUE)//se indíviduo 1 domina j
						{
							pop[i].dominated[k]=j;
							k++;
						}
						else if(dominates(pop,i,j)==FALSE)//se individuo 1 é dominado por j
						{
							np=np+1;
						}

					}
				}//for j

				if(np == 0)//se individuo 1 não é dominado por nenhum j, então individuo 1 entra no nível atual
				{
					SetLevel(pop,i,level);
					counter++;
				}
			}
		}//for i
		level++;
	}//while

}


void collectBest(Individual* joinedPop,Individual* pop,int popsize,int number_genes)
{
	int i=0;
	int level=1;
	vector <int> index;
	while(i<popsize)
	{
		for(int j=0;j<(2*popsize);j++)
		{
			if(GetLevel(joinedPop,j)==level)
			{
				index.push_back(j);
				i++;
			}
		}
		level=level+1;
	}
	for(int j=0;j<popsize;j++)
	{
		int* ind = (int*) malloc(number_genes*sizeof(int));
		GetInd(ind,joinedPop,index[j],number_genes);
		SetInd(pop,j,ind,number_genes);
		setAccConf(pop,j,getAccuracy(joinedPop,index[j]),getConfidence(joinedPop,index[j]),getClassifiers(joinedPop,index[j]));
		SetLevel(pop,j,GetLevel(joinedPop,index[j]));
		free(ind);
	}
}


void uniformCrossover(int *child2, int *child1,int number_genes)
{

	int *mask = (int*) malloc(number_genes*sizeof(int));
	int i;
	for(i=0;i<number_genes;i++)
	{
		if(rand() % 100 < 20)
			mask[i]=1;
		else
			mask[i]=0;
	}

	for(i=0;i<number_genes;i++)
	{
		if(mask[i]==1)
		{
			int aux = child1[i];
			child1[i]=child2[i];
			child2[i]=aux;
		}

	}
	free(mask);
}


void mutation(int *ind,int number_genes,int mutation_rate_x,int mutation_rate_y)
{

	int mutation_probability;

	for(int i=0;i<number_genes;i++)
	{
		mutation_probability = rand() % mutation_rate_x;
		if(mutation_probability <= mutation_rate_y)
		{
			if(ind[i] == 1)
				ind[i]=0;
			else
				ind[i]=1;
		}
	}

}


void binaryTournamentSelection(Individual* joinedPop,Individual* pop,int popsize,int number_genes)
{
	srand (time (NULL));
	int i=0;
	Individual* newPop = (Individual*) malloc(popsize*sizeof(Individual));
	CreatePopNull(newPop,popsize,number_genes);

	while(i < popsize-1)
	{
		//int* child = (int*) malloc(number_genes*sizeof(int));//para uniformCrossover
		int* child1 = (int*) malloc(number_genes*sizeof(int));
		int* child2 = (int*) malloc(number_genes*sizeof(int));

		int levelA, levelB, levelC, levelD;
		int pai1,pai2,mae1,mae2;

		//CreateInd(child,number_genes);

		pai1 = rand () % popsize;
		levelA = GetLevel(pop, pai1);

		pai2 = rand () % popsize;
		levelB = GetLevel(pop, pai2);

		if (levelA <= levelB)
			GetInd(child1,pop,pai1,number_genes);
		else
			GetInd(child1,pop,pai2,number_genes);

		mae1 = rand () % popsize;
		levelC = GetLevel(pop, mae1);

		mae2 = rand () % popsize;
		levelD = GetLevel(pop, mae2);

		if (levelC <= levelD)
			GetInd(child2,pop,mae1,number_genes);
		else
			GetInd(child2,pop,mae2,number_genes);

		uniformCrossover(child2,child1,number_genes);

		mutation(child2,number_genes,100,9);//mutation_rate_x=100,mutation_rate_y=9
		mutation(child1,number_genes,100,9);

		SetInd(newPop,i,child1,number_genes);
		//setAccConf(newPop,i,0,0,0);
		//SetLevel(newPop,i,0);
		SetInd(newPop,i+1,child2,number_genes);
		//setAccConf(newPop,i+1,0,0,0);
		//SetLevel(newPop,i+1,0);
		i=i+2;
		//free(child);
		free(child1);
		free(child2);
	}
	//juntando a populacão-pai com a população-filho


	int a = 0;
	for(int j=0;j<(2*popsize);j++)
	{
		int* ind = (int*) malloc(number_genes*sizeof(int));
		if(j>=popsize)
		{
			GetInd(ind,newPop,a,number_genes);
			SetInd(joinedPop,j,ind,number_genes);
			setAccConf(joinedPop,j,0,0,0);
			SetLevel(joinedPop,j,0);
			a++;
		}
		else
		{
			GetInd(ind,pop,j,number_genes);
			SetInd(joinedPop,j,ind,number_genes);
			setAccConf(joinedPop,j,0,0,0);
			SetLevel(joinedPop,j,0);
		}
		free(ind);
	}

/*	for(i=0;i<popsize;i++)
	{
		SetInd(pop,i,GetInd(newPop,i,number_genes),number_genes);
		setAccConf(pop,i,0,0,0);
		SetLevel(pop,i,0);
	}*/
	free(newPop);
}


int main(int argc, char** argv)
{

	int popsize = 6;
	int number_genes = 6;
	int maxGenerations = 5;

	Individual *pop = (Individual*) malloc(popsize*sizeof(Individual));
	CreatePop(pop,popsize,number_genes);

	Individual* joinedPop = (Individual*) malloc(2*popsize*sizeof(Individual));
	CreatePopNull(joinedPop,2*popsize,number_genes);

	/* main loop */
	printf("Generation %d\n",1);
	loadDataTest(pop,popsize,number_genes);
	markDominance(pop,popsize);
	print(pop,popsize,number_genes);
	binaryTournamentSelection(joinedPop,pop,popsize,number_genes);
	printf("\n\n");
	//print(joinedPop,(2*popsize),number_genes);
	//test(joinedPop,(2*popsize),number_genes);
	//printf("\n\n");
	//print(joinedPop,(2*popsize),number_genes);
	//loadDataTest(joinedPop,(2*popsize),number_genes);
	//markDominance(joinedPop,(2*popsize));
	//collectBest(joinedPop,pop,popsize,number_genes);

	//printf("\n\n");
	//print(joinedPop,(2*popsize),number_genes);
	//printf("\n\n");
	//print(pop,popsize,number_genes);
	for(int i=1;i<maxGenerations;i++)
	{
		printf("Generation %d\n",i+1);
		loadDataTest(joinedPop,(2*popsize),number_genes);
		markDominance(joinedPop,(2*popsize));
		collectBest(joinedPop,pop,popsize,number_genes);
		print(pop,popsize,number_genes);
		binaryTournamentSelection(joinedPop,pop,popsize,number_genes);
		printf("\n\n");
	}

	//textGenerate(pop,popsize,number_genes);
	free(pop);
	free(joinedPop);
	return 0;
}
