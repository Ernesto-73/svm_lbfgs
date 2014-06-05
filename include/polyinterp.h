/*
	\file: polyinterp.h
	\author: Javier Zhang
	\date: 3-31-2014
*/ 

#ifndef _POLYINTERP_H_
#define _POLYINTERP_H_

#include <vector>
#include <cassert>
#include <cmath>

/*
* \def inline min & max
*/
inline double min(double a, double b) {return a > b? b:a;}

inline double max(double a, double b) {return a > b? a:b;}

/*
* \def Triple structure
* This double triplet is used for polynomial interpolation.
*/
typedef struct{
	double x;
	double y;
	double g;
}Triple;

/*
* \def polyInterp(std::vector<Triple> &options)
* Solve polynomial interpolation with more than one points.
*/
double polyInterp(std::vector<Triple> &pionts);

/*
* \def polyInterp(double x1, double f1, double g1, double x2, double f2, double g2)
* Solve polynomial interpolation with two points.
*/
double polyInterp(double x1, double f1, double g1, double x2, double f2, double g2);

#endif
