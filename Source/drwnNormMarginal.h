/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnNormMarginal.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;
using boost::math::normal;

template <class G> class drwnSamples;

class drwnNormMarginal	//: public drwnStdObjIface
{
public:
	//! Domain type
	typedef double domain_type;

	//! Range type
	typedef double range_type;

protected:
	//! Mean
	domain_type _mean;

	//! Standard deviation
	domain_type _sd;

	//! Normal distribution
	static mynormal _norm;	// Standard normal distribution.

public:
	//! Default construct. Initializes to standard normal.
    drwnNormMarginal();

    //! Copy constructor
    drwnNormMarginal(const drwnNormMarginal& m);

	//! Default destructor
    ~drwnNormMarginal();

	//! Initilize to standard normal
	void initialize();

	//! Return type
    virtual const char *type() const { return "drwnNormMarginal"; }

    //! Make copy of Bayesian network from existing one
    virtual drwnNormMarginal *clone() const { return new drwnNormMarginal(*this); }

    //! number of bytes comsumed by the factor data (excludes variables lists, etc)
    size_t memory() const { return sizeof(drwnNormMarginal); }

	//! Get mean of distribution
	domain_type getMean() { return _mean; }

	//! Get standard deviation of distribution
	domain_type getStandardDeviation() { return _sd; }

	//! Set mean of distribution
	void setMean(domain_type mean) { _mean = mean; }

	//! Set standard deviation of distribution
	void setStandardDeviation(domain_type sd) { _sd = sd; }

	//! Evaluate density
	range_type density(domain_type x) const;

	//! Evaluate cumulative distribution function
	range_type cdf(domain_type x) const;

	//! Evaluate quantile (inverse CDF)
	domain_type quantile(range_type x) const;

	//! Reset parameters to defaults
	void reset()
	{
		_mean = 0;
		_sd = 1.0;
	}

	//! Function to learn parameters by MLE from sample
	template <class G>
	void learn(unsigned int varIndx, drwnSamples<G>& samples);
};

//! Function to learn parameters by MLE from sample
template <class G>
void drwnNormMarginal::learn(unsigned int varIndx, drwnSamples<G>& samples)
{
	// Calculate mean of x_i and x_i^2 
	range_type meanVar = 0, meanSquaredVar = 0;
	unsigned int numSamples = 0;
	for (unsigned int i = 0; i < samples.numSamples(); i++) {
		if (!boost::math::isinf(samples.sample(i)[varIndx])) {
			meanVar += samples.sample(i)[varIndx] / (double) samples.numSamples();
			meanSquaredVar += pow(samples.sample(i)[varIndx], 2.0) / (double) samples.numSamples();
			numSamples++;
		}
	}

	// Adjust if missing samples
	if (numSamples != samples.numSamples()) {
		meanVar *= (double) samples.numSamples() / (double) numSamples;
		meanSquaredVar *= (double) samples.numSamples() / (double) numSamples;
	}

	//meanSquaredVar /= (double) samples.numSamples();
	
	// Set mean and standard deviation
	_mean = meanVar;

	// NOTE: In samples where all variables take same value, then SD will evaluate as IND. So set to a small number.
	_sd = sqrt((double) numSamples / ((double) numSamples - 1.0) * (meanSquaredVar - pow(meanVar, 2.0)));
	if (boost::math::isnan(_sd))
		_sd = 1e-1;
}