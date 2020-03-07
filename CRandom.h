#pragma once
#include "pch.h"
#include <random>
class CRandom {
public:
	int getInt(int start, int end)
	{
		mt19937 gen(rd());
		uniform_int_distribution<int> distribution(start, end);

		return distribution(gen);
	}
	
	double getDouble(double start, double end)
	{
		mt19937 gen(rd());
		uniform_real_distribution<double> distribution(start, end);

		return distribution(gen);
	}

private:
	random_device rd;

};