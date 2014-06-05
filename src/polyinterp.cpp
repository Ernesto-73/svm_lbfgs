/*
	\file: polyinterp.cpp
	\author: Javier Zhang
	\date: 3-31-2014
*/ 
#include "polyinterp.h"

double polyInterp(double x1, double f1, double g1, double x2, double f2, double g2)
{
	assert(x1 != x2);
	
	double d1 = g1 + g2 - 3. * (f1 - f2) / (x1 - x2);
	double d2 = d1 * d1 - g1 * g2;
	
	double t, minPos;
	if(d2 > 0.)	
	{
		d2 = sqrt(d2);
		if(x2 > x1)
		{
			t = x2 - (x2 - x1) * ((g2 + d2 - d1) / (g2 - g1 + 2. * d2));
			minPos = min(max(t, x1), x2);
		}
		else {
			t = x1 - (x1 - x2) * ((g1 + d2 - d1) / (g1 - g2 + 2. * d2));
			minPos = min(max(t, x2), x1);
		}
	}
	else {
		minPos = (x1 + x2) / 2.;
	}
	return minPos;
}