/*
	\file: svm.h
	\author: Javier Zhang
	\date: 6-4-2014
*/ 
#ifndef _SVM_H_
#define _SVM_H_

#include <cmath>
#include "minconf_tmp.h"

inline double sign(double x) { return x > 0 ? 1 : (x < 0 ? -1 : 0 ); }

enum{SVM_RBF = 0, SVM_LINEAR};

typedef struct SVMOpt{

	SVMOpt(int _kernel, int _C, int _sigma)
	{
		assert(_kernel == SVM_RBF);
		kernel = _kernel;
		C = _C;
		sigma = _sigma;
	}
	
	SVMOpt(int _kernel)
	{
		assert(_kernel == SVM_LINEAR);
	}
	
	int kernel;
	int C;
	int sigma;
	Options minconf_opt;
}SVMOpt;

/*
* Solve SVM dual problems with no bias.
* -> min_alpha (1/2)alpha'*A*alpha - sum(alpha), s.t. 0 <= alpha <= C, where A_ij = y_iy_jK(x_i,x_j), and we use the RBF kernel with sigma = 1.
*/
class SVM
{
public:
	SVM();
	SVM(double *_input, double *_response, int _nVars, int _nInstances, SVMOpt *_opt);
	double train();
	double *predict(double *_input, int _nVars, int _nInstances);
	~SVM();
	
private:
	int nVars;
	int nInstances;
	double *input;
	double *response;
	SVMOpt *opt;
	double trainErr;
	double *alpha;
	
	double getTrainErr(Array &alpha, Array &y, Array &K);
	Array kernelRBF(Array &X1, Array &X2, double sigma);
	Array kernelLinear(Array &X1, Array &X2);
};

double dualSVMLoss_noBias(int n, const double *alpha, const double *A, double *g);

#endif