/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnLearnModel.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//! Class for learning model parameters using an arbitrary optimizer class
template <class G = normalCumulativeNet, class C = drwnGradientDescent>
class drwnLearnModel : public C
{
protected:
	//! File streams for output
	wofstream _solutions, _likelihoods, _gradients;

	//! Copula samples, with marginals learnt by the constructor
	drwnCopulaSamples<G>& _copulaSamples;

	//! Clique tree to perform inference on model
	drwnCliqueTree<G>& _ct;

	//! Maximum number of iterations during optimization
	static const int maxIter = 100;
	
	//! The gradient is calculated one sample at a time so we have to have some extra memory for it
	volatile double* _dfSingle;

	bool _isCmarData, _takeLog;

	unsigned int _iter;

public:
	//! Default constructor from clique tree and array of samples
	drwnLearnModel(drwnCliqueTree<G>& ct, drwnSamples<G>& samples, bool learnMarginals = true, bool isCmarData = false)
		: _copulaSamples(*(new drwnCopulaSamples<G>(samples, ct, learnMarginals))), _ct(ct), C(ct.model().numFactors()), _isCmarData(isCmarData)
	{
		_dfSingle = new double[ct.model().numFactors()];
	}

	//! Default constructor from clique tree and array of copula samples
	drwnLearnModel(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, bool isCmarData = false)
		: _copulaSamples(samples), _ct(ct), C(ct.model().numFactors()), _isCmarData(isCmarData)
	{
		_dfSingle = new double[ct.model().numFactors()];
	}


	//! Default destructor
	~drwnLearnModel()
	{
		delete [] _dfSingle;
	}

	// Have to abstract how to access iteration, because different mechanism for piecewise learning
	virtual unsigned int& iterations() { return _iter; }

	//! Learn model parameters
	// NOTE: You have to initialize parameters yourself, defaults to all zeros
	double learn(const double eps = 1e-6, wchar_t* solnOutput = L"solutions.txt", wchar_t* llOutput = L"loglikelihoods.txt", wchar_t* gradOutput = L"gradients.txt", bool showOutput = true, bool takeLog = false)
	{
		iterations() = 0;
		_takeLog = takeLog;

		// Initialize output files if required
		if (solnOutput) {
			_solutions.open(solnOutput);
			_solutions.precision(std::numeric_limits<double>::digits10);
			for (unsigned int i = 0; i < _ct.model().numFactors(); i++)
				_solutions << "rho" << i << " ";
			_solutions << endl;
		}
		if (llOutput) {
			_likelihoods.open(llOutput);
			_likelihoods.precision(std::numeric_limits<double>::digits10);
			_likelihoods << "loglikes" << endl;
		}
		if (gradOutput) {
			_gradients.open(gradOutput);
			_gradients.precision(std::numeric_limits<double>::digits10);	
			_gradients << "normGradients" << endl;
		}

		// Initialize the parameters with the ones in the model
		for (unsigned int i = 0; i < _n; i++)
			_x[i] = _ct.model().factor(i).getParameter();

		// Output likelihood and parameters before first iteration
		if (showOutput)
			monitor(0, objective(_x));

		// This function was really just a wrapper for the solve from parent class
		// NOTE: First parameter is maximum number of iterations
		double value = solve(100, eps, showOutput);	// maxIter

		// Close files
		if (solnOutput)
			_solutions.close();
		if (llOutput)
			_likelihoods.close();
		if(gradOutput)
			_gradients.close();

		return value;
	}

	// Returns value of objective function at point x
	virtual double objective(const double *x) const
	{
		// We need to copy parameters from memory into the respective factors
		for (unsigned int i = 0; i < _n; i++) {
			// Return Inf if parameter is out of bounds
			if (!_ct.model().factor(i).isParameterValid(x[i]))
				return std::numeric_limits<double>::max();

			_ct.model().factor(i).setParameter(x[i]);
		}

		// Calculate objective
		double negLogLikelihood = 0.0;
		for (unsigned int j = 0; j < _copulaSamples.numSamples(); j++) {
			if (_isCmarData)
				_ct.generateMasks(_copulaSamples[j], true);
			// TODO: Replace "false" with "takeLog" in the following line after tested log-space gradient
			negLogLikelihood -= drwnCumulativeUtils::calculateGradient(_ct, _copulaSamples[j], (typename G::factor_type::domain_type*) _dfSingle, _takeLog) / (double) _copulaSamples.numSamples();
		}

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
			if (!_ct.model().factor(i).isParameterValid(x[i])) {
				for (unsigned int j = 0; j < _n; j++)
					df[j] = 0;
				return std::numeric_limits<double>::max();
			}

			_ct.model().factor(i).setParameter(x[i]);
		}

		// Initialize log-likelihood and gradient
		double negLogLikelihood = 0.0;
		for (unsigned int i = 0; i < _n; i++)
			df[i] = 0.0;

		// Calculate gradient for each sample
		for (unsigned int j = 0; j < _copulaSamples.numSamples(); j++) {
			if (_isCmarData)
				_ct.generateMasks(_copulaSamples[j], true);
			// TODO: Replace "false" with "takeLog" in the following line after tested log-space gradient
			negLogLikelihood -= drwnCumulativeUtils::calculateGradient(_ct, _copulaSamples[j], (typename G::factor_type::domain_type*) _dfSingle, _takeLog) / (double) _copulaSamples.numSamples();
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

		if (_solutions.is_open())
			_ct.model().printParameters(_solutions);
		if (_likelihoods.is_open())
			_likelihoods << objValue << endl;
	}
};