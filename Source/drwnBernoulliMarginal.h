/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnBernoulliMarginal.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

template <class G> class drwnSamples;

class drwnBernoulliMarginal
{
public:
	//! Domain type
	// NOTE: I am worried about bugs if I make domain integer, hence set it to double
	//typedef unsigned int domain_type;
	typedef double domain_type;

	//! Range type
	typedef double range_type;

protected:
	//! Theta parameter
	range_type _theta;

public:
	//! Default construct. Initializes to standard normal.
    drwnBernoulliMarginal();

    //! Copy constructor
    drwnBernoulliMarginal(const drwnBernoulliMarginal& m);

	//! Default destructor
    ~drwnBernoulliMarginal();

	//! Initilize to standard normal
	void initialize();

	//! Return type
    virtual const char *type() const { return "drwnBernoulliMarginal"; }

    //! Make copy of Bayesian network from existing one
    virtual drwnBernoulliMarginal *clone() const { return new drwnBernoulliMarginal(*this); }

    //! number of bytes comsumed by the factor data (excludes variables lists, etc)
    size_t memory() const { return sizeof(drwnBernoulliMarginal); }

	//! Get theta of distribution
	range_type getTheta() { return _theta; }

	//! Set theta of distribution
	void setTheta(range_type theta) { _theta = theta; }

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
void drwnBernoulliMarginal::learn(unsigned int varIndx, drwnSamples<G>& samples)
{
	// Sum all instances of this variable
	unsigned int sumVar = 0;
	for (unsigned int i = 0; i < samples.numSamples(); i++)
		sumVar += samples.sample(i)[varIndx];
	
	// MLE theta is proportion of variable equal to 1
	_theta = (double) sumVar / (double) samples.numSamples();
}