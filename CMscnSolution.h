#pragma once
#include "pch.h"
#include "Matrix.h"

class CMscnSolution
{
public:
	Matrix<double>& getXd()
	{
		return xd;
	}
	Matrix<double>& getXf()
	{
		return xf;
	}
	Matrix<double>& getXm()
	{
		return xm;
	}

	void readSolutionFromFile(string path)
	{
		char buffer[200];
		int value = 0;
		int d = 0;
		int f = 0;
		int m = 0;
		int s = 0;
		double dValue = 0;

		FILE * file;
		file = fopen(path.c_str(), "r+");

		fscanf(file, "%s", &buffer); //D
		fscanf(file, "%d", &value);
		d = value;

		fscanf(file, "%s", &buffer); //F
		fscanf(file, "%d", &value);
		f = value;

		fscanf(file, "%s", &buffer); //M
		fscanf(file, "%d", &value);
		m = value;

		fscanf(file, "%s", &buffer); //S
		fscanf(file, "%d", &value);
		s = value;

		size = d * f + f * m + m * s;

		xd.adjustMatrix(d, f);
		xf.adjustMatrix(f, m);
		xm.adjustMatrix(m, s);

		fscanf(file, "%s", &buffer); //xd

		for (int i = 0; i < d; i++)
		{
			for (int j = 0; j < f; j++)
			{
				fscanf(file, "%lf", &dValue);
				xd.set(dValue, i, j);
			}
		}

		fscanf(file, "%s", &buffer); //xf

		for (int i = 0; i < f; i++)
		{
			for (int j = 0; j < m; j++)
			{
				fscanf(file, "%lf", &dValue);
				xf.set(dValue, i, j);
			}
		}

		fscanf(file, "%s", &buffer); //xm

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < s; j++)
			{
				fscanf(file, "%lf", &dValue);
				xm.set(dValue, i, j);

			}
		}
	}

	double * convertSolutionObjectIntoDoubleArray()
	{

		double * pdSolution = new double[size];

		int dAmount = xd.getHeight();
		int fAmount = xd.getWidth();
		int mAmount = xm.getHeight();
		int sAmount = xm.getWidth();

		for (int i = 0; i < dAmount; i++)
		{
			for (int j = 0; j < fAmount; j++)
			{
				pdSolution[i*fAmount + j] = xd.get(i, j);
			}
		}

		for (int i = 0; i < fAmount; i++)
		{
			for (int j = 0; j < mAmount; j++)
			{
				pdSolution[xd.getSize() + i*mAmount + j] = xf.get(i, j);
			}
		}

		for (int i = 0; i < mAmount; i++)
		{
			for (int j = 0; j < sAmount; j++)
			{
				pdSolution[xd.getSize() + xf.getSize() + i * sAmount + j] = xm.get(i, j);
			}
		}

		return pdSolution;
	}

	void saveSolutionToFile(string path) {

		FILE * f;
		f = fopen(path.c_str(), "w+");

		int dAmount = xd.getHeight();
		int fAmount = xd.getWidth();
		int mAmount = xm.getHeight();
		int sAmount = xm.getWidth();

		fprintf(f, "D ");
		fprintf(f, to_string(dAmount).c_str()); fprintf(f, "\n");

		fprintf(f, "F ");
		fprintf(f, to_string(fAmount).c_str()); fprintf(f, "\n");

		fprintf(f, "M ");
		fprintf(f, to_string(mAmount).c_str()); fprintf(f, "\n");

		fprintf(f, "S ");
		fprintf(f, to_string(sAmount).c_str()); fprintf(f, "\n");


		//xd
		fprintf(f, "xd"); fprintf(f, "\n");//print the header
		string value;

		for (int i = 0; i < dAmount; i++)
		{
			for (int j = 0; j < fAmount; j++)
			{
				value = to_string(xd.get(i, j));
				fprintf(f, value.c_str());
				fprintf(f, " ");
			}
			fprintf(f, "\n");
		}

		//xf
		fprintf(f, "xf"); fprintf(f, "\n");//print the header


		for (int i = 0; i < fAmount; i++)
		{
			for (int j = 0; j < mAmount; j++)
			{
				value = to_string(xf.get(i, j));
				fprintf(f, value.c_str());
				fprintf(f, " ");
			}
			fprintf(f, "\n");
		}

		//xm
		fprintf(f, "xm"); fprintf(f, "\n");//print the header
		
		for (int i = 0; i < mAmount; i++)
		{
			for (int j = 0; j < sAmount; j++)
			{
				value = to_string(xm.get(i, j));
				fprintf(f, value.c_str());
				fprintf(f, " ");
			}
			fprintf(f, "\n");
		}

		fclose(f);
	}

	int getSize()
	{
		return size;
	}

private:
	int size = 0;
	Matrix<double> xd;
	Matrix<double> xf;
	Matrix<double> xm;
};
