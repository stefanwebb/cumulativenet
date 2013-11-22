/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnClaytonCopula.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

// Initialize variables
void drwnClaytonCopula::initialize()
{
	_partialDerivs = 0;
	_numSubsets = 0;
	theta = 1;
}

// Create an empty copula
drwnClaytonCopula::drwnClaytonCopula()
{
	initialize();
}

// Create copula with given variables
drwnClaytonCopula::drwnClaytonCopula(const drwnClique& c)
{
	// Need this in here, so doesn't call setVariables from parent class
	initialize();
	setVariables(c);
}

// Default destructor
drwnClaytonCopula::~drwnClaytonCopula()
{
	if (_partialDerivs)
		delete [] _partialDerivs;
}

// Add multiple variables
void drwnClaytonCopula::setVariables(const drwnClique& c)
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
drwnClaytonCopula::range_type drwnClaytonCopula::generator(range_type t, unsigned int n)
{
	DRWN_ASSERT_MSG(n <= 4, "Archimedian generator derivatives higher than 4 not implemented!")
	if (n == 0)
		return pow(t*theta+1.0,-1.0/theta);
	else if (n == 1)
		return -pow(t*theta+1.0,-1.0/theta-1.0);
	else if (n == 2) 
		return theta*(1.0/theta+1.0)*pow(t*theta+1.0,-1.0/theta-2.0);
	else if (n == 3)
		return -(theta*theta)*(1.0/theta+1.0)*(1.0/theta+2.0)*pow(t*theta+1.0,-1.0/theta-3.0);
	else if (n == 4)
		return (theta*theta*theta)*(1.0/theta+1.0)*(1.0/theta+2.0)*(1.0/theta+3.0)*pow(t*theta+1.0,-1.0/theta-4.0);
	
	return 1.0;
}

//! Calculate inverse generator or n-th derivative
drwnClaytonCopula::range_type drwnClaytonCopula::inverseGenerator(range_type t)
{
	//if (!isDeriv)
		return (pow(t,-theta)-1.0)/theta;
	//else
	//	return -(theta*pow(-log(t),theta-1.0))/t;
}

// Evaluate copula CDF of sample
drwnClaytonCopula::range_type drwnClaytonCopula::evaluateCdf(domain_type* sample, bool takeLog)
{
	/*// Add inverse generators
	double sumInvGen = 0.0;
	for (unsigned int i = 0; i < size(); i++) {
		if (sample[i] != 1.0)
			sumInvGen += inverseGenerator(sample[i]);
	}

	// Return generator of that value
	return generator(sumInvGen, 0);*/

	// NOTE: Think I fixed a bug here...
	return partialDerivative(sample, drwnSubset(0), takeLog);
}

// Calculate a partial derivative of copula, evaluated at sample
drwnClaytonCopula::range_type drwnClaytonCopula::partialDerivative(domain_type* sample, drwnSubset& isDiffed, bool takeLog, drwnClaytonCopula::range_type& grad)
{
	// Factor that is composed of derivatives of inv generators, and sum of inv generators
	double numerator = 0.0, denominator = 0.0, minU = std::numeric_limits<double>::infinity();
	unsigned int whereMin = 0;
	unsigned int margedVars = 0, lastNonMarged, numDiffed = 0;
	for (unsigned int i = 0, currentBit = 1; i < size(); i++, currentBit <<= 1) {
		// Check if zero
		if (!sample[_variables[i]]) {
			if (!takeLog) {
				if (&grad)
					grad = 0;
				return 0.0;
			}
			else {
				if (&grad)
					grad = -std::numeric_limits<double>::infinity();
				return -std::numeric_limits<double>::infinity();
			}
		}

		// Only do variables that are not marginalized
		else if (sample[_variables[i]] != 1.0) {
			lastNonMarged = _variables[i];
						
			//numerator += pow(sample[_variables[i]], -theta);
			//numerator += exp(-theta * log(sample[_variables[i]]));
			if (sample[_variables[i]] < minU) {
				minU = sample[_variables[i]];
				whereMin = i;
			}

			// If this variable is differentiated, then have to multiply into front factor
			if (currentBit & isDiffed._subset) {
				denominator += log(sample[_variables[i]]);
				numDiffed++;
			}
		}

		else {
			// If this variable is differentiated, then return 0
			if (currentBit & isDiffed._subset) {
				if (!takeLog) {
					if (&grad)
						grad = 0;
					return 0.0;
				}
				else {
					if (&grad)
						grad = -std::numeric_limits<double>::infinity();
					return -std::numeric_limits<double>::infinity();
				}
			}

			margedVars++;
		}
	}

	// If we've marginalized all variables then return one
	if (margedVars == size()) {
		if (!takeLog) {
			if (&grad)
				grad = 0;
			return 1.0;
		}
		else {
			if (&grad)
				grad = -std::numeric_limits<double>::infinity();
			return 0.0;
		}
	}

	// If we've marginalized all variables except for one, then this will be identity or unity, depending on whether we are diffing variable
	else if (margedVars == (size() - 1)) {
		// If the only variable is differentiated, then return 1.0
		if (numDiffed) {
			if (!takeLog) {
				if (&grad)
					grad = 0;
				return 1.0;
			}
			else {
				if (&grad)
					grad = -std::numeric_limits<double>::infinity();
				return 0.0;
			}
		}

		// Otherwise, we will have unity, so return variable
		else {
			if (!takeLog) {
				if (&grad)
					grad = 0;
				return sample[lastNonMarged];
			}
			else {
				if (&grad)
					grad = -std::numeric_limits<double>::infinity();
				return log(sample[lastNonMarged]);
			}
		}
	}
	
	double frontFactor = 0;

	// Calculate numerator, numerically stably
	for (unsigned int i = 0; i < size(); i++) {
		// Only do variables that are not marginalized
		if (sample[_variables[i]] != 1.0) {
			if (i != whereMin) {
				numerator += pow(sample[_variables[whereMin]] / sample[_variables[i]], theta);
			}
		}
	}

	if (numDiffed > 1) {
		frontFactor += (numDiffed  - 1.0) * log(theta);
		for (unsigned int i = 1; i <= numDiffed - 1; i++)
			frontFactor += log(1.0 / theta + i);
	}

	double logSumU = boost::math::log1p(numerator + (1.0 - (size() - margedVars)) * pow(sample[_variables[whereMin]], theta)) - theta * log(sample[_variables[whereMin]]);
	double logDen = frontFactor - (1.0 / theta + numDiffed) * logSumU - theta * (1.0 / theta + 1.0) * denominator;
	
	if (&grad) {
		double firstTerm = 0, secondTerm = 0, thirdTerm = 0, fourthTerm = 0, fifthTerm = 0;
		// Calculate first term
		if (numDiffed) {
			for (unsigned int i = 0; i < numDiffed; i++)
				firstTerm -= 1.0 / (theta * (1.0 + i * theta));
		}

		// Calculate second term
		secondTerm = -denominator;

		// Calculate third term
		thirdTerm = numDiffed / theta;

		// Calculate fourth term
		fourthTerm = 1.0 / pow(theta, 2.0) * logSumU;

		// Calculate fifth term
		double fifthNumerator = 0;
		for (unsigned int i = 0; i < size(); i++) {
			if (sample[_variables[i]] != 1.0)
				fifthNumerator -= log(sample[_variables[i]]) * pow(sample[_variables[whereMin]] / sample[_variables[i]], theta);
		}
		fifthNumerator = log(fifthNumerator) - theta * log(sample[_variables[whereMin]]);
		fifthTerm = -(1/theta + numDiffed) * exp(fifthNumerator - logSumU);

		if (numerator != std::numeric_limits<double>::infinity())
			grad = (firstTerm + secondTerm + thirdTerm + fourthTerm + fifthTerm) * exp(logDen);
		else
			grad = 0.0;
	}

	if (!takeLog) {
		if (numerator != std::numeric_limits<double>::infinity())
			return exp(logDen);
		else
			return 0.0;
	}
	else {
		if (numerator != std::numeric_limits<double>::infinity())
			return logDen;
		else
			return -std::numeric_limits<double>::infinity();
	}
}

/*// Calculate log of gradient of partial derivative of copula, evaluated at sample
drwnClaytonCopula::range_type drwnClaytonCopula::logGradient(domain_type* sample, drwnSubset& isDiffed, bool takeLog)
{
	// Factor that is composed of derivatives of inv generators, and sum of inv generators
	double numerator = 0.0, denominator = 0.0, minU = std::numeric_limits<double>::infinity();
	unsigned int whereMin = 0;
	unsigned int margedVars = 0, lastNonMarged, numDiffed = 0;
	for (unsigned int i = 0, currentBit = 1; i < size(); i++, currentBit <<= 1) {
		// Check if zero
		if (!sample[_variables[i]]) {
			if (!takeLog)
				return 0.0;
			else
				return -std::numeric_limits<double>::infinity();
		}

		// Only do variables that are not marginalized
		else if (sample[_variables[i]] != 1.0) {
			lastNonMarged = _variables[i];

			if (sample[_variables[i]] < minU) {
				minU = sample[_variables[i]];
				whereMin = i;
			}

			// If this variable is differentiated, then have to multiply into front factor
			if (currentBit & isDiffed._subset) {
				denominator += theta * (1 / theta + 1) * log(sample[_variables[i]]);
				numDiffed++;
			}
		}

		else {
			// If this variable is differentiated, then return 0
			if (currentBit & isDiffed._subset) {
				if (!takeLog)
					return 0.0;
				else
					return -std::numeric_limits<double>::infinity();
			}

			margedVars++;
		}
	}

	// If we've marginalized all variables then return one
	if (margedVars == size()) {
		if (!takeLog)
			return 1.0;
		else
			return 0.0;
	}

	// If we've marginalized all variables except for one, then this will be identity or unity, depending on whether we are diffing variable
	else if (margedVars == (size() - 1)) {
		// If the only variable is differentiated, then return 1.0
		if (numDiffed) {
			if (!takeLog)
				return 1.0;
			else
				return 0.0;
		}

		// Otherwise, we will have unity, so return variable
		else {
			if (!takeLog)
				return sample[lastNonMarged];
			else
				return log(sample[lastNonMarged]);
		}
	}

	// Calculate numerator, numerically stably
	for (unsigned int i = 0; i < size(); i++) {
		// Only do variables that are not marginalized
		if (sample[_variables[i]] != 1.0) {
			if (i != whereMin) {
				numerator += pow(sample[_variables[whereMin]] / sample[_variables[i]], theta);
			}
		}
	}

	if (numDiffed > 1) {
		frontFactor += (numDiffed  - 1) * log(theta);
		for (unsigned int i = 1; i <= numDiffed - 1; i++)
			frontFactor += log(1 / theta + i);
	}

	//int numVar = (1 - (size() - margedVars));
	//double r2 = (boost::math::log1p(numerator + (1.0 - (size() - margedVars)) * pow(sample[_variables[whereMin]], theta)) - theta * log(sample[_variables[whereMin]]));

	double firstTerm = 0, secondTerm = 0, thirdTerm = 0, fourthTerm = 0, fifthTerm = 0;


	if (!takeLog) {
		if (numerator != std::numeric_limits<double>::infinity())
			result = exp(frontFactor - (1 / theta + numDiffed) * (boost::math::log1p(numerator + (1.0 - (size() - margedVars)) * pow(sample[_variables[whereMin]], theta)) - theta * log(sample[_variables[whereMin]])) - denominator);
		else
			result = 0.0;
	}
	else {
		if (numerator != std::numeric_limits<double>::infinity())
			result = frontFactor - (1 / theta + numDiffed) * (boost::math::log1p(numerator + (1.0 - (size() - margedVars)) * pow(sample[_variables[whereMin]], theta)) - theta * log(sample[_variables[whereMin]])) - denominator;
		else
			result = -std::numeric_limits<double>::infinity();
	}

	// Last resort, return 0. Hopefully numerical stability has been sorted out by taking logs
	// NOTE: Actually, I don't think our calculation can be NaN anymore...
	//if (isnan(result))
	//	return 0.0;

	//else
	return result;
}*/

// Calculate and cache all partial derivatives evaluated at sample. Optionally calculate gradients as well
void drwnClaytonCopula::cacheDerivatives(domain_type* sample, bool gradientAlso, bool takeLog, bool printDerivatives)
{
	// DEBUG
	vector<int> varNames;
	if (printDerivatives) {
		wcout << "Partial derivatives" << endl;
		for (unsigned int i = 0; i < size(); i++)
			varNames.push_back(i + 1);
	}

	// Loop through all subsets of variables and fill in partial derivatives
	drwnSubset currentSubset = 0;
	do {
		if (!gradientAlso)
			_partialDerivs[currentSubset._subset] = partialDerivative(sample, currentSubset, takeLog);
		else
			_partialDerivs[currentSubset._subset] = partialDerivative(sample, currentSubset, takeLog, _partialDerivs[currentSubset._subset + _numSubsets]);

		// DEBUG
		if (printDerivatives) {
			wcout << "subset ";
			drwnSubset::printSubset(currentSubset._subset, varNames);
			// DEBUG DEBUG: Make it so always printed in log-space
			if (takeLog)
				wcout << ", value " << _partialDerivs[currentSubset._subset] << endl;
			else
				//wcout << ", value " << log(_partialDerivs[currentSubset._subset]) << endl;
				wcout << ", value " << _partialDerivs[currentSubset._subset] << endl;
		}
	} while (!drwnSubset::isEmptyset(currentSubset = drwnSubset::nextSubset(currentSubset, drwnSubset(_numSubsets - 1))));

	// TODO: Calculate gradient
	if (gradientAlso && printDerivatives) {
		currentSubset = 0;
		wcout << endl << "Partial derivatives of gradient" << endl;
		do {
			// DEBUG
			
			if (printDerivatives) {
				wcout << "subset ";
				drwnSubset::printSubset(currentSubset._subset, varNames);
				// DEBUG DEBUG: Make it so always printed in log-space
				if (takeLog)
					wcout << ", value " << _partialDerivs[currentSubset._subset + _numSubsets] << endl;
				else
					//wcout << ", value " << log(_partialDerivs[currentSubset._subset]) << endl;
					wcout << ", value " << _partialDerivs[currentSubset._subset + _numSubsets] << endl;
			}
		} while (!drwnSubset::isEmptyset(currentSubset = drwnSubset::nextSubset(currentSubset, drwnSubset(_numSubsets - 1))));
	}
}