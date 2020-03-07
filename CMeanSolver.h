#pragma once
#include "pch.h"
#include "CProblem.h"
#include "COptimizer.h"

class CMeanSolver : public COptimizer
{
public:

	CMeanSolver(CProblem *p) { setProblem(p); }

	vector <double> getBestSolution(int duration, string bestSolutionPath)
	{
		int solutionSize = (*problem).getSize();
		vector<double> solution;
		solution.resize(solutionSize);


		for (int i = 0; i < solutionSize; i++)
		{
			solution[i] = ((*problem).getMinMaxValues()[i].first + (*problem).getMinMaxValues()[i].second) /2;
		}

		(*problem).saveVectorSolutionToFile(solution, bestSolutionPath);

		return solution;
	}
};