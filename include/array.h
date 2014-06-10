/*
	\file: array.h
	\author: Javier Zhang
	\date: 3-20-2014
*/ 

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <cassert>
#include <cstring>
#include <cmath>
#include <vector>

/*
* \define class Array: data structure
*/
class Array{
public:
	/* Constructors */
	Array();
	
	Array(double *_data, int _rows, int _cols);
	
	Array(double *_data, int _size);
	
	Array(int _rows, int _cols);
	
	Array(int _size);
	
	/* Copy constructor. */
	Array(const Array &arr);
	
	/* Destructor */
	~Array();
	
	void setZeros();
	
	void setOnes();
	
	void copyTo(Array dst);
	
	void create(int _size);
	
	void create(int _rows, int _cols);
	
	void release(void);

	/* Getters && Setters*/
	double *getData() {	return data; }
	
	double *getData() const {	return data; }
	
	int cols;
	int rows;
	int size;
	
private:
	double *data;
};

double norm_L2(const Array &src, std::vector<int> &idx);

double norm_L2(const Array &src);

double arrayMul(const Array &a, const Array &b);

Array arraySub(const Array &a, const Array &b);

#endif
