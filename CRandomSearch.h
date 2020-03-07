#pragma once
#include "pch.h"
#include "CMscnProblem.h"
#include "CMscnSolution.h"
#include "COptimizer.h"
#include "CProblem.h"
#include "CTimer.h"

class CRandomSearch : COptimizer
{
	
public:

	CRandomSearch(CProblem *p) { setProblem(p); }


	vector<double> getNext() 
	{
		

		CRandom random_generator;
		int solutionSize = (*problem).getSize();
		vector<double> solution;
		solution.resize(solutionSize);

		
		for (int i = 0; i < solutionSize; i++)
		{
			solution[i] = random_generator.getDouble((*problem).getMinMaxValues()[i].first, (*problem).getMinMaxValues()[i].second);
		}
	
		return solution;
	}

	vector<double> getNextValid()
	{
		int solutionSize = (*problem).getSize();
		vector<double> candidate;
		
		bool s;
		do 
		{
			candidate = getNext();
			s = (*problem).bConstraintsSatisfied(candidate);
		}
		while (!s);
		return candidate;
	}


	vector<double> getBestSolution(int duration, string bestSolutionPath)
	{
		int solutionSize = (*problem).getSize();
		vector<double> solution;
		solution.resize(solutionSize);

		vector<double> best = getNextValid();
		int err;

		CTimer timer;
		timer.StartCounter();

		do {
			vector<double> candidate = getNextValid();
			
			if ((*problem).dGetQuality(candidate) > (*problem).dGetQuality(best))
			{
				best = candidate;
			}

		
		} while (timer.GetCounter() < (double)duration);

		cout << "Najlepszy znaleziony zysk: " << (*problem).dGetQuality(best)<<endl;
		(*problem).saveVectorSolutionToFile(best, bestSolutionPath);
		return best;
	}


};