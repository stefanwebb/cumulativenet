/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCumulativeUtils.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

template<class G>
class drwnSampleCliques;

template<class G>
class drwnSamples;

// A stack term top-down dynamic programming
// TODO: Put in anonymous namespace?
struct derivTerm {
	drwnSubset A;
	drwnSubset B;
	unsigned int i;		// How many factors in this term
	bool isInspected;	// Whether this term has been examined before
};

//! Utility functions operating on a Cumulative Distribution network
namespace drwnCumulativeUtils
{
	// Calculate the gradient
	template<class G>
	typename G::factor_type::range_type calculateGradient(drwnCliqueTree<G>& ct, typename G::factor_type::domain_type* sample, typename G::factor_type::domain_type* gradient, bool takeLog);

	// Calculate the gradient over a subset of model
	// NOTE: This method has nothing to do with subgradients!
	template<class G>
	typename G::factor_type::range_type calculateSubgradient(drwnCliqueTree<G>& ct, const vector<pair<unsigned int, unsigned int> >& optFactors, typename G::factor_type::domain_type* sample, typename G::factor_type::domain_type* gradient, bool takeLog);

	// Calculate all messages in clique tree (i.e. do two passes)
	// NOTE: It makes no sense for this method to work in log-space since its purpose is to calculate gradient efficiently)
	template<class G>
	void calibrateCliqueTree(drwnCliqueTree<G>& ct, void (*f)(drwnCliqueTree<G>&, int, int, typename G::factor_type::domain_type*, bool, bool, int), typename G::factor_type::domain_type* sample, bool takeLog);

	//! Product rule to calculate copula density of cumulative network over clique tree (which may be over subset of variables)
	template<class G>
	typename G::factor_type::range_type copulaDensity(drwnCliqueTree<G>& cliqueTree, drwnSamples<G>& samples, bool useCached = false, bool takeLog = false, const vector<int>& extras = (*(vector<int>*)0));

	//! Product rule to calculate copula (not copula density) of single variable.
	template<class G>
	typename G::factor_type::range_type copulaMarginal(G& g, int var, drwnSamples<G>& samples);

	//! Debug message calculator
	template<class G>
	void debugCalculateMsg(drwnCliqueTree<G>& cliqueTree, int i, int j, typename G::factor_type::domain_type* sample, bool useCached, int diffFactor = -1);

	//! Product rule to calculate density P(x) of cumulative network over clique tree (which may be over subset of variables)
	// NOTE: Set other variables to +Inf etc. to marginalize out if over subset of variables.
	template<class G>
	typename G::factor_type::range_type density(drwnCliqueTree<G>& cliqueTree, drwnSamples<G>& samples, unsigned int num, bool takeLog = false, const vector<int>& extras = (*(vector<int>*)0));

	//! Differentiate product of clique factors in clique i with respect to all subsets of some set of variables
	template<class G>
	void diffClique(drwnCliqueTree<G>& cliqueTree, int i, int j, typename G::factor_type::range_type* output, typename G::factor_type::domain_type* sample, bool useCached, bool takeLog = false, int diffFactor = -1);

	//! Calculate the derivative of drwnPolyCopula factors for debugging
	template<class G>
	void diffDebugFactors(drwnCliqueTree<G>& ct, int i, int j, typename G::factor_type::range_type* output, typename G::factor_type::domain_type* sample);

	//! Calculate derivative (evaluated at sample) of all drwnPolyCopula factors in clique tree
	template<class G>
	void diffDebugModel(drwnCliqueTree<G>& ct, typename G::factor_type::domain_type* sample, typename G::factor_type::range_type result);

	//! Draw samples from network and save to memory
	template <class G>
	drwnSamples<G>& drawSamples(const G& cn, unsigned int numSamples);

	//! Derivative-sum-product Message Calculate
	template<class G>
	void dspCalculateMsg(drwnCliqueTree<G>& cliqueTree, int i, int j, typename G::factor_type::domain_type* sample, bool useCached, bool takeLog = false, int factorDiff = -1);

	//! Evaluate factors at sample and multiply together
	template<class G, class S, class T>
	typename G::factor_type::range_type evaluateFactors(const G& g, const S& set, typename T* sample, bool takeLog);

	//! Calculate (per sample) log-likelihood and gradient of log-likelihood for full sample
	template<class G>
	typename G::factor_type::range_type likelihoodGradient(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, typename G::factor_type::domain_type* gradient, typename G::factor_type::domain_type* gradientSum, bool takeLog);

	//! Pass messages through clique tree
	template<class G>
	typename G::factor_type::range_type passMessages(drwnCliqueTree<G>& cliqueTree, void (*f)(drwnCliqueTree<G>&, int, int, typename G::factor_type::domain_type*, bool, bool, int), typename G::factor_type::domain_type* sample, bool useCached = false, bool takeLog = false, bool hasMarginalized = false);

	//! Pass messages through clique tree to given root
	template<class G>
	typename G::factor_type::range_type passMessages(drwnCliqueTree<G>& cliqueTree, void (*f)(drwnCliqueTree<G>&, int, int, typename G::factor_type::domain_type*, bool, bool, int), typename G::factor_type::domain_type* sample, unsigned int cliqueIndx, bool useCached = false, bool takeLog = false);

	//! Print clique tree. Used for debug
	template <class G>
	void printCliqueTree(drwnCliqueTree<G>& cliqueTree);

	//! Print clique tree array
	template <class G>
	void printSampleCliques(drwnSampleCliques<G>& sc);

	//! Print samples to csv file
	template <class G>
	bool printSamples(const G& cn, unsigned int numSamples, const wchar_t* filename);

	//! Sample using sampling clique trees structure (using Brent's method)
	template <class G>
	void sample(drwnSampleCliques<G>& sc, drwnSamples<G>& samples, int samplenum = -1, bool takeLog = false);

	//! Sample using sampling clique trees structure (and bisection / Newton's method)
	template <class G>
	void sample2(drwnSampleCliques<G>& sc, drwnSamples<G>& samples, int sampleNum = -1);

	//! Learn the copula parameters using optimizer class C
	//template<class G, class C>
	//void learn(drwnCliqueTree<G>& ct, drwnCopulaSamples<G> copulaSamples, wchar_t* solnOutput = L"solutions.txt", wchar_t* llOutput = L"loglikelihoods.txt", const double eps = 1e-6, wchar_t* gradOutput = L"gradients.txt");

	//! Find root of functor using Brent's method
	template <typename F>
	double zbrent(F& func, const double x1, const double x2, const double tol);

	//! Find root of functor using a safe Newton's method (i.e. combining with bisection)
	template <class F>
	double rtsafe(F& func, const double x1, const double x2, const double xacc);
	
	//! Log-sum-exp function, for performing addition of 2 numbers in log-space
	double logSumExp(const double&, const double&);
	
	//! Log-sum-exp function, for performing addition of n numbers in log-space
	// TODO: Version for Eigen vector
	double logSumExp(const double*, const unsigned int);

	template <class G>
	void printGradient(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& copulaSamples, bool takeLog);

	//! Random number generator;
	extern boost::mt19937 rng;

	//! Uniform distribution on [0, 1)
	extern boost::uniform_real<> unif;
	//boost::uniform_01<> unif2();

	//! Random number generator
	extern boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die;

	// DEBUG: Flag for breakpoint
	extern bool stopYet;
	extern bool stopYet2;
};

template <class G>
void drwnCumulativeUtils::printGradient(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& copulaSamples, bool takeLog = false)
{
	// DEBUG: Calculate gradient of full model
	unsigned int n = ct.model().numFactors();
	double* df = new double[n];
	double* dfSingle = new double[n];
	double* x = new double[n];
	for (unsigned int i = 0; i < n; i++) {
		x[i] = ct.model().factor(i).getParameter();
	}

	// Initialize log-likelihood and gradient
	double negLogLikelihood = 0.0;
	for (unsigned int i = 0; i < n; i++)
		df[i] = 0.0;

	// Calculate gradient for each sample
	for (unsigned int j = 0; j < copulaSamples.numSamples(); j++) {
		negLogLikelihood -= drwnCumulativeUtils::calculateGradient(ct, copulaSamples[j], (typename G::factor_type::domain_type*) dfSingle, takeLog) / (double) copulaSamples.numSamples();
		for (unsigned int i = 0; i < n; i++)
			df[i] -= dfSingle[i] / (double) copulaSamples.numSamples();	// Take average to avoid overflow
	}

	// Print gradient
	wcout << "Gradient ";
	for (unsigned int i = 0; i < n; i++)
		wcout << df[i] << ", ";
	wcout << endl;

	delete [] df;
	delete [] x;
	delete [] dfSingle;
}

// Calculate the gradient
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::calculateGradient(drwnCliqueTree<G>& ct, typename G::factor_type::domain_type* sample, typename G::factor_type::domain_type* gradient, bool takeLog)
{
	// Calibrate messages in clique tree, so can calculate density
	calibrateCliqueTree(ct, drwnCumulativeUtils::dspCalculateMsg<G>, sample, takeLog);

	// Get density
	// TODO: Would be faster to choose smallest clique for this
	drwnCumulativeUtils::dspCalculateMsg<G>(ct, 0, -1, sample, true, takeLog);

	G::factor_type::range_type den;
	if (!ct.isMarged())
		den = ct.lastMessage(0);
	else
		den = ct.message(0, -1, ct.mask(0));
	
	// NOTE: density may be negative when very small due to rounding errors
	if (den < 0 && !takeLog)
		den = abs(den);

	// NOTE: Don't reset messages for log-space, since we are doing clique with gradient in normal-space (grad may be negative)
	//ct.resetFinalMessages(0);

	// DEBUG: Print sample
	/*wcout << "copula sample" << endl;
	for (unsigned int i = 0; i < ct.model().numVariables(); i++) {
		wcout << sample[i] << " ";
	}
	wcout << endl;*/

	// DEBUG: Convoluted debug flags
	if (stopYet == true) {
		stopYet = false;
		stopYet2 = true;
	}
	
	// Only calculate gradient if nonzero address passed in (to speed up just calculating objective)
	if (gradient) {
		// Loop over cliques
		for (unsigned int i = 0; i < ct.numCliques(); i++) {
			// Loop over factors in clique
			for (unsigned int j = 0; j < ct.numFactors(i); j++) {
				// Calculate derivative of log density with respect to copula parameter (see thesis for formula)
				int thisFactor = ct.factors(i)[j];
				ct.resetFinalMessages(i);
				drwnCumulativeUtils::dspCalculateMsg<G>(ct, i, -1, sample, true, takeLog, thisFactor);
				G::factor_type::range_type grad;
				if (!ct.isMarged())
					grad = ct.lastMessage(i);
				else
					grad = ct.message(i, -1, ct.mask(i));
				if (!takeLog)
					gradient[thisFactor] = grad / den;
				else
					gradient[thisFactor] = grad * exp(-den);
				//ct.resetFinalMessages(i);
			}
		}
	}

	// Normalize gradient?
	// NOTE: It appears changing norm of gradient stuffs up L-BFGS and also makes gradient descent oscillate around solution...
	// I.e. not a good idea! May want to calculate maximum step size in line search instead.
	/*G::factor_type::range_type normGradient = 0;
	for (unsigned int i = 0; i < ct.model().numFactors(); i++)
		normGradient += gradient[i] * gradient[i];
	normGradient = sqrt(normGradient);
	for (unsigned int i = 0; i < ct.model().numFactors(); i++)
		gradient[i] /= normGradient;*/

	if (!takeLog)
		return log(den);
	else
		return den;
}

// Calculate the gradient over a subset of model
// NOTE: This method has nothing to do with subgradients!
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::calculateSubgradient(drwnCliqueTree<G>& ct, const vector<pair<unsigned int, unsigned int> >& optFactors, typename G::factor_type::domain_type* sample, typename G::factor_type::domain_type* gradient, bool takeLog)
{
	// Calibrate messages in clique tree, so can calculate density
	calibrateCliqueTree(ct, drwnCumulativeUtils::dspCalculateMsg<G>, sample, takeLog);

	// Get density
	// TODO: Would be faster to choose smallest clique for this
	drwnCumulativeUtils::dspCalculateMsg<G>(ct, 0, -1, sample, true, takeLog);

	G::factor_type::range_type den = ct.lastMessage(0);
		
	// NOTE: density may be negative when very small due to rounding errors
	if (den < 0 && !takeLog)
		den = abs(den);

	// Only calculate gradient if nonzero address passed in (to speed up just calculating objective)
	if (gradient) {
		// Loop over cliques
		unsigned int gradEntry = 0;
		for (auto& optPair : optFactors) {
			// Calculate derivative of log density with respect to copula parameter (see thesis for formula)
			int thisFactor = ct.factors(optPair.first)[optPair.second];
			ct.resetFinalMessages(optPair.first);
			drwnCumulativeUtils::dspCalculateMsg<G>(ct, optPair.first, -1, sample, true, takeLog, thisFactor);
			G::factor_type::range_type grad = ct.lastMessage(optPair.first);
			if (!takeLog)
				gradient[gradEntry] = grad / den;
			else
				gradient[gradEntry] = grad * exp(-den);
			gradEntry++;
		}
	}

	if (!takeLog)
		return log(den);
	else
		return den;
}

// Calculate all messages in clique tree (i.e. do two passes)
template<class G>
void drwnCumulativeUtils::calibrateCliqueTree(drwnCliqueTree<G>& ct, void (*f)(drwnCliqueTree<G>&, int, int, typename G::factor_type::domain_type*, bool, bool, int), typename G::factor_type::domain_type* sample, bool takeLog)
{
	// Zero temporary messages (and regular messages?)
	ct.resetMessages(takeLog);

	// Calculate cached results for all factors in clique tree
	for (unsigned int i = 0; i < ct.numCliques(); i++) {
		for (auto j : ct.factors(i)) {
			ct.model().factor(j).cacheDerivatives(sample, true, takeLog);
		}
	}

	// Too hard to do this on disconnected clique tree (have to keep track of connected components)
	DRWN_ASSERT_MSG(ct.roots().size() == 1, "Cannot calibrate a disconnected clique tree")

	// Pass messages forward
	for (int i = 0; i < ct.numCliques(); i++) {
		if (ct.child(i) != -1)
			f(ct, i, ct.child(i), sample, true, takeLog, -1);
	}
	
	// Pass messages backwards
	for (int i = ct.numCliques() - 1; i >= 0; i--) {
		if (ct.child(i) != -1)
			f(ct, ct.child(i), i, sample, true, takeLog, -1);
	}
}

// Product rule to calculate copula density of cumulative network over clique tree (which may be over subset of variables)
// NOTE: Put values in samples.temp()
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::copulaDensity(drwnCliqueTree<G>& ct, drwnSamples<G>& samples, bool useCached, bool takeLog, const vector<int>& extras)
{
	// Flag to pass to message passing method to say when root clique contains marginalized variables
	// NOTE: If root variables are marginalized, then the final message is a different one
	bool hasMarginalizedVariables = false;

	// Raise to power of 1 / how many times variable appears
	// NOTE: When using cached derivatives, we expect these values to already be calculated (since factors are cached evaluated at these)
	if (!useCached) {
		for (unsigned int i = 0; i < samples.numVars(); i++)
			samples.copula()[i] = pow(samples.transform()[i], 1.0 / ((double) ct.model().howManyScopes(i)));
	}

	// Calculate product of terms resulting from transformation of variables
	// DEBUG: Calculating these in log-space
	//G::factor_type::range_type productTransform = 1;
	//G::factor_type::domain_type productProbit = 1;
	G::factor_type::range_type productTransform = 0;
	G::factor_type::domain_type productProbit = 0;
	for (int i = 0; i < ct.numCliques(); i++) {
		for (unsigned int j = 0; j < ct.numVars(i); j++) {
			// If this variable is in the clique minus the sepset vars (we don't want to include more than once) ...
			if ((1 << j) & ct.cliqueMinusSepset(i)._subset) {
				if (samples.transform()[ct.vars(i)[j]]) {
					// Don't multiply in terms when marginalized
					if (samples.transform()[ct.vars(i)[j]] != 1.0)
						//productTransform *= 1.0 / ((double) ct.model().howManyScopes(ct.vars(i)[j])) * samples.copula()[ct.vars(i)[j]] / samples.transform()[ct.vars(i)[j]];
						productTransform += -log(((double) ct.model().howManyScopes(ct.vars(i)[j]))) + log(samples.copula()[ct.vars(i)[j]]) - log(samples.transform()[ct.vars(i)[j]]);
					else {
						// Make a record when contains marginalized variables
						hasMarginalizedVariables = true;
					}
				}
				else
					return 0.0;
				if (G::factor_type::isTransformCopula()) {
					// If we have cached the values then we expect the final copula arguments to be in samples.temp()
					// Don't multiple in terms when marginalized
					if (samples.transform()[ct.vars(i)[j]] != 1.0)
						//productProbit *= G::factor_type::derivativeArgument(samples.copula()[ct.vars(i)[j]]);
						productProbit += log(G::factor_type::derivativeArgument(samples.copula()[ct.vars(i)[j]]));
				}
			}
		}
	}

	// When copula is formed by derivative method, we need to transform arguments by inverse CDF, and calculate product of derivative of inverse CDF
	if (G::factor_type::isTransformCopula() && !useCached) {
		for (unsigned int i = 0; i < samples.numVars(); i++)
			samples.copula()[i] = G::factor_type::transformArgument(samples.copula()[i]);
	}

	// If extra factors are specified, multiply them in
	G::factor_type::range_type extraFactors = -1;
	if (&extras != 0) {
		if (!useCached || !G::factor_type::isTransformCopula()) {
			extraFactors = evaluateFactors(ct.model(), extras, samples.copula(), takeLog);
		}
		else {
			extraFactors = evaluateFactors(ct.model(), extras, samples.temp(), takeLog);
		}
	}

	// Calculate density of copula by derivative-sum-product
	G::factor_type::range_type copulaDensity;
	if (G::factor_type::isTransformCopula() && useCached) {
		// DEBUG: Print vars
		/*for (unsigned int i = 0; i < samples.numVars(); i++)
			wcout << samples.temp()[i] << " ";
		wcout << endl;*/

		copulaDensity = passMessages(ct, drwnCumulativeUtils::dspCalculateMsg<G>, samples.temp(), useCached, takeLog, hasMarginalizedVariables);
	}

	else {
		// DEBUG: Print vars
		/*for (unsigned int i = 0; i < samples.numVars(); i++)
			wcout << samples.copula()[i] << " ";
		wcout << endl;*/

		copulaDensity = passMessages(ct, drwnCumulativeUtils::dspCalculateMsg<G>, samples.copula(), useCached, takeLog, hasMarginalizedVariables);
	}

	// DEBUG: Print results separately
	/*if (extraFactors != -1)
		wcout << L"Product of extra factors: " << extraFactors << endl;
	wcout << L"Product of transformed terms: " << productTransform << endl;
	wcout << L"Product of derivative of probit terms: " << productProbit << endl;
	wcout << L"Derivative of copula: " << copulaDensity << endl;*/

	// DEBUG: Catch here, so can check value of message passing
	if (stopYet)
		bool gotHere = true;

	// Return result
	if (&extras == 0) {
		if (!takeLog)
			return exp(productTransform + productProbit) * copulaDensity;
			//return productTransform * productProbit * copulaDensity;
		else
			return productTransform + productProbit + copulaDensity;
			//return log(productTransform) + log(productProbit) + copulaDensity;
	}
	else {
		if (!takeLog)
			return exp(productTransform + productProbit) * copulaDensity * extraFactors;
			//return productTransform * productProbit * copulaDensity * extraFactors;
		else
			return productTransform + productProbit + copulaDensity + extraFactors;
			//return log(productTransform) + log(productProbit) + copulaDensity + extraFactors;
	}
}

// Product rule to calculate copula (not copula density) of single variable.
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::copulaMarginal(G& g, int var, drwnSamples<G>& samples)
{
	// Transform argument for preparation to input to copula
	//vector<G::marginal_type::range_type> sampleCdfTransformed(sample.size());
	for (unsigned int i = 0; i < samples.numVars(); i++) {
		samples.copula()[i] = pow(samples.transform()[i], 1.0 / ((double) g.howManyScopes(i)));
		if (G::factor_type::isTransformCopula())
			samples.copula()[i] = G::factor_type::transformArgument(samples.copula()[i]);
	}

	// Multiply together factors evaluated at sample
	return evaluateFactors(g, g.varNeighbors(var), samples.copula());
}

// Debug message calculator
template<class G>
void drwnCumulativeUtils::debugCalculateMsg(drwnCliqueTree<G>& cliqueTree, int i, int j, typename G::factor_type::domain_type* sample, bool useCached, int diffFactor)
{
	wcout << "Passed message from clique " << i << " to clique " << j << endl;
}

// Product rule to calculate density P(x) of cumulative network over clique tree (which may be over subset of variables)
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::density(drwnCliqueTree<G>& ct, drwnSamples<G>& samples, unsigned int num, bool takeLog, const vector<int>& extras)
{
	// TODO: Break the following two "paragraphs" into different function, so we can eliminate some repetition of calculation when performing multiple inferences
	// Calculate product of marginals
	G::marginal_type::range_type productMarginals = (!takeLog ? 1 : 0);
	// TODO: Make an iterator that will iterate over all variables in clique tree
	for (unsigned i = 0; i < ct.numCliques(); i++) {
		for (unsigned int j = 0; j < ct.numVars(i); j++) {
			// If this variable is in the clique minus the sepset vars (we don't want to include more than once) ...
			// TODO: Log version for marginals
			if ((1 << j) & ct.cliqueMinusSepset(i)._subset) {
				if (!takeLog)
					productMarginals *= ct.model().marginal(ct.vars(i)[j]).density(samples[num][ct.vars(i)[j]]);
				else
					productMarginals += log(ct.model().marginal(ct.vars(i)[j]).density(samples[num][ct.vars(i)[j]]));
			}
		}
	}

	// Transform sample according to marginal CDF
	// TODO: Move these to clique tree for speed
	//vector<G::marginal_type::range_type> sampleCdf(sample.size());
	for (unsigned int i = 0; i < samples.numVars(); i++)
		samples.transform()[i] = ct.model().marginal(i).cdf(samples[num][i]);

	// DEBUG: Print product of marginals, useful for seeing if variables are independent in copula
	//wcout << L"Product of marginals: " << productMarginals << endl;
	if (!takeLog)
		return productMarginals * copulaDensity(ct, samples, extras);
	else
		return productMarginals + copulaDensity(ct, samples, extras);
}

// Differentiate product of clique factors with respect to all subsets of some set of variables
template<class G>
void drwnCumulativeUtils::diffClique(drwnCliqueTree<G>& ct, int i, int j, typename G::factor_type::range_type* output, typename G::factor_type::domain_type* sample, bool useCached, bool takeLog, int diffFactor)
{
	// Create sepset subset (relative to clique variables, not universe variables)
	// Note that in any case, we can have more than 31 universe variables, so we are forced to represent
	// subsets as relative to the clique variables.
	//unsigned int countSepsetSubsets = cliqueTree.getSepsets(i).size() ? 1 << cliqueTree.getSepsets(i).size() : countCliqueSubsets;

	// DEBUG: Catch case where fails
	//if (sample[0] == G::factor_type::transformArgument(0.001) && ct.numVars(i) == 3)
	//	bool gotHere = true;

	// DEBUG: Catch here, so can check value of message passing
	/*if (stopYet && i == 1 && j == -1)
		bool gotHere = true;

	double& outputValue1 = output[0];
	double& outputValue2 = output[1];*/

	// Calculate number of parents
	unsigned int numParents = ct.numParents(i, j);

	// For convenience, number of factors
	unsigned int numFactors = ct.numFactors(i);

	// Convenient flag to tell whether this clique is a root
	bool isRoot = (j == -1);

	// Create sepset subset (relative to clique variables, not universe variables)
	unsigned int countDiffSubsets = (isRoot || numParents) ? ct.numSubsets(i) : ct.numSepsetSubsets(i, j);
	
	// If root, then we differentiate with respect to clique variables. Otherwise, with respect to sepset | clique minus sepset

	const drwnSubset& diffSubset = (isRoot || numParents) ? ct.subset(i) : ct.sepsetSubset(i, j);
		
	// Create a bit list of variables in clique minus the ones in the sepset
	// TODO: Have to mask this with the non-marginalized variables if flag set
	drwnSubset cliqueMinusSepset = isRoot ? 0 : ct.cliqueMinusSepset(i, j);

	// If variables were marginalized, then apply mask to clique minus sepset
	if (ct.isMarged())
		cliqueMinusSepset._subset &= ct.mask(i);

	// Case 0: 0 factors
	if (!numFactors) {
		//drwnSubset currentSubset = 1;
		if (!takeLog)
			output[0] = 1;
		else
			output[0] = 0;
		for (unsigned int k = 1; k < countDiffSubsets; k++) {
			if (!takeLog)
				output[k] = 0;
			else
				output[k] = -std::numeric_limits<double>::infinity();
		}
	}

	// Case 1: 1 factor
	else if (numFactors == 1) {
		// DEBUG: Print information
		if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
			wcout << L"1 factor, ";
			if (!numParents)
				wcout << L"no parents";
			else
				wcout << numParents << " parents";
			wcout << endl;
		}

		// DEBUG
		//if (stopYet)
		//	bool gotHere = true;

		// Loop through subsets A of sepset S(i, j), or of clique scope if parents
		drwnSubset currentSubset = 0;
		for (unsigned int k = 0; k < countDiffSubsets; k++, currentSubset = drwnSubset::nextSubset(currentSubset, (drwnSubset&) diffSubset)) {
			// Calculate the variables with respect to which the factor is being differentiated
			const G::factor_type& f = ct.model().factor(*ct.factors(i).begin());
			drwnSubset diffVars = numParents ? currentSubset : drwnSubset::setUnion(cliqueMinusSepset, currentSubset);
			drwnSubset factorVars = drwnSubset::extractSubset(diffVars, f.getVars(), ct.index(i));

			// DEBUG
			/*if (stopYet && diffVars._subset == 3)
				bool gotHere = true;*/

			// Set final message. Derivative only nonzero if scope of factor includes all variables to be differentiated.
			G::factor_type::range_type deriv;
			if (!takeLog) {
				if (useCached) {
					if (diffFactor != -1)
						deriv = (drwnSubset::countElements(diffVars) == drwnSubset::countElements(factorVars)) ? ((G::factor_type&) f).cachedDerivative(factorVars, diffFactor == *ct.factors(i).begin()) : 0;
					else
						deriv = (drwnSubset::countElements(diffVars) == drwnSubset::countElements(factorVars)) ? ((G::factor_type&) f).cachedDerivative(factorVars) : 0;
				}
				else
					deriv = (drwnSubset::countElements(diffVars) == drwnSubset::countElements(factorVars)) ? ((G::factor_type&) f).partialDerivative(sample, factorVars, takeLog) : 0;
			}
			else {
				if (useCached) {
					// NOTE: Made change here for log-space gradient
					if (diffFactor != -1)
						deriv = (drwnSubset::countElements(diffVars) == drwnSubset::countElements(factorVars)) ? ((G::factor_type&) f).cachedDerivative(factorVars, diffFactor == *ct.factors(i).begin()) : 0;	// -std::numeric_limits<double>::infinity()
					else
						deriv = (drwnSubset::countElements(diffVars) == drwnSubset::countElements(factorVars)) ? ((G::factor_type&) f).cachedDerivative(factorVars) : -std::numeric_limits<double>::infinity();
				}
				else
					deriv = (drwnSubset::countElements(diffVars) == drwnSubset::countElements(factorVars)) ? ((G::factor_type&) f).partialDerivative(sample, factorVars, takeLog) : -std::numeric_limits<double>::infinity();
			}
			output[k] = deriv;

			// DEBUG: Print term (now function diffDebugFactors takes care of this)
			if (boost::is_same<G::factor_type, drwnPolyCopula>::value || stopYet) {
				if (!takeLog)
					wcout << "\td(" << *ct.factors(i).begin();
				else
					wcout << "\tlg(d(" << *ct.factors(i).begin();
				//drwnSubset::printSubset(drwnSubset::makeSubset(cliqueTree.getCliqueVars(i), cliqueIndex), cliqueTree.getCliqueVars(i), *graphicalModel.getUniverse());
				wcout << ")/d(";
				drwnSubset::printSubset(diffVars, ct.vars(i), ct.model().getUniverse());
				if (!takeLog)
					wcout << ")) = " << deriv << endl;
					//wcout << ") = " << log(deriv) << endl;
				else
					wcout << ")) = " << deriv << endl;
			}
		}
	}

	// Case 2: many factors
	else {
		// DEBUG: Print information
		if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
			wcout << L"" << numFactors << " factors, ";
			if (!numParents)
				wcout << L"no parents";
			else
				wcout << numParents << " parents";
			wcout << endl;
		}

		// Loop over factors, filling values
		// TODO: When clique tree over subgraph, we may have variables in scope of factors that are not in clique
		// In this case, we do not need to calculate partial derivative for these
		vector<int>::const_iterator jt = ct.factors(i).begin();
		for (unsigned int k = 0; k < numFactors; k++, jt++) {
			G::factor_type& f = (G::factor_type&) ct.model().factor(*jt);
			// Loop over subsets of factor scope
			drwnSubset superset = (1 << f.getVars().size()) - 1;
			drwnSubset factorVars = 0;
			do {
				// TODO: Type conversion from drwnSubset to unsigned int, to make following neater
				// NOTE: The diffFactor specifies which factor we are taking derivative of with respect to its copula parameter
				if (useCached) {
					if (diffFactor != -1) {
						// NOTE: Made change here for log-gradient
						if (!takeLog)
							ct.tempValue(i, k, factorVars._subset) = f.cachedDerivative(factorVars, diffFactor == *jt);
						else {
							if (diffFactor == *jt)
								ct.tempValue(i, k, factorVars._subset) = f.cachedDerivative(factorVars, true);
							else
								ct.tempValue(i, k, factorVars._subset) = exp(f.cachedDerivative(factorVars));
						}
					}
					else
						ct.tempValue(i, k, factorVars._subset) = f.cachedDerivative(factorVars);
				}
				else
					ct.tempValue(i, k, factorVars._subset) = f.partialDerivative(sample, factorVars, takeLog);
			} while (!drwnSubset::isEmptyset(factorVars = drwnSubset::nextSubset(factorVars, superset)));
		}
		
		// Fill temporary derivatives of product of factors
		// Take care of empty subset, i.e. fill m_i(0, 0)
		if (numParents) {
			G::factor_type::range_type productFactors = ((takeLog && diffFactor == -1) ? 0 : 1);
			if (!takeLog) {
				for (unsigned int k = 0; k < numFactors; k++)				
					productFactors *= ct.tempValue(i, k, 0);	// This is factor k, with no derivative, evaluated at sample
			}

			else {
				// NOTE: Made change here for log-gradient
				if (diffFactor == -1) {
					for (unsigned int k = 0; k < numFactors; k++)
						productFactors += ct.tempValue(i, k, 0);	// This is factor k, with no derivative, evaluated at sample
				}
				else {
					for (unsigned int k = 0; k < numFactors; k++)				
						productFactors *= ct.tempValue(i, k, 0);	// This is factor k, with no derivative, evaluated at sample
				}
			}

			output[0] = productFactors;
		}

		// Reset memory for temporary derivatives if necessary
		if (numFactors != 2)
			ct.resetDerivMsgs(i);

		// DEBUG: Catch here, so can check value of message passing
		//if (stopYet && i == 1 && j == -1)
		//	bool gotHere = true;

		// Loop through subsets A of clique C_i, or subsets of S_{ij} (final messages) if no parents
		drwnSubset A = 0;
		if (numParents)
			A = drwnSubset::nextSubset(A, diffSubset);
		unsigned int messageIndx = 0;
		do {
			// Adjust A, depending on whether we are doing clique subsets or sepset subsets
			drwnSubset realA = numParents ? A : drwnSubset::setUnion(A, cliqueMinusSepset);
			
			// Loop through subsets of scope(phi) v A, put initial terms on stack
			drwnSubset scopeAndA = drwnSubset::setIntersection(ct.scope(i, numFactors - 1), realA), B = 0;
			do {
				// If A \ B is a subset of scope(phi_1 * ... * phi_{n-1}), then this term is nonzero
				if (drwnSubset::isSubset(drwnSubset::setMinus(realA, B), ct.cumulativeScope(i, numFactors - 2))) {
					// If the term involves a single factor then take care of it straight away
					if (numFactors == 2) {
						// Calculate (or rather, fetch) contribution terms
						const G::factor_type& fn1 = ct.model().factor(ct.factors(i)[0]);
						const G::factor_type& fn = ct.model().factor(ct.factors(i)[1]);
						
						G::factor_type::range_type dPhidB = ct.tempValue(i, 1, drwnSubset::extractSubset(B, fn.getVars(), ct.index(i))._subset);
						G::factor_type::range_type dProdPhidAminusB = ct.tempValue(drwnSubset::extractSubset(drwnSubset::setMinus(realA, B), fn1.getVars(), ct.index(i))._subset);

						// If there are parents then it contributes to the temporary messages
						// Else if there are no parents, then we will add it into the final messages
						if (!takeLog) {
							double& outputValue = output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)];

							output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)] += dPhidB * dProdPhidAminusB;

							// DEBUG
							//wcout << outputValue << " ";
						}
						else {
							// NOTE: Made change here for log-gradient
							if (diffFactor == -1)
								output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)] = logSumExp(output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)], dPhidB + dProdPhidAminusB);
							else
								output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)] +=  dPhidB * dProdPhidAminusB;
						}

						// DEBUG: Print event information
						if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
							wcout << "\td(" << ct.factors(i)[0] << " " << ct.factors(i)[1] << ")/d("; 
							drwnSubset::printSubset(realA, ct.vars(i), ct.model().getUniverse());
							wcout << ") += d(" << ct.factors(i)[1] << ")/d(";
							drwnSubset::printSubset(B, ct.vars(i), ct.model().getUniverse());
							wcout << ") [" << dPhidB <<"] * d(" << ct.factors(i)[0] << ")/d(";
							drwnSubset::printSubset(drwnSubset::setMinus(realA, B), ct.vars(i), ct.model().getUniverse());
							wcout << ") [" << dProdPhidAminusB << "]" << endl;
						}
					}

					// Otherwise, add to stack and take care of later
					else {
						// See if this term has already been calculated
						G::factor_type::range_type dProdPhidAminusB = ct.tempDeriv(i, numFactors - 3, drwnSubset::whatSubset(drwnSubset::setMinus(realA, B), ct.cumulativeScope(i, numFactors - 2)));
						
						// Add to stack
						derivTerm tempTerm = {realA, B, numFactors - 1, !boost::math::isnan(dProdPhidAminusB)};
						ct.stack.push_back(tempTerm);
					}
				}
			} while (!drwnSubset::isEmptyset(B = drwnSubset::nextSubset(B, scopeAndA)));

			// Loop until stack is empty
			while (ct.stack.size()) {
				// Inspect last element, reference relevant factor
				derivTerm& backTerm = ct.stack.back();
				const G::factor_type& fn = ct.model().factor(ct.factors(i)[backTerm.i]);
				
				// If it has already been inspected, we need to add it to correct term
				if (backTerm.isInspected) {
					// Calculate (or rather, fetch) contribution terms
					G::factor_type::range_type dPhidB = ct.tempValue(i, backTerm.i, drwnSubset::extractSubset(backTerm.B, fn.getVars(), ct.index(i))._subset);
					G::factor_type::range_type dProdPhidAminusB = ct.tempDeriv(i, backTerm.i - 2, drwnSubset::whatSubset(drwnSubset::setMinus(backTerm.A, backTerm.B), ct.cumulativeScope(i, backTerm.i - 1)));

					// If this term involves n - 1 factors...
					if (backTerm.i == numFactors - 1) {
						// ...and there are parents then it contributes to the temporary messages
						// ...else, there are no parents, then we will add it into the final messages
						if (!takeLog)
							output[numParents ? backTerm.A._subset : drwnSubset::whatSubset(backTerm.A, diffSubset)] += dPhidB * dProdPhidAminusB;
						else {
							// NOTE: Made change here for log-gradient
							if (diffFactor == -1)
								output[numParents ? backTerm.A._subset : drwnSubset::whatSubset(backTerm.A, diffSubset)] = logSumExp(output[numParents ? backTerm.A._subset : drwnSubset::whatSubset(backTerm.A, diffSubset)], dPhidB + dProdPhidAminusB);
							else
								output[numParents ? backTerm.A._subset : drwnSubset::whatSubset(backTerm.A, diffSubset)] += dPhidB * dProdPhidAminusB;
						}
					}

					// Otherwise it gets added into temporary derivative terms
					else {
						// Reset if its a NaN
						if (!takeLog) {
							if (boost::math::isnan(ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i)))))
								ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))) = dPhidB * dProdPhidAminusB;
							else
								ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))) += dPhidB * dProdPhidAminusB;
						}
						else {
							// NOTE: Made change here for log-gradient
							if (diffFactor == -1) {
								if (boost::math::isnan(ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i)))))
									ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))) = dPhidB + dProdPhidAminusB;
								else
									ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))) = logSumExp(ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))), dPhidB + dProdPhidAminusB);
							}
							else {
								if (boost::math::isnan(ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i)))))
									ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))) = dPhidB * dProdPhidAminusB;
								else
									ct.tempDeriv(i, backTerm.i - 1, drwnSubset::whatSubset(backTerm.A, ct.cumulativeScope(i, backTerm.i))) += dPhidB * dProdPhidAminusB;
							}
						}
					}

					// DEBUG: Print event information
					if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
						wcout << "\td(" << ct.factors(i)[0];
						for (unsigned int k = 1; k < backTerm.i + 1; k++) {
							wcout << " " << ct.factors(i)[k];
						}
						wcout << ")/d("; 
						drwnSubset::printSubset(backTerm.A, ct.vars(i), ct.model().getUniverse());
						wcout << ") += d(" << ct.factors(i)[backTerm.i] << ")/d(";
						drwnSubset::printSubset(backTerm.B, ct.vars(i), ct.model().getUniverse());
						wcout << ") [" << dPhidB <<"] * d(" << ct.factors(i)[0];
						for (unsigned int k = 1; k < backTerm.i; k++) {
							wcout << " " << ct.factors(i)[k];
						}
						wcout << ")/d(";
						drwnSubset::printSubset(drwnSubset::setMinus(backTerm.A, backTerm.B), ct.vars(i), ct.model().getUniverse());
						wcout << ") [" << dProdPhidAminusB << "]" << endl;
					}

					// Pop it off stack since we are finished with it
					ct.stack.pop_back();
				}

				// Otherwise we need to check what terms it requires, add calculated ones into it and push uncalculated ones onto stack
				else {
					// Loop through subsets of scope(phi(n-1)) v (A \ B), put initial terms on stack
					drwnSubset newA = drwnSubset::setMinus(backTerm.A, backTerm.B);
					drwnSubset newScopeAndA = drwnSubset::setIntersection(ct.scope(i, backTerm.i - 1), newA), B = 0;
					bool addedTerms = false;
					do {
						// If A \ B is a subset of scope(phi_1 * ... * phi_{n-1}), then this term is nonzero
						drwnSubset newAminusB = drwnSubset::setMinus(newA, B);
						if (drwnSubset::isSubset(newAminusB, ct.cumulativeScope(i, backTerm.i - 2))) {
							// If the term involves a single factor then take care of it straight away
							if (backTerm.i == 2) {
								// Calculate (or rather, fetch) contribution terms
								const G::factor_type& fn = ct.model().factor(ct.factors(i)[1]);
								const G::factor_type& fn1 = ct.model().factor(ct.factors(i)[0]);
								G::factor_type::range_type dPhidB = ct.tempValue(i, 1, drwnSubset::extractSubset(B, fn.getVars(), ct.index(i))._subset);
								G::factor_type::range_type dProdPhidAminusB = ct.tempValue(drwnSubset::extractSubset(newAminusB, fn1.getVars(), ct.index(i))._subset);

								if (!takeLog) {
									if (boost::math::isnan(ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1)))))
										ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) = dPhidB * dProdPhidAminusB;
									else
										ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) += dPhidB * dProdPhidAminusB;
								}
								else {
									// NOTE: Made change here for log-gradient
									if (diffFactor == -1) {
										if (boost::math::isnan(ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1)))))
											ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) = dPhidB + dProdPhidAminusB;
										else
											ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) = logSumExp(ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) , dPhidB + dProdPhidAminusB);
									}
									else {
										if (boost::math::isnan(ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1)))))
											ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) = dPhidB * dProdPhidAminusB;
										else
											ct.tempDeriv(drwnSubset::whatSubset(newA, ct.cumulativeScope(i, backTerm.i - 1))) += dPhidB * dProdPhidAminusB;
									}
								}

								// DEBUG: Print event information
								/*if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
									wcout << "\td(" << ct.factors(i)[0] << " " << ct.factors(i)[1] << ")/d("; 
									drwnSubset::printSubset(newA, ct.vars(i), ct.model().getUniverse());
									wcout << ") += d(" << ct.factors(i)[1] << ")/d(";
									drwnSubset::printSubset(B, ct.vars(i), ct.model().getUniverse());
									wcout << ") [" << dPhidB <<"] * d(" << ct.factors(i)[0] << ")/d(";
									drwnSubset::printSubset(newAminusB, ct.vars(i), ct.model().getUniverse());
									wcout << ") [" << dProdPhidAminusB << "]" << endl;
								}*/
							}

							// Otherwise, add to stack and take care of later
							else {
								addedTerms = true;
								derivTerm tempTerm = {newA, B, backTerm.i - 1, false};
								ct.stack.push_back(tempTerm);
							}
						}
					} while (!drwnSubset::isEmptyset(B = drwnSubset::nextSubset(B, newScopeAndA)));

					backTerm.isInspected = true;
				}
			}

			// DEBUG: Print term (now function diffDebugFactors takes care of this)
			/*if (stopYet) {
				if (!takeLog)
					wcout << "\td(";
				else
					wcout << "\tlg(d(";
				drwnSubset::printSubset(ct.factors(i));
				wcout << ")/d(";
				drwnSubset::printSubset(realA, ct.vars(i), ct.model().getUniverse());
				if (!takeLog)
					wcout << ") = ";
				else
					wcout << ")) = ";

				// DEBUG: Take log of non log-space answers so can make comparison
				if (!takeLog)
					wcout << log(output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)]) << endl;
				else
					wcout << output[numParents ? realA._subset : drwnSubset::whatSubset(realA, diffSubset)] << endl;
			}*/

			// DEBUG: Print a new line to make output pretty
			//if (boost::is_same<G::factor_type, drwnPolyCopula>::value)
			//	wcout << endl;
		} while (!drwnSubset::isEmptyset(A = drwnSubset::nextSubset(A, diffSubset)));


	}

	// DEBUG: Calculate actual values of derivatives of product of factors
	if (boost::is_same<G::factor_type, drwnPolyCopula>::value)
	//if (sample[0] == G::factor_type::transformArgument(0.001) && ct.numVars(i) == 3)
		diffDebugFactors(ct, i, j, output, sample);
}

// Calculate the derivative of drwnPolyCopula factors for debugging
template<class G>
void drwnCumulativeUtils::diffDebugFactors(drwnCliqueTree<G>& ct, int i, int j, typename G::factor_type::range_type* output, typename G::factor_type::domain_type* sample)
{
	// TODO: This function relies on fact that variables in scope of some factor in clique but not in clique (this can happen when over subgraph)
	// are set to 1 (marginalized). We should modify the following so when these variables are not 1, result is modified accordingly.
	// Do this firstly by setting varCount to number of model variables, not number of clique variables.

	// loop through factors, keep count of how many times each variable appears in this clique
	vector<unsigned int> varCount(ct.vars(i).size());
	for (unsigned int k = 0; k < ct.numFactors(i); k++) {
		// Loop through factor variables
		const G::factor_type& f = ct.model().factor(ct.factors(i)[k]);
		for (vector<int>::const_iterator var = f.getVars().begin(); var != f.getVars().end(); var++) {
			map<int, int>::const_iterator elem = ct.index(i).find(*var);
			if (elem != ct.index(i).end())
				varCount[elem->second]++;
		}
	}

	// Calculate number of parents
	unsigned int numParents = ct.numParents(i, j);

	// Convenient flag to tell whether this clique is a root
	bool isRoot = (j == -1);

	// Create sepset subset (relative to clique variables, not universe variables)
	unsigned int countDiffSubsets = (isRoot || numParents) ? ct.numSubsets(i) : ct.numSepsetSubsets(i, j);
	
	// If root, then we differentiate with respect to clique variables. Otherwise, with respect to sepset | clique minus sepset
	const drwnSubset& diffSubset = (isRoot || numParents) ? ct.subset(i) : ct.sepsetSubset(i, j);
		
	// Create a bit list of variables in clique minus the ones in the sepset
	const drwnSubset& cliqueMinusSepset = isRoot ? 0 : ct.cliqueMinusSepset(i, j);
		
	// loop through subsets of sepset
	vector<G::factor_type::range_type> trueValues(countDiffSubsets, 1.0);
	drwnSubset A = 0;
	unsigned int howManyBits = drwnSubset::countElements(ct.subset(i)), countCurrentSubset = 0;
	do {
		// Adjust A
		drwnSubset realA = numParents ? A : drwnSubset::setUnion(A, cliqueMinusSepset);

		// Calculate what result should be
		G::factor_type::range_type result = 1.0;
		for (unsigned int p = 0, k = 0; p < howManyBits; p++, k++) {
			// Loop to next set bit to be considered
			while(!(ct.subset(i)._subset & (1 << k)))
				k++;

			// If not differentiating with respect to this variable, adjust result
			if (!(realA._subset & (1 << k)))
				result *= (G::factor_type::range_type) pow((G::factor_type::range_type) sample[ct.vars(i)[k]], (G::factor_type::range_type) varCount[k]);

			// If differentiating variable, it is more complicated
			else {
				// When variable doesn't appear we need to set result to zero.
				if (!varCount[k]) {
					result = 0;
					break;
				}

				// Otherwise, multiply in derivative term
				result *= ((G::factor_type::range_type) varCount[k]) * pow(sample[ct.vars(i)[k]], (G::factor_type::range_type) (varCount[k] - 1.0));
			}
		}

		// Print results
		trueValues[countCurrentSubset] = result;
		G::factor_type::range_type calculatedValue = output[countCurrentSubset];

		// Print event information
		wcout << "d(";
		drwnSubset::printSubset(ct.factors(i));
		wcout << ")/d("; 
		drwnSubset::printSubset(realA, ct.vars(i), ct.model().getUniverse());
		wcout << ") = " << result << (calculatedValue == result ? " = " : " != ") << calculatedValue << endl;
			
		countCurrentSubset++;
	} while (!drwnSubset::isEmptyset(A = drwnSubset::nextSubset(A, diffSubset)));
}

// Calculate derivative (evaluated at sample) of all drwnPolyCopula factors in clique tree
template<class G>
void drwnCumulativeUtils::diffDebugModel(drwnCliqueTree<G>& ct, typename G::factor_type::domain_type* sample, typename G::factor_type::range_type result)
{
	// Count number of times that variable appears in factors in cliques
	vector<unsigned int> varCount(ct.model().numVariables());
	for (unsigned int i = 0; i < ct.numCliques(); i++) {
		for (unsigned int k = 0; k < ct.numFactors(i); k++) {
			// Loop through factor variables
			const G::factor_type& f = ct.model().factor(ct.factors(i)[k]);
			for (vector<int>::const_iterator var = f.getVars().begin(); var != f.getVars().end(); var++)
				varCount[*var]++;
		}
	}

	// DEBUG DEBUG: Print count of each variables
	/*wcout << "Variable Count" << endl;
	for (unsigned int i = 0; i < ct.model().numVariables(); i++) {
		if (varCount[i]) {
			wcout << ct.model().getUniverse()->varName(i).c_str() << ": " << varCount[i] << " ";
		}
	}
	wcout << endl;*/

	// Loop through variables in cliques, diffing in
	G::factor_type::range_type trueResult = 1;
	for (unsigned int i = 0; i < ct.numCliques(); i++) {
		for (vector<int>::const_iterator jt = ct.vars(i).begin(); jt != ct.vars(i).end(); jt++) {
			if (varCount[*jt]) {
				trueResult *= (G::factor_type::range_type) varCount[*jt] * pow(sample[*jt], varCount[*jt] - 1);

				// Set var count to zero, so we will know remaining variables afterwards
				varCount[*jt] = 0;
			}
		}
	}

	// Multiply in those variables with respect to which we do not differentiate
	for (unsigned int i = 0; i < ct.model().numVariables(); i++) {
		if (varCount[i])
			trueResult *= pow(sample[i], varCount[i]);
	}

	wcout << endl << "Ans: " << result << endl;
	wcout << "True ans: " << trueResult << endl;
}

// Derivative-sum-product Message Calculate
template<class G>
void drwnCumulativeUtils::dspCalculateMsg(drwnCliqueTree<G>& ct, int i, int j, typename G::factor_type::domain_type* sample, bool useCached, bool takeLog, int factorDiff)
{
	// DEBUG: Print information
	if (boost::is_same<G::factor_type, drwnPolyCopula>::value || stopYet)
		wcout << endl << L"Passing message from " << i << L" to " << j << endl;

	// Convenient flag to tell whether this clique is a root
	bool isRoot = (j == -1);

	// Calculate number of parents
	unsigned int numParents = ct.numParents(i, j);

	// If no parents then we will store derivatives in final message structure
	if (!numParents) {
		diffClique(ct, i, j, &ct.message(i, j, 0), sample, useCached, takeLog, factorDiff);

		if (stopYet) {
			//ct.model().factor(0).cacheDerivatives(sample);

			/*wcout << "sample of dspCalculateMsg" << endl;
			for (unsigned int k = 0; k < ct.numVars(i); k++)
				wcout << sample[k] << " ";
			wcout << endl;*/
			if (!isRoot)
				wcout << "messages from " << i << " to " << j << endl;
			else
				wcout << "final messages" << endl;
			for (unsigned int k = 0; k < (!isRoot ? ct.numSepsetSubsets(i) : ct.numSubsets(i)); k++) {
				if (!takeLog)
					//wcout << log(ct.message(i, j, k)) << " ";
					wcout << ct.message(i, j, k) << " ";
				else
					wcout << ct.message(i, j, k) << " ";
			}
			wcout << endl;
			if (isRoot) {
				wcout << "last message" << endl;
				wcout << ct.lastMessage(i) << endl;
			}
		}

		// TODO: Did I want to not return here for root cliques so debug messages can be printed?
		return;
	}

	// Zero temporary messages
	// NOTE: Changed following line for log-space gradient
	ct.resetTempMsgs(takeLog && factorDiff == -1);

	// DEBUG
	/*if (stopYet2 && j == -1) {
		bool gotHere = true;
	}*/

	// Otherwise, we need to store messages in temporary message structure
	diffClique(ct, i, j, &ct.tempMessage(0), sample, useCached, takeLog, factorDiff);

	// DEBUG: Stop on flag and sending to root
	/*if (stopYet2 && j == -1 || factorDiff == 3) {
		wcout << endl << "results of diffClique" << endl;
		for (unsigned int k = 0; k < ct.numSubsets(i); k++)
			wcout << ct.tempMessage(k) << " ";
			//wcout << ct.message(i, -1, k) << " ";
		wcout << endl << endl;
		//wcout << "last messages" << endl;
		//wcout << ct.lastMessage(i) << endl;
		stopYet2 = false;
	}*/

	// Create sepset subset (relative to clique variables, not universe variables)
	unsigned int countDiffSubsets = isRoot ? ct.numSubsets(i) : ct.numSepsetSubsets(i, j);
	
	// If root, then we differentiate with respect to clique variables. Otherwise, with respect to sepset | clique minus sepset
	const drwnSubset& diffSubset = isRoot ? ct.subset(i) : ct.sepsetSubset(i, j);

	// Create a bit list of variables in clique minus the ones in the sepset
	drwnSubset cliqueMinusSepset = isRoot ? 0 : ct.cliqueMinusSepset(i, j);

	// If variables were marginalized, then apply mask to clique minus sepset
	if (ct.isMarged())
		cliqueMinusSepset._subset &= ct.mask(i);

	// DEBUG: Print extra line here to make output look nice!
	if (boost::is_same<G::factor_type, drwnPolyCopula>::value)
		wcout << endl;

	// DEBUG
	if (stopYet)
		bool gotHere = true;

	// Loop over parents
	vector<int>::const_iterator it = ct.parents(i).begin();
	for (unsigned int k = 1; k <= numParents; k++, it++) {
		// Calculate parent
		// TODO: Iterator design pattern to enumerate all parents for message from i to j
		// TODO: Likewise, iterator pattern to enumerate all subsets relative to a superset
		unsigned int parent;

		// DEBUG: Catch case that fails
		//if (i == 4 && j == -1 && k == numParents)
		//	bool gotHere = true;

		// If we have exhausted all parents in topological ordering, then must take child
		if (it == ct.parents(i).end())
			parent = ct.child(i);

		// Otherwise...
		else {
			// ... if parent from set is clique we are passing to then continue
			if (*it == j) {
				k--;
				continue;
			}

			// ... else just set to parent from parents
			else
				parent = *it;
		}
		
		// Create parent sepset subset (relative to clique variables, not universe variables)
		const drwnSubset& parentSepsetSubset = ct.sepsetSubset(i, parent);
		
		// Loop through subsets A of clique or sepset (depending on whether last parent)
		drwnSubset currentSubset = 0, superSet = ((k == numParents) ? diffSubset : ct.subset(i));
		unsigned int numMessages = ((k == numParents) ? countDiffSubsets : ct.numSubsets(i));
		for (unsigned messageNum = 0; messageNum < numMessages; messageNum++, currentSubset = drwnSubset::nextSubset(currentSubset, superSet)) {
			// Loop through subsets B of intersection of (adjusted) A and parent sepset
			drwnSubset realCurrentSubset = (k == numParents) ? drwnSubset::setUnion(currentSubset, (drwnSubset&) cliqueMinusSepset) : currentSubset;
			drwnSubset currentTermSubset = 0, termSuperset = drwnSubset::setIntersection(realCurrentSubset, parentSepsetSubset);
			//unsigned int countTerms = 1 << drwnSubset::countElements(termSuperset);
			do {
				// Retrieve correct messages (optimizer knows what to do...)
				G::factor_type::range_type& firstTerm = ct.message(parent, i, drwnSubset::whatSubset(currentTermSubset, (drwnSubset&) parentSepsetSubset));
				G::factor_type::range_type& secondTerm = ct.tempMessage((k - 1) * ct.numSubsets(i) + drwnSubset::setMinus(realCurrentSubset, currentTermSubset)._subset);

				// DEBUG:
				/*if (factorDiff == 7) {
					wcout << (!takeLog ? firstTerm : exp(firstTerm)) << " " << secondTerm << endl;
				}*/
				
				// DEBUG: Print term event
				if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
					wcout << "d(c" << i << ", p";
					//vector<int>::const_iterator jt = ct.parents(i).begin();
					ct.printParents(i, j, k);
					wcout << ")/d(";
					drwnSubset::printSubset(realCurrentSubset, ct.vars(i), ct.model().getUniverse());
					wcout << ") += d(c" << parent; // << ", p";
					if (ct.numParents(parent, i)) {
						wcout << ", p";
						ct.printParents(parent, i, ct.numParents(parent, i));
					}
					wcout << ")/d(";
					drwnSubset::printSubset(currentTermSubset, ct.vars(i), ct.model().getUniverse());
					wcout << ") [" << (!takeLog ? firstTerm : exp(firstTerm)) << "] * d(c" << i; // << ", ";
					if (k > 1) {
						wcout << ", p";
						ct.printParents(i, j, k - 1);
					}
					wcout << ")/d(";
					drwnSubset::printSubset(drwnSubset::setMinus(realCurrentSubset, currentTermSubset), ct.vars(i), ct.model().getUniverse());
					wcout << ") [" << secondTerm << "]" << endl;
				}
				
				// If last parent, we have to fill final messages
				if  (k == numParents) {
					if (!takeLog) {
						ct.message(i, j, messageNum) += firstTerm * secondTerm;

						// DEBUG: catch case where fails
						/*if (factorDiff == 7) {
							bool gotHere = true;
							wcout << messageNum << " " << ct.message(i, j, messageNum) << endl;
						}*/
					}
					else {
						// NOTE: Changed following for log-space gradient
						if (factorDiff == -1)
							ct.message(i, j, messageNum) = logSumExp(ct.message(i, j, messageNum), firstTerm + secondTerm);
						else {
							//if (k == 1)
							ct.message(i, j, messageNum) += exp(firstTerm) * secondTerm;

							// DEBUG: catch case where fails
							/*if (factorDiff == 7) {
								bool gotHere = true;
								//wcout << exp(firstTerm) << " " << secondTerm << endl;
								wcout << messageNum << " " << ct.message(i, j, messageNum) << endl;
							}*/

							//else
							//	ct.message(i, j, messageNum) += exp(firstTerm) * secondTerm;
						}
					}
				}

				// Otherwise, fill temporary messages
				else {
					if (!takeLog)
						ct.tempMessage(k * ct.numSubsets(i) + messageNum) += firstTerm * secondTerm;
					else {
						// NOTE: Changed following for log-space gradient
						if (factorDiff == -1)
							ct.tempMessage(k * ct.numSubsets(i) + messageNum) = logSumExp(ct.tempMessage(k * ct.numSubsets(i) + messageNum) , firstTerm + secondTerm);
						else {
							//if (k == 1)
								ct.tempMessage(k * ct.numSubsets(i) + messageNum) += exp(firstTerm) * secondTerm;
							//else
							//	ct.tempMessage(k * ct.numSubsets(i) + messageNum) = exp(firstTerm + secondTerm);
						}
					}
				}
			} while (!drwnSubset::isEmptyset(currentTermSubset = drwnSubset::nextSubset(currentTermSubset, termSuperset)));
		}

		// DEBUG: Calculate actual message values
		if (boost::is_same<G::factor_type, drwnPolyCopula>::value) {
			// Calculate the cliques used in these messages and what additional variables with respect to which to differentiate
			vector<unsigned int> previousCliques;
			previousCliques.push_back(i);
			unsigned int l = 0;
			set<unsigned int> diffVars;
			for (drwnCliqueTree<G>::parent_iterator it = ct.beginParents(i, j); l < k; it++, l++) {
				vector<unsigned int> cliqueBranch = ct.previousCliques(i, *it);
				previousCliques.insert(previousCliques.end(), cliqueBranch.begin(), cliqueBranch.end());
				set<unsigned int> newDiffVars = ct.diffVars(*it, i);
				diffVars.insert(newDiffVars.begin(), newDiffVars.end());
			}

			// loop through cliques, determine how many times each variable appears
			vector<unsigned int> varCount(ct.model().numVariables());
			for (vector<unsigned int>::iterator jt = previousCliques.begin(); jt != previousCliques.end(); jt++) {
				// loop through factors, keep count of how many times each variable appears
				for (vector<int>::const_iterator kt = ct.factors(*jt).begin(); kt != ct.factors(*jt).end(); kt++) {
					// Loop through factor variables
					const G::factor_type& f = ct.model().factor(*kt);
					for (vector<int>::const_iterator var = f.getVars().begin(); var != f.getVars().end(); var++)
						varCount[*var]++;
				}
			}

			// loop through subsets of sepset
			//vector<G::factor_type::range_type> trueValues(countDiffSubsets, 1.0);
			drwnSubset A = 0;
			//unsigned int howManyBits = drwnSubset::countElements((k < numParents) ? ct.subset(i) : diffSubset), 
			unsigned int countCurrentSubset = 0;
			do {
				// Adjust A
				drwnSubset realA = ((k < numParents) ? A : drwnSubset::setUnion(A, cliqueMinusSepset));

				// Convert subset into set of variables
				set<unsigned int> addVars;

				G::factor_type::range_type result = 1.0;
				unsigned int howManyBits = drwnSubset::countElements(realA);
				for (unsigned int p = 0, m = 0; p < howManyBits; p++, m++) {
					// Loop to next set bit to be considered
					//while(!(((k < numParents) ? ct.subset(i) : diffSubset)._subset & (1 << m)))
					while(!(realA._subset & (1 << m)))
						m++;

					// If differentiate variable, add to set
					//if (realA._subset & (1 << m))
						addVars.insert(ct.vars(i)[m]);
				}

				// Differentiate product of factors (loop over variables in model)
				for (unsigned int p = 0; p < varCount.size(); p++) {
					// If differentiating ...
					if (diffVars.count(p) || addVars.count(p)) {
						// When variable doesn't appear we need to set result to zero.
						if (!varCount[p]) {
							result = 0;
							break;
						}

						// Otherwise, multiply in derivative term
						result *= ((G::factor_type::range_type) varCount[p]) * pow(sample[p], (G::factor_type::range_type) (varCount[p] - 1.0));
					}

					// If not differentiating ...
					else {
						result *= (G::factor_type::range_type) pow((G::factor_type::range_type) sample[p], (G::factor_type::range_type) varCount[p]);
					}
				}
				
				// Print results
				// TODO: fetch value from temp or final depending on whether calculating for final parent
				G::factor_type::range_type calculatedValue = (k < numParents ? ct.tempMessage(k * ct.numSubsets(i) + countCurrentSubset) : ct.message(i, j, countCurrentSubset));

				// Print event information
				wcout << "d(c" << i << ", p";
				ct.printParents(i, j, k);
				wcout << ")/d(";
				drwnSubset::printSubset(realA, ct.vars(i), ct.model().getUniverse());
				wcout << ") = " << result << (calculatedValue == result ? " = " : " != ") << calculatedValue << endl;
			
				countCurrentSubset++;
			} while (!drwnSubset::isEmptyset(A = drwnSubset::nextSubset(A, ((k == numParents) ? diffSubset : ct.subset(i)))));
		}

		// Break so doesn't go past end
		if (it == ct.parents(i).end())
			break;
	}

	// DEBUG: Output final messages
	if (boost::is_same<G::factor_type, drwnPolyCopula>::value || stopYet) {
		wcout << endl;
		drwnSubset currentSubset = 0;
		for (unsigned messageNum = 0; messageNum < countDiffSubsets; messageNum++, currentSubset = drwnSubset::nextSubset(currentSubset, diffSubset)) {
			drwnSubset realCurrentSubset = drwnSubset::setUnion(currentSubset, cliqueMinusSepset);
			wcout << "d(c" << i << ", p";
			ct.printParents(i, j, ct.numParents(i, j));
			wcout << ")/d(";
			drwnSubset::printSubset(realCurrentSubset, ct.vars(i), ct.model().getUniverse());
			if (!takeLog)
				//wcout << ") = " << log(ct.message(i, j, messageNum)) << endl;
				wcout << ") = " << ct.message(i, j, messageNum) << endl;
			else
				wcout << ") = " << ct.message(i, j, messageNum) << endl;
		}
	}
}

// Evaluate factors at sample and multiply together
template<class G, class S, class T>
typename G::factor_type::range_type drwnCumulativeUtils::evaluateFactors(const G& g, const S& set, T* sample, bool takeLog)
{
	// Multiply together factors evaluated at sample
	G::factor_type::range_type result = (!takeLog ? 1.0 : 0);
	for (S::const_iterator it = set.begin(); it != set.end(); it++) {
		if (!takeLog)
			result *= g.factor(*it).evaluateCdf(sample, takeLog);
		else
			result += g.factor(*it).evaluateCdf(sample, takeLog);
	}

	return result;
}

// Pass messages through clique tree
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::passMessages(drwnCliqueTree<G>& ct, void (*f)(drwnCliqueTree<G>&, int, int, typename G::factor_type::domain_type*, bool, bool, int), typename G::factor_type::domain_type* sample, bool useCached, bool takeLog, bool hasMarginalized)
{
	G::factor_type::range_type result = (!takeLog ? 1 : 0);

	// Zero temporary messages (and regular messages?)
	ct.resetMessages(takeLog);

	// DEBUG: Catch here, so can check value of message passing
	if (stopYet)
		bool gotHere = true;

	// Since the clique tree is in topological order, we can pass messages in clique order
	for (int i = 0; i < ct.numCliques(); i++) {
		f(ct, i, ct.child(i), sample, useCached, takeLog, -1);

		// DEBUG: Print final messages for root
		if (stopYet && ct.child(i) == -1) {
			wcout << "Final messages for root" << endl;
			for (unsigned int j = 0; j < ct.numSubsets(i); j++) {
				wcout << ct.message(i, -1, j) << " ";
			}
			wcout << endl;
		}

		// Multiply in final message for roots
		if (ct.child(i) == -1) {
			// No variables marginalized from root, then result is simply last message
			if (!ct.isMarged()) {
				if (!takeLog)
					result *= ct.lastMessage(i);
				else
					result += ct.lastMessage(i);
			}

			// Otherwise, we have to get root clique differentiated with respect to non-marginalized variables
			else {
				//unsigned int nonMargSubset = ;
				
				if (!takeLog)
					result *= ct.message(i, -1, ct.mask(i));
				else
					result += ct.message(i, -1, ct.mask(i));
			}

			// Reset final messages if necessary
			if (i < ct.numCliques() - 1)
				ct.resetFinalMessages(i, takeLog);
		}
	}

	// DEBUG: For polynomial copula, compare calculated answer and correct answer
	//if (boost::is_same<G::factor_type, drwnPolyCopula>::value)
	//	diffDebugModel(ct, sample, result);

	return result;
}

// Pass messages through clique tree to given root
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::passMessages(drwnCliqueTree<G>& ct, void (*f)(drwnCliqueTree<G>&, int, int, typename G::factor_type::domain_type*, bool, bool, int), typename G::factor_type::domain_type* sample, unsigned int root, bool useCached, bool takeLog)
{
	// Final result
	typename G::factor_type::range_type result = (!takeLog ? 1 : 0);

	// Zero temporary messages (and regular messages?)
	ct.resetMessages(takeLog);

	// Initialize number of unused neighbours for each clique
	ct.resetUnusedNeighbours();

	// Initialize priority queue with all leaves that aren't the root
	vector<int> leaves;
	leaves.reserve(ct.leaves().size());
	for (vector<int>::const_iterator it = ct.leaves().begin(); it != ct.leaves().end(); it++) {
		if (*it != root)
			leaves.push_back(*it);
	}

	// If the root clique is disjoint (no parents or children) then we have to add it as well
	if (ct.child(root) == -1 && !ct.parents(root).size())
		leaves.push_back(root);

	priority_queue< int, vector<int>, std::greater<int> > nextMsg(std::greater<int>(), leaves);

	// Pass messages
	while (nextMsg.size()) {
		int i = nextMsg.top(), j = -1;
		
		// Need to work out who passing message to if outgoing messages haven't been sent
		if (ct.numUnused(i)) {
			// NOTE: If I discover a bug in the future for some case with multiple roots, then comment out following "if"!
			// If root is greater than this one, then by topological ordering, root must be downstream
			if (root > i) {
				j = ct.child(i);
				// If root is less than child ...
				if (j != -1 && root < j) {
					// ... and this parent is equal to lowest unused parent, then increment lowest unused parent
					if (ct.parents(j)[ct.lowestUnusedParent(j)] == i)
						ct.lowestUnusedParent(j)++;
				}
			}

			// Otherwise, root must be upstream and we need to choose which parent
			else {
				// If lowest unused parent is equal to number of parents then we have used all parents and must choose child
				if (ct.lowestUnusedParent(i) == ct.parents(i).size()) {
					j = ct.child(i);
					if (j != -1) {
						if (ct.parents(j)[ct.lowestUnusedParent(j)] == i)
							ct.lowestUnusedParent(j)++;
					}
				}

				// Otherwise, choose lowest unused parent
				else
					j = ct.parents(i)[ct.lowestUnusedParent(i)];

				//cliqueTree.lowestUnusedParent(i) = 0;
			}
		}

		// Pass message and pop clique
		//DRWN_ASSERT_MSG(cliqueTree.numUnused(i) < 2, "There are no messages ready to be passed!");
		f(ct, i, j, sample, useCached, takeLog, -1);
		nextMsg.pop();

		// Multiply in final message
		if (j == -1) {
			if (!takeLog)
				result *= ct.lastMessage(i);
			else
				result += ct.lastMessage(i);
		}
				
		// If this message wasn't to itself...
		if (j != -1) {
			// ... decrement number of used neighbours of clique
			ct.numUnused(j)--;

			// ... push it if it is root and has 0 unused neighbours, or has 1 unused neighbour otherwise
			if (((j == root || !ct.isLeaf(j)) && !ct.numUnused(j)) || (j != root && ct.numUnused(j) == 1))
				nextMsg.push(j);
		}
	}

	// DEBUG: For polynomial copula, compare calculated answer and correct answer
	//if (boost::is_same<G::factor_type, drwnPolyCopula>::value)
	//	diffDebugModel(ct, sample, result);

	return result;
}

// Print clique tree. Used for debug
template <class G>
void drwnCumulativeUtils::printCliqueTree(drwnCliqueTree<G>& cliqueTree)
{
	// Loop over cliques
	for (unsigned int i = 0; i < cliqueTree.numCliques(); i++) {
		// DEBUG: Print out clique information
		wcout << "Clique " << i << " - Factors ";
		for (vector<int>::const_iterator it = cliqueTree.factors(i).begin(); it != cliqueTree.factors(i).end(); it++)
			wcout << *it << " ";
		wcout << " - Variables ";
		for (vector<int>::const_iterator it = cliqueTree.vars(i).begin(); it != cliqueTree.vars(i).end(); it++)
			wcout << cliqueTree.model().getUniverse()->varName(*it).c_str() << " ";
		wcout << " - Child " << cliqueTree.child(i) << " Sepset - ";
		for (vector<int>::const_iterator it = cliqueTree.sepset(i).begin(); it != cliqueTree.sepset(i).end(); it++)
			wcout << cliqueTree.model().getUniverse()->varName(*it).c_str() << " ";
		wcout << " Parents - ";
		for (vector<int>::const_iterator it = cliqueTree.parents(i).begin(); it != cliqueTree.parents(i).end(); it++)
			wcout << *it << " ";

		wcout << endl;
	}
}

// Print clique tree array
template <class G>
void drwnCumulativeUtils::printSampleCliques(drwnSampleCliques<G>& sc)
{
	wcout << "Array of Clique Trees for sampling" << endl << endl;
	for (unsigned int i = 0; i < sc.numVars(); i++) {
		wcout << "Variable " << i << " is " << sc.model().getUniverse()->varName(sc.samplingOrder(i)).c_str() << endl;
		wcout << "Clique Tree with Variable" << endl;
		drwnCumulativeUtils::printCliqueTree(sc.cliqueTree(sc.samplingOrder(i)));
		if (&sc.cliqueTreeMinus(sc.samplingOrder(i)) == 0)
			wcout << "No clique tree without variable" << endl;
		else {
			wcout << "Clique Tree without Variable" << endl;
			drwnCumulativeUtils::printCliqueTree(sc.cliqueTreeMinus(sc.samplingOrder(i)));
		}
		if (!sc.extraFactors(sc.samplingOrder(i)).size())
			wcout << "No extra Factors" << endl;
		else {
			wcout << "Extra factors" << endl;
			drwnSubset::printSubset(sc.extraFactors(sc.samplingOrder(i)));
			wcout << endl;
		}
		wcout << endl;
	}
}

// Print samples to csv file
template <class G>
bool drwnCumulativeUtils::printSamples(const G& cn, unsigned int numSamples, const wchar_t* filename)
{
	// DEBUG: Reseed random number generator
	/*unsigned int seed = 1355985858;		//std::time(0);
	rng.seed(seed);
	wcout << "Seed: " << seed << endl;*/

	// Open file
	wofstream myfile;
	myfile.open(filename);

	// Check that we were able to open file
	DRWN_ASSERT_MSG(myfile.is_open(), "Unable to open file")
		
	// Write first column with variable names
	for (unsigned int i = 0; i < cn.numVariables(); i++) {
		myfile << cn.getUniverse()->varName(i).c_str();
		if (i < cn.numVariables() - 1)
			myfile << " ";
		else
			myfile << endl;
	}

	// Create array of clique trees for sampling and data structure to hold temporary sample
	// DEBUG: fix our random seed, so that we have same sample cliques every time
	//srand (10);
	drwnSampleCliques<G> sc(cn);
	drwnSamples<G> samples(cn.numVariables(), 0);

	// DEBUG: Print the array of cliques
	//printSampleCliques(sc);

	// Generate samples
	for (unsigned int i = 0; i < numSamples; i++) {
		if (i % 200 == 0)
			wcout << "Sample #" << i << endl;
		drwnCumulativeUtils::sample(sc, samples);

		// Print sample to file
		for (unsigned int j = 0; j < cn.numVariables(); j++) {
			myfile << samples.temp()[j];
			if (j < cn.numVariables() - 1)
				myfile << " ";
			else
				myfile << endl;
		}
		/*for (unsigned int j = 0; j < cn.numVariables(); j++) {
			wcout << samples.temp()[j];
			if (j < cn.numVariables() - 1)
				wcout << " ";
			else
				wcout << endl;
		}*/
	}

	// Close file
	myfile.close();

	return true;
}

template<class T>
inline const T SIGN(const T &a, const T &b)
{
	return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}

// TODO: Make this into the same design pattern as drwnOptimizer etc., where it is encapsulated in a class, and you overload virtual method for function
template <typename F>
double drwnCumulativeUtils::zbrent(F& func, const double x1, const double x2, const double tol)
{
	const int ITMAX=100;
	const double EPS = numeric_limits<double>::epsilon();
	int iter;
	double a = x1, b = x2, c = x2, d, e, min1, min2;
	double fa = func(a), fb = func(b), fc, p, q, r, s, tol1, xm;

	if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
		DRWN_LOG(DRWN_LL_FATAL, "Root must be bracketed in zbrent");

	fc = fb;

	for (iter = 0; iter < ITMAX; iter++) {
		if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
			c = a;
			fc = fa;
			e = d = b - a;
		}

		if (fabs(fc) < fabs(fb)) {
			a = b;
			b = c;
			c = a;
			fa = fb;
			fb = fc;
			fc = fa;
		}

		tol1 = 2.0 * EPS * fabs(b) + 0.5 * tol;
		xm = 0.5 * (c - b);

		if (fabs(xm) <= tol1 || fb == 0.0)
			return b;

		if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
			s = fb/fa;
			if (a == c) {
				p = 2.0 * xm * s;
				q = 1.0 - s;
			}
			
			else {
				q = fa / fc;
				r = fb / fc;
				p = s * (2.0 * xm * q * (q-r) - (b-a) * (r - 1.0));
				q = (q - 1.0) * (r - 1.0) * (s - 1.0);
			}

			if (p > 0.0)
				q = -q;
			p = fabs(p);
			min1 = 3.0 * xm * q - fabs(tol1 * q);
			min2 = fabs(e * q);

			if (2.0 * p < (min1 < min2 ? min1 : min2)) {
				e = d;
				d = p / q;
			}
			
			else {
				d = xm;
				e = d;
			}
		}
		
		else {
			d = xm;
			e = d;
		}

		a = b;
		fa = fb;

		if (fabs(d) > tol1)
			b += d;
		else
			b += SIGN(tol1, xm);

		fb = func(b);
	}

	DRWN_LOG(DRWN_LL_FATAL, "Maximum number of iterations exceeded in zbrent");
	return 0.0;
}

template <class F>
double drwnCumulativeUtils::rtsafe(F& func, const double x1, const double x2, const double xacc)
{
	const int MAXIT=100;
	int j;
	double df, dx, dxold, f, fh, fl, temp, xh, xl, rts;
	func(x1, fl);
	func(x2, fh);

	if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
		DRWN_LOG(DRWN_LL_FATAL, "Root must be bracketed in rtsafe");

	if (fl == 0.0)
		return x1;

	if (fh == 0.0)
		return x2;

	if (fl < 0.0) {
		xl = x1;
		xh = x2;
	}
	
	else {
		xh = x1;
		xl = x2;
	}

	rts = 0.5 * (x1 + x2);
	dxold = fabs(x2 - x1);
	dx = dxold;
	func(rts, f, df);

	for (j = 0; j < MAXIT; j++) {
		if ((((rts - xh) * df - f) * ((rts - xl) * df - f) > 0.0) || (fabs(2.0 * f) > fabs(dxold * df))) {
			dxold = dx;
			dx = 0.5 * (xh - xl);
			rts = xl + dx;

			if (xl == rts)
				return rts;
		}
		
		else {
			dxold = dx;
			dx = f / df;
			temp = rts;
			rts -= dx;

			if (temp == rts)
				return rts;
		}

		if (fabs(dx) < xacc)
			return rts;

		func(rts, f, df);

		if (f < 0.0)
			xl = rts;

		else
			xh = rts;
	}

	DRWN_LOG(DRWN_LL_FATAL, "Maximum number of iterations exceeded in rtsafe");
	return 0.0;
}

// Sample using sampling clique trees structure (using Brent's method)
template <class G>
void drwnCumulativeUtils::sample(drwnSampleCliques<G>& sc, drwnSamples<G>& samples, int sampleNum, bool takeLog)
{
	class optFunc
	{
	public:
		G::marginal_type::range_type* sample, *copulaSample, *tempSample;
		drwnSamples<G>& samples_;
		drwnSampleCliques<G>& sc_;
		unsigned int thisVar;
		G::factor_type::range_type denom;
		G::factor_type::range_type k;
		bool takeLog_;

		drwnCliqueTree<G>* cliqueTree;
		drwnCliqueTree<G>* cliqueTreeMinus;
		const vector<int>* extraFactors;

		optFunc(drwnSampleCliques<G>& sc, drwnSamples<G>& samples, bool takeLog)
			: samples_(samples), sc_(sc), takeLog_(takeLog)
		{
			sample = samples.transform();
			copulaSample = samples.copula();
			tempSample = samples.temp();

			// Initialize sample when doing sampling over full model
			if (sc.numVars() == sc.model().numVariables()) {
				for (int i = 0; i < sc.model().numVariables(); i++) {
					sample[i] = 1.0;
					copulaSample[i] = 1.0;
					if (G::factor_type::isTransformCopula())
						tempSample[i] = G::factor_type::transformArgument(copulaSample[i]);
				}
			}

			// Otherwise, initialize sample when we are sampling over a subset of variables 
			else {
				// Marginalize out sampled variables
				for (unsigned int i = 0; i < sc.numVars(); i++) {
					sample[sc.samplingOrder(i)] = 1.0;
					copulaSample[sc.samplingOrder(i)] = 1.0;
					if (G::factor_type::isTransformCopula())
						tempSample[sc.samplingOrder(i)] = G::factor_type::transformArgument(copulaSample[sc.samplingOrder(i)]);
				}

				// Set conditioned variables to their conditioned values
				for (auto& v: sc.condVars()) {
					sample[v] = samples.cond()[v];
					copulaSample[v] = pow(sample[v], 1.0 / ((double) sc_.model().howManyScopes(v)));
					if (G::factor_type::isTransformCopula())
						tempSample[v] = G::factor_type::transformArgument(copulaSample[v]);
				}

				// Update cached derivatives of relevant factors
				for (auto& f: sc.condFactors()) {
					if (!G::factor_type::isTransformCopula())
						(sc_.model().factor(f)).cacheDerivatives(copulaSample, false, takeLog_);
					else
						(sc_.model().factor(f)).cacheDerivatives(tempSample, false, takeLog_);
				}
			}
		}

		double operator()(double x)
		{
			sample[thisVar] = x;
			copulaSample[thisVar] = pow(sample[thisVar], 1.0 / ((double) sc_.model().howManyScopes(thisVar)));
			if (G::factor_type::isTransformCopula())
				tempSample[thisVar] = G::factor_type::transformArgument(copulaSample[thisVar]);
			
			for (set<int>::const_iterator it = sc_.model().varNeighbors(thisVar).begin(); it != sc_.model().varNeighbors(thisVar).end(); it++) {
				if (!G::factor_type::isTransformCopula())
					(sc_.model().factor(*it)).cacheDerivatives(copulaSample, false, takeLog_);
				else
					(sc_.model().factor(*it)).cacheDerivatives(tempSample, false, takeLog_);
			}

			if (sc_.extraFactors(thisVar).size()) {
				if (!takeLog_)
					return copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true, false, sc_.extraFactors(thisVar)) / denom - k;
				else
					return copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true, true, sc_.extraFactors(thisVar)) - denom - k;
			}
			else {
				if (!takeLog_)
					return copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true) / denom - k;
				else
					return copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true, true) - denom - k;
			}
		}
	};

	optFunc thisOpt(sc, samples, takeLog);
	
	//wcout << "sample " << sampleNum << endl;

	// Loop over variables
	for (unsigned int i = 0; i < sc.numVars(); i++) {
		// If first variable, then just sample from uniform normal distribution
		thisOpt.thisVar = sc.samplingOrder(i);
		if (&sc.cliqueTreeMinus(thisOpt.thisVar) == 0) {
			thisOpt.sample[thisOpt.thisVar] = drwnCumulativeUtils::die();

			// DEBUG: Output first variable to file
			//firstVar << thisOpt.sample[thisOpt.thisVar] << endl;

			thisOpt.copulaSample[thisOpt.thisVar] = pow(thisOpt.sample[thisOpt.thisVar], 1.0 / ((double) sc.model().howManyScopes(thisOpt.thisVar)));
			if (G::factor_type::isTransformCopula())
				thisOpt.tempSample[thisOpt.thisVar] = G::factor_type::transformArgument(thisOpt.copulaSample[thisOpt.thisVar]);

			// Cache all factors with this variable
			for (set<int>::const_iterator it = sc.model().varNeighbors(thisOpt.thisVar).begin(); it != sc.model().varNeighbors(thisOpt.thisVar).end(); it++) {
				if (!G::factor_type::isTransformCopula())
					(sc.model().factor(*it)).cacheDerivatives(thisOpt.copulaSample, false, takeLog);
				else
					(sc.model().factor(*it)).cacheDerivatives(thisOpt.tempSample, false, takeLog);
			}
		}
	
		// Else, have to perform a root solving method to invert conditional CDF
		else {
			// Fetch clique trees
			thisOpt.cliqueTree = &sc.cliqueTree(thisOpt.thisVar);
			thisOpt.cliqueTreeMinus = &sc.cliqueTreeMinus(thisOpt.thisVar);
			thisOpt.extraFactors = &sc.extraFactors(thisOpt.thisVar);

			// Calculate denominator
			thisOpt.denom = copulaDensity(*thisOpt.cliqueTreeMinus, thisOpt.samples_, true, takeLog);

			// DEBUG: Output denominator to file
			//drwnCumulativeUtils::denom << denom << endl;
						
			// Choose a random [0, 1] value
			if (!takeLog)
				thisOpt.k = drwnCumulativeUtils::die();
			else
				thisOpt.k = log(drwnCumulativeUtils::die());

			// DEBUG: Breakpoint on troubled sample, print objective at that point
			/*if (sampleNum == 438) {
				//bool gotHere = true;

				// Output values of objective to file
				// DEBUG: Output values on grid
				wofstream grid;
				grid.open(L"rootFinding.txt");
				grid.precision(std::numeric_limits<double>::digits10);
				const unsigned int fineness = 1000;
				for (unsigned int j = 0; j < fineness; j++) {
					double xCoord = 0 + j * (1.0 / ((double) fineness - 1));
			
					grid << thisOpt(xCoord) << " ";
				}
				grid.close();
			}*/

			// DEBUG: Output k's to file
			//kS << thisOpt.k << endl;

			// If near 1.0 still gives you -k then set equal to first variable so far
			// TODO: Only do this for Archimedean case
			/*if (thisOpt(1 - 1e-9) == thisOpt(1e-9)) {
				//bool gotHere = true;
				thisOpt.sample[thisOpt.thisVar] = thisOpt.sample[sc.samplingOrder(0)];
			}*/

			// Use Brent's method to find root
			//else
			zbrent(thisOpt, 0, 1, 1e-8);
		}
	}

	// Transform sample using quantiles of marginals
	typename G::marginal_type::range_type* finalSample = ((sampleNum == -1) ? samples.temp() : samples[sampleNum]);
	for (int i = 0; i < sc.model().numVariables(); i++)
		finalSample[i] = sc.model().marginal(i).quantile(thisOpt.sample[i]);
}

// Sample using sampling clique trees structure (and bisection / Newton's method)
template <class G>
void drwnCumulativeUtils::sample2(drwnSampleCliques<G>& sc, drwnSamples<G>& samples, int sampleNum)
{
    class optFunc
	{
	public:
		G::marginal_type::range_type* sample, *copulaSample, *tempSample;
		drwnSamples<G>& samples_;
		drwnSampleCliques<G>& sc_;
		unsigned int thisVar;
		G::factor_type::range_type denom;
		G::factor_type::range_type k;

		drwnCliqueTree<G>* cliqueTree;
		drwnCliqueTree<G>* cliqueTreeMinus;
		const vector<int>* extraFactors;

		optFunc(drwnSampleCliques<G>& sc, drwnSamples<G>& samples)
			: samples_(samples), sc_(sc)
		{
			// Initialize sample
			sample = samples.transform();
			copulaSample = samples.copula();
			tempSample = samples.temp();
			for (int i = 0; i < sc.model().numVariables(); i++) {
				sample[i] = 1.0;
				copulaSample[i] = 1.0;
				if (G::factor_type::isTransformCopula())
					tempSample[i] = G::factor_type::transformArgument(copulaSample[i]);
			}
		}

		void operator()(double x, double& val)
		{
			sample[thisVar] = x;
			copulaSample[thisVar] = pow(sample[thisVar], 1.0 / ((double) sc_.model().howManyScopes(thisVar)));
			if (G::factor_type::isTransformCopula())
				tempSample[thisVar] = G::factor_type::transformArgument(copulaSample[thisVar]);
			
			for (set<int>::const_iterator it = sc_.model().varNeighbors(thisVar).begin(); it != sc_.model().varNeighbors(thisVar).end(); it++) {
				if (!G::factor_type::isTransformCopula())
					(sc_.model().factor(*it)).cacheDerivatives(copulaSample);
				else
					(sc_.model().factor(*it)).cacheDerivatives(tempSample);
			}

			if (sc_.extraFactors(thisVar).size())
				val = copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true, false, sc_.extraFactors(thisVar)) / denom - k;
			else
				val = copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true) / denom - k;
		}

		void operator()(double x, double& val, double& deriv)
		{
			sample[thisVar] = x;
			copulaSample[thisVar] = pow(sample[thisVar], 1.0 / ((double) sc_.model().howManyScopes(thisVar)));
			if (G::factor_type::isTransformCopula())
				tempSample[thisVar] = G::factor_type::transformArgument(copulaSample[thisVar]);
			
			for (set<int>::const_iterator it = sc_.model().varNeighbors(thisVar).begin(); it != sc_.model().varNeighbors(thisVar).end(); it++) {
				if (!G::factor_type::isTransformCopula())
					(sc_.model().factor(*it)).cacheDerivatives(copulaSample);
				else
					(sc_.model().factor(*it)).cacheDerivatives(tempSample);
			}

			// Calculate value
			if (sc_.extraFactors(thisVar).size())
				val = copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true, false, sc_.extraFactors(thisVar)) / denom - k;
			else
				val = copulaDensity(sc_.cliqueTreeMinus(thisVar), samples_, true) / denom - k;

			// Calculate derivative
			deriv = copulaDensity(sc_.cliqueTree(thisVar), samples_, true) / denom;
		}
	};

	optFunc thisOpt(sc, samples);
		
	// Loop over variables
	for (int i = 0; i < sc.model().numVariables(); i++) {
		// If first variable, then just sample from uniform normal distribution
		thisOpt.thisVar = sc.samplingOrder(i);
		if (&sc.cliqueTreeMinus(thisOpt.thisVar) == 0) {
			thisOpt.sample[thisOpt.thisVar] = drwnCumulativeUtils::die();

			// DEBUG: Output first variable to file
			//firstVar << thisOpt.sample[thisOpt.thisVar] << endl;

			thisOpt.copulaSample[thisOpt.thisVar] = pow(thisOpt.sample[thisOpt.thisVar], 1.0 / ((double) sc.model().howManyScopes(thisOpt.thisVar)));
			if (G::factor_type::isTransformCopula())
				thisOpt.tempSample[thisOpt.thisVar] = G::factor_type::transformArgument(thisOpt.copulaSample[thisOpt.thisVar]);

			// Cache all factors with this variable
			for (set<int>::const_iterator it = sc.model().varNeighbors(thisOpt.thisVar).begin(); it != sc.model().varNeighbors(thisOpt.thisVar).end(); it++) {
				if (!G::factor_type::isTransformCopula())
					(sc.model().factor(*it)).cacheDerivatives(thisOpt.copulaSample);
				else
					(sc.model().factor(*it)).cacheDerivatives(thisOpt.tempSample);
			}
		}
	
		// Else, have to perform a root solving method to invert conditional CDF
		else {
			// Fetch clique trees
			thisOpt.cliqueTree = &sc.cliqueTree(thisOpt.thisVar);
			thisOpt.cliqueTreeMinus = &sc.cliqueTreeMinus(thisOpt.thisVar);
			thisOpt.extraFactors = &sc.extraFactors(thisOpt.thisVar);

			// Calculate denominator
			thisOpt.denom = copulaDensity(*thisOpt.cliqueTreeMinus, thisOpt.samples_, true);

			// DEBUG: Output denominator to file
			//drwnCumulativeUtils::denom << denom << endl;
						
			// Choose a random [0, 1] value
			thisOpt.k = drwnCumulativeUtils::die();

			// DEBUG: Output k's to file
			//kS << thisOpt.k << endl;

			// Use Brent's method to find root
			rtsafe(thisOpt, 0, 1, 1e-8);
		}
	}

	// Transform sample using quantiles of marginals
	typename G::marginal_type::range_type* finalSample = ((sampleNum == -1) ? samples.temp() : samples[sampleNum]);
	for (int i = 0; i < sc.model().numVariables(); i++)
		finalSample[i] = sc.model().marginal(i).quantile(thisOpt.sample[i]);
}

// Calculate (per sample) log-likelihood and gradient of log-likelihood for full sample
template<class G>
typename G::factor_type::range_type drwnCumulativeUtils::likelihoodGradient(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, typename G::factor_type::domain_type* gradient, typename G::factor_type::domain_type* gradientSum, bool takeLog)
{
	// DEBUG: Save sample densities to file
	//densities.open(L"density.txt");
	//densities.precision(std::numeric_limits<double>::digits10);
	//densities << "density" << endl;

	// Calculate full log gradient
	typename G::factor_type::range_type logLikelihood = 0;
	for (unsigned int i = 0; i < ct.model().numFactors(); i++)
		gradientSum[i] = 0;
	for (unsigned int j = 0; j < samples.numSamples(); j++) {
		// DEBUG: Set flag on second sample
		//if (j == 1)
		//	stopYet = true;

		G::factor_type::range_type sampleLogLikelihood = calculateGradient(ct, samples[j], gradient, takeLog);	// / samples.numSamples();
		//densities << sampleLogLikelihood << endl;

		logLikelihood += sampleLogLikelihood / samples.numSamples();
		for (unsigned int i = 0; i < ct.model().numFactors(); i++) {
			gradientSum[i] += gradient[i] / samples.numSamples();	// ;	// Take average to avoid overflow 
		}
	}

	// DEBUG
	//densities.close();

	return logLikelihood;
}