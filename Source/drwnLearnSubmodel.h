/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnLearnSubmodel.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//! Class for learning model parameters using an arbitrary optimizer class
template <class G = normalCumulativeNet, class C = drwnOptimizer2>
class drwnLearnSubmodel : public C
{
protected:
	//! Copula samples, with marginals learnt by the constructor
	drwnCopulaSamples<G>& _copulaSamples;

	//! Clique tree to perform inference on model
	drwnCliqueTree<G>& _ct;

	//! Vector of (clique, factor index) pairs to calculate gradient for
	vector<pair<unsigned int, unsigned int> > _optFactors;

	//! Maximum number of iterations during optimization
	static const int maxIter = 100;
	
	//! The gradient is calculated one sample at a time so we have to have some extra memory for it
	volatile double* _dfSingle;

	unsigned int& _iter;

	bool _takeLog, _isActive;

	unsigned int _order;

	//! Parameters at last outer iteration
	VectorXd _lastPars;

	//! Gradient at last outer iteration
	VectorXd _lastGrad;

	//! Log-likelihood at last outer iteration
	double _lastLoglike;

	typename G::factor_type& factor(unsigned int i) const { return _ct.model().factor(_ct.factors(_optFactors[i].first)[_optFactors[i].second]); }

public:
	//! Default constructor from clique tree and array of copula samples
	drwnLearnSubmodel(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, set<int>& factors, double* sharedGradient, unsigned int& sharedIterCount, unsigned int order = 0)
		: _copulaSamples(samples), _ct(ct), C(factors.size()), _dfSingle(sharedGradient), _iter(sharedIterCount), _order(order), _lastPars(factors.size()), _lastGrad(factors.size()), _isActive(false), _lastLoglike(std::numeric_limits<double>::infinity())
	{
		// DEBUG
		//drwnCumulativeUtils::printCliqueTree(ct);

		// Fill optFactors
		_optFactors.reserve(factors.size());
		for (unsigned int i = 0; i < ct.numCliques(); i++) {
			for (unsigned int j = 0; j < ct.factors(i).size(); j++) {
				if (factors.count(ct.factors(i)[j]))
					_optFactors.push_back(make_pair(i, j));
			}
		}
	}


	//! Default destructor
	~drwnLearnSubmodel()
	{
		// NOTE: I don't think a reference is deleted automatically, so probably a memory leak in other class...
		delete &_ct;
	}

	// Have to abstract how to access iteration, because different mechanism for piecewise learning
	virtual unsigned int& iterations() { return _iter; }

	bool isActive() const { return _isActive; }

	//! Print factors and clique tree over this submodel, for debug purposes
	void print()
	{
		wcout << "Factors" << endl;
		for (auto& optPair : _optFactors)
			wcout << "clique " << optPair.first << ", factor " << optPair.second << endl;
		wcout << "Clique Tree" << endl;
		drwnCumulativeUtils::printCliqueTree(_ct);
	}

	void printOrder()
	{
		wcout << _order << " ";
	}

	void printGradient()
	{
		drwnCumulativeUtils::printGradient(_ct, _copulaSamples, _takeLog);
	}

	void printSubgradient()
	{
		objectiveAndGradient(_x, _df);
		wcout << "Subgradient: ";
		for (unsigned int i = 0; i < _n; i++)
			wcout << _df[i] << " ";
		wcout << endl;
	}

	void printSubgradient2()
	{
		// Push all factors
		vector<pair<unsigned int, unsigned int> > optFactors;
		for (unsigned int i = 0; i < _ct.numCliques(); i++) {
			for (unsigned int j = 0; j < _ct.numFactors(i); j++) {
				optFactors.push_back(make_pair(i, j));
			}
		}
		
		/*optFactors.erase(optFactors.begin());
		optFactors.erase(optFactors.begin() + 1);*/

		unsigned int n = optFactors.size();
		double* df = new double[n];
		double* dfSingle = new double[n];
		double* x = new double[n];

		// We need to copy parameters from memory into the respective factors
		for (unsigned int i = 0; i < n; i++) {
			x[i] = _ct.model().factor(_ct.factors(optFactors[i].first)[optFactors[i].second]).getParameter();
		}

		// Initialize log-likelihood and gradient
		double negLogLikelihood = 0.0;
		for (unsigned int i = 0; i < n; i++)
			df[i] = 0.0;

		// Calculate gradient for each sample
		for (unsigned int j = 0; j < _copulaSamples.numSamples(); j++) {
			negLogLikelihood -= drwnCumulativeUtils::calculateSubgradient(_ct, optFactors, _copulaSamples[j], (typename G::factor_type::domain_type*) dfSingle, _takeLog) / (double) _copulaSamples.numSamples();
			for (unsigned int i = 0; i < n; i++)
				df[i] -= dfSingle[i] / (double) _copulaSamples.numSamples();	// Take average to avoid overflow 
		}

		wcout << "Subgradient: ";
		for (unsigned int i = 0; i < n; i++)
			wcout << df[i] << " ";
		wcout << endl;

		delete [] df;
		delete [] x;
		delete [] dfSingle;
	}

	//! Learn model parameters
	// NOTE: You have to initialize parameters yourself, defaults to all zeros
	double learn(const double eps = 1e-6, bool showOutput = true, bool takeLog = false)
	{
		_takeLog = takeLog;

		// Initialize the parameters with the ones in the model
		for (unsigned int i = 0; i < _n; i++)
			_x[i] = factor(i).getParameter();

		// Output likelihood and parameters before first iteration
		if (showOutput)
			monitor(0, objective(_x));

		// This function was really just a wrapper for the solve from parent class
		double value = solve(100, eps, showOutput);

		// Test if submodel is "active," that is, it has converged with respect to outer iterations
		Map<VectorXd> _learntPars(_x, _n), _learntGrad(_df, _n);
		if (((_learntPars - _lastPars).squaredNorm() / (double) _n < eps) || (abs(_lastLoglike - value) < eps))
			_isActive = true;

		// Once a subproblem has become active, it can still switch back to being inactive
		else
			_isActive = false;

		// Make a record of values for next outer iteration
		_lastPars = _learntPars;
		_lastGrad = _learntGrad;
		_lastLoglike = value;
		
		/*unsigned int maxOuterIter = 10;
		double value;
		do {
			value = solve(100 - iterations(), eps, showOutput);	// maxIter
			maxOuterIter--;
			//if (showOutput)
			//	monitor(outerIter++, objective(_x));	// AndGradient , _df
		} while (!isSuccess() && maxOuterIter && iterations() < 100);*/
		return value;
	}

	// Returns value of objective function at point x
	virtual double objective(const double *x) const
	{
		// We need to copy parameters from memory into the respective factors
		for (unsigned int i = 0; i < _n; i++) {
			// Return Inf if parameter is out of bounds
			if (!factor(i).isParameterValid(x[i]))
				return std::numeric_limits<double>::max();

			factor(i).setParameter(x[i]);
		}

		// Calculate objective
		double negLogLikelihood = 0.0;
		for (unsigned int j = 0; j < _copulaSamples.numSamples(); j++)
			negLogLikelihood -= drwnCumulativeUtils::calculateSubgradient(_ct, _optFactors, _copulaSamples[j], (typename G::factor_type::domain_type*) _dfSingle, _takeLog) / (double) _copulaSamples.numSamples();

		// Need to normalize gradient so can't take step outside
		return negLogLikelihood;
	}

	// Populates gradient of objective function at point x
	virtual void gradient(const double *x, double *df) const
	{
		objectiveAndGradient(x, df);
	}

	// Returns value of objective function and populates gradient df at point x
	virtual double objectiveAndGradient(const double *x, double *df) const
	{
		// We need to copy parameters from memory into the respective factors
		for (unsigned int i = 0; i < _n; i++) {
			// Return Inf if parameter is out of bounds
			if (!factor(i).isParameterValid(x[i])) {
				for (unsigned int j = 0; j < _n; j++)
					df[j] = 0;
				return std::numeric_limits<double>::max();
			}

			factor(i).setParameter(x[i]);
		}

		// Initialize log-likelihood and gradient
		double negLogLikelihood = 0.0;
		for (unsigned int i = 0; i < _n; i++)
			df[i] = 0.0;

		// Calculate gradient for each sample
		for (unsigned int j = 0; j < _copulaSamples.numSamples(); j++) {
			negLogLikelihood -= drwnCumulativeUtils::calculateSubgradient(_ct, _optFactors, _copulaSamples[j], (typename G::factor_type::domain_type*) _dfSingle, _takeLog) / (double) _copulaSamples.numSamples();
			for (unsigned int i = 0; i < _n; i++)
				df[i] -= _dfSingle[i] / (double) _copulaSamples.numSamples();	// Take average to avoid overflow 
		}
		
		return negLogLikelihood;
	}

	//! Callback for each iteration during optimization (if \b bMonitor is \a true)
    virtual void monitor(unsigned iter, double objValue)
	{
		// TODO: Output to files if open
		wcout << "iteration " << iter << ", objective " << objValue << endl;
	}
};