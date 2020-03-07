#pragma once
#include "pch.h"
class CProblem
{
public:
	virtual double dGetQuality(vector<double> & pdSolution) = 0;
	virtual bool bConstraintsSatisfied(vector<double> & pdSolution) = 0;
	virtual int getSize() = 0;
	virtual vector<pair<double,double>> getMinMaxValues() = 0;
	virtual void saveVectorSolutionToFile(vector<double> & pdSolution, string path) = 0;
};