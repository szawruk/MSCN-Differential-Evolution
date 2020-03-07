#pragma once
#include "pch.h"
#include "Matrix.h"
#include "CMscnSolution.h"
#include "CRandom.h"
#include "constans.h"
#include "CProblem.h"

class CMscnProblem : public CProblem
{
public:
	CMscnProblem();
	
	bool setDAmount(int newAmount);
	bool setFAmount(int newAmount);
	bool setMAmount(int newAmount);
	bool setSAmount(int newAmount);

	bool setInCdMatrix(double value, int row, int column);
	bool setInCfMatrix(double value, int row, int column);
	bool setInCmMatrix(double value, int row, int column);

	bool setInMatrixhelper(Matrix<double> &matrix, double value, int row, int column);

	bool setInSdVector(double value, int position);
	bool setInSfVector(double value, int position);
	bool setInSmVector(double value, int position);
	bool setInSsVector(double value, int position);

	bool setInUdVector(double value, int position);
	bool setInUfVector(double value, int position);
	bool setInUmVector(double value, int position);
	bool setInPVector(double value, int position);

	bool setInVectorhelper(vector<double> &vector, double value, int position);

	double getKt(Matrix<double> &xd, Matrix<double> &xf, Matrix<double> &xm);
	double getKu(Matrix<double> &xd, Matrix<double> &xf, Matrix<double> &xm);

	double dGetQuality(double * pdSolution, int solutionLength, int &errorCode);
	double dGetQuality(vector<double> & pdSolution);

	bool bConstraintsSatisfied(double * pdSolution, int solutionLength, int &errorCode);
	bool bConstraintsSatisfied(vector<double> & pdSolution);

	vector<pair<double, double>> getMinMaxValues();

	void serialize(string path);
	void deserialize(string path);
	
	void vGenerateInstance(int D, int F, int M, int S);

	double * makeDoubleArraySolutionFromFile(string path);
	int getSizeSolutionFromFile(string path);

	int getdAmount() { return dAmount; };
	int getfAmount() { return fAmount; };
	int getmAmount() { return mAmount; };
	int getsAmount() { return sAmount; };

	vector<double> & getsd() { return sd;};
	vector<double> & getsf() { return sf; };
	vector<double> & getsm() { return sm; };
	vector<double> & getss() { return ss; };

	Matrix<pair<double, double>> & getxdminmax() { return xdminmax; };
	Matrix<pair<double, double>> & getxfminmax() { return xfminmax; };
	Matrix<pair<double, double>> & getxmminmax() { return xmminmax; };

	int getSize() { return dAmount * fAmount + fAmount * mAmount + mAmount * sAmount; };

	void saveVectorSolutionToFile(vector<double> & pdSolution, string path);

	

protected:

	CRandom random_generator;

	int dAmount;
	int fAmount;
	int mAmount;
	int sAmount;

	//Ceny
	Matrix<double> cd;
	Matrix<double> cf;
	Matrix<double> cm;

	//macierze dla min max
	Matrix<pair<double, double>> xdminmax;
	Matrix<pair<double, double>> xfminmax;
	Matrix<pair<double, double>> xmminmax;

	//Ograniczenia
	vector<double> sd;
	vector<double> sf;
	vector<double> sm;
	vector<double> ss;

	//Koszty stale
	vector<double> ud;
	vector<double> uf;
	vector<double> um;

	//zyski
	vector<double> p;

	//Czy dany dostawca, fabryka, magazyn gdziekolwiek dostarczy produkty
	bool ifTakePartInOrder(Matrix<double> &matrix, int row);

	//zwraca zysk ze sprzedazy
	double getP(Matrix<double> &xd, Matrix<double> &xf, Matrix<double> &xm);

	//Funkcja pomocnicza do sprawdzania czy w wierszu macierzy jest jakas wartosc dotatnia co oznacza ze dana jednostka bedzie cos produkowac/dostarczac
	int checkSolution(double *pdSolution, int solutionLength);

	//zamienia tablice *double na trzy macierze
	CMscnSolution changeSolutionToMatrix(double *pdSolution);

	//zamienia wektor na trzy macierze
	CMscnSolution changeSolutionToMatrix(vector<double> & pdSolution);

	//deserializacja pomocnicze funkcje
	void deserializeVectorHelper(FILE * f, vector<double> & vec, int range);
	void deserializeMatrixCHelper(FILE * f, Matrix<double> & matrix, int range1, int range2);
	void deserializeMatrixMinMaxHelper(FILE * f, Matrix<pair<double,double>> & matrix, int range1, int range2);

	//serializacja pomocnicze funkcje
	void serializeVectorHelper(FILE * f, vector<double> & vec, int range, string name);
	void serializeMatrixCHelper(FILE * f, Matrix<double> & matrix, int range1, int range2, string name);
	void serializeMatrixMinMaxHelper(FILE * f, Matrix<pair<double, double>> & matrix, int range1, int range2, string name);

	//przygotowanie klasy problemu(przygotowanie talbic i macierzy - ustawienie rozmiarow itp)
	void prepareProblem(int D, int F, int M, int S);

	void randomMatrixHelper(Matrix<double> & matrix, int range1, int range2);
};