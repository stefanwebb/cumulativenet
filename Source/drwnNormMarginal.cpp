/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnNormMarginal.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnNormMarginal.h"

using boost::math::normal;

// Normal distribution
mynormal drwnNormMarginal::_norm;	// Standard normal distribution.

// Default construct. Initializes to standard normal.
drwnNormMarginal::drwnNormMarginal()
{
	initialize();
}

// Copy constructor
drwnNormMarginal::drwnNormMarginal(const drwnNormMarginal& m)
	: _mean(m._mean), _sd(m._sd)
{
	// Do nothing
}

// Default destructor
drwnNormMarginal::~drwnNormMarginal()
{
	// Do nothing
}

// Initilize to standard normal
void drwnNormMarginal::initialize()
{
	_mean = 0;
	_sd = 1;
}

// Evaluate density
drwnNormMarginal::range_type drwnNormMarginal::density(domain_type x) const
{
	return pdf(_norm, (x - _mean) / _sd);
}

// Evaluate cumulative distribution function
drwnNormMarginal::range_type drwnNormMarginal::cdf(domain_type x) const
{
	return boost::math::cdf(_norm, (x - _mean) / _sd);
}

// Evaluate quantile (inverse CDF)
drwnNormMarginal::domain_type drwnNormMarginal::quantile(range_type x) const
{
	return _mean + _sd * boost::math::quantile(_norm, x);
}