/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnNormCopula.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include <boost/random.hpp>

using namespace std;
using namespace Eigen;
using boost::math::normal;



namespace drwnCumulativeUtils
{
	extern boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die;
	extern double logSumExp(const double&, const double&);
};

class drwnNormCopula : public drwnCdf	//: public drwnStdObjIface 
{
	// Must Declare these first, because they're needed in protected block
public:
	public:
	//! Domain type
	typedef double domain_type;

	//! Range type (should match domain type of marginal)
	typedef double range_type;

	//! Type for copula parameter
	typedef double parameter_type;

protected:
	//! Memory used to store correlation and precision coefficients
	domain_type* _coefs;

	//! Memory used to store moments of truncated normal distribution
	domain_type* _moments;

	//! Maps subsets of variables into correlation and standard deviation coefficients in _coefs
	unsigned int* _correlIndex;

	//! Maps subsets of variables into precision coefficients in _coefs
	unsigned int* _precisionIndex;

	//! Number of parameters in this copula (number of correlation coefficients)
	unsigned int _numParameters;

	//! Number of coefficients in _coefs
	unsigned int _numCoefs;

	//! Corresponding preallocated memory
	domain_type* _sigmaXXmem, *_sigmaYXmem, *_sigmaYYmem, *_sigmaYgivenXmem, *_sigmaInvXXmem;

	//! Maps to variance submatrices
	Map<MatrixXd> _sigmaXX, _sigmaYX, _sigmaYY, _sigmaYgivenX, _sigmaInvXX;

	//! Array of products of Sigma(YX) * inv(Sigma(XX))
	MatrixXd* _muSigmas;

	//! Memory to store subsets of values
	domain_type* _a, *_b, *_c, *_precisionZ, *_d;

	//! Maps to vector of values
	Map<VectorXd> _aVec, _bVec, _cVec;
	
	//! List of zeros to pass to Fortan routine
	int* _zeros;

	//! Normalization factors
	domain_type* _normFactors;

	//! Cached partial derivatives
	range_type* _partialDerivs;

	//! Number of (nonempty) subsets of full covariance matrix
	unsigned int _numSubsets;

	//! Flag to indicate whether we need to build cached parameters
	bool _isCacheValid;

	//! Normal distribution
	static mynormal _norm;	// Standard normal distribution.

	//! Fill vector of variable values from whole sample
	void extractValues(domain_type* sample);

	//! Evaluate multivariate normal CDF with zero mean and given correlation coefficients and possibly non-unity standard deviations
	range_type standardCdf(domain_type* values, domain_type* correl, unsigned int n, bool hasSd = false);

	//! Evaluate multivariate normal density with zero mean and arbitrary precision coefficients
	range_type standardDensity(domain_type* values, domain_type* precision, domain_type normFactor, unsigned int n);

	//! Evaluate derivative of probit function
	static range_type derivativeProbit(domain_type value);

	//! Cache precalculated parameters
	void cacheParameters();
		
	//! Initialize variables
	void initialize();

	//! Free all memory held with object
	void destroy();

	//! Calculate the single moments of truncated normal distribution
	void calculateTruncSingleMoments(domain_type* sample, drwnSubset& diffedVars);

	//! Calculate the first and second order moments of truncated normal distribution
	void calculateTruncMoments(domain_type* sample, drwnSubset& diffedVars);

public:
	//! Whether copula produced by transformation method
	// TODO: Can make this better with a clever metaprogramming template trick?
	// I think this should be a type, not a method!
	static bool isTransformCopula() { return true; }

	//! Inverse of copula CDF to transform argument (for transformation copulae)
	static domain_type transformArgument(domain_type value) { return quantile(_norm, value); }

	//! Derivative of inverse of copula CDF (for transformation copulae)
	static domain_type derivativeArgument(domain_type value) { return derivativeProbit(value); }

	//! Value that represents a marginalized value for this copulas
	static domain_type marginalizedValue() { return std::numeric_limits<double>::infinity(); }

    //! Create an empty copula
    drwnNormCopula();

	//! Create copula with given variables
	drwnNormCopula(const drwnClique& c);

    //! Copy constructor
    //drwnNormCopula(const drwnNormCopula& f);

	//! Default destructor
	~drwnNormCopula();

	//! Return type
    virtual const char *type() const { return "drwnNormCopula"; }

	static const char* name() { return "Normal"; }

    //! Make copy
    virtual drwnNormCopula *clone() const { return new drwnNormCopula(*this); }

    //! Add multiple variables
    void setVariables(const drwnClique& c);
    
	// TODO: Instead of passing whole sample and having to extract entries every time,
	// make function accept vector values, then the calling function can be responsible for
	// extracting variables (and only do this once!)
	// TODO: Or make a sample object that caches subsets of samples itself as needed!

    //! Evaluate copula CDF of sample
	range_type evaluateCdf(domain_type* sample, bool takeLog = false);

	//! Evaluate copula density of sample
	range_type evaluateDensity(domain_type* sample);
	
	//! Calculate a partial derivative of copula, evaluated at sample
	range_type partialDerivative(domain_type* sample, drwnSubset& isDiffed, bool takeLog = false, range_type& cdfValue = *((drwnNormCopula::range_type*) 0));

	//! Retrieve a cached partial derivative
	range_type cachedDerivative(drwnSubset& isDiffed, bool diffParameter = false) const 
	{
		if (!diffParameter)
			return _partialDerivs[isDiffed._subset];
		else
			return _partialDerivs[_numSubsets + 1 + isDiffed._subset];
	}

	//! Calculate and cache all partial derivatives evaluated at sample. Optionally calculate gradients as well
	void cacheDerivatives(domain_type* sample, bool gradientAlso = false, bool takeLog = false, bool printDerivatives = false);

	//! Set correlation
	void setParameter(parameter_type correl);

	//! Get correlation
	parameter_type getParameter() { return _coefs[0]; }

	//! Set parameter to random value
	void randomizeParameter() {
		setParameter((drwnCumulativeUtils::die() * (1.0 + 1.0 / ((double)size() - 1.0))) - 1.0 / ((double)size() - 1.0));
	}

	//! Check that parameter is within bounds
	bool isParameterValid(const parameter_type& p) {
		return (p > -1.0 / ((double)size() - 1.0) && p < 1.0);
	}

	//! Calculate barrier function term
	double barrierFunction(double t)
	{
		//return -1.0 / t * (log(1 / (getParameter() + 1.0 / (size() - 1.0))) + log(1.0 - getParameter()));
		// NOTE: Think line above is incorrect...
		//return -1.0 / t * (log(getParameter() + 1.0 / (size() - 1.0)) + log(1.0 - getParameter()));
		return -1.0 / t * (log(((double)size() - 1.0) / (1.0 + getParameter() * ((double)size() - 1.0))) + log(1.0 - getParameter()));
	}

	//! Calculate derivative of barrier function term
	double barrierDerivative(double t)
	{
		return 1.0 / t * (1.0 / (1.0 - getParameter()) - ((double)size() - 1.0) / (1.0 + getParameter() * ((double)size() - 1.0)));
	}

	//! Return number of parameters
	unsigned int numParameters() const { return _numParameters; }
};