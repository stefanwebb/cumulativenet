/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCopulaSamples.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//#include "drwnCumulativeNet.h"

using namespace std;

template <class G> class drwnCliqueTree;

//! This class is a wrapper for holding the transformed samples in order to learn the copulae.
template <class G>
class drwnCopulaSamples
{
private:
	// Initialize structures
	void initialize(drwnSamples<G>& samples, G& cn, bool learnMarginals);

	//! Memory for samples
	typename G::factor_type::domain_type* copulaSamples_;

	//! Memory for transform of sample
	typename G::factor_type::range_type* tempSamples_;

	//! Memory for gradient
	//V* gradient_;

	//! Number of variables in sample
	unsigned int numVars_;

	//! Number of samples in this object (note that a sample can be represented as a number of these objects if more convenient)
	unsigned int numSamples_;

public:
	//! Construct copula samples by transforming samples in sample object
	drwnCopulaSamples(drwnSamples<G>& samples, G& cn, bool learnMarginals = false);

	//! Convenience method that constructs copula samples from the model associated with a clique tree
	drwnCopulaSamples(drwnSamples<G>& samples, drwnCliqueTree<G>& cn, bool learnMarginals = false);
	
	//! Default destructor
	~drwnCopulaSamples();

	//! Returns number of samples in object
	unsigned int numSamples() const { return numSamples_; }

	//! Returns number of variables in a sample
	unsigned int numVars() const { return numVars_; }

	//! Marginalize variable
	void marginalizeVariable(unsigned int i)
	{
		DRWN_ASSERT_MSG(i < numVars_, "Invalid variable number to marginalize.")

		// Marginalizing a variable means setting it to 1
		for (unsigned int j = 0; j < numSamples_; j++) {
			if (G::factor_type::isTransformCopula())
				sample(j)[i] = G::factor_type::transformArgument(1.0);
			else
				sample(j)[i] = 1.0;
		}
	}

	//! Make a given percentage of data missing
	void makeMissing(double fraction)
	{
		DRWN_ASSERT_MSG(fraction > 0 && fraction < 1, "Invalid fraction of data to make missing.")
		unsigned int numEntries = numVars_ * numSamples_, numToClear = ceiling((double) numEntries * (double) fraction);

		// Calculate marginalized value
		G::factor_type::domain_type margValue;
		if (G::factor_type::isTransformCopula())
			margValue = G::factor_type::transformArgument(1.0);
		else
			margValue = 1.0;

		// Continue until we have cleared correct number of samples
		// NOTE: This could go on forever if variables have been marginalized already, and if fraction is high it will take a long time
		// TODO: Better method for selecting fraction of data
		while(numToClear) {
			unsigned int randEntry = rand() % numEntries;
			if (copulaSamples_[randEntry] != margValue) {
				copulaSamples_[randEntry] = margValue;
				numToClear--;
			}
		}

	}

	//! Return reference to start of sample
	typename G::factor_type::domain_type* operator[](int i) { return &copulaSamples_[numVars() * i]; }

	//! Get sample
	typename G::factor_type::domain_type* const sample(unsigned int i) const { return &copulaSamples_[numVars() * i]; }

	//! Get memory to store temporary sample
	typename G::factor_type::range_type* temp() const { return tempSamples_; }

	//! Get memory to store temporary transform of sample
	typename G::factor_type::range_type* transform() const { return &tempSamples_[numVars()]; }

	//! Get memory to store temporary sample transformed for copula
	typename G::factor_type::range_type* copula() const { return &tempSamples_[numVars() << 1]; }

	//! Print samples to csv file
	void print(const G& cn, const wchar_t* filename);

};

// Print samples to csv file
template <class G>
void drwnCopulaSamples<G>::print(const G& cn, const wchar_t* filename)
{
	// Open file
	wofstream myfile;
	myfile.open(filename);
	myfile.precision(std::numeric_limits<double>::digits10);

	// Check that we were able to open file
	DRWN_ASSERT_MSG(myfile.is_open(), "Unable to open file")
		
	// Write first column with variable names
	for (int i = 0; i < cn.numVariables(); i++) {
		myfile << cn.getUniverse()->varName(i).c_str();
		if (i < cn.numVariables() - 1)
			myfile << " ";
		else
			myfile << endl;
	}

	// Generate samples
	for (unsigned int i = 0; i < numSamples(); i++) {
		// Print sample to file
		for (int j = 0; j < cn.numVariables(); j++) {
			myfile << sample(i)[j];
			if (j < cn.numVariables() - 1)
				myfile << " ";
			else if (i != numSamples() - 1)
				myfile << endl;
		}
	}

	// Close file
	myfile.close();
}

// Construct copula samples by transforming samples in sample object
template <class G>
drwnCopulaSamples<G>::drwnCopulaSamples(drwnSamples<G>& samples, G& cn, bool learnMarginals)
{
	initialize(samples, cn, learnMarginals);
}

// Initialize structures
template <class G>
void drwnCopulaSamples<G>::initialize(drwnSamples<G>& samples, G& cn, bool learnMarginals)
{
	// Learn marginals if necessary
	if (learnMarginals) {
		// Loop over variables in model
		for (int i = 0; i < cn.numVariables(); i++) {
			// Learn the marginal parameters from samples
			// NOTE: May want to put a breakpoint here to verify
			cn.marginal(i).learn(i, samples);
		}
	}
	
	// Assign memory
	numVars_ = cn.numVariables();
	numSamples_ = samples.numSamples();
	if (numSamples_)
		copulaSamples_ = new typename G::factor_type::domain_type[numVars_ * numSamples_];
	else
		copulaSamples_ = 0;
	tempSamples_ = new typename G::factor_type::range_type[numVars_ * 3];
	
	// Calculate samples
	for (unsigned int j = 0, offset = 0; j < numSamples_; j++, offset += numVars_) {
		// Transform sample according to marginal CDF then raise to power of 1 / how many times variable appears
		for (unsigned int i = 0; i < numVars_; i++) {
			typename G::factor_type::domain_type tempValue = cn.marginal(i).cdf((samples[j])[i]);

			// When copula is formed by derivative method, we need to transform arguments by inverse CDF
			if (G::factor_type::isTransformCopula()) {
				tempValue = pow(tempValue, 1.0 / ((double) cn.howManyScopes(i)));
				copulaSamples_[offset + i] = G::factor_type::transformArgument(tempValue);
			}
			else
				copulaSamples_[offset + i] = pow(tempValue, 1.0 / ((double) cn.howManyScopes(i)));
		}	
	}
}

// Convenience method that constructs copula samples from the model associated with a clique tree
template <class G>
drwnCopulaSamples<G>::drwnCopulaSamples(drwnSamples<G>& samples, drwnCliqueTree<G>& cn, bool learnMarginals) 
{
	initialize(samples, cn.model(), learnMarginals);
}

// Default destructor
template <class G>
drwnCopulaSamples<G>::~drwnCopulaSamples()
{
	if (copulaSamples_)
		delete [] copulaSamples_;
	delete [] tempSamples_;
}