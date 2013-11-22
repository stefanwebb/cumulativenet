/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnBernoulliMarginal.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnBernoulliMarginal.h"

// Default construct. Initializes to standard normal.
drwnBernoulliMarginal::drwnBernoulliMarginal()
{
	initialize();
}

// Copy constructor
drwnBernoulliMarginal::drwnBernoulliMarginal(const drwnBernoulliMarginal& m)
{
	// TODO
}

// Default destructor
drwnBernoulliMarginal::~drwnBernoulliMarginal()
{
	// Do nothing
}

// Initilize to standard normal
void drwnBernoulliMarginal::initialize()
{
	_theta = 0.5;
}

// Evaluate density
drwnBernoulliMarginal::range_type drwnBernoulliMarginal::density(domain_type x) const
{
	if (x == 0)
		return 1 - _theta;

	else
		return _theta;
}

// Evaluate cumulative distribution function
drwnBernoulliMarginal::range_type drwnBernoulliMarginal::cdf(domain_type x) const
{
	if (x < 1)
		return 1 - _theta;

	else
		return 1;
}

// Evaluate quantile (inverse CDF)
drwnBernoulliMarginal::domain_type drwnBernoulliMarginal::quantile(range_type x) const
{
	if (x < 1 - _theta)
		return 0;

	else
		return 1;
}