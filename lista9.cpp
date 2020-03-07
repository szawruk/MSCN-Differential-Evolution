// lista9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CMscnProblem.h"
#include "CMscnSolution.h"
#include "CRandomSearch.h"
#include "Individual.h"
#include "CDiffEvol.h"
#include "CProblem.h"
#include "COptimizer.h"
#include "CTimer.h"
#include "CMeanSolver.h"

void test(string testPath, string solutionPath)
{//this function uses double array solution
	cout << "Problem file: " + testPath + ", File with solution: " + solutionPath << endl;

	CMscnProblem problem;

	double * solution;
	int errorCode = 0;
	bool constraintsSatisfied;

	problem.deserialize(testPath);
	solution = problem.makeDoubleArraySolutionFromFile(solutionPath);
	testPath.size();
	problem.serialize(testPath.substr(0,testPath.size()-4) + "_save.txt");

	cout << "Profit: " << problem.dGetQuality(solution, problem.getSizeSolutionFromFile(solutionPath), errorCode) << ", Error code: " << errorCode <<endl;

	constraintsSatisfied = problem.bConstraintsSatisfied(solution, problem.getSizeSolutionFromFile(solutionPath), errorCode);

	cout << "Solution correct: "; constraintsSatisfied ? cout<< "true" : cout<< "false"; cout << ", Error code: " << errorCode << endl;
}



void testSolutionObject(string testPath, string solutionPath)
{//this function uses object solution
	cout << "Problem file: " + testPath + ", File with solution: " + solutionPath << endl;

	CMscnProblem problem;

	int errorCode = 0;
	bool constraintsSatisfied;

	problem.deserialize(testPath);

	CMscnSolution solutionObject;
	solutionObject.readSolutionFromFile(solutionPath);

	problem.serialize(testPath.substr(0, testPath.size() - 4) + "_save.txt");

	cout << "Profit: " << problem.dGetQuality(solutionObject.convertSolutionObjectIntoDoubleArray(), solutionObject.getSize(), errorCode) << ", Error code: " << errorCode << endl;

	constraintsSatisfied = problem.bConstraintsSatisfied(solutionObject.convertSolutionObjectIntoDoubleArray(), solutionObject.getSize(), errorCode);

	cout << "Solution correct: "; constraintsSatisfied ? cout << "true" : cout << "false"; cout << ", Error code: " << errorCode << endl;

}

void randomProblemGenerateTest(string randomProblemPath, int D, int F, int M, int S)
{//this function generates randomly problem instance and saves to file
	cout << "Problem file generated automatically and saved to: " + randomProblemPath << endl;

	CMscnProblem problem;
	problem.vGenerateInstance(D,F,M,S);
	problem.serialize(randomProblemPath);

}

void RandomSearchTest(string randomProblemPath, string randomSolutionPath, int duration)
{//this function search best solution randomly and saves best found solution to file
	cout << "Drawing best solution: "<< endl;

	CMscnProblem problem;
	problem.deserialize(randomProblemPath);

	CRandomSearch randomSearch(&problem);


	randomSearch.getBestSolution(duration, randomSolutionPath);
}



void DifferentialEvolutionTest(string randomProblemPath, string evolutionSolutionPath, int duration)
{//this function uses differential equation and saves best found solution to file
	cout << "Differential evolution: " << endl;


	CMscnProblem  problem;
	problem.deserialize(randomProblemPath);


	CDiffEvol evolution(&problem);

	evolution.getBestSolution(duration, evolutionSolutionPath);


}


int main()
{
	//test("test1.txt","solution1.txt"); //Example of using the functions above

}

