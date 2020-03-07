#pragma once
#include "pch.h"
#include "CProblem.h"
#include "CMscnProblem.h"

class COptimizer
{
public:
	virtual void setProblem(CProblem *problem) { this->problem = problem; }
	virtual vector<double> getBestSolution(int duration, string bestSolutionPath) = 0;

protected:
	CProblem * problem = NULL;
};