/*
	\file: array.cpp
	\author: Javier Zhang
	\date: 3-20-2014
*/

#include "array.h"

Array::Array()
{
	data = NULL;
	cols = 0;
	rows = 0;
	size = 0;
}

Array::Array(const Array &arr)
{
	data = arr.getData();
	cols = arr.cols;
	rows = arr.rows;
	size = arr.size;
}

Array::Array(double *_data, int _rows, int _cols)
{
	data = _data;
	cols = _cols;
	rows = _rows;
	size = cols * rows;
}

Array::Array(double *_data, int _size)
{
	data = _data;
	cols = _size;
	rows = 1;
	size = _size;
}

// Generate a 2D array.
Array::Array(int _rows, int _cols)
{
	cols = _cols;
	rows = _rows;
	size = cols * rows;
	
	data = new double[cols * rows];
}	

// Generate a vector.
Array::Array(int _size)
{
	cols = _size;
	rows = 1;
	size = _size;
	
	data = new double[size];
}	

// Set the array elements to be 0.
void Array::setZeros()	
{
	assert(data);
	memset(data, 0, sizeof(double) * size);
}

// Set the array elements to be 1.
void Array::setOnes()
{
	for(int i = 0;i < size;i++)
		data[i] = 1.;
}
	
Array::~Array()
{

}

// Create an array, new heap.
void Array::create(int _size)
{
	assert(!data);
	cols = _size;
	rows = 1;
	size = _size;
	
	data = new double[size];
}

// Create an array, new heap.
void Array::create(int _rows, int _cols)
{
	assert(!data);
	cols = _cols;
	rows = _rows;
	size = _cols * rows;
	
	data = new double[size];
}

// Copy data to another objects.
void Array::copyTo(Array dst)
{
	assert(dst.data && data && size == dst.size);
	double *_dst = dst.getData();
	for(int i = 0;i < size;++i)
		_dst[i] = data[i];
}

// Release memory.
void Array::release(void)
{
	if(data)
	{
		delete [] data;
		data = NULL;
	}
	cols = 0;
	rows = 0;
	size = 0;
}

// L2-Norm with indices.
double norm_L2(const Array &src, std::vector<int> &idx)
{
	assert(idx.size() <= static_cast<unsigned>(src.size));
	
	double ans = 0;
	for(std::vector<int>::iterator iter = idx.begin();iter != idx.end();++iter)
		ans += src.getData()[*iter] * src.getData()[*iter];
		
	return sqrt(ans);
}

// L2-Norm.
double norm_L2(const Array &src)
{
	double ans = 0;
	for(int i = 0;i < src.size;++i)
		ans += src.getData()[i] * src.getData()[i];
		
	return sqrt(ans);
}
 
// Array(vector) product.
double arrayMul(const Array &a, const Array &b)
{
	assert(a.size == b.size);
	
	double ans = 0;
	for(int i = 0;i < a.size;i++)
		ans += a.getData()[i] * b.getData()[i];
	
	return ans;
}

// Array(vector) subtract.
Array arraySub(const Array &a, const Array &b)
{
	assert(a.size == b.size);
	
	Array c(a.size);
	double *_c = c.getData();
	const double *_a = a.getData();
	const double *_b = b.getData();
	
	for(int i = 0;i < a.size;i++)
		_c[i] = _a[i] - _b[i];
	
	return c;
}
