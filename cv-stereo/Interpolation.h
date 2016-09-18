#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__
#include<stdio.h>
#include<math.h>
#define n_array 32
struct Interpolation{
private:
	double x[n_array],y[n_array];
public:
	double result(double var);
};

#endif
