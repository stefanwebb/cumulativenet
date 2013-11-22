/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnExponentialMarginal.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

class drwnExponentialMarginal
{
public:
	//! Domain type
	typedef double domain_type;

	//! Range type
	typedef double range_type;

protected:
	//! Mean
	domain_type _lambda;

public:
	//! Default construct. Initializes to standard normal.
    drwnExponentialMarginal();

    //! Copy constructor
    drwnExponentialMarginal(const drwnExponentialMarginal& m);

	//! Default destructor
    ~drwnExponentialMarginal();

	//! Initilize to standard normal
	void initialize();

	//! Return type
    virtual const char *type() const { return "drwnExponentialMarginal"; }

    //! Make copy of Bayesian network from existing one
    virtual drwnExponentialMarginal *clone() const { return new drwnExponentialMarginal(*this); }

    //! number of bytes comsumed by the factor data (excludes variables lists, etc)
    size_t memory() const { return sizeof(drwnExponentialMarginal); }

	//! Get mean of distribution
	domain_type getLambda() { return _lambda; }

	//! Set mean of distribution
	void setLambda(domain_type lambda) { _lambda = lambda; }

	//! Evaluate density
	range_type density(domain_type x) const;

	//! Evaluate cumulative distribution function
	range_type cdf(domain_type x) const;

	//! Evaluate quantile (inverse CDF)
	domain_type quantile(range_type x) const;

	//! Function to learn parameters by MLE from sample
	template <class G>
	void learn(unsigned int varIndx, drwnSamples<G>& samples);
};

//! Function to learn parameters by MLE from sample
template <class G>
void drwnExponentialMarginal::learn(unsigned int varIndx, drwnSamples<G>& samples)
{
	// Calculate mean of sample, avoiding overflow
	_lambda = 0;
	for (unsigned int i = 0; i < samples.numSamples(); i++)
		_lambda += samples.sample(i)[varIndx] / samples.numSamples();

	// Lambda is one over the mean
	_lambda = 1.0 / _lambda;
}