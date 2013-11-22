/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnLearnRestartOnError.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include "drwnLearnModel.h"

//! Class for learning model parameters subject to box constraints on parameters using barrier method
template <class G = normalCumulativeNet, class C = drwnOptimizer2>
class drwnLearnRestartOnError : public drwnLearnModel<G, C>
{
protected:
	//! True parameters, used to calculate MSE
	VectorXd _truePars;

	Map<VectorXd> _learntPars;

	double _lastTime;

public:
	//! Default constructor from clique tree and array of samples
	drwnLearnRestartOnError(drwnCliqueTree<G>& ct, drwnSamples<G>& samples, bool learnMarginals = true, bool isCmarData = false)
		: drwnLearnModel<G, C>(ct, samples, learnMarginals, isCmarData), _learntPars(_x, ct.model().numFactors()), _truePars(ct.model().numFactors())
	{	
	}

	//! Default constructor from clique tree and array of copula samples
	drwnLearnRestartOnError(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, bool isCmarData = false)
		: drwnLearnModel<G, C>(ct, samples, isCmarData), _learntPars(_x, ct.model().numFactors()), _truePars(ct.model().numFactors())
	{
	}

	void loadTruePars()
	{
		for (unsigned int i = 0; i < _n; i++)
			_truePars[i] = _ct.model().factor(i).getParameter();
	}

	// TODO: Instead of having to load learnt parameters each time, get directly from model
	double mse()
	{
		return (_truePars - _learntPars).squaredNorm() / (double) _truePars.size();
	}

	double time() { return _lastTime; }

	//! Learn model parameters
	// NOTE: You have to initialize parameters yourself, defaults to all zeros
	double learn(const double eps = 1e-6, wchar_t* solnOutput = L"solutions.txt", wchar_t* llOutput = L"loglikelihoods.txt", wchar_t* gradOutput = L"gradients.txt", bool showOutput = true, bool takeLog = false, bool showOuterIter = false)
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
			for (unsigned int i = 0; i < _ct.model().numFactors(); i++)
				_gradients << "dRho" << i << " ";
			_gradients << endl;
		}

		// Initialize the parameters with the ones in the model
		for (unsigned int i = 0; i < _n; i++)
			_x[i] = _ct.model().factor(i).getParameter();

		// DEBUG: For learning shape model
		if (showOutput)
			wcout << "Calculating objective" << endl;

		clock_t t = clock();

		// Output likelihood and parameters before first iteration
		unsigned int outerIter = 0;
		if (showOutput)
			monitor(outerIter++, objectiveAndGradient(_x, _df));

		// Outer iterations of barrier method
		// TODO: If fails 3 times in a row, terminate with failure

		// DEBUG: For learning shape model
		if (showOutput)
			wcout << "Starting first iteration" << endl;

		unsigned int maxOuterIter = 10;
		double value;
		do {
			value = solve(100 - iterations(), eps, showOutput);	// maxIter
			maxOuterIter--;
			//if (showOutput)
			//	monitor(outerIter++, objective(_x));	// AndGradient , _df
		} while (!isSuccess() && maxOuterIter && iterations() < 100);

		t = clock() - t;
		_lastTime = (((double)t) / (double)CLOCKS_PER_SEC);

		// DEBUG: I think the parameters should be equal in the model and _x, so following isn't necessary
		//for (unsigned int i = 0; i < _n; i++)
		//	_x[i] = _ct.model().factor(i).getParameter();

		// Close files
		if (solnOutput)
			_solutions.close();
		if (llOutput)
			_likelihoods.close();
		if(gradOutput)
			_gradients.close();

		return value;
	}
	
	// Callback for each iteration during optimization (if \b bMonitor is \a true)
	virtual void monitor(unsigned iter, double objValue)
	{
		// TODO: Output to files if open
		wcout << "iteration " << iter << ", objective " << objValue << endl;

		if (_solutions.is_open())
			_ct.model().printParameters(_solutions);
		if (_likelihoods.is_open())
			_likelihoods << objValue << endl;
		if (_gradients.is_open()) {
			for (unsigned int i = 0; i < _n; i++)
				_gradients << _df[i] << " ";
			_gradients << endl;
		}
	}
};