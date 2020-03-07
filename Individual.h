#pragma once
#include "pch.h"
#include "CMscnProblem.h"


class Individual {
public:
	Individual()
	{
		
	}

	Individual(int size)
	{
		pdIndividual.resize(size);
	}


	void generateIndividual(CMscnProblem & mscnProblem)
	{
		CRandom random_generator;

		dAmount = mscnProblem.getdAmount();
		fAmount = mscnProblem.getfAmount();
		mAmount = mscnProblem.getmAmount();
		sAmount = mscnProblem.getsAmount();

		vector<double> sd = mscnProblem.getsd();
		vector<double> sf = mscnProblem.getsf();
		vector<double> sm = mscnProblem.getsm();
		vector<double> ss = mscnProblem.getss();

		
		
		int err=0;

		pdIndividual.resize(mscnProblem.getSize());//ustawiam liczbe genow
		
		
			do {

				//losuje xd
				for (int i = 0; i < dAmount; i++)
				{
					double delivererPosibilities = sd[i];
					for (int j = 0; j < fAmount; j++)
					{
						double maxAmountDeliverer = delivererPosibilities / (fAmount - j);
						double generatedValue = random_generator.getDouble(0, maxAmountDeliverer);

						pdIndividual[i * fAmount + j] = generatedValue;

						delivererPosibilities -= generatedValue;
					}
				}

				//losuje xf
				for (int i = 0; i < fAmount; i++)
				{
					double fabricPosibilities = sf[i];
					for (int j = 0; j < mAmount; j++)
					{
						double maxAmountFabric = fabricPosibilities / (mAmount - j);
						double generatedValue = random_generator.getDouble(0, maxAmountFabric);

						pdIndividual[dAmount * fAmount + i * mAmount + j] = generatedValue;

						fabricPosibilities -= generatedValue;
					}
				}

				//wypelniam pomocnicza tablice z zapotrzebowaniem sklepow
				vector<double> shopsNeeds;
				for (int i = 0; i < sAmount; i++)
				{
					shopsNeeds.push_back(ss[i]);
				}

				//losuje xm
				for (int i = 0; i < mAmount; i++)
				{
					double magazinePosibilities = sm[i];
					for (int j = 0; j < sAmount; j++)
					{
						double maxAmountMagazine = magazinePosibilities / (sAmount - j);
						double generatedValue = random_generator.getDouble(0, min(maxAmountMagazine, shopsNeeds[j]));

						pdIndividual[dAmount * fAmount + fAmount * mAmount + i * sAmount + j] = generatedValue;

						magazinePosibilities -= generatedValue;
						shopsNeeds[j] -= generatedValue;
					}
				}
				
				
			} while (mscnProblem.bConstraintsSatisfied(pdIndividual));
		
			
	}

	void saveIndividualToFile(string path) {

		FILE * f;
		f = fopen(path.c_str(), "w+");

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
				value = to_string(pdIndividual[i * fAmount + j]);
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
				value = to_string(pdIndividual[dAmount * fAmount + i * mAmount + j]);
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
				value = to_string(pdIndividual[dAmount * fAmount + fAmount * mAmount + i * sAmount + j]);
				fprintf(f, value.c_str());
				fprintf(f, " ");
			}
			fprintf(f, "\n");
		}

		fclose(f);
	}


	
	vector<double> & getGenes()
	{
		return pdIndividual;
	}

	void setGenes(vector<double> genes)
	{
		this->pdIndividual = genes;
	}
	
private:
	vector<double> pdIndividual;
	
	int dAmount;
	int fAmount;
	int mAmount;
	int sAmount;

};