#pragma once
#include "pch.h"
#include "CMySmartPointer.h"

template<typename T>

class Matrix
{
public:

	Matrix()
	{
		adjustMatrix(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	}
	
	Matrix(int height, int width)
	{
		adjustMatrix(height, width);
	}

	void adjustMatrix(int height, int width)
	{
		this->width = width;
		this->height = height;
		this->size = width * height;

		matrix = CMySmartPointer<T>(new T[size](), true);
	}

	void set(T val, int row, int column)
	{
		matrix[(row * width) + column] = val;
	}

	void setPairF(double val, int row, int column)
	{
		matrix[(row * width) + column].first = val;
	}

	void setPairS(double val, int row, int column)
	{
		matrix[(row * width) + column].second = val;
	}

	T get(int row, int column)
	{
		return matrix[(row * width) + column];
	}

	int getWidth()
	{
		return width;
	}

	int getHeight()
	{
		return height;
	}

	int getSize()
	{
		return size;
	}

	double rowSum(int row)
	{
		double sum = 0;

		for (int i = 0; i < width; i++)
		{
			sum += this->get(row, i);
		}

		return sum;
	}

	double columnSum(int column)
	{
		double sum = 0;

		for (int i = 0; i < height; i++)
		{
			sum += this->get(i, column);
		}

		return sum;
	}

private:

	CMySmartPointer<T> matrix = NULL;

	int width;
	int height;
	int size;
};
