/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnFrankCopula.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include <boost/random.hpp>

using namespace std;
using boost::math::normal;

namespace drwnCumulativeUtils
{
	extern boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die;
};

// TODO: A drwnArchimedeanCopula parent class
class drwnFrankCopula : public drwnCdf	//: public drwnStdObjIface
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
	//! Cached partial derivatives
	range_type* _partialDerivs;

	//! 2 ^ (how many variables)
	unsigned int _numSubsets;

	//! Copula parameter
	parameter_type theta;

	//! Initialize variables
	void initialize();

	//! Free all memory held with object
	//void destroy();

private:
	//! Calculate generator or n-th derivative
	range_type generator(range_type t, unsigned int n = 0);

	//! Calculate inverse generator or n-th derivative
	range_type inverseGenerator(range_type t);

public:
	//! Whether copula produced by transformation method
	// TODO: Can make this better with a clever metaprogramming template trick?
	// I think this should be a type, not a method!
	static bool isTransformCopula() { return false; }

	//! Inverse of copula CDF to transform argument (for transformation copulae)
	static domain_type transformArgument(domain_type value) { return 1.0; }

	//! Derivative of inverse of copula CDF (for transformation copulae)
	static domain_type derivativeArgument(domain_type value) { return 1.0; }

	//! Value that represents a marginalized value for this copulas
	static domain_type marginalizedValue() { return 1.0; }

	//! Evaluate copula CDF of sample
	range_type evaluateCdf(domain_type* sample);

    //! Create an empty copula
    drwnFrankCopula();

	//! Create copula with given variables
	drwnFrankCopula(const drwnClique& c);

    //! Copy constructor
    //drwnNormCopula(const drwnNormCopula& f);

	//! Default destructor
	~drwnFrankCopula();

	//! Return type
    virtual const char *type() const { return "drwnFrankCopula"; }

    //! Make copy
    virtual drwnFrankCopula *clone() const { return new drwnFrankCopula(*this); }

    //! Add multiple variables
    void setVariables(const drwnClique& c);
    
	// TODO: Instead of passing whole sample and having to extract entries every time,
	// make function accept vector values, then the calling function can be responsible for
	// extracting variables (and only do this once!)
	// TODO: Or make a sample object that caches subsets of samples itself as needed!

    //! Evaluate copula CDF of sample
	//range_type evaluateCdf(domain_type* sample);

	//! Evaluate copula density of sample
	//range_type evaluateDensity(domain_type* sample);
	
	//! Calculate a partial derivative of copula, evaluated at sample
	range_type partialDerivative(domain_type* sample, drwnSubset& isDiffed);

	//! Retrieve a cached partial derivative
	range_type cachedDerivative(drwnSubset& isDiffed, bool diffParameter = false) const 
	{
		if (!diffParameter)
			return _partialDerivs[isDiffed._subset];
		else
			return _partialDerivs[_numSubsets + isDiffed._subset];
	}

	//! Calculate and cache all partial derivatives evaluated at sample. Optionally calculate gradients as well
	void cacheDerivatives(domain_type* sample, bool gradientAlso = false);

	//! Set correlation
	void setParameter(parameter_type newTheta) { theta = newTheta; }

	//! Get correlation
	parameter_type getParameter() { return theta; }

	//! Set parameter to random value
	// TODO: How to sensibly set parameter?
	void randomizeParameter() {
		// Can be negative for dim = 2
		if (size() == 2) {
			setParameter((drwnCumulativeUtils::die() * 600) - 300);
		}

		else {
			setParameter((drwnCumulativeUtils::die() * 300));
		}
	}

	//! Return number of parameters
	unsigned int numParameters() const { return 1; }
};