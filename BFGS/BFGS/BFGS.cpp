// BFGS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <iostream>
#include <vector>
#include <functional>
#include <iostream>
#define N 2

double h = 1e-8;

double z(double v)
{
	return (-(1. / ((v - 1.)*(v - 1.) + 0.2)) - (1. / (2. * (v - 2.)*(v - 2.) + 0.15)) - (1. / (3. * (v - 3.)*(v - 3.) + 0.3)));
}


double f1(double* x)
{
	/*return x[0]*x[0];*/
	return z(x[0]);
}

double f2(double* x)
{
	return z(x[1]);
	/*return x[1] * x[1];*/
}

double f(double *x)
{
	return -f1(x)*f2(x);
	/*return x[0] * x[0] - x[0] * x[1] + x[1] * x[1] + 9 * x[0] - 6 * x[1] + 20;*/
	//return z(x[0]) + z(x[1]);
	/*return 2 * x1*x1 + x1*x2 + x2*x2;*/
	/*double VHML_Result;
	double z1 = -(1. / ((x - 1.)*(x - 1.) + 0.2)) - (1. / (2.*(x - 2.)*(x - 2.) + 0.15)) - (1. / (3.*(x - 3.)*(x
		- 3.) + 0.3));
	double z2 = -(1. / ((y - 1.)*(y - 1.) + 0.2)) - (1. / (2.*(y - 2.)*(y - 2.) + 0.15)) - (1. / (3.*(y - 3.)*(y
		- 3.) + 0.3));
	VHML_Result = z1 + z2;
	return VHML_Result;*/
	/*return (x1 - 5)*(x1 - 5)*(x2 - 4)*(x2 - 4) + (x1 - 5)*(x1 - 5) + (x2 - 4)*(x2 - 4) + 1;*/
}

double dfdx1(double* x)
{
	double y[2];
	y[0] = x[0] + h;
	y[1] = x[1];
	return (f(y) - f(x)) / h;
}

double dfdx2(double *x)
{
	/*return 2 * (x[1] - 4) * ((x[0] - 5) * (x[0] - 5) + 1);*/
	double y[2];
	y[0] = x[0];
	y[1] = x[1] + h;
	return (f(y) - f(x)) / h;
	//return (6.*(-3. + x[1]) / ((0.3 + 3.*(-3. + x[1])*(-3. + x[1]))*(0.3 + 3.*(-3. + x[1])*(-3. + x[1])))) + (4.*(-2. + x[1]) / ((0.15 + 2.*(-2. + x[1])*(-2. + x[1]))*(0.15 + 4.*(-2. + x[1])*(-2. + x[1])))) + (2.*(-1. + x[1]) / ((0.2 + (-1. + x[1])*(-1. + x[1]))*(0.2 + (-1. + x[1])*(-1. + x[1]))));
}

void multiplyVector(double** matrix, double* vector, double* solution)
{
	
	for (int i = 0; i < N; i++)
	{
		solution[i] = 0;
		for (int j = 0; j < N; j++)
			solution[i] += matrix[i][j] * vector[j];
	}
}

void multiplyMatrix(double** A, double** B, double** C)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			C[i][j] = 0;
			for (int k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
}



void outerProduct(double* vector1, double* vector2, double** matrix)
{
	matrix[0][0] = vector1[0] * vector2[0];
	matrix[0][1] = vector1[0] * vector2[1];
	matrix[1][0] = vector1[1] * vector2[0];
	matrix[1][1] = vector1[1] * vector2[1];
}

double search(double *x0, double *p)
{
	
	double a = 0.1;
	double temp = 0;
	double *x = new double[N];
	double *xnew = new double[N];
	for (int i = 0; i < N; i++)
	{
		x[i] = x0[i];
		xnew[i] = x[i];
	}

	
	for (int i = 0; i < N; i++)
	{
		xnew[i] = x[i] + a * p[i];
	}
	while (dfdx1(xnew)*dfdx1(xnew) + dfdx2(xnew)*dfdx2(xnew) > 1e-4)
	{
		
		for (int i = 0; i < N; i++)
		{
			x[i] = xnew[i];
			xnew[i] = x[i] + a * p[i];
		}
		std::cout << f(xnew) << std::endl;
	}
	
	a = (x[0] - x0[0]) / p[0];

	return a;
}


double scalarProduct(double* vector1, double* vector2)
{
	double scalar = 0.;
	for (int i = 0; i < N; i++)
		scalar += vector1[i] * vector2[i];
	return scalar;
}

double wolfe(double *x, double *p, double max_iter)
{
	double alpha = 0, beta = 1000, step = 0.1, c1 = 0.001, c2 = 0.9;
	double leftf, rightf;
	int i = 0;

	double *valf = new double[N];
	double *grad = new double[N];
	double *gradf = new double[N];

	grad[0] = dfdx1(x);
	grad[1] = dfdx2(x);

	while (i <= max_iter)
	{
		
		valf[0] = x[0] + step * c1*scalarProduct(grad, p);
		valf[1] = x[1] + step * c1*scalarProduct(grad, p);
		rightf = f(valf);
		valf[0] = x[0] + step * p[0];
		valf[1] = x[1] + step * p[1];
		leftf = f(valf);
		gradf[0] = dfdx1(valf);
		gradf[1] = dfdx2(valf);

		if (leftf > rightf)
		{
			beta = step;
			step = .5*(alpha + beta);

		}
		else if (scalarProduct(gradf, p) <= c2 * scalarProduct(p, grad))
		{
			alpha = step;
			if (beta > 100)
				step = 2 * alpha;
			else
				step = 0.5*(alpha + beta);
		}
		else
			break;

		i += 1;

	}

	return step;
}

int main()
{
	double a = 0, ro = 0, k = 0;

	double *x = new double[N];
	double *xnew = new double[N];
	double *s = new double[N];
	double *p = new double[N];
	double *grad = new double[N];
	double *gradnew = new double[N];
	double *y = new double[N];

	double **I = new double *[N];
	double **H = new double *[N];
	double **A = new double *[N];
	double **B = new double *[N];
	double **C = new double *[N];
	double **D = new double *[N];
	double **E = new double *[N];

	for (int i = 0; i < N; i++)
	{
		I[i] = new double[N];
		H[i] = new double[N];
		A[i] = new double[N];
		B[i] = new double[N];
		C[i] = new double[N];
		D[i] = new double[N];
		E[i] = new double[N];

		x[i] = 0;
		xnew[i] = 0;
		s[i] = 0;
		p[i] = 0;
		y[i] = 0;
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			I[i][j] = 0;
			H[i][j] = 0;
			A[i][j] = 0;
			B[i][j] = 0;
			C[i][j] = 0;
			D[i][j] = 0;
			E[i][j] = 0;
		}

		I[i][i] = 1;
		H[i][i] = I[i][i];
	}

	x[0] = 1.7;
	x[1] = 1.7;

	grad[0] = dfdx1(x);
	grad[1] = dfdx2(x);
	while (sqrt(grad[0] * grad[0] + grad[1] * grad[1]) > 1e-5)
	{
		k++;
		multiplyVector(H, grad, p);
		for (int i = 0; i < N; i++)
			p[i] = -p[i];
		a = wolfe(x, p, 100);
		for (int i = 0; i < N; i++)
		{
			xnew[i] = x[i] + a * p[i];
			s[i] = xnew[i] - x[i];
		}
		gradnew[0] = dfdx1(xnew);
		gradnew[1] = dfdx2(xnew);
		for (int i = 0; i < N; i++)
			y[i] = gradnew[i] - grad[i];
		ro = scalarProduct(y, s);
		ro = 1. / ro;
		outerProduct(s, y, A);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				A[i][j] = A[i][j] * ro;
				A[i][j] = I[i][j] - A[i][j];
			}
		multiplyMatrix(A, H, B);
		outerProduct(y, s, C);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				C[i][j] = C[i][j] * ro;
				C[i][j] = I[i][j] - C[i][j];
			}
		multiplyMatrix(A, C, D);
		outerProduct(s, s, E);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				E[i][j] = E[i][j] * ro;
				H[i][j] = D[i][j] + E[i][j];
			}
		for (int i = 0; i < N; i++)
			x[i] = xnew[i];
		grad[0] = dfdx1(x);
		grad[1] = dfdx2(x);
	}
	std::cout << x[0] << " " << x[1] << " " << f(x) << std::endl;
	
	std::cout << k << std::endl;
	system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
