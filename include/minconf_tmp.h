/*
	\file: minconf_tmp.h
	\author: Javier Zhang
	\date: 3-20-2014
*/ 

#ifndef _MINCONF_TMP_H_
#define _MINCONF_TMP_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <deque>
#include "array.h"
#include "polyinterp.h"

inline double abs(double x) { return x >= 0 ? x : -x;}

typedef double (*FuncObj)(int , const double *, const double *, double *);
/*	
* \define structure Options
*/
enum{SD = 0, LBFGS, BFGS, NEWTON};

typedef struct Options{
	/*
		Default Options
	*/
	Options(){
		verbose = 0;		// Output message level
		optTol = 1e-6;
		maxIter = 500;		
		suffDec = 1e-4;
		interp = 1;
		method = LBFGS;
		corrections = 100;
		damped = false;
	};
		
	int verbose;
	double optTol;
	int maxIter;
	double suffDec;
	int interp;
	int method;
	int corrections;
	bool damped;

}Options;

/*	
* \define class minConf_TMP
*/
class minConf_TMP
{
public:
	/* 
		constructors 
	*/
	minConf_TMP();
	
	/*
		_x: 	variables
		_LB:	upper bound
		_UB:	lower bound
		_opt:	options
	*/
	minConf_TMP(Array _x, Array _LB, Array _UB);
	
	minConf_TMP(Array _x, Array _LB, Array _UB, Options *_opt);	
	
	/* 
		destructor 
	*/
	~minConf_TMP();
	
	/* 
		main process function 
	*/
	void process();
	
	/* 
		set the objective function 
	*/
	void setFunObj(FuncObj _ff);
	
	/* 
		load train data and responses 
	*/
	void loadData(int _nInstance, int _dim, double *_input, double *_output);
private:
	/*
		compute working set.
	*/
	void computeWorkingSet();
	
	/*
		constrain x integer the [LB, UB], implement two metric projection.
	*/
	void projectBounds(Array *_x = NULL);
	
	void lbfgs(const Array &g, const std::deque<Array> &s_set, const std::deque<Array> &y_set, const double Hdiag, const std::vector<int> &cols, const std::vector<int> &rows, Array &d);
	
	void lbfgsUpdate(const Array &y, const Array &s, int corrections, std::deque<Array> &old_dirs, std::deque<Array> &old_stps, double &Hdiag);
		
	double funObj(Array &x, Array &g);
	
	FuncObj func;
	
	Options *opt;	// Parameters
	Array LB;
	Array UB;	
	
	int nVars;		// Object variable's dim
	
	// following variables will be update continuously
	Array g;		// gradient
	Array x;		// Object variable
	Array H;		// Hessian matrix
	double f;		// output 

	std::vector<int> working;	// working set
	
	int funEvalMultiplier;
	int funEvals; 
	
	int dim;		// dim of input
	int nInstance;	// amount of instances;
	double *input;	// X
	double *output;	// y
};
#endif
