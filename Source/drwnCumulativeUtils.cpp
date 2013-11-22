/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCumulativeUtils.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

namespace drwnCumulativeUtils
{
	//! Random number generator;
	boost::mt19937 rng((unsigned int) std::time(0));

	//! Uniform distribution on [0, 1)
	boost::uniform_real<> unif(0, 1.0);
	//boost::uniform_01<> unif2();

	//! Random number generator
	boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(rng, unif);

	// DEBUG: Flag for breakpoint
	bool stopYet = false, stopYet2 = false;

	// DEBUG: Files to output values during sampling
	//wofstream firstVar, secondVar, kS, firstTerms, secondTerms, denom, densities, parameters;
};

// Log-sum-exp function, for performing addition of 2 numbers in log-space
double drwnCumulativeUtils::logSumExp(const double& x, const double& y)
{
	if (x == -std::numeric_limits<double>::infinity() && y == -std::numeric_limits<double>::infinity())
		return -std::numeric_limits<double>::infinity();
	else
		return max(x, y) + boost::math::log1p(exp(-abs(x - y)));
}

// Log-sum-exp function, for performing addition of n numbers in log-space
// TODO: Version for Eigen vector
double drwnCumulativeUtils::logSumExp(const double* x, const unsigned int n)
{
	// Get maximum index
	unsigned int whereMax = 0;
	double xMax = -std::numeric_limits<double>::infinity();
	for (unsigned int i = 0; i < n; i++) {
		if (x[i] > xMax) {
			whereMax = i;
			xMax = x[i];
		}
	}

	// Calculate argument to log1p
	double arg = 0;
	for (unsigned int i = 0; i < n; i++) {
		// No contribution from maximum element, since this is 1
		if (i == whereMax)
			continue;

		// Otherwise, add in term
		arg += exp(x[i] - x[whereMax]);
	}

	return x[whereMax] + boost::math::log1p(arg);
}