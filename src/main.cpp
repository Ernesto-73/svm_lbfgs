/*
	\file: main.cpp
	\author: Javier Zhang
	\date: 6-4-2014
*/ 
#include <svm.h>
#include <fstream>
/*
* a test for svm with lbfgs.
*/
int main()
{
	int nVars = 2;
	int nInstances = 250;
	int len = nVars * nInstances;
	
	double *X = new double[len];
	double *y = new double[nInstances];
	
	// reading data
	std::cout<<"Reading data..."<<std::endl;
	std::fstream fs;
	
	fs.open("./test_data/SVM_X", std::fstream::in);
	if(!fs)
		std::cout<<"File does not exist."<<std::endl;
		
	for(int i = 0;i < len;i++)
 		fs>>X[i];
	fs.close();
	
	fs.open("./test_data/SVM_Y", std::fstream::in);
	if(!fs)
		std::cout<<"File does not exist."<<std::endl;
		
	for(int i = 0;i < nInstances;i++)
		fs>>y[i];
	fs.close();
	
	// split the data into train and test set.
	int nTest = 0.4 * nInstances;
	int nTrain = nInstances - nTest;
	
	double *XTrain = new double [nTrain * nVars];
	double *XTest = new double [nTest * nVars];
	double *yTrain = new double [nTrain];
	double *yTest = new double [nTest];
	
	for(int i = 0;i < nTrain;i++)
	{
		XTrain[2 * i] = X[2 * i];
		XTrain[2 * i + 1] = X[2 * i + 1];
		yTrain[i] = y[i];
	}
	
	for(int i = 0;i < nTest;i++)
	{
		XTest[2 * i] = X[2 * (i + nTrain)];
		XTest[2 * i + 1] = X[2 * (i + nTrain) + 1];
		yTest[i] = y[i + nTrain];
	}
	std::cout<<std::endl;
	
	// train
	SVMOpt opt(SVM_RBF, 100., 1.);
	SVM svm(XTrain, yTrain, nVars, nTrain, &opt);
	std::cout<<"Train Error:"<<svm.train()<<std::endl;
	
	// predict
	double *ans = svm.predict(XTest, nVars, nTest);
	
	double err = 0;
	for(int i = 0;i < nTest;i++)
		err += (ans[i] != yTest[i]);
	
	err = err / nTest;
	std::cout<<"Generalized Error:"<<err<<std::endl;
	
	// release memory
	delete[] ans;
	ans = NULL;
	
	delete[] XTest;
	XTest = NULL;
	
	delete[] XTrain;
	XTrain = NULL;

	delete[] yTest;
	yTest = NULL;
	
	delete[] yTrain;
	yTrain = NULL;
	
	delete[] X;
	X = NULL;
	
	delete[] y;
	y = NULL;
	
	return 1;
}
