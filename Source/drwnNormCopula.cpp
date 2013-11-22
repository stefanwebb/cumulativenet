/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnNormCopula.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnNormCopula.h"
#include "mvnpack.h"

using boost::math::normal;
using Eigen::MatrixXd;

// Normal distribution
mynormal drwnNormCopula::_norm;	// Standard normal distribution.

// DEBUG: Forward declaration
//namespace drwnCumulativeUtils
//{
//	extern bool stopYet;
//};

// Create an empty copula
drwnNormCopula::drwnNormCopula()
	: _sigmaXX(_sigmaXXmem, 1, 1), _sigmaYX(_sigmaYXmem, 1, 1), _sigmaYY(_sigmaYYmem, 1, 1), _sigmaYgivenX(_sigmaYgivenXmem, 1, 1), _sigmaInvXX(_sigmaInvXXmem, 1, 1), _aVec(_b, 1, 1), _bVec(_b, 1, 1), _cVec(_b, 1, 1)
{
	initialize();
}

// Create copula with given variables
drwnNormCopula::drwnNormCopula(const drwnClique& c)
	: _sigmaXX(_sigmaXXmem, c.size(), c.size()), _sigmaYX(_sigmaYXmem, c.size(), c.size()), _sigmaYY(_sigmaYYmem, c.size(), c.size()), _sigmaYgivenX(_sigmaYgivenXmem, c.size(), c.size()), _sigmaInvXX(_sigmaInvXXmem, c.size(), c.size()), _aVec(_b, c.size(), c.size()), _bVec(_b, c.size(), c.size()), _cVec(_b, c.size(), c.size())
{
	// Need this in here, so doesn't call setVariables from parent class
	initialize();
	setVariables(c);
}

//! Initialize variables
void drwnNormCopula::initialize()
{
	_coefs = 0;
	_moments = 0;
	_correlIndex = 0;
	_precisionIndex = 0;
	_numParameters = 0;
	_sigmaXXmem = 0;
	_sigmaYXmem = 0;
	_sigmaYYmem = 0;
	_sigmaYgivenXmem = 0;
	_sigmaInvXXmem = 0;
	_muSigmas = 0;
	_a = 0;
	_b = 0;
	_c = 0;
	_d = 0;
	_precisionZ = 0;
	_zeros = 0;
	_normFactors = 0;
	_partialDerivs = 0;
	_isCacheValid = false;
}

// Copy constructor
/*drwnNormCopula::drwnNormCopula(const drwnNormCopula& f)
	: _sigma(f._sigma)
{
	// TODO: These are members of parent class, hence above method does not seem to work...
	// Is this how I should do this?
	_variables = f._variables;
	_varIndex = f._varIndex;
	_zeros = new int[_variables.size()];
	for (unsigned int i = 0; i < _variables.size(); i++)
		_zeros[i] = 0;
	_correl = new double[_variables.size() * (_variables.size() - 1) / 2];
	_values = new range_type[_variables.size()];
	_a = new range_type[_variables.size()];
	_b = new range_type[_variables.size()];
	for (unsigned int i = 0; i < _variables.size() * (_variables.size() - 1) / 2; i++)
		_correl[i] = 0;
	_tempCorrel = new double[_variables.size() * (_variables.size() - 1) / 2];
	_numParameters = f._numParameters;
}*/

// Default destructor
drwnNormCopula::~drwnNormCopula()
{
	destroy();
}

// Free all memory held with object
void drwnNormCopula::destroy()
{
	if (_coefs)
		delete [] _coefs;
	if (_moments)
		delete [] _moments;
	if (_correlIndex)
		delete [] _correlIndex;
	if (_precisionIndex)
		delete [] _precisionIndex;
	if (_sigmaXXmem)
		delete [] _sigmaXXmem;
	if (_sigmaYXmem)
		delete [] _sigmaYXmem;
	if (_sigmaYYmem)
		delete [] _sigmaYYmem;
	if (_sigmaYgivenXmem)
		delete [] _sigmaYgivenXmem;
	if (_sigmaInvXXmem)
		delete [] _sigmaInvXXmem;
	if (_muSigmas)
		delete [] _muSigmas;
	if (_a)
		delete [] _a;
	if (_b)
		delete [] _b;
	if (_c)
		delete [] _c;
	if (_d)
		delete [] _d;
	if (_precisionZ)
		delete [] _precisionZ;
	if (_zeros)
		delete [] _zeros;
	if (_normFactors)
		delete [] _normFactors;
	if (_partialDerivs)
		delete [] _partialDerivs;
}

// Set correlation
void drwnNormCopula::setParameter(parameter_type correl)
{
	// Update correlation parameters
	for (unsigned int i = 0; i < numParameters(); i++)
		_coefs[i] = correl;

	// Update cached parameters (essentially required functions of correlation parameters)
	_isCacheValid = false;

	// DEBUG: Do caching
	cacheParameters();
}

// Add multiple variables
void drwnNormCopula::setVariables(const drwnClique& c)
{
	// Must have size 2, 3, or 4 (for now)
	DRWN_ASSERT_MSG(c.size() >= 2 && c.size() <= 4, "Normal copula must have size between 2 and 4")
	
	// Could potentially call set variables a second time
	destroy();

	// Call set variables from parent
	drwnCdf::setVariables(c);
	_numParameters = size() * (size() - 1) / 2;

	// Calculate how many coefficients there are
	// Loop over all subsets of variables
	_numSubsets = (1 << size()) - 1;
	_numCoefs = 0;
	for (drwnSubset subset = 0; subset._subset != _numSubsets; subset._subset++) {
		unsigned int numElements = size() - drwnSubset::countElements(subset);
		_numCoefs += numElements * (numElements + 1);
	}
	_numCoefs -= size();	// Subtract size since don't store SD's for Sigma(YY) (they're 1)

	// Allocate memory
	_coefs = new domain_type[_numCoefs];	
	if (size() > 2)
		_moments = new domain_type[(size() - 2) * (size() + 1) / 2];
	_correlIndex = new unsigned int[(1 << size()) - 1];
	_precisionIndex = new unsigned int[(1 << size()) - 1];
	_normFactors = new domain_type[(1 << size()) - 1];
	_sigmaXXmem = new domain_type[size() * size()];
	_sigmaYYmem = new domain_type[size() * size()];
	_sigmaYXmem = new domain_type[(unsigned int) ceil(size() / 2.0) * (unsigned int) ceil(size() / 2.0)];
	_sigmaYgivenXmem = new domain_type[size() * size()];
	_sigmaInvXXmem = new domain_type[size() * size()];
	_muSigmas = new MatrixXd[(1 << size()) - 2];
	_a = new domain_type[_variables.size()];
	_b = new domain_type[_variables.size()];
	_c = new domain_type[_variables.size()];
	_d = new domain_type[_variables.size()];
	_precisionZ = new domain_type[_variables.size()];
	_zeros = new int[size()];
	_partialDerivs = new range_type[(1 << (size() + 2))];

	// Initialize to zero correlation between variables
	for (unsigned int i = 0; i < size() * (size() - 1) / 2; i++)
		_coefs[i] = 0;	

	// DEBUG: Cache coefficients here, so I can test it out
	//cacheParameters();
}

// Cache precalculate parameters
void drwnNormCopula::cacheParameters()
{
	// If cache is valid then just return
	if (_isCacheValid)
		return;

	// Deal with empty subset, that is, Sigma(YY) etc.
	unsigned int currentWriteCoef = 0;
	_correlIndex[0] = currentWriteCoef;

	// Fill out Sigma(YY)
	new (&_sigmaYY) Map<MatrixXd>(_sigmaYYmem, size(), size());
	for (unsigned int i = 0; i < size(); i++)
		_sigmaYY.coeffRef(i, i) = 1.0;
	for (unsigned int i = 1; i < size(); i++) {
		for (unsigned int j = 0; j < i; j++) {
			_sigmaYY.coeffRef(i, j) = _coefs[currentWriteCoef];
			_sigmaYY.coeffRef(j, i) = _coefs[currentWriteCoef++];
		}
	}

	// Calculate inv(SigmaYY)
	new (&_sigmaInvXX) Map<MatrixXd>(_sigmaInvXXmem, size(), size());
	_sigmaInvXX = _sigmaYY.inverse();

	// Calculate normalization factor
	_normFactors[0] = Eigen::MatrixXd(_sigmaInvXX.llt().matrixL()).diagonal().prod() * pow(2.0 * boost::math::constants::pi<double>(), -(double)size() / 2.0);

	/*wcout << L"SigmaYY" << endl;
	cout << _sigmaYY << endl;
	wcout << L"inv(SigmaYY)" << endl;
	cout << _sigmaInvXX << endl;
	cout << L"Determinate: " << Eigen::MatrixXd(_sigmaInvXX.llt().matrixL()).diagonal().prod() << " Pi factor: " << pow(2.0 * boost::math::constants::pi<double>(), -(double)size() / 2.0);*/

	// Calculate precision coefficients
	_precisionIndex[0] = currentWriteCoef;
	for (unsigned int i = 0; i < size(); i++)
		_coefs[currentWriteCoef++] = _sigmaInvXX.coeff(i, i);
	for (unsigned int i = 1; i < size(); i++) {
		for (unsigned int j = 0; j < i; j++)
			_coefs[currentWriteCoef++] = _sigmaInvXX.coeff(i, j);
	}

	// Loop over subsets
	for (drwnSubset isDiffed = 1; isDiffed._subset < _numSubsets; isDiffed._subset++) {
		// Calculate number of differentiated variables, etc.
		unsigned int numDiffed = drwnSubset::countElements(isDiffed), numUndiffed = size() - numDiffed;

		// Resize sigmas without doing any memory allocation
		new (&_sigmaYY) Map<MatrixXd>(_sigmaYYmem, numUndiffed, numUndiffed);
		new (&_sigmaYX) Map<MatrixXd>(_sigmaYXmem, numUndiffed, numDiffed);
		new (&_sigmaXX) Map<MatrixXd>(_sigmaXXmem, numDiffed, numDiffed);
		new (&_sigmaYgivenX) Map<MatrixXd>(_sigmaYgivenXmem, numUndiffed, numUndiffed);
		new (&_sigmaInvXX) Map<MatrixXd>(_sigmaInvXXmem, numDiffed, numDiffed);

		// Loop over all j < i variable pairs
		unsigned int currentRowX = 0, currentRowY = 0, currentCoeff = 0;
		for (unsigned int i = 0, currentFirstBit = 1; i < size(); i++, currentFirstBit <<= 1) {
			// Reset column
			unsigned int currentColumnX = 0, currentColumnY = 0;

			// Row corresponds to a differentiated variable
			if (isDiffed._subset & currentFirstBit) {
				// Loop through all columns
				for (unsigned int j = 0, currentSecondBit = 1; j <= i ; j++, currentSecondBit <<= 1) {
					// Column corresponds to a differentiated variable
					if (isDiffed._subset & currentSecondBit) {
						// Deal with diagonal
						if (i == j)
							_sigmaXX.coeffRef(currentRowX, currentColumnX++) = 1.0;

						// Otherwise we set entries of sigmaXX
						else {
							_sigmaXX.coeffRef(currentRowX, currentColumnX) = _coefs[currentCoeff];
							_sigmaXX.coeffRef(currentColumnX++, currentRowX) = _coefs[currentCoeff++];
						}
					}

					// Column corresponds to a non-differentiated variable
					else {
						_sigmaYX.coeffRef(currentColumnY++, currentRowX) = _coefs[currentCoeff++];
					}
				}
				currentRowX++;
			}

			// Row corresponds to a non-differentiated variable
			else {
				// Loop through all columns
				for (unsigned int j = 0, currentSecondBit = 1; j <= i ; j++, currentSecondBit <<= 1) {
					// Column corresponds to a differentiated variable
					if (isDiffed._subset & currentSecondBit) {
						_sigmaYX.coeffRef(currentRowY, currentColumnX++) = _coefs[currentCoeff++];
					}

					// Column corresponds to a non-differentiated variable
					else {
						// Deal with diagonal
						if (i == j)
							_sigmaYY.coeffRef(currentRowY, currentColumnY++) = 1.0;

						// Otherwise we set entries of sigmaXX
						else {
							_sigmaYY.coeffRef(currentRowY, currentColumnY) = _coefs[currentCoeff];
							_sigmaYY.coeffRef(currentColumnY++, currentRowY) = _coefs[currentCoeff++];
						}
					}
				}
				currentRowY++;
			}
		}

		// Calculate precision coefficients for complement of differentiated variables
		_sigmaInvXX = _sigmaXX.inverse();

		// Calculate normalization factor (determinate of precision matrix is calculated by Cholesky decomposition)
		_normFactors[_numSubsets - isDiffed._subset] = Eigen::MatrixXd(_sigmaInvXX.llt().matrixL()).diagonal().prod() * pow(2.0 * boost::math::constants::pi<double>(), -_sigmaInvXX.rows() / 2.0);
		//wcout << L"Determinate: " << Eigen::MatrixXd(_sigmaInvXX.llt().matrixL()).diagonal().prod() << L" Pi factor: " << pow(2.0 * boost::math::constants::pi<double>(), -_sigmaInvXX.rows() / 2.0) << endl;

		// Calculate product of Sigma(YX) * inv(Sigma(XX))
		_muSigmas[isDiffed._subset - 1] = _sigmaYX * _sigmaInvXX;

		// Calculate variance matrix of Y given X
		_sigmaYgivenX = _sigmaYY - _muSigmas[isDiffed._subset - 1] * _sigmaYX.transpose();

		// DEBUG: Print our matrices
		/*wcout << L"Subset: " << isDiffed._subset << endl;
		wcout << L"Sigma(YY)" << endl;
		cout << _sigmaYY << endl;
		wcout << L"Sigma(XX)" << endl;
		cout << _sigmaXX << endl;
		wcout << L"Sigma(YX)" << endl;
		cout << _sigmaYX << endl;
		wcout << L"Sigma(Y|X)" << endl;
		cout << _sigmaYgivenX << endl;
		wcout << L"inv(Sigma(XX))" << endl;
		cout << _sigmaInvXX << endl;
		wcout << L"Normalization factor: " << _normFactors[numSubsets - isDiffed._subset] << endl;
		wcout << endl;*/

		// DEBUG
		if (isDiffed._subset == 3)
			bool gotHere = true;

		// Calculate correlation coefficients (loop over lower triangular part) and SDs for Sigma(Y|X)
		_correlIndex[isDiffed._subset] = currentWriteCoef;

		for (unsigned int i = 1; i < numUndiffed; i++) {
			for (unsigned int j = 0; j < i; j++)
				_coefs[currentWriteCoef++] = _sigmaYgivenX.coeff(i, j) / ::sqrt(_sigmaYgivenX.coeff(i, i) * _sigmaYgivenX.coeff(j,j));
		}
		for (unsigned int i = 0; i < numUndiffed; i++)
			_coefs[currentWriteCoef++] = ::sqrt(_sigmaYgivenX.coeff(i, i));

		// Fill in entries of upper lower part of precision matrix
		_precisionIndex[_numSubsets - isDiffed._subset] = currentWriteCoef;
		for (unsigned int i = 0; i < numDiffed; i++)
			_coefs[currentWriteCoef++] = _sigmaInvXX.coeff(i, i);
		for (unsigned int i = 1; i < numDiffed; i++) {
			for (unsigned int j = 0; j < i; j++)
				_coefs[currentWriteCoef++] = _sigmaInvXX.coeff(i, j);
		}
	}

	//DRWN_ASSERT_MSG(currentWriteCoef == _numCoefs, "Did not write correct number of coefficients");

	_isCacheValid = true;
}

//bool gotHere;

// Calculate the single moments of truncated normal distribution
// TODO: If ever do an optimized version of cacheDerivatives, this function becomes unnecessary. I will calculate moments only once.
void drwnNormCopula::calculateTruncSingleMoments(domain_type* sample, drwnSubset& diffedVars)
{
	unsigned int numDiffed = drwnSubset::countElements(diffedVars), numVars = _variables.size(), numUndiffed = numVars - numDiffed;

	// Extract a and b from sample
	unsigned int currentA = 0, currentB = 0, margIdx = 0, numMarged = 0;
	for (unsigned int i = 0, currentBit = 1; i < size(); i++, currentBit <<= 1) {
		// Add to a or b, depending on whether variable is differentiated
		if (diffedVars._subset & currentBit)
			_a[currentA++] = sample[_variables[i]];
		else {
			if (sample[_variables[i]] != std::numeric_limits<double>::infinity())
				_b[currentB++] = sample[_variables[i]];

			// Have to make adjustments when variable is marginalized
			else {
				margIdx = currentB;
				_b[currentB++] = sample[_variables[i]];
				numMarged++;
			}
		}
	}

	// Subtract _muSigma * a from b
	new (&_aVec) Map<VectorXd>(_a, numDiffed);
	new (&_bVec) Map<VectorXd>(_b, numUndiffed);
	new (&_cVec) Map<VectorXd>(_c, numUndiffed);
	_cVec = _muSigmas[diffedVars._subset - 1] * _aVec;
	_bVec -= _cVec;

	// DEBUG
	/*wcout << "muSigma" << endl;
	cout << _muSigmas[diffedVars._subset - 1] << endl;

	wcout << "a" << endl;
	for (unsigned int i = 0; i < numDiffed; i++)
		wcout << _a[i] << " ";
	wcout << endl;

	wcout << "b" << endl;
	for (unsigned int i = 0; i < numUndiffed; i++)
		wcout << _b[i] << " ";
	wcout << endl;*/

	// Sigma = &_coefs[_correlIndex[diffedVars._subset]]
	// Mu = _c

	// Univariate case
	if (numUndiffed - numMarged == 1) {
		// DEBUG
		//wcout << _coefs[_correlIndex[diffedVars._subset]] << endl;

		double sigma = pow(_coefs[_correlIndex[diffedVars._subset] + numUndiffed * (numUndiffed - 1) / 2], 2.0);
		double mu;
		mu = _c[0];
		if (numMarged && margIdx == 0)
			mu = _c[1];

		double h;
		h = _b[0] / sqrt(sigma);	// Note: mean already subtracted!
		if (numMarged && margIdx == 0)
			h = _b[1] / sqrt(sigma);
		
		_moments[0] = mu - sqrt(sigma) * pdf(_norm, h) / cdf(_norm, h);
	}

	// Bivariate case
	else if (numUndiffed - numMarged == 2) {
		//cout << "b " << _bVec << endl;
		double rho = _coefs[_correlIndex[diffedVars._subset]];
		double sigma1 = pow(_coefs[_correlIndex[diffedVars._subset] + 1], 2.0);
		double sigma2 = pow(_coefs[_correlIndex[diffedVars._subset] + 2], 2.0);
		double mu1 = _c[0], mu2 = _c[1];
		double h = _b[0] / sqrt(sigma1);
		double k = _b[1] / sqrt(sigma2);
		double c = 1 / sqrt(1.0 - pow(rho, 2.0));
		//double p = standardCdf(_b, &_coefs[_correlIndex[diffedVars._subset]], numUndiffed, true);
		double p = _partialDerivs[2 * (_numSubsets + 1) + diffedVars._subset];

		// E[X]
		double term1 = pdf(_norm, h) * cdf(_norm, (k - rho * h) * c);
		double term2 = rho * pdf(_norm, k) * cdf(_norm, (h - rho * k) * c);
		double m10 = -(term1 + term2) / p;

		// E[Y]
		term1 = pdf(_norm, k) * cdf(_norm, (h - rho * k) * c);
		term2 = rho * pdf(_norm, h) * cdf(_norm, (k - rho * h) * c);
		double m01 = -(term1 + term2) / p;

		// Set means
		_moments[0] = m10 * sqrt(sigma1) + mu1;	// E[X]
		_moments[1] = m01 * sqrt(sigma2) + mu2;	// E[Y]
	}

	// Error!
	else {
		// TODO: Implement general formula
		DRWN_LOG(DRWN_LL_FATAL, "Moments of truncated multivariate norm not implemented for n > 2");
	}
}

// Calculate the first and second order moments of truncated normal distribution
void drwnNormCopula::calculateTruncMoments(domain_type* sample, drwnSubset& diffedVars)
{
	unsigned int numDiffed = drwnSubset::countElements(diffedVars), numVars = _variables.size(), numUndiffed = numVars - numDiffed;

	// Extract a and b from sample
	unsigned int currentA = 0, currentB = 0, margIdx = 0, numMarged = 0;
	for (unsigned int i = 0, currentBit = 1; i < size(); i++, currentBit <<= 1) {
		// Add to a or b, depending on whether variable is differentiated
		if (diffedVars._subset & currentBit)
			_a[currentA++] = sample[_variables[i]];
		else {
			if (sample[_variables[i]] != std::numeric_limits<double>::infinity())
				_b[currentB++] = sample[_variables[i]];

			// Have to make adjustments when variable is marginalized
			else {
				margIdx = currentB;
				_b[currentB++] = sample[_variables[i]];
				numMarged++;
			}
		}
	}

	// Subtract _muSigma * a from b
	new (&_aVec) Map<VectorXd>(_a, numDiffed);
	new (&_bVec) Map<VectorXd>(_b, numUndiffed);
	new (&_cVec) Map<VectorXd>(_c, numUndiffed);

	// DEBUG
	/*cout << "diffed vars" << endl << _aVec << endl;
	cout << "undiffed vars" << endl << _bVec << endl;
	cout << "muSigma" << endl << _cVec << endl;*/

	_cVec = _muSigmas[diffedVars._subset - 1] * _aVec;
	_bVec -= _cVec;

	// DEBUG
	//cout << "adjusted undiffed vars" << endl << _bVec << endl << endl;

	// NOTE:
	// Sigma^{-1} = &_coefs[_correlIndex[diffedVars._subset]] (first diagonal, then elements below diagonal left to right, top to bottom)
	// Mu = _c

	// Univariate case
	if (numUndiffed - numMarged == 1) {
		double sigma = pow(_coefs[_correlIndex[diffedVars._subset] + numUndiffed * (numUndiffed - 1) / 2], 2.0);
		double mu;
		mu = _c[0];
		if (numMarged && margIdx == 0)
			mu = _c[1];

		double h;
		h = _b[0] / sqrt(sigma);	// Note: mean already subtracted!
		if (numMarged && margIdx == 0)
			h = _b[1] / sqrt(sigma);
		
		_moments[0] = mu - sqrt(sigma) * pdf(_norm, h) / cdf(_norm, h);
		_moments[1] = sigma + pow(mu, 2.0) - (sigma * h + 2.0 * mu * sqrt(sigma)) * pdf(_norm, h) / cdf(_norm, h);
	}

	// Bivariate case
	else if (numUndiffed - numMarged == 2) {
		double rho = _coefs[_correlIndex[diffedVars._subset]];
		double sigma1 = pow(_coefs[_correlIndex[diffedVars._subset] + 1], 2.0);
		double sigma2 = pow(_coefs[_correlIndex[diffedVars._subset] + 2], 2.0);
		double mu1 = _c[0], mu2 = _c[1];
		double h = _b[0] / sqrt(sigma1);
		double k = _b[1] / sqrt(sigma2);
		double c = 1 / sqrt(1.0 - pow(rho, 2.0));
		//double p = standardCdf(_b, &_coefs[_correlIndex[diffedVars._subset]], numUndiffed, true);
		double p = _partialDerivs[2 * (_numSubsets + 1) + diffedVars._subset];

		// E[X]
		double term1 = pdf(_norm, h) * cdf(_norm, (k - rho * h) * c);
		double term2 = rho * pdf(_norm, k) * cdf(_norm, (h - rho * k) * c);
		double m10 = -(term1 + term2) / p;

		// E[Y]
		term1 = pdf(_norm, k) * cdf(_norm, (h - rho * k) * c);
		term2 = rho * pdf(_norm, h) * cdf(_norm, (k - rho * h) * c);
		double m01 = -(term1 + term2) / p;

		// E[X^2]
		term1 = h * pdf(_norm, h) * cdf(_norm, (k - h * rho) * c);
		term2 = pow(rho, 2.0) * k * pdf(_norm, k) * cdf(_norm, (h - k * rho) * c);
		double term3 = rho / c / sqrt(2.0 * M_PI) * pdf(_norm, sqrt(pow(h, 2.0) - 2 * rho * h * k + pow(k, 2.0)) * c);
		double m20 = (p - term1 - term2 + term3) / p;

		// E[Y^2]
		term1 = k * pdf(_norm, k) * cdf(_norm, (h - k * rho) * c);
		term2 = pow(rho, 2.0) * h * pdf(_norm, h) * cdf(_norm, (k - h * rho) * c);
		term3 = rho / c / sqrt(2.0 * M_PI) * pdf(_norm, sqrt(pow(k, 2.0) - 2.0 * rho * k * h + pow(h, 2.0)) * c);
		double m02 = (p - term1 - term2 + term3) / p;

		// E[XY]
		term1 = rho * h * pdf(_norm, h) * cdf(_norm, (k - rho * h) * c);
		term2 = rho * k * pdf(_norm, k) * cdf(_norm, (h - rho * k) * c);
		term3 = 1.0 / c / sqrt(2.0 * M_PI) * pdf(_norm, -sqrt(pow(h, 2.0) - 2.0 * h * k * rho + pow(k, 2.0)) * c);
		double m11 = (rho * p - term1 - term2 + term3) / p;

		// Set means
		_moments[0] = m10 * sqrt(sigma1) + mu1;	// E[X]
		_moments[1] = m01 * sqrt(sigma2) + mu2;	// E[Y]
		
		// Set E[X^2] and E[Y^2]
		_moments[2] = (m20 - pow(m10, 2.0)) * sigma1 + pow(_moments[0], 2.0);	// E[X^2]
		_moments[3] = (m02 - pow(m01, 2.0)) * sigma2 + pow(_moments[1], 2.0);	// E[Y^2]

		// Set E[XY]
		_moments[4] = (m11 - m10 * m01) * sqrt(sigma1 * sigma2) + _moments[0] * _moments[1];
	}

	// Error!
	else {
		// TODO: Implement general formula
		DRWN_LOG(DRWN_LL_FATAL, "Moments of truncated multivariate norm not implemented for n > 2");
	}
}

// Calculate and cache all partial derivatives evaluated at sample
void drwnNormCopula::cacheDerivatives(domain_type* sample, bool gradientAlso, bool takeLog, bool printDerivatives)
{
	// NOTE: What happens if sample variables are infinite?

	// DEBUG
	if (printDerivatives)
		wcout << "Partial derivatives" << endl;

	unsigned int cdfIdx = 2 * (_numSubsets + 1);

	// DEBUG
	vector<int> varNames;
	if (printDerivatives) {
		for (unsigned int i = 0; i < size(); i++)
			varNames.push_back(i + 1);
	}

	// Loop through all subsets of variables and fill in partial derivatives
	drwnSubset currentSubset = 0;
	do {
		// DEBUG
		/*if (drwnCumulativeUtils::stopYet == true && currentSubset._subset == 3) {
			gotHere = true;
			wcout << "sample" << endl;
			for (unsigned int i = 0; i < _variables.size(); i++) {
				wcout << sample[_variables[i]] << " ";
			}
			wcout << endl;
		}*/

		_partialDerivs[currentSubset._subset] = partialDerivative(sample, currentSubset, takeLog, _partialDerivs[cdfIdx + currentSubset._subset]);

		// DEBUG
		if (printDerivatives) {
			wcout << "subset ";
			drwnSubset::printSubset(currentSubset._subset, varNames);
			wcout << ", value " << _partialDerivs[currentSubset._subset] << endl;
		}

		// DEBUG
		//gotHere = false;
	} while (!drwnSubset::isEmptyset(currentSubset = drwnSubset::nextSubset(currentSubset, drwnSubset(_numSubsets))));

	// DEBUG
	if (printDerivatives)
		wcout << endl;
	
	// Calculate gradients if required
	if (gradientAlso) {
		unsigned int startIdx = _numSubsets + 1;
		// First have to calculate Sigma^{-1}z
		
		// Extract variables from sample, and make mask of marginalized variables
		unsigned int margMask = 0, numMarged = 0;
		for (unsigned int i = 0; i < _variables.size(); i++) {
			if (sample[_variables[i]] != std::numeric_limits<double>::infinity())
				_d[i] = sample[_variables[i]];
			else {
				_d[i] = 0.0;
				margMask |= 1 << i;
				numMarged++;
			}
		}

		// DEBUG
		//wcout << "Diagonal elements of precision matrix" << endl;
		//for (unsigned int i = 0;

		// Calculate the contribution from the diagonal elements of Sigma^{-1}
		unsigned int precisionIdx = _precisionIndex[margMask];
		for (unsigned int i = 0; i < size(); i++) {
			if ((1 << i) & margMask) {
				//precisionIdx++;
				continue;
			}
			//wcout << _coefs[precisionIdx] << " " << endl;
			_precisionZ[i] = _coefs[precisionIdx++] * _d[i];
		}

		// DEBUG
		//wcout << endl << endl << "Elements of precision matrix below diagonal" << endl;
		//wcout << _coefs[_precisionIndex[0]] << " " << _coefs[_precisionIndex[0] + size()] << " " << _coefs[_precisionIndex[0] + size() - 1] << endl;

		// Loop over elements below the diagonal, each contributes to two terms
		for (unsigned int i = 1; i < size(); i++) {
			for (unsigned int j = 0; j < i; j++) {
				if (((1 << i) | (1 << j)) & margMask) {
					//precisionIdx++;
					continue;
				}

				//wcout << _coefs[precisionIdx] << " " << endl;
				_precisionZ[i] += _coefs[precisionIdx] * _d[j];
				_precisionZ[j] += _coefs[precisionIdx++] * _d[i];
			}
		}

		// DEBUG
		//wcout << endl;

		// DEBUG:
		if (printDerivatives)
			wcout << "Partial derivatives of gradient" << endl;
		
		// Calculate partial derivatives of derivative of factor with respect to copula parameter
		currentSubset = 0;
		do {
			_partialDerivs[startIdx + currentSubset._subset] = 0;

			// If this is differentiating with respect to a subset containing a marginalized variable then continue
			if (currentSubset._subset & margMask) {
				if (printDerivatives) {
					wcout << "subset ";
					drwnSubset::printSubset(currentSubset._subset, varNames);
					wcout << ", value " << _partialDerivs[startIdx + currentSubset._subset] << endl;
				}
				continue;
			}

			// DEBUG: Catching case where fails
			if (currentSubset._subset == 3)
				bool gotHere = true;

			// Loop over all pairs of variables
			//precisionIdx = _precisionIndex[margMask] + size() - numMarged;
			for (unsigned int i = 1; i < size(); i++) {
				for (unsigned int j = 0; j < i; j++) { // , precisionIdx++

			//for (unsigned int i = 1; i < 2; i++) {
			//	for (unsigned int j = 0; j < 1; j++, precisionIdx++) {
					// DEBUG
					/*i = 2;
					j = 1;
					precisionIdx++;
					precisionIdx++;*/

					// DEBUG
					if (currentSubset._subset == 1)
						bool gotHere = true;	//*/

					// Create subset with those two variables
					drwnSubset varSubset((1 << i) | (1 << j));

					// Skip those which contain marginalized variables
					if (varSubset._subset & margMask)
						continue;

					// Find where they intersect
					drwnSubset varsInBoth(drwnSubset::setIntersection(varSubset, currentSubset));
					drwnSubset diffedVars(drwnSubset::setUnion(varSubset, currentSubset));
					drwnSubset nonDiffedVars(drwnSubset::setMinus(drwnSubset((1 << size()) - 1), diffedVars));

					// Three cases, depending on overlap (i.e. how many variables we differentiate twice)
					unsigned int numOverlap = drwnSubset::countElements(varsInBoth);
					if (numOverlap == 0) {
						// Differentiating with respect to all variables once, so fetch corresponding factor
						if (!takeLog)
							_partialDerivs[startIdx + currentSubset._subset] += _partialDerivs[diffedVars._subset];
						else
							_partialDerivs[startIdx + currentSubset._subset] += exp(_partialDerivs[diffedVars._subset]);
						
					}

					else if (numOverlap == 1) {
						// Find out which variable is being differentiated twice
						unsigned int diffVar = ((varsInBoth._subset == (1 << i)) ? i : j);
						range_type factor = 0;

						// If all variables are differentiated then it's simple
						if ((nonDiffedVars._subset & ~margMask) == 0)
							 factor = -_precisionZ[diffVar];

						// Otherwise, have to use moments of truncated normal distribution
						else {
							// Calculate the single moments of truncated normal distribution
							calculateTruncSingleMoments(sample, diffedVars);

							// Loop over variables
							unsigned int undiffedIdx = 0;
							for (unsigned int k = 0; k < size(); k++) {
								// Skip marginalized variables
								if (margMask & (1 << k))
									continue;

								// Since all covariances equal, sigma inverse is one of two possibilities
								range_type sigmaInverse = ((diffVar == k) ? _coefs[_precisionIndex[margMask]] : _coefs[_precisionIndex[margMask] + size() - numMarged]);

								// If variable is diffed then add in simple term
								if (1 << k & diffedVars._subset) {
									factor -= _d[k] * sigmaInverse;
								}

								// Otherwise, have to calculate expected value of truncated normal distribution
								else {
									factor -= sigmaInverse * _moments[undiffedIdx++];
								}
							}


						}

						if (!takeLog)
							_partialDerivs[startIdx + currentSubset._subset] += factor * _partialDerivs[diffedVars._subset];
						else
							_partialDerivs[startIdx + currentSubset._subset] += factor * exp(_partialDerivs[diffedVars._subset]);
					}

					else {
						range_type factor = 0;

						// If all variables are differentiated then it's simple
						if ((nonDiffedVars._subset & ~margMask) == 0)
							// Differentiating with respect to both variables twice
							factor = _precisionZ[i] * _precisionZ[j];

						// Otherwise, have to use moments of truncated normal distribution
						else {
							// Calculate the single moments of truncated normal distribution
							calculateTruncMoments(sample, diffedVars);

							// Add in all x_i * x_j terms, and calculate factor for E[y_i] terms
							double factorYmeans = 0;
							for (unsigned int k = 0; k < size(); k++) {
								for (unsigned int l = 0; l < size(); l++) {
									// If both variables are diffed then add in simple term
									if (((1 << k) & diffedVars._subset) && ((1 << l) & diffedVars._subset)) {
										// Since all covariances equal, sigma inverse is one of two possibilities
										range_type sigmaInverseI = ((i == k) ? _coefs[_precisionIndex[margMask]] : _coefs[_precisionIndex[margMask] + size() - numMarged]);
										range_type sigmaInverseJ = ((j == l) ? _coefs[_precisionIndex[margMask]] : _coefs[_precisionIndex[margMask] + size() - numMarged]);
										factor += _d[k] * _d[l] * sigmaInverseI * sigmaInverseJ;
									}
								}

								// If variable is diffed then add in simple term
								if (1 << k & diffedVars._subset) {
									range_type sigmaInverseI = ((i == k) ? _coefs[_precisionIndex[margMask]] : _coefs[_precisionIndex[margMask] + size() - numMarged]);
									range_type sigmaInverseJ = ((j == k) ? _coefs[_precisionIndex[margMask]] : _coefs[_precisionIndex[margMask] + size() - numMarged]);
									factorYmeans += _d[k] * (sigmaInverseI + sigmaInverseJ) * _coefs[_precisionIndex[margMask] + size() - numMarged];
								}
							}

							// DEBUG: Catching case where fails
							if (currentSubset._subset == 3)
								bool gotHere = true;

							// Add in all E[y_i] terms
							double sumYmeans = 0;
							unsigned int numUndiffedVars = drwnSubset::countElements(nonDiffedVars)  - numMarged;
							for (unsigned int l = 0; l < numUndiffedVars; l++)
								sumYmeans += _moments[l];
							factor += factorYmeans * sumYmeans;

							// Add in all E[y_i^2] terms
							sumYmeans = 0;
							for (unsigned int l = 0; l < numUndiffedVars; l++)
								sumYmeans += _moments[l + numUndiffedVars];
							factor += pow(_coefs[_precisionIndex[margMask] + size() - numMarged], 2.0) * sumYmeans;

							// Add in all E[y_i * y_j] terms
							if (numUndiffedVars > 1) {
								sumYmeans = 0;
								// TODO: Loop over correct number of these coefficients
								for (unsigned int l = 0; l < numUndiffedVars; l++)
									sumYmeans += _moments[l + 2 * numUndiffedVars];
								factor += 2.0 * pow(_coefs[_precisionIndex[margMask] + size() - numMarged], 2.0) * sumYmeans;
							}
						}

						if (!takeLog)
							_partialDerivs[startIdx + currentSubset._subset] += (factor - _coefs[_precisionIndex[margMask] + size() - numMarged]) * _partialDerivs[diffedVars._subset];
						else
							_partialDerivs[startIdx + currentSubset._subset] += (factor - _coefs[_precisionIndex[margMask] + size() - numMarged]) * exp(_partialDerivs[diffedVars._subset]);
					}
				}
			}

			// Take log if required
			//if (takeLog)
			//	_partialDerivs[startIdx + currentSubset._subset] = log(_partialDerivs[startIdx + currentSubset._subset]);

			// DEBUG: Print answer (useful for comparing to R output)
			/*wcout << "subset: ";
			drwnSubset::printSubset(currentSubset, _variables);
			wcout << ", answer: " << _partialDerivs[startIdx + currentSubset._subset] << endl;*/
			if (printDerivatives) {
				wcout << "subset ";
				drwnSubset::printSubset(currentSubset._subset, varNames);
				wcout << ", value " << _partialDerivs[startIdx + currentSubset._subset] << endl;
			}


		} while (!drwnSubset::isEmptyset(currentSubset = drwnSubset::nextSubset(currentSubset, drwnSubset(_numSubsets))));
	}
}

// Fill vector of variable values from whole sample
void drwnNormCopula::extractValues(drwnNormCopula::domain_type* sample)
{
	// Extract variables from sample
	for (unsigned int i = 0; i < _variables.size(); i++)
		_a[i] = sample[_variables[i]];
}

// Evaluate copula CDF of sample
drwnNormCopula::range_type drwnNormCopula::evaluateCdf(domain_type* sample, bool takeLog)
{
	// Cache parameters if necessary
	cacheParameters();

	// Extract values from sample
	extractValues(sample);

	// DEBUG
	/*for (unsigned int i = 0; i < size(); i++)
		wcout << _a[i] << " ";
	wcout << endl;*/

	// Call function for values
	if (!takeLog)
		return standardCdf(_a, _coefs, size());
	else
		return log(standardCdf(_a, _coefs, size()));
}

// Evaluate multivariate normal CDF for given mu and either sqrt(sigma) or inv(sqrt(sigma))
drwnNormCopula::range_type drwnNormCopula::standardCdf(domain_type* values, domain_type* correl, unsigned int n, bool hasSd)
{
	// Number of correlation coefficients
	unsigned int countCorrels = n * (n - 1) / 2;
	
	// Transform variables so each has standard normal distribution
	if (hasSd) {
		for (unsigned int i = 0; i < n; i++)
			values[i] /= correl[countCorrels + i];
	}
	
	// Set integration limits
	// NOTE: We need to do this, because routine seems to fail when value is too big
	for (unsigned int i = 0; i < n; i++) {
		if (values[i] > 100)
			_zeros[i] = -1;
		else if (values[i] < -100) {
			_zeros[i] = 0;
			values[i] = -100.0;
		}
		else
			_zeros[i] = 0;
	}

	// Call Fortan routine
	integer numVars = n;
	integer maxpts = 2000 * n;
	doublereal abseps = 1e-6;
	doublereal releps = 0;
	doublereal error = 0;
	doublereal value = 0;
	integer inform = 0;
	sadmvn_(&numVars, 0, values, (integer*) _zeros, correl, &maxpts, &abseps, &releps, &error, &value, &inform);

	return value;
}

// Evaluate copula density of sample
drwnNormCopula::range_type drwnNormCopula::evaluateDensity(domain_type* sample)
{
	// Extract variables from sample
	unsigned int numVars = 0, subset = 0;
	for (unsigned int i = 0, bit = 1; i < _variables.size(); i++, bit <<= 1) {
		// Only extract finite entries
		if (!boost::math::isinf(sample[_variables[i]])) {
			_a[numVars++] = sample[_variables[i]];
		}

		// Make a record of infinite entries
		else {
			// DEBUG: If variable was marginalized then density must be 0
			// NOTE: I can't remember why I didn't do this before, hopefully won't stuff anything up...!
			//return 0;

			subset |= bit;
		}
	}

	// DEBUG: Print precisions
	/*wcout << "Precisions: ";
	for (unsigned int i = 0; i < numVars * (numVars + 1) / 2; i++)
		wcout << _coefs[_precisionIndex[subset] + i] << " ";
	wcout << endl;*/

	// Call function for values
	return standardDensity(_a, &_coefs[_precisionIndex[subset]], _normFactors[subset], numVars);
}

// Evaluate multivariate normal density with zero mean and arbitrary precision matrix
drwnNormCopula::range_type drwnNormCopula::standardDensity(domain_type* values, domain_type* precision, domain_type normFactor, unsigned int n)
{
	// Loop over rows and columns of precision matrix, calculating quadratric form (and divide by 2)
	// Add in half diagonal
	range_type quadraticForm = 0;
	unsigned int numCoef = 0;
	for (unsigned int i = 0; i < n; i++)
		quadraticForm += precision[numCoef++] * values[i] * values[i];
	quadraticForm /= 2.0;

	// Add in lower triangular part (since symmetric and halving)
	for (unsigned int i = 1; i < n; i++) {
		for (unsigned int j = 0; j < i; j++)
			quadraticForm += precision[numCoef++] * values[i] * values[j];
	}

	return normFactor * exp(-quadraticForm);
}

// Evaluate derivative of probit function
drwnNormCopula::range_type drwnNormCopula::derivativeProbit(drwnNormCopula::domain_type value)
{
	return 1.0 / pdf(_norm, quantile(_norm, value));
}

// Calculate a partial derivative of copula, evalued at sample
drwnNormCopula::range_type drwnNormCopula::partialDerivative(domain_type* sample, drwnSubset& isDiffed, bool takeLog, drwnNormCopula::range_type& cdfValue)
{
	// DEBUG
	//if (gotHere)
	//	bool meaninglessFlag = true;

	// Extract a and b from sample
	unsigned int currentA = 0, currentB = 0, numMarged = 0;
	for (unsigned int i = 0, currentBit = 1; i < size(); i++, currentBit <<= 1) {
		// Add to a or b, depending on whether variable is differentiated
		if (isDiffed._subset & currentBit) {
			// If the variable was marginalized, then the derivative will be 0
			if (sample[_variables[i]] == std::numeric_limits<double>::infinity()) {
				if (!takeLog)
					return 0.0;
				else
					return -std::numeric_limits<double>::infinity();
			}

			_a[currentA++] = sample[_variables[i]];
		}
		else {
			if (sample[_variables[i]] != std::numeric_limits<double>::infinity())
				_b[currentB++] = sample[_variables[i]];
			else {
				_b[currentB++] = sample[_variables[i]];
				numMarged++;
			}
		}
	}

	// Test whether all or none of variables are diffed, and if so handle those cases
	unsigned int numDiffed = drwnSubset::countElements(isDiffed), numVars = _variables.size(), numUndiffed = numVars - numDiffed;
	if (!numDiffed) {
		// When there are no differentiated variables and all undifferentiated variables have been marginalized, then return 1
		if (!(numUndiffed - numMarged)) {
			if (!takeLog)
				return 1.0;
			else
				return 0.0; //-std::numeric_limits<double>::infinity();
		}

		if (!takeLog)
			return evaluateCdf(sample);
		else
			return log(evaluateCdf(sample));
	}

	// If no undiffed vars then return density
	if (numUndiffed - numMarged == 0) {
		if (!takeLog)
			return evaluateDensity(sample);
		else
			return log(evaluateDensity(sample));
	}

	// Cache parameters if necessary
	cacheParameters();

	// Subtract _muSigma * a from b
	new (&_aVec) Map<VectorXd>(_a, numDiffed);
	new (&_bVec) Map<VectorXd>(_b, numUndiffed);
	new (&_cVec) Map<VectorXd>(_c, numUndiffed);

	/*cout << "diffed vars" << endl << _aVec << endl;
	cout << "undiffed vars" << endl << _bVec << endl;
	cout << "muSigma" << endl << _cVec << endl;*/

	_cVec = _muSigmas[isDiffed._subset - 1] * _aVec;
	_bVec -= _cVec;

	//cout << "adjusted undiffed vars" << endl << _bVec << endl << endl;

	double tempValue = standardCdf(_b, &_coefs[_correlIndex[isDiffed._subset]], numUndiffed, true);

	if (&cdfValue != 0)
		cdfValue = tempValue;

	// Calculate partial derivative of CDF
	double result = standardDensity(_a, &_coefs[_precisionIndex[_numSubsets - isDiffed._subset]], _normFactors[_numSubsets - isDiffed._subset], numDiffed) * tempValue;
	if (!takeLog)
		return result;
	else
		return log(result);
}