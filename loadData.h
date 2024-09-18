/*
 * loadData.h
 *
 *  Created on: Dec 28, 2015
 *      Author: sandro
 */

#ifndef LOADDATA_H_
#define LOADDATA_H_

#include <iostream>

using namespace std;

void loadDataTest(Individual *pop,int popsize,int number_genes)// carrega o arquivo txt de combinações
{

	vector <float> acuracia;
	vector <float> desvio;
	vector <float> combinado;

	FILE *basetxt;
	float blend, precision, pattern;

	if((basetxt = fopen("wlout.txt","rt"))==NULL)
	{
		printf("The file can not be opened.\n");
		exit(1);
	}

	do
	{
		 fscanf(basetxt, "%f %f %f", &blend, &precision, &pattern);
		 combinado.push_back(blend);
		 acuracia.push_back(precision);
		 desvio.push_back(pattern);

	}while(!feof(basetxt)); //Verificar se pode haver problema
	fclose(basetxt);

	for(int index=0;index<popsize;index++)
	{
		float n_classifiers=0.0,means_acc=0.0,ic_acc=0.0;

		vector <int> comb;

		for(int i=0; i<number_genes; i++)
		{
			if(pop[index].chromosome[i]==1)
			{
				comb.push_back(i+1);
				n_classifiers++;
			}
		}

		int ver = 0;
		int aux;
		char cara[7]="";
		for(int i=0; i<number_genes; i++)
		{
			if(pop[index].chromosome[i]==1)
			{
				aux = i+1;

				if(aux==1)
					strcat(cara,"1");
				if(aux==2)
					strcat(cara,"2");
				if(aux==3)
					strcat(cara,"3");
				if(aux==4)
					strcat(cara,"4");
				if(aux==5)
					strcat(cara,"5");
				if(aux==6)
					strcat(cara,"6");
			}
			else
			{
				ver++;
			}
		}

		int x;
		if (ver==number_genes)
		{
			srand( (unsigned)time(NULL) );
			x = 1 + ( rand() % 6 );
			if(x==1)
				strcat(cara,"1");
			if(x==2)
				strcat(cara,"2");
			if(x==3)
				strcat(cara,"3");
			if(x==4)
				strcat(cara,"4");
			if(x==5)
				strcat(cara,"5");
			if(x==6)
				strcat(cara,"6");

			n_classifiers = 1;
		}

		float combinando;
		istringstream inacc(cara);
		inacc >> combinando;

		for(vector<float> ::size_type i=0; i< combinado.size(); i++)
		{
			if(combinando == combinado[i])
			{
				means_acc = acuracia[i];
				ic_acc = desvio[i];
			}
		}

		setAccConf(pop,index, means_acc, ic_acc,n_classifiers);
	}

}

void loadDataReal(Individual *pop,int popsize,int number_genes)// carrega o arquivo txt de combinações
{

	for(int index=0;index<popsize;index++)
	{
		float n_classifiers=0.0,means_acc=0.0,ic_acc=0.0;
		vector <int> comb;

		for(int i=0; i<number_genes; i++)
		{
			if(pop[index].chromosome[i]==1)
			{
				comb.push_back(i+1);
				n_classifiers++;
			}
		}

		ofstream blend_outfile;
		blend_outfile.open("combinations.txt");
		if (blend_outfile.is_open()) {
			for (vector<int>::size_type i = 0; i < comb.size(); i++){
				blend_outfile << comb[i] << " ";
			}
			blend_outfile << endl;

			blend_outfile.close();
		} else {
			printf("\n\nERRO ao abrir arquivo: combinations.txt\n");
		}

		/////////////////////////////////

		system("./run_perform_wlclp.sh");

		/////////////////////////////////

		string inputacc;
		ifstream myfileacc ("validation_accuracy.txt"); // ifstream = padrão ios:in
		if (myfileacc.is_open())
		{
			while (! myfileacc.eof() ) //enquanto end of file for false continua
			{
				getline (myfileacc,inputacc); // como foi aberto em modo texto(padrão)
			}
			myfileacc.close();
		}
		else cout << "Unable to open file";

		string inputic;
		ifstream myfile ("validation_ic.txt"); // ifstream = padrão ios:in
		if (myfile.is_open())
		{
			while (! myfile.eof() ) //enquanto end of file for false continua
			{
				getline (myfile,inputic); // como foi aberto em modo texto(padrão)
			}
			myfile.close();
		}
		else cout << "Unable to open file";

		istringstream inacc(inputacc);
		inacc >> means_acc;
		istringstream inic(inputic);
		inic >> ic_acc;

		setAccConf(pop,index, means_acc, ic_acc,n_classifiers);
	}

}



#endif /* LOADDATA_H_ */
