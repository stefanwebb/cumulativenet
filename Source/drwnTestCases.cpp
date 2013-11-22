/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnTestCases.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnTestCases.h"

//! Output some values of normal copula, its subsets and gradients
// NOTE: Use this one to test formula against numerical derivatives calculated in R
void drwnTestCases::evalNormalCopula()
{
	/*// Create bivariate Normal copula
	drwnNormCopula copula;
	drwnClique c;
	c.insert(0);
	c.insert(1);
	copula.setVariables(c);

	// Set parameter and transform sample
	copula.setParameter(0.5);
	double x[2] = {0.2, 0.6};
	for (unsigned int i = 0; i < 2; i++)
		x[i] = copula.transformArgument(x[i]);*/

	// Create bivariate Normal copula
	drwnNormCopula copula, copula2;
	drwnClique c;
	c.insert(0);
	c.insert(1);
	//c.insert(2);
	copula2.setVariables(c);

	c.clear();
	c.insert(1);
	c.insert(0);
	c.insert(2);
	c.insert(3);
	copula.setVariables(c);

	// Set parameter and transform sample
	copula.setParameter(0.5);
	copula2.setParameter(0.5);
	double x[4] = {1.0, 0.2, 0.6, 1.0};
	//double x[3] = {0.2, 0.6, 1.0};
	for (unsigned int i = 0; i < copula.size(); i++)
		x[i] = copula.transformArgument(x[i]);

	//double x2[3] = {0.2, 0.6, 0.1};
	double x2[2] = {1.0, 1.0};
	for (unsigned int i = 0; i < copula2.size(); i++)
		x2[i] = copula.transformArgument(x2[i]);

	// Output cached values
	copula2.cacheDerivatives(x2, true, false, true);
	wcout << endl;
	copula.cacheDerivatives(x, true, false, true);

	//wcout << "Three dimensional copula" << endl;
	//wcout << copula2.partialDerivative(x2, drwnSubset(1), false) << endl << endl;

	//wcout << "Four dimensional copula, with one dimension marginalized" << endl;
	wcout << copula2.partialDerivative(x2, drwnSubset(0), false);
}

//! Example testing numerical differentiation
void drwnTestCases::testNumericalDerivative()
{
	class DerivSin : public drwnNumericalDerivative
	{
	public:
		DerivSin() : drwnNumericalDerivative(0.01) { }
		~DerivSin() { }

		double function(const double& x) const
		{
			return sin(x);
		}
	};

	DerivSin dSin;
	wcout << dSin(2) - cos(2) << endl;
}

//! Output some values of Clayton copula, its subsets and gradients
void drwnTestCases::evalClaytonCopula()
{
	// Create bivariate Normal copula
	drwnClaytonCopula copula;
	drwnClique c;
	c.insert(0);
	c.insert(1);
	c.insert(2);
	c.insert(3);
	copula.setVariables(c);

	drwnClaytonCopula copula2;
	c.clear();
	c.insert(0);
	c.insert(1);
	//c.insert(2);
	copula2.setVariables(c);

	// Set parameter and transform sample
	copula.setParameter(5);
	copula2.setParameter(5);
	double x[4] = {1.0, 0.2, 1.0, 1.0};
	//double x2[3] = {0.2, 0.5, 0.7};
	double x2[2] = {1.0, 1.0};
	
	// Output cached values
	copula.cacheDerivatives(x, true, false, true);
	wcout << endl;
	copula2.cacheDerivatives(x2, true, false, true);
}

// Code that generates samples for figure that shows stable sampling
void drwnTestCases::stableSampleFigure()
{
	drwnTestCases::sampleCopula<drwnClaytonCopula>(2, 300, L"samplesClayton300.txt");
	drwnTestCases::sampleCopula<drwnClaytonCopula>(2, 1000, L"samplesClayton1000.txt");
	drwnTestCases::sampleCopula<drwnClaytonCopula>(2, 10000, L"samplesClayton10000.txt");

	//drwnTestCases::sampleCopula<drwnClaytonCopula>(2, 100000000, L"samplesClayton.txt");
	//drwnTestCases::sampleCopula<drwnFrankCopula>(2, 50, L"samplesFrank.txt");
}
