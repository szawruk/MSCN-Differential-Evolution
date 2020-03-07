#include "CMscnProblem.h"
#include "Individual.h"

//Constructors
CMscnProblem::CMscnProblem()
{
	prepareProblem(DEFAULT_D_AMOUNT, DEFAULT_F_AMOUNT, DEFAULT_M_AMOUNT, DEFAULT_S_AMOUNT);
}

//Przygotowanie klasy problemu
void CMscnProblem::prepareProblem(int D, int F, int M, int S)
{
	dAmount = D;
	fAmount = F;
	mAmount = M;
	sAmount = S;

	cd = Matrix<double>(dAmount, fAmount);
	cf = Matrix<double>(fAmount, mAmount);
	cm = Matrix<double>(mAmount, sAmount);

	xdminmax = Matrix<pair<double, double>>(dAmount, fAmount);
	xfminmax = Matrix<pair<double, double>>(fAmount, mAmount);
	xmminmax = Matrix<pair<double, double>>(mAmount, sAmount);

	sd.resize(dAmount);
	sf.resize(fAmount);
	sm.resize(mAmount);
	ss.resize(sAmount);

	ud.resize(dAmount);
	uf.resize(fAmount);
	um.resize(mAmount);

	p.resize(sAmount);
}



//Setters

	//Amount setters
bool CMscnProblem::setDAmount(int newAmount)
{
	if (newAmount < 0) return false;

	dAmount = newAmount;
	cd.adjustMatrix(dAmount, fAmount);
	xdminmax.adjustMatrix(dAmount, fAmount);

	sd.resize(dAmount);
	ud.resize(dAmount);
	
	return true;
}

bool CMscnProblem::setFAmount(int newAmount)
{
	if (newAmount < 0) return false;

	fAmount = newAmount;
	cd.adjustMatrix(dAmount, fAmount);
	cf.adjustMatrix(fAmount, mAmount);

	xdminmax.adjustMatrix(dAmount, fAmount);
	xfminmax.adjustMatrix(fAmount, mAmount);

	sf.resize(fAmount);
	uf.resize(fAmount);

	return true;
}

bool CMscnProblem::setMAmount(int newAmount)
{
	if (newAmount < 0) return false;

	mAmount = newAmount;
	cf.adjustMatrix(fAmount, mAmount);
	cm.adjustMatrix(mAmount, sAmount);

	xfminmax.adjustMatrix(fAmount, mAmount);
	xmminmax.adjustMatrix(mAmount, sAmount);

	sm.resize(mAmount);
	um.resize(mAmount);

	return true;
}

bool CMscnProblem::setSAmount(int newAmount)
{
	if (newAmount < 0) return false;

	sAmount = newAmount;
	cm.adjustMatrix(mAmount, sAmount);

	xmminmax.adjustMatrix(mAmount, sAmount);

	ss.resize(sAmount);
	p.resize(sAmount);

	return true;
}


	//Values in matrix - setters
bool CMscnProblem::setInCdMatrix(double value, int row, int column)
{
	return setInMatrixhelper(cd, value, row, column);
}

bool CMscnProblem::setInCfMatrix(double value, int row, int column)
{
	return setInMatrixhelper(cf, value, row, column);
}

bool CMscnProblem::setInCmMatrix(double value, int row, int column)
{
	return setInMatrixhelper(cm, value, row, column);
}

		//SET HELPER
bool CMscnProblem::setInMatrixhelper(Matrix<double>& matrix, double value, int row, int column)
{
	if (value < 0) return false;

	matrix.set(value, row, column);
	return true;
}


	//Values in vectors setters
	//S
bool CMscnProblem::setInSdVector(double value, int position)
{
	return setInVectorhelper(sd, value, position);
}

bool CMscnProblem::setInSfVector(double value, int position)
{
	return setInVectorhelper(sf, value, position);
}


bool CMscnProblem::setInSmVector(double value, int position)
{
	return setInVectorhelper(sm, value, position);
}

bool CMscnProblem::setInSsVector(double value, int position)
{
	return setInVectorhelper(ss, value, position);
}

	//U
bool CMscnProblem::setInUdVector(double value, int position)
{
	return setInVectorhelper(ud, value, position);
}

bool CMscnProblem::setInUfVector(double value, int position)
{
	return setInVectorhelper(uf, value, position);
}

bool CMscnProblem::setInUmVector(double value, int position)
{
	return setInVectorhelper(um, value, position);
}

bool CMscnProblem::setInPVector(double value, int position)
{
	return setInVectorhelper(p, value, position);
}

		//SET HELPER
bool CMscnProblem::setInVectorhelper(vector<double>& vector, double value, int position)
{
	if (value < 0) return false;

	vector[position] = value;
	return true;
}


//Koszty transportu przechowywania i wytworzenia
double CMscnProblem::getKt(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm)
{
	double sum = 0;

	//DF
	for (int i = 0; i < dAmount; i++)
	{
		for (int j = 0; j < fAmount; j++)
		{
			sum += cd.get(i, j) * xd.get(i, j);
		}
	}

	//FM
	for (int i = 0; i < fAmount; i++)
	{
		for (int j = 0; j < mAmount; j++)
		{
			sum += cf.get(i, j) * xf.get(i, j);
		}
	}

	//MS
	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			sum += cm.get(i, j) * xm.get(i, j);
		}
	}

	return sum;
}

//Koszty stale
double CMscnProblem::getKu(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm)
{
	double sum = 0;

	for (int i = 0; i < dAmount; i++)
	{
		if (ifTakePartInOrder(xd, i)) sum += ud[i];
	}

	for (int i = 0; i < fAmount; ++i)
	{
		if (ifTakePartInOrder(xf, i)) sum += uf[i];
	}

	for (int i = 0; i < mAmount; ++i)
	{
		if (ifTakePartInOrder(xm, i)) sum += um[i];
	}

	return sum;
}

//Funkcja pomocnicza do sprawdzania czy w wierszu macierzy jest jakas wartosc dotatnia co oznacza ze dana jednostka bedzie cos produkowac/dostarczac
bool CMscnProblem::ifTakePartInOrder(Matrix<double>& matrix, int row)
{
		for (int i = 0; i < matrix.getWidth(); i++)
		{
			if (matrix.get(row, i) > 0) return true;
		}

		return false;
}

double CMscnProblem::getP(Matrix<double>& xd, Matrix<double>& xf, Matrix<double>& xm)
{
	double profit = 0;

	//licze sam przychod
	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			profit += xm.get(i, j) * p[j];
		}
	}
	
	return profit - getKu(xd, xf, xm) - getKt(xd, xf, xm);
}

//Sprawdzam czy tablica ma adres NULL, ma nieprawid³ow¹ d³ugoœæ, lub zawiera wartoœci ujemne
int CMscnProblem::checkSolution(double * pdSolution, int solutionLength)
{
	if (pdSolution == NULL) return 1;
	

	int requiredLength = dAmount * fAmount + fAmount * mAmount + mAmount * sAmount;

	if (requiredLength != solutionLength) return 2;

	for (int i = 0; i < solutionLength; i++)
	{
		//cout << pdSolution[i] << " ";
		if (pdSolution[i] < 0) return 3;
	}
	return 0;
}

CMscnSolution CMscnProblem::changeSolutionToMatrix(double * pdSolution)
{
	CMscnSolution solution;
	solution.getXd() = Matrix<double>(dAmount, fAmount);
	solution.getXf() = Matrix<double>(fAmount, mAmount);
	solution.getXm() = Matrix<double>(mAmount, sAmount);

	for (int i = 0; i < dAmount; i++)
	{
		for (int j = 0; j < fAmount; j++)
		{
			solution.getXd().set(pdSolution[i*fAmount + j], i, j);
		}
	}
		
	for (int i = 0; i < fAmount; i++)
	{
		for (int j = 0; j < mAmount; j++)
		{
			solution.getXf().set(pdSolution[solution.getXd().getSize() + i * mAmount + j], i, j);
		}
	}	

	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			solution.getXm().set(pdSolution[solution.getXd().getSize() + solution.getXf().getSize() + i * sAmount + j], i, j);

		}
	}
				
	return solution;
}

double CMscnProblem::dGetQuality(double * pdSolution, int solutionLength, int &errorCode)
{

	errorCode = checkSolution(pdSolution, solutionLength);

	if (errorCode != 0) return 0;

	CMscnSolution solution = changeSolutionToMatrix(pdSolution);

	return getP(solution.getXd(), solution.getXf(), solution.getXm());
}
CMscnSolution CMscnProblem::changeSolutionToMatrix(vector<double> & pdSolution)
{
	CMscnSolution solution;
	solution.getXd() = Matrix<double>(dAmount, fAmount);
	solution.getXf() = Matrix<double>(fAmount, mAmount);
	solution.getXm() = Matrix<double>(mAmount, sAmount);

	for (int i = 0; i < dAmount; i++)
	{
		for (int j = 0; j < fAmount; j++)
		{
			solution.getXd().set(pdSolution[i*fAmount + j], i, j);
		}
	}

	for (int i = 0; i < fAmount; i++)
	{
		for (int j = 0; j < mAmount; j++)
		{
			solution.getXf().set(pdSolution[solution.getXd().getSize() + i * mAmount + j], i, j);
		}
	}

	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			solution.getXm().set(pdSolution[solution.getXd().getSize() + solution.getXf().getSize() + i * sAmount + j], i, j);

		}
	}

	return solution;
}


//dla wektorow z rozwiazamiem 
double CMscnProblem::dGetQuality(vector<double> & pdSolution)
{
	if (bConstraintsSatisfied(pdSolution) == false)
	{
		Individual individual;
		individual.generateIndividual(*this);
		CMscnSolution solution = changeSolutionToMatrix(individual.getGenes());
		return getP(solution.getXd(), solution.getXf(), solution.getXm());
	}
	else
	{
		CMscnSolution solution = changeSolutionToMatrix(pdSolution);
		return getP(solution.getXd(), solution.getXf(), solution.getXm());
	}
	
}

bool CMscnProblem::bConstraintsSatisfied(double * pdSolution, int solutionLength, int &errorCode)
{
	errorCode = checkSolution(pdSolution, solutionLength);

	if (errorCode != 0) return 0;

	CMscnSolution solution = changeSolutionToMatrix(pdSolution);


	//Sumaryczna iloœæ surowca zamówiona u danego dostawcy nie mo¿e przekroczyæ jego mocy produkcyjnych 
	for (int i = 0; i < dAmount; ++i)
	{
		if (solution.getXd().rowSum(i) > sd[i]) return false;
	}

	//Sumaryczna iloœæ produktów zamówiona w danej fabryce nie mo¿e przekroczyæ jej mocy produkcyjnych
	for (int i = 0; i < fAmount; ++i)
	{
		if (solution.getXf().rowSum(i) > sf[i]) return false;
	}

	//Sumaryczna iloœæ produktów zamówiona z danego magazynu nie mo¿e przekroczyæ jego pojemnoœci
	for (int i = 0; i < mAmount; ++i)
	{
		if (solution.getXm().rowSum(i) > sm[i]) return false;
	}

	//Sumaryczna iloœæ produktów dostarczana do danego sklepu nie mo¿e przekroczyæ zapotrzebowania rynkowego dla tego sklepu
	for (int i = 0; i < sAmount; ++i)
	{
		if (solution.getXm().columnSum(i) > ss[i]) return false;
	}

	//Fabryka musi dostaæ wystarczaj¹c¹ iloœæ surowca, ¿eby wyprodukowaæ wystarczaj¹c¹ iloœæ produktów
	for (int i = 0; i < fAmount; ++i)
	{
		if (solution.getXd().columnSum(i) < solution.getXf().rowSum(i)) return false;
	}

	//Magazyn musi otrzymaæ z fabryk wystarczaj¹c¹ iloœæ produktów, ¿eby móc dostarczyæ je do sklepów
	for (int i = 0; i < mAmount; ++i)
	{
		if (solution.getXf().columnSum(i) < solution.getXm().rowSum(i)) return false;
	}

	return true;
}

bool CMscnProblem::bConstraintsSatisfied(vector<double> & pdSolution)
{
	CMscnSolution solution = changeSolutionToMatrix(pdSolution);


	//Sumaryczna iloœæ surowca zamówiona u danego dostawcy nie mo¿e przekroczyæ jego mocy produkcyjnych 
	for (int i = 0; i < dAmount; ++i)
	{
		if (solution.getXd().rowSum(i) > sd[i]) return false;
	}

	//Sumaryczna iloœæ produktów zamówiona w danej fabryce nie mo¿e przekroczyæ jej mocy produkcyjnych
	for (int i = 0; i < fAmount; ++i)
	{
		if (solution.getXf().rowSum(i) > sf[i]) return false;
	}

	//Sumaryczna iloœæ produktów zamówiona z danego magazynu nie mo¿e przekroczyæ jego pojemnoœci
	for (int i = 0; i < mAmount; ++i)
	{
		if (solution.getXm().rowSum(i) > sm[i]) return false;
	}

	//Sumaryczna iloœæ produktów dostarczana do danego sklepu nie mo¿e przekroczyæ zapotrzebowania rynkowego dla tego sklepu
	for (int i = 0; i < sAmount; ++i)
	{
		if (solution.getXm().columnSum(i) > ss[i]) return false;
	}

	//Fabryka musi dostaæ wystarczaj¹c¹ iloœæ surowca, ¿eby wyprodukowaæ wystarczaj¹c¹ iloœæ produktów
	for (int i = 0; i < fAmount; ++i)
	{
		if (solution.getXd().columnSum(i) < solution.getXf().rowSum(i)) return false;
	}

	//Magazyn musi otrzymaæ z fabryk wystarczaj¹c¹ iloœæ produktów, ¿eby móc dostarczyæ je do sklepów
	for (int i = 0; i < mAmount; ++i)
	{
		if (solution.getXf().columnSum(i) < solution.getXm().rowSum(i)) return false;
	}

	return true;
}

vector<pair<double, double>> CMscnProblem::getMinMaxValues()
{
	vector<pair<double,double>> minmax;

	for (int i = 0; i < dAmount; i++)
	{
		for (int j = 0; j < fAmount; j++)
		{
			
			minmax.push_back(make_pair( xdminmax.get(i, j).first, xdminmax.get(i, j).second));
		}
			
	}
		
	for (int i = 0; i < fAmount; i++)
	{
		for (int j = 0; j < mAmount; j++)
		{

			minmax.push_back(make_pair(xfminmax.get(i, j).first, xfminmax.get(i, j).second));
		}
			
	}
		

	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			minmax.push_back(make_pair(xmminmax.get(i, j).first, xmminmax.get(i, j).second ));
		}
			
	}
		
	return minmax;
}



void CMscnProblem::serializeVectorHelper(FILE * f, vector<double> & vec, int range, string name)
{
	fprintf(f, name.c_str()); fprintf(f, "\n");//print the header
	string value;
	
	for (int i = 0; i < range; i++)
	{
		value = to_string(vec[i]);
		fprintf(f, value.c_str());
		fprintf(f, " ");
	}
	fprintf(f, "\n");
}

void CMscnProblem::serializeMatrixCHelper(FILE * f, Matrix<double> & matrix, int range1, int range2, string name)
{
	fprintf(f, name.c_str()); fprintf(f, "\n");//print the header
	string value;

	for (int i = 0; i < range1; i++)
	{
		for (int j = 0; j < range2; j++)
		{
			value = to_string(matrix.get(i, j));
			fprintf(f, value.c_str());
			fprintf(f, " ");
		}
		fprintf(f, "\n");
	}
}

void CMscnProblem::serializeMatrixMinMaxHelper(FILE * f, Matrix<pair<double, double>> & matrix, int range1, int range2, string name)
{
	fprintf(f, name.c_str()); fprintf(f, "\n");//print the header
	string value;

	for (int i = 0; i < range1; i++)
	{
		for (int j = 0; j <range2; j++)
		{
			value = to_string(matrix.get(i, j).first);
			fprintf(f, value.c_str());
			fprintf(f, " ");
			value = to_string(matrix.get(i, j).second);
			fprintf(f, value.c_str());
			fprintf(f, " ");
		}
		fprintf(f, "\n");
	}
}


void CMscnProblem::serialize(string path)
{
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

	serializeVectorHelper(f, sd, dAmount,"sd"); //sd

	serializeVectorHelper(f, sf, fAmount, "sf"); //sf

	serializeVectorHelper(f, sm, mAmount, "sm"); //sm

	serializeVectorHelper(f, ss, sAmount, "ss"); //ss


	serializeMatrixCHelper(f, cd, dAmount, fAmount, "cd"); //cd

	serializeMatrixCHelper(f, cf, fAmount, mAmount, "cf"); //cf

	serializeMatrixCHelper(f, cm, mAmount, sAmount, "cm"); //cm


	serializeVectorHelper(f, ud, dAmount, "ud"); //ud

	serializeVectorHelper(f, uf, fAmount, "uf"); //uf

	serializeVectorHelper(f, um, mAmount, "um"); //um

	serializeVectorHelper(f, p, sAmount, "p"); //p


	serializeMatrixMinMaxHelper(f, xdminmax, dAmount, fAmount, "xdminmax"); //xdminmax

	serializeMatrixMinMaxHelper(f, xfminmax, fAmount, mAmount, "xfminmax"); //xfminmax

	serializeMatrixMinMaxHelper(f, xmminmax, mAmount, sAmount, "xmminmax"); //xsminmax


}

void CMscnProblem::deserializeVectorHelper(FILE * f, vector<double> & vec, int range)
{
	double dValue = 0;
	char buffer[200];

	fscanf(f, "%s", &buffer); //read the header
	
	for (int i = 0; i < range; i++)
	{
		fscanf(f, "%lf", &dValue);
		setInVectorhelper(vec, dValue, i);	
	}
}

void CMscnProblem::deserializeMatrixCHelper(FILE * f, Matrix<double> & matrix, int range1, int range2)
{
	double dValue = 0;
	char buffer[200];

	fscanf(f, "%s", &buffer); //read the header

	for (int i = 0; i < range1; i++)
	{
		for (int j = 0; j < range2; j++)
		{
			fscanf(f, "%lf", &dValue);

			setInMatrixhelper(matrix, dValue, i, j);
		}
	}
}

void CMscnProblem::deserializeMatrixMinMaxHelper(FILE * f, Matrix<pair<double,double>> & matrix, int range1, int range2)
{
	double dValue = 0;
	char buffer[200];

	fscanf(f, "%s", &buffer); //read the header

	for (int i = 0; i < range1; i++)
	{
		for (int j = 0; j < 2 * range2; j++)
		{
			fscanf(f, "%lf", &dValue);
			if (j % 2 == 0)
			{
				matrix.setPairF(dValue, i, j / 2);

			}
			else
			{
				matrix.setPairS(dValue, i, j / 2);
			}

		}
	}
}

void CMscnProblem::deserialize(string path)
{
	char buffer[200];
	int value = 0;
	int actualPos = 0;
	double dValue = 0;

	FILE * f;
	f = fopen(path.c_str(), "r+");
	
	fscanf(f, "%s", &buffer); //D
	fscanf(f, "%d", &value); 
	setDAmount(value);

	fscanf(f, "%s", &buffer); //F
	fscanf(f, "%d", &value);
	setFAmount(value);

	fscanf(f, "%s", &buffer); //M
	fscanf(f, "%d", &value);
	setMAmount(value);

	fscanf(f, "%s", &buffer); //S
	fscanf(f, "%d", &value);
	setSAmount(value);

	deserializeVectorHelper(f, sd, dAmount); //sd

	deserializeVectorHelper(f, sf, fAmount); //sf

	deserializeVectorHelper(f, sm, mAmount); //sm

	deserializeVectorHelper(f, ss, sAmount); //ss


	deserializeMatrixCHelper(f, cd, dAmount, fAmount); //cd
	
	deserializeMatrixCHelper(f, cf, fAmount, mAmount); //cf

	deserializeMatrixCHelper(f, cm, mAmount, sAmount); //cm
	

	deserializeVectorHelper(f, ud, dAmount); //ud

	deserializeVectorHelper(f, uf, fAmount); //uf

	deserializeVectorHelper(f, um, mAmount); //um

	deserializeVectorHelper(f, p, sAmount); //p


	deserializeMatrixMinMaxHelper(f, xdminmax, dAmount, fAmount); //xdminmax

	deserializeMatrixMinMaxHelper(f, xfminmax, fAmount, mAmount); //xfminmax

	deserializeMatrixMinMaxHelper(f, xmminmax, mAmount, sAmount); //xmminmax
	
	
	fclose(f);
}

void CMscnProblem::randomMatrixHelper(Matrix<double>& matrix, int range1, int range2)
{
	for (int i = 0; i < range1; i++)
	{
		for (int j = 0; j < range2; j++)
		{
			setInMatrixhelper(matrix, random_generator.getDouble(10,100), i, j);
		}
	}
}

void CMscnProblem::vGenerateInstance(int D, int F, int M, int S)
{//generuje losowo problem

	double deliverersPosibilities = 0; //tutaj bede orzechowywal laczne mozliwosci wszystkich dostawcow
	double fabricsPosibilities = 0; //tutaj bede orzechowywal laczne mozliwosci wszystkich fabryk
	double magazinesPosibilities = 0; //tutaj bede orzechowywal laczne mozliwosci wszystkich magazynow

	//majpierw beda tu sumy potem policze srednia arytmetyczna
	double averageDelivererCost = 0;
	double averageFabricCost = 0;
	double averageMagazineCost = 0;

	prepareProblem(D, F, M, S);

	//generuje cd
	for (int i = 0; i < dAmount; i++)
	{
		for (int j = 0; j < fAmount; j++)
		{
			setInMatrixhelper(cd, random_generator.getDouble(DEFAULT_CD_MIN, DEFAULT_CD_MAX), i, j);
			averageDelivererCost += cd.get(i, j);
		}
	}
	averageDelivererCost /= (dAmount*fAmount);

	//generuje cf
	for (int i = 0; i < fAmount; i++)
	{
		for (int j = 0; j < mAmount; j++)
		{
			setInMatrixhelper(cf, random_generator.getDouble(DEFAULT_CF_MIN, DEFAULT_CF_MAX), i, j);
			averageFabricCost += cf.get(i, j);
		}
	}
	averageFabricCost /= (fAmount*mAmount);

	//generuje cm
	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			setInMatrixhelper(cm, random_generator.getDouble(DEFAULT_CM_MIN, DEFAULT_CM_MAX), i, j);
			averageMagazineCost += cm.get(i, j);
		}
	}
	averageMagazineCost /= (mAmount*sAmount);

	//generuje ud
	for (int i = 0; i < dAmount; i++)
	{
		setInVectorhelper(ud, random_generator.getDouble(DEFAULT_UD_MIN, DEFAULT_UD_MAX), i);
	}

	//generuje uf
	for (int i = 0; i < fAmount; i++)
	{
		setInVectorhelper(uf, random_generator.getDouble(DEFAULT_UF_MIN, DEFAULT_UF_MAX), i);
	}

	//generuje um
	for (int i = 0; i < mAmount; i++)
	{
		setInVectorhelper(um, random_generator.getDouble(DEFAULT_UM_MIN, DEFAULT_UM_MAX), i);
	}

	//generuje sd
	for (int i = 0; i < dAmount; i++)
	{
		setInVectorhelper(sd, random_generator.getDouble(DEFAULT_SD_MIN, DEFAULT_SD_MAX), i);
		deliverersPosibilities += sd[i];
	}

	double fabricsMin = 0;
	double fabricsMax = 0;
	
	//generuje sf
	for (int i = 0; i < fAmount; i++)
	{
		fabricsMin = (deliverersPosibilities / (fAmount-i)) / 2;
		fabricsMax = deliverersPosibilities / (fAmount - i);

		setInVectorhelper(sf, random_generator.getDouble(fabricsMin, fabricsMax), i);
		fabricsPosibilities += sf[i];
		deliverersPosibilities -= sf[i];
	}

	double magazinesMin = 0;
	double magazinesMax = 0;

	//generuje sm
	for (int i = 0; i < mAmount; i++)
	{
		magazinesMin = (fabricsPosibilities / (mAmount - i)) / 2;
		magazinesMax = fabricsPosibilities / (mAmount - i);

		
		setInVectorhelper(sm, random_generator.getDouble(magazinesMin, magazinesMax), i);
		magazinesPosibilities += sm[i];
		fabricsPosibilities -= sm[i];
	}

	double shopsMin = 0;
	double shopsMax = 0;

	//generuje ss
	for (int i = 0; i < sAmount; i++)
	{
		shopsMin = (magazinesPosibilities / (sAmount - i)) / 2;
		shopsMax = magazinesPosibilities / (sAmount - i);

		setInVectorhelper(ss, random_generator.getDouble(shopsMin, shopsMax), i);

		magazinesPosibilities -= ss[i];
	}
	
	double goodPriceToSell = averageDelivererCost + averageFabricCost + averageMagazineCost;
	goodPriceToSell *= 1.1; //teraz cena sprzedazy jest super zeby dobrze zarobic

	//generuje ceny
	for (int i = 0; i < sAmount; i++)
	{
		setInVectorhelper(p, random_generator.getDouble(goodPriceToSell, goodPriceToSell *1.5), i);

	}

	//generuje xdminmax, xfminmax i xmminmax
	for (int i = 0; i < dAmount; i++)
	{
		for (int j = 0; j < fAmount; j++)
		{
			xdminmax.set(make_pair(0, DEFAULT_XD_MAX), i, j);
		}
	}

	for (int i = 0; i < fAmount; i++)
	{
		for (int j = 0; j < mAmount; j++)
		{
			xfminmax.set(make_pair(0, DEFAULT_XF_MAX), i, j);
		}
	}

	for (int i = 0; i < mAmount; i++)
	{
		for (int j = 0; j < sAmount; j++)
		{
			xmminmax.set(make_pair(0, DEFAULT_XM_MAX), i, j);
		}
	}

}

double * CMscnProblem::makeDoubleArraySolutionFromFile(string path) 
{//tworze tabice pdSolution zawierajaca 3 macierze zapisana jako tablica doubli
	char buffer[200];
	int value = 0;
	int d = 0;
	int f = 0;
	int m = 0;
	int s = 0;
	double dValue = 0;

	double * solution;
	

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
	
	
	solution = new double[d*f + f * m + m * s];

	fscanf(file, "%s", &buffer); //xd
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < f; j++)
		{
			fscanf(file, "%lf", &dValue);
			solution[i * f + j] = dValue;
		}
	}

	fscanf(file, "%s", &buffer); //xf
	for (int i = 0; i < f; i++)
	{
		for (int j = 0; j < m; j++)
		{
			fscanf(file, "%lf", &dValue);
			solution[d * f + i * m + j] = dValue;
		}
	}

	fscanf(file, "%s", &buffer); //xm
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < s; j++)
		{
			fscanf(file, "%lf", &dValue);
			solution[d * f + f * m + i * s + j] = dValue;
		}
	}
	
	fclose(file);
	return solution;
}

int CMscnProblem::getSizeSolutionFromFile(string path) //pobieram rozmiar talbicy pdSolution zawierajaca 3 macierze jako jedna tablica doubli
{
	char buffer[200];
	int value = 0;
	int d = 0;
	int f = 0;
	int m = 0;
	int s = 0;
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

	return d * f + f * m + m * s;
}

void CMscnProblem::saveVectorSolutionToFile(vector<double> & pdSolution, string path) {

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
			value = to_string(pdSolution[i*fAmount + j]);
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
			value = to_string(pdSolution[(dAmount * fAmount) + (i * mAmount + j)]);
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
			value = to_string(pdSolution[(dAmount * fAmount) + (fAmount * mAmount) + (i * sAmount + j)]);
			fprintf(f, value.c_str());
			fprintf(f, " ");
		}
		fprintf(f, "\n");
	}

	fclose(f);
}


