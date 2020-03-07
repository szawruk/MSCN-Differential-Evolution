#pragma once
#pragma once
#pragma once
#include "pch.h"
#include "CRefCounter.h"

template <typename T> class CMySmartPointer
{
public:
	CMySmartPointer(T *pcPointer, bool ifArray = false);
	CMySmartPointer(const CMySmartPointer &pcOther);
	~CMySmartPointer();
	T& operator*() { return(*pc_pointer); }
	T* operator->() { return(pc_pointer); }
	CMySmartPointer<T>& operator=(const CMySmartPointer<T> &pcOther);
	T& operator[](int index);
	CMySmartPointer<T> duplicate();
	void swap(CMySmartPointer<T> &pcOther);

private:
	CRefCounter *pc_counter;
	T *pc_pointer;
	bool ifArray;
};//class CMySmartPointer


//----------------------------------------------------------implementacja metod
template <typename T>
CMySmartPointer<T>::CMySmartPointer(T *pcPointer, bool ifArray )
{
	ifArray = ifArray;
	//cout << "Konstruktor inteligentnego wskaznika" << endl;
	pc_pointer = pcPointer;
	pc_counter = new CRefCounter();
	if (pcPointer != NULL) {
		pc_counter->iAdd();
	}

}//CMySmartPointer(CSellData *pcPointer)


template <typename T>
CMySmartPointer<T>::CMySmartPointer(const CMySmartPointer &pcOther)
{
	//cout << "Kopiowanie inteligentnego wskaznika" << endl;
	ifArray = pcOther.ifArray;
	pc_pointer = pcOther.pc_pointer;
	pc_counter = pcOther.pc_counter;
	pc_counter->iAdd();
}//CMySmartPointer(const CMySmartPointer &pcOther)


template <typename T>
CMySmartPointer<T>::~CMySmartPointer()
{
	//cout << "Destruktor inteligentnego wskaznika" << endl;
	if (pc_counter->iDec() == 0)
	{
		//cout << "Zwalnianie pamieci na ktory wskazywal inteligentny wskaznik!" << endl;
		delete pc_counter;

		if(ifArray) delete[] pc_pointer;
		else delete pc_pointer;
		
	}//if (pc_counter->iDec())
}//~CMySmartPointer()


template <typename T>
CMySmartPointer<T>& CMySmartPointer<T>::operator=(const CMySmartPointer<T> &pcOther) {
	//cout << "Operator = inteligentnego wskaznika" << endl;

	if (pc_pointer == pcOther.pc_pointer) return *this;

	if (pc_pointer != NULL) {
		if (pc_counter->iDec() == 0)
		{
			//cout << "wskaznik zmienia adres, nic wiecej nie wskazuje na stary adres, wiec zwalnia pamiec" << endl;
			delete pc_pointer;
			delete pc_counter;
		}//if (pc_counter->iDec())
	}
	ifArray = pcOther.ifArray;
	pc_pointer = pcOther.pc_pointer;
	pc_counter = pcOther.pc_counter;
	pc_counter->iAdd();

	return *this;

}//CMySmartPointer<T>::operator=(const CMySmartPointer<T> &ctable)

template <typename T>
T& CMySmartPointer<T>::operator[](int index)
{
	if (ifArray) return pc_pointer[index];
}

template <typename T>
CMySmartPointer<T> CMySmartPointer<T>::duplicate() {
	T *newPointer;
	newPointer = new T(*pc_pointer);

	CMySmartPointer<T> newSmartPointer(newPointer);
	cout << &newSmartPointer << endl;
	return newSmartPointer;
}

template <typename T>
void CMySmartPointer<T>::swap(CMySmartPointer<T> &pcOther) {

	if (pc_pointer != pcOther.pc_pointer) {
		//tymczasowe wskazniki
		T *tempPointer;
		CRefCounter *tempCounter;

		tempPointer = pcOther.pc_pointer;
		tempCounter = pcOther.pc_counter;

		pcOther.pc_pointer = this->pc_pointer;
		pcOther.pc_counter = this->pc_counter;

		this->pc_pointer = tempPointer;
		this->pc_counter = tempCounter;
	}

}