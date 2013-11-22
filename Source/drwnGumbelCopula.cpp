/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnGumbelCopula.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

// Initialize variables
void drwnGumbelCopula::initialize()
{
	_partialDerivs = 0;
	_numSubsets = 0;
	theta = 1;
}

// Create an empty copula
drwnGumbelCopula::drwnGumbelCopula()
{
	initialize();
}

// Create copula with given variables
drwnGumbelCopula::drwnGumbelCopula(const drwnClique& c)
{
	// Need this in here, so doesn't call setVariables from parent class
	initialize();
	setVariables(c);
}

// Default destructor
drwnGumbelCopula::~drwnGumbelCopula()
{
	if (_partialDerivs)
		delete [] _partialDerivs;
}

// Add multiple variables
void drwnGumbelCopula::setVariables(const drwnClique& c)
{
	// Must have size 2, 3, or 4 (for now)
	DRWN_ASSERT_MSG(c.size() >= 2 && c.size() <= 4, "Archimedian copula must have size between 2 and 4")

	// Call set variables from parent
	drwnCdf::setVariables(c);
	
	// Calculate number of subsets of variables
	_numSubsets = 1 << size();
	_partialDerivs = new range_type[_numSubsets << 1];
}

//! Calculate generator or n-th derivative
drwnGumbelCopula::range_type drwnGumbelCopula::generator(range_type t, unsigned int n)
{
	DRWN_ASSERT_MSG(n <= 4, "Archimedian generator derivatives higher than 4 not implemented!")
	if (n == 0)
		return exp(-pow(t,1.0/theta));
	else if (n == 1)
		return -(pow(t,1.0/theta-1.0)*exp(-pow(t,1.0/theta)))/theta;
	else if (n == 2) 
		return pow(t,2.0/theta-2.0)*1.0/(theta*theta)*exp(-pow(t,1.0/theta))-(pow(t,1.0/theta-2.0)*exp(-pow(t,1.0/theta))*(1.0/theta-1.0))/theta;
	else if (n == 3)
		return -pow(t,1.0/theta-1.0)*pow(t,2.0/theta-2.0)*1.0/(theta*theta*theta)*exp(-pow(t,1.0/theta))+pow(t,2.0/theta-3.0)*1.0/(theta*theta)*exp(-pow(t,1.0/theta))*(2.0/theta-2.0)-(pow(t,1.0/theta-3.0)*exp(-pow(t,1.0/theta))*(1.0/theta-1.0)*(1.0/theta-2.0))/theta+pow(t,1.0/theta-1.0)*pow(t,1.0/theta-2.0)*1.0/(theta*theta)*exp(-pow(t,1.0/theta))*(1.0/theta-1.0);
	else if (n == 4)
		return pow(t,4.0/theta-4.0)*1.0/(theta*theta*theta*theta)*exp(-pow(t,1.0/theta))+pow(t,2.0/theta-4.0)*1.0/(theta*theta)*exp(-pow(t,1.0/theta))*pow(1.0/theta-1.0,2.0)+pow(t,2.0/theta-4.0)*1.0/(theta*theta)*exp(-pow(t,1.0/theta))*(2.0/theta-2.0)*(2.0/theta-3.0)-pow(t,1.0/theta-2.0)*pow(t,2.0/theta-2.0)*1.0/(theta*theta*theta)*exp(-pow(t,1.0/theta))*(1.0/theta-1.0)*2.0-pow(t,1.0/theta-1.0)*pow(t,2.0/theta-3.0)*1.0/(theta*theta*theta)*exp(-pow(t,1.0/theta))*(2.0/theta-2.0)*2.0+pow(t,1.0/theta-1.0)*pow(t,1.0/theta-3.0)*1.0/(theta*theta)*exp(-pow(t,1.0/theta))*(1.0/theta-1.0)*(1.0/theta-2.0)*2.0-(pow(t,1.0/theta-4.0)*exp(-pow(t,1.0/theta))*(1.0/theta-1.0)*(1.0/theta-2.0)*(1.0/theta-3.0))/theta;

	return 1.0;
}

//! Calculate inverse generator or n-th derivative
drwnGumbelCopula::range_type drwnGumbelCopula::inverseGenerator(range_type t)
{
	//if (!isDeriv)
		return pow(-log(t),theta);
	//else
	//	return -(theta*pow(-log(t),theta-1.0))/t;
}

// Evaluate copula CDF of sample
drwnGumbelCopula::range_type drwnGumbelCopula::evaluateCdf(domain_type* sample)
{
	// Add inverse generators
	double sumInvGen = 0.0;
	for (unsigned int i = 0; i < size(); i++) {
		if (sample[i] != 1.0)
			sumInvGen += inverseGenerator(sample[i]);
	}

	// Return generator of that value
	return generator(sumInvGen, 0);
}

// Calculate a partial derivative of copula, evaluated at sample
drwnGumbelCopula::range_type drwnGumbelCopula::partialDerivative(domain_type* sample, drwnSubset& isDiffed)
{
	// Factor that is composed of derivatives of inv generators, and sum of inv generators
	double derivFactor = 1.0, sumInvGen = 0.0;
	unsigned int margedVars = 0, lastNonMarged;
	for (unsigned int i = 0, currentBit = 1; i < size(); i++, currentBit <<= 1) {
		// Check if zero
		if (!sample[_variables[i]])
			return 0.0;

		// Only do variables that are not marginalized
		else if (sample[_variables[i]] != 1.0) {
			lastNonMarged = _variables[i];
			
			double valInvGen = inverseGenerator(sample[_variables[i]]);
			sumInvGen += valInvGen;

			// If this variable is differentiated, then have to multiply into front factor
			if (currentBit & isDiffed._subset)
				derivFactor *= generator(valInvGen, 1);
		}

		else {
			// If this variable is differentiated, then return 0
			if (currentBit & isDiffed._subset)
				return 0.0;

			margedVars++;
		}
	}

	// If we've marginalized all variables then return one
	if (margedVars == size())
		return 1.0;

	// If we've marginalized all variables except for one, then this will be identity or unity, depending on whether we are diffing variable
	else if (margedVars == (size() - 1)) {
		// If the only variable is differentiated, then return 1.0
		if (derivFactor != 1.0)
			return 1.0;

		// Otherwise, we will have unity, so return variable
		else
			return sample[lastNonMarged];
	}


	double result = generator(sumInvGen, drwnSubset::countElements(isDiffed)) / derivFactor;

	if (boost::math::isnan(result)) {
		// TODO: Does this really work? I think I need a more sophisticated test here! May need to return 0.0 in some cases
		//if (drwnSubset::countElements(isDiffed) >= size() - margedVars)
		//	return 1.0;
		//else

		//double u1 = sample[0], u2 = sample[1];

		return 0.0;
	}
	
	else
		return result;
}

// Calculate and cache all partial derivatives evaluated at sample. Optionally calculate gradients as well
void drwnGumbelCopula::cacheDerivatives(domain_type* sample, bool gradientAlso)
{
	// Loop through all subsets of variables and fill in partial derivatives
	drwnSubset currentSubset = 0;
	do {
		_partialDerivs[currentSubset._subset] = partialDerivative(sample, currentSubset);

		// DEBUG
		//gotHere = false;
	} while (!drwnSubset::isEmptyset(currentSubset = drwnSubset::nextSubset(currentSubset, drwnSubset(_numSubsets - 1))));	
}