#pragma once
#include "pch.h"
#include "Individual.h"
#include "CMscnProblem.h"
#include "CRandomSearch.h"

class CDiffEvol : public COptimizer 
{
public:

	CDiffEvol(CProblem *p) { setProblem(p); }

	vector<double> getBestSolution(int duration, string bestSolutionPath)
	{
		int numberOfIndividuals = NUMBER_OF_INDIVIDUALS;
		double crossp = CROSSP;
		double mut = MUTATION_FACTOR;

		double wyniczek = 0;
		int err = 0;

		//utowrzenie populacji
		vector<vector<double>> population(numberOfIndividuals);

		CRandomSearch randomSearch(problem);

		//dodanie wylosowanie osobnikow w populacji
		for (int i = 0; i < numberOfIndividuals; i++)
		{
			population[i] = randomSearch.getNextValid();
			cout << "... ";
		}
		cout << endl;
		
		//ilosc genow
		int genesAmount = (*problem).getSize();

		int baseInd, addInd1, addInd2;
		cout << "poczatkowa populacja ustalona! " << endl;

		vector<double> newInd(genesAmount);//osobnik, ktory bedzie nowy

		CTimer timer;
		timer.StartCounter();

		do {
			for (int actualInd = 0; actualInd < numberOfIndividuals; actualInd++)//iteruje po osobnikach populacji
			{

				baseInd = random_generator.getInt(0, numberOfIndividuals - 1);
				addInd1 = random_generator.getInt(0, numberOfIndividuals - 1);
				addInd2 = random_generator.getInt(0, numberOfIndividuals - 1);

				if (individualsAreDifferent(actualInd, baseInd, addInd1, addInd2)) //jezeli wszystkie wysolowane osobniki sa rozne
				{
					for (int gene = 0; gene < genesAmount; gene++)
					{

						if (random_generator.getDouble(0, 1) > crossp) //prawdopodobienstwo mutacji
						{//zamieniam dany gen na zmutowany
							newInd[gene] = population[baseInd][gene] + mut * (population[addInd1][gene] - population[addInd2][gene]);

							//jezeli wartosc jest niedopuszczalna to ja zmieniam
							double first = 0; //wartosc min
							double second = 0; //wartosc max

							
							first = (*problem).getMinMaxValues()[gene].first;
							second = (*problem).getMinMaxValues()[gene].second;
							//cout << second << " ";
							//Sleep(500);
								
							geneMinMaxRepair(newInd, gene, first, second);
						}
						else
						{//nie zamieniam genu
							newInd[gene] = population[actualInd][gene];
						}
					}
					//teraz sprawdzam czy nowy osobnik przyniesie wiekszy zysk niz stary
					
					if ((*problem).dGetQuality(population[actualInd]) < (*problem).dGetQuality(newInd) && (*problem).bConstraintsSatisfied(newInd))
					{
						population[actualInd] = newInd;		
					}
					
				}
				//cout << fixed << (*problem).dGetQuality(population[0])<<" " << endl;
			}
		
			
		} while ((timer.GetCounter() < (double)duration));
		
		double bestDeal = INT16_MIN;
		int bestIndex = INT16_MIN;
		for (int i = 0; i < numberOfIndividuals; i++)
		{
			if ((*problem).dGetQuality(population[i]) > bestDeal)
			{
				bestDeal = (*problem).dGetQuality(population[i]);
				bestIndex = i;
			}
		}
		//wypisanie najlepszego zysku
		cout << "Najlepszy znaleziony zysk: ";
		cout << fixed << bestDeal << endl;

		//zapisanie najlepszego rozwiazania do pliku
		(*problem).saveVectorSolutionToFile(population[bestIndex], bestSolutionPath);
		
		return population[bestIndex];

	}

private:
	
	CRandom random_generator;

	bool individualsAreDifferent(int a, int b, int c, int d)
	{
		if (a != b && b != c && a!=c && a!=d && b!=d && c !=d)
		{
			return true;
		}
		return false;
	}

	void geneMinMaxRepair(vector<double> & individual, int gene, int min, int max)
	{
		if (individual[gene] < min)
		{
			individual[gene] = min;
		}
		if (individual[gene] > max)
		{
			individual[gene] = max;
		}
	}

};
