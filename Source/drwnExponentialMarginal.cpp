/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnExponentialMarginal.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnExponentialMarginal.h"

// Default construct. Initializes to standard normal.
drwnExponentialMarginal::drwnExponentialMarginal()
{
	initialize();
}

// Copy constructor
drwnExponentialMarginal::drwnExponentialMarginal(const drwnExponentialMarginal& m)
{
	// TODO
}

// Default destructor
drwnExponentialMarginal::~drwnExponentialMarginal()
{
	// Do nothing
}

// Initilize to standard normal
void drwnExponentialMarginal::initialize()
{
	_lambda = 1;
}

// Evaluate density
drwnExponentialMarginal::range_type drwnExponentialMarginal::density(domain_type x) const
{
	if (x >= 0)
		return _lambda * exp(-_lambda * x);

	else
		return 0;
}

// Evaluate cumulative distribution function
drwnExponentialMarginal::range_type drwnExponentialMarginal::cdf(domain_type x) const
{
	if (x >= 0)
		return 1 - exp(-_lambda * x);

	else
		return 0;
}

// Evaluate quantile (inverse CDF)
drwnExponentialMarginal::domain_type drwnExponentialMarginal::quantile(range_type p) const
{
	return -log(1 - p) / _lambda;
}