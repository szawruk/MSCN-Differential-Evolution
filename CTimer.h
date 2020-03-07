#pragma once
#include "pch.h"
#include <windows.h>
class CTimer
{
public:

	void StartCounter()
	{
		QueryPerformanceFrequency(&frequency);
		
		QueryPerformanceCounter(&li1);
	}
	double GetCounter()
	{
		QueryPerformanceCounter(&li2);
		return double(li2.QuadPart - li1.QuadPart) / frequency.QuadPart;
	}

private:
	LARGE_INTEGER li1, li2, frequency;
};