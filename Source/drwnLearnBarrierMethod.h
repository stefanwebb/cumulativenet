/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnLearnBarrierMethod.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include "drwnLearnModel.h"

//! Class for learning model parameters subject to box constraints on parameters using barrier method
template <class G = normalCumulativeNet, class C = drwnOptimizer2>
class drwnLearnBarrierMethod : public drwnLearnModel<G, C>
{
protected:
	// Controls trade off between regular objective and barrier function
	// As t->Inf, solution approaches optimal solution
	float _t;

public:
	//! Default constructor from clique tree and array of samples
	drwnLearnBarrierMethod(drwnCliqueTree<G>& ct, drwnSamples<G>& samples, bool learnMarginals = true, bool isCmarData = false)
		: drwnLearnModel<G, C>(ct, samples, learnMarginals, isCmarData), _t(1.0)
	{	
	}

	//! Default constructor from clique tree and array of copula samples
	drwnLearnBarrierMethod(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, bool isCmarData = false)
		: drwnLearnModel<G, C>(ct, samples, isCmarData), _t(1.0)
	{
	}
	
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
			for (unsigned int i = 0; i < _ct.model().numFactors(); i++)
				_gradients << "dRho" << i << " ";
			_gradients << endl;
		}

		// Initialize the parameters with the ones in the model
		for (unsigned int i = 0; i < _n; i++)
			_x[i] = _ct.model().factor(i).getParameter();

		// Output likelihood and parameters before first iteration
		unsigned int outerIter = 0;
		if (showOutput)
			monitor(outerIter++, objectiveAndGradient(_x, _df));

		// DEBUG: Experiment with starting value for t
		_t = 2;
		const double mu = 10;

		// Outer iterations of barrier method
		// TODO: If fails 3 times in a row, terminate with failure
		

		// Outer iterations of barrier method
		double value;
		do {
			//value = solve(1000, eps, showOutput);	// maxIter

			// Do an inner iteration of restart-on-error
			unsigned int maxOuterIter = 10;
			do {
				value = solve(100 - iterations(), eps, showOutput);	// maxIter
				maxOuterIter--;
				//if (showOutput)
				//	monitor(outerIter++, objective(_x));	// AndGradient , _df
			} while (!isSuccess() && maxOuterIter && iterations() < 100);
			
			_t *= mu;
			if (showOutput)
				monitor(outerIter++, value);	// AndGradient , _df
		} while (_n / _t >= eps);

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
		// NOTE: Need to do this first so factors are set to x
		double obj = drwnLearnModel<G, C>::objective(x);

		// Loop over factors, calculating barrier function terms
		for (unsigned int i = 0; i < _n; i++)
			obj += _ct.model().factor(i).barrierFunction(_t);

		if (boost::math::isnan(obj))
			return std::numeric_limits<double>::infinity();
		else
			return obj;
	}

	// Populates gradient of objective function at point x
	virtual void gradient(const double *x, double *df) const
	{
		objectiveAndGradient(x, df);
	}

	// Returns value of objective function and populates gradient df at point x
	virtual double objectiveAndGradient(const double *x, double *df) const
	{
		double obj = drwnLearnModel<G, C>::objectiveAndGradient(x, df);

		// Loop over factors, calculating barrier function terms and adjusting derivatives
		// Loop over factors, calculating barrier function terms
		for (unsigned int i = 0; i < _n; i++) {
			obj += _ct.model().factor(i).barrierFunction(_t);
			df[i] += _ct.model().factor(i).barrierDerivative(_t);
		}

		if (boost::math::isnan(obj))
			return std::numeric_limits<double>::infinity();
		else
			return obj;
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