/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnPolyCopula.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

// Calculate a partial derivative of copula, evalued at parameters
drwnPolyCopula::range_type drwnPolyCopula::partialDerivative(domain_type* sample, drwnSubset& isDiffed) const
{	
	// Calculate a polynomial function for debug purposes
	range_type result = 1;

	// Loop over variables in factor
	for (unsigned int i = 0; i < _variables.size(); i++) {
		// We are not differentiating with respect to the variable, then it must be multiplied into result
		if (!(isDiffed._subset & (1 << i))) {
			result *= sample[_variables[i]];
		}
	}

	return result;
}