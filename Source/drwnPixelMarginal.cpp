/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnPixelMarginal.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnPixelMarginal.h"

using boost::math::normal;

// Normal distribution
mynormal drwnPixelMarginal::_norm;	// Standard normal distribution.

// Default construct. Initializes to standard normal.
drwnPixelMarginal::drwnPixelMarginal()
{
	initialize();
}

// Copy constructor
drwnPixelMarginal::drwnPixelMarginal(const drwnPixelMarginal& m)
	: _mean(m._mean), _sd(m._sd)
{
	// Do nothing
}

// Default destructor
drwnPixelMarginal::~drwnPixelMarginal()
{
	// Do nothing
}

// Initilize to standard normal
void drwnPixelMarginal::initialize()
{
	_mean = 0;
	_sd = 1;
}

// Evaluate density
drwnPixelMarginal::range_type drwnPixelMarginal::density(domain_type x) const
{
	return pdf(_norm, (x - _mean) / _sd);
}

// Evaluate cumulative distribution function
drwnPixelMarginal::range_type drwnPixelMarginal::cdf(domain_type x) const
{
	return boost::math::cdf(_norm, (x - _mean) / _sd);
}

// Evaluate quantile (inverse CDF)
drwnPixelMarginal::domain_type drwnPixelMarginal::quantile(range_type x) const
{
	return _mean + _sd * boost::math::quantile(_norm, x);
}