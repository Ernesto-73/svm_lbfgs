/*
	\file: svm.cpp
	\author: Javier Zhang
	\date: 6-4-2014
*/ 
#include "svm.h"

double dualSVMLoss_noBias(int n, const double *alpha, const double *A, double *g)
{
	double f = 0;
	int step = n;
	
	double sum = 0;
	for(int i = 0; i < n;i++)
		sum += alpha[i];
		
	for(int i = 0 ;i < n ;i++)
	{
		double sc = 0;
		for(int j = 0;j < n;j++)
		{
			sc += alpha[j] * A[j * step + i];
		}
		g[i] = sc - 1;
		f += sc * alpha[i];
	}
	f = f * 0.5 - sum;
	return f;
}

SVM::SVM()
{
	std::cerr<<"Must have parameters."<<std::endl;
	exit(-1);
}

SVM::SVM(double *_input, double *_response, int _nVars, int _nInstances, SVMOpt *_opt)
{
	input = _input;
	response = _response;
	nVars = _nVars;
	nInstances = _nInstances;
	opt = _opt;
}

/* \define kernelRBF
* RBF kernel for support vector machine.
*/
Array SVM::kernelRBF(Array &X1, Array &X2, double sigma)
{
	int n1 = X1.rows;
	int n2 = X2.rows;
	int step = X1.cols;
	
	Array XX(n1, n2);
	
	double *_X1 = X1.getData();
	double *_X2 = X2.getData();
	double *_XX = XX.getData();

	double Z = 1 / sqrt(2 * M_PI * sigma * sigma);
	for(int i = 0;i < n1;i++)
	{
		for(int j = 0;j < n2;j++)
		{
			double dist = 0;
			for(int k = 0;k < step;k++)
				dist += ( _X1[i * step + k] - _X2[j * step + k] ) * ( _X1[i * step + k] - _X2[j * step + k] );
			
			_XX[i * n2 + j ] = Z * exp( -dist / (2 * sigma * sigma));
		}
	}
	return XX;
}

/* \define kernelLinear
* Linear kernel for support vector machine.
*/
Array SVM::kernelLinear(Array &X1, Array &X2)
{
	int n1 = X1.rows;
	int n2 = X2.rows;
	
	Array XX(n1, n2);
	
	double *_X1 = X1.getData();
	double *_X2 = X2.getData();
	double *_XX = XX.getData();

	int step = XX.cols;
	for(int i = 0;i < XX.rows;i++)
	{
		for(int j = 0;j < XX.cols;j++)
			_XX[i * step + j ] = _X1[i] * _X2[j];
	}
	return XX;
}

/* \define getTrainErr
* Calculate the train error of SVM.
*/
double SVM::getTrainErr(Array &alpha, Array &y, Array &K)
{
	double err = 0;
	Array Khat(K.rows, K.cols);
	
	double *_alpha = alpha.getData();
	double *_y = y.getData();
	double *_K = K.getData();
	double *_Khat = Khat.getData();
	
	int step = Khat.cols;
	for(int i = 0;i < Khat.rows;i++)
	{
		for(int j = 0;j < Khat.cols;j++)
			_Khat[i * step + j] = _K[i * step + j] * _y[i] * _alpha[i];
	}
	
	for(int j = 0;j < Khat.cols;j++)
	{
		double sum = 0;
		for(int i = 0;i < Khat.rows;i++)
			sum += _Khat[i * step +j];
			
		err += (sign(sum) != _y[j]);
	}
	Khat.release();
	return err / y.size;
}

double SVM::train()
{
	Array X(input, nInstances, nVars);
	Array y(response, nInstances);
	Array K;
	
	if(opt->kernel == SVM_RBF)
	{
		K = kernelRBF(X, X, opt->sigma);
	}
	else if(opt->kernel == SVM_LINEAR)
	{
		K = kernelLinear(X, X);
	}
	else{
		exit(-1);
	}
	
	// K must be a symmetric matrix
	assert(K.rows == K.cols);
	
	double *_K = K.getData();

	Array A(K.rows, K.cols);
	double *_A = A.getData();
	double *_y = y.getData();
	
	int step = A.cols;
	for(int i = 0;i < K.rows; i++)
	{
		for(int j = 0;j < K.cols;j++)
			_A[i * step + j] = _y[i] * _y[j] * _K[i * step + j];
	}	
	
	Array LB(nInstances);
	Array UB(nInstances);
	
	double *_LB = LB.getData();
	double *_UB = UB.getData();
	
	for(int i = 0;i < UB.size;i++)
		_UB[i] = opt->C;
		
	for(int i = 0;i < LB.size;i++)
		_LB[i] = 0;

	Array x(nInstances);
	x.setZeros();
	
#ifdef DEBUG
	opt->minconf_opt.verbose = 3;
#endif

	minConf_TMP trainer(x, LB, UB, &(opt->minconf_opt));
	trainer.loadData(nInstances, nInstances, _A, _y);
	trainer.setFunObj(&dualSVMLoss_noBias);
	trainer.process();
	
	trainErr = getTrainErr(x, y, K);
	
	alpha = x.getData();
	
	// Release memory.
	A.release();
	K.release();
	LB.release();
	UB.release();
	
	return trainErr;
}

SVM::~SVM()
{
	delete [] alpha;
	alpha = NULL;
	
#ifdef DEBUG
	std::cout<<"SVM Destroyed."<<std::endl;
#endif
}

double *SVM::predict(double *_input, int _nVars, int _nInstances)
{
	Array XTrain(input, nInstances, nVars);
	Array XPre(_input, _nInstances, _nVars);
	Array yTrain(response, nInstances);
	Array K;
	
	if(opt->kernel == SVM_RBF)
	{
		K = kernelRBF(XTrain, XPre, opt->sigma);
	}
	else if(opt->kernel == SVM_LINEAR)
	{
		K = kernelLinear(XTrain, XPre);
	}
	else {
		exit(-1);
	}
	
	double *_yTrain = yTrain.getData();
	double *_K = K.getData();
	
	int step = K.cols;
	for(int i = 0;i < K.rows;i++)
	{
		for(int j = 0;j < K.cols;j++)
			_K[i * step + j] = _K[i * step + j] * _yTrain[i] * alpha[i];
	}
	
	assert(K.cols == _nInstances);
	double *_yPre = new double[K.cols];
	for(int j = 0;j < K.cols;j++)
	{
		double sum = 0;
		for(int i = 0;i < K.rows;i++)
			sum += _K[i * step +j];
			
		_yPre[j]= sign(sum);
	}
	
	// release memory
	K.release();
	return _yPre;
}