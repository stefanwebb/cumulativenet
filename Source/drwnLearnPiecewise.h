/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnLearnPiecewise.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include <algorithm>
#include "drwnLearnSubmodel.h"

using namespace Eigen;

enum SubmodelType { PAIRS, SINGLES, BOTH };

//! Class for learning model parameters using an arbitrary optimizer class
template <class G = normalCumulativeNet, class C = drwnOptimizer2>
class drwnLearnPiecewise
{
protected:
	//! File streams for output
	wofstream _solutions, _likelihoods, _gradients;

	//! Copula samples, with marginals learnt by the constructor
	drwnCopulaSamples<G>& _copulaSamples;

	//! Clique tree to perform inference on model
	drwnCliqueTree<G>& _ct;

	//! Maximum number of iterations during optimization
	static const int _maxIters = 100;
	
	//! The gradient is calculated one sample at a time so we have to have some extra memory for it
	volatile double* _dfSingle;

	double* _x;

	unsigned int _n;

	unsigned int _iter;
	
	unsigned int _lastIterations;

	//! True parameters, used to calculate MSE
	VectorXd _truePars;

	//! Array of submodel structures to learn over
	vector<drwnLearnSubmodel<G, C>* > _submodels;

	Map<VectorXd> _learntPars;

	bool _takeLog, _hasTruePars;

	double _lastTime;

public:
	//! Default constructor from clique tree and array of copula samples
	drwnLearnPiecewise(drwnCliqueTree<G>& ct, drwnCopulaSamples<G>& samples, bool takeLog, SubmodelType submodelType = SINGLES)
		: _copulaSamples(samples), _ct(ct), _n(ct.model().numFactors()), _takeLog(takeLog), _learntPars(_x, ct.model().numFactors()), _truePars(ct.model().numFactors()), _hasTruePars(false)
	{
		_x = new double[_n];
		_dfSingle = new double[_n];
		new (&_learntPars) Map<VectorXd>(_x, _n);

		// Build submodels over each variable
		if (submodelType == SINGLES || submodelType == BOTH) {
			for(int i = 0; i < _ct.model().numFactors(); i++) {
				set<int> factors;
				factors.insert(i);
				set<int> vars(_ct.model().factor(i).getVars().begin(), _ct.model().factor(i).getVars().end());
				_submodels.push_back(new drwnLearnSubmodel<G,C>(*(new drwnCliqueTree<G>(_ct.model(), vars)), _copulaSamples, factors, (double*) _dfSingle, iterations(), _submodels.size()));
			}
		}

		// Build submodels over adjacent pairs of factors
		if (submodelType == PAIRS || submodelType == BOTH) {
			// Construct list of neighbours of each factor
			vector<set<int> > neighbours(_ct.model().numFactors());
			for(int i = 0; i < _ct.model().numFactors(); i++) {
				// For nonempty factors
				// NOTE: I totally forgot that factors can be removed...
				const G::factor_type& factor = _ct.model().factor(i);
				if (&factor) {
					// Loop over all variables in factor
					for (auto& v : factor.getVars()) {
						// Loop over all factors that have variable in its scope
						for (auto& f : _ct.model().varNeighbors(v)) {
							if (f != i)
								neighbours[i].insert(f);
						}
					}
				}
			}
	
			// Create submodel learning object over all pairs
			for (unsigned int i = 0; i < _ct.model().numFactors(); i++) {
				for (auto& f : neighbours[i]) {
					// Don't want to create pair twice
					if (f > i) {
						// Calculate union of scope of factors
						set<int> vars(_ct.model().factor(i).getVars().begin(), _ct.model().factor(i).getVars().end());
						vars.insert(_ct.model().factor(f).getVars().begin(), _ct.model().factor(f).getVars().end());
	
						// Create set of factors
						set<int> factors;
						factors.insert(f);
						factors.insert(i);
	
						// Create submodel learning object
						// submodel = new drwnLearnSubmodel<G,C>(
						_submodels.push_back(new drwnLearnSubmodel<G,C>(*(new drwnCliqueTree<G>(_ct.model(), vars)), _copulaSamples, factors, (double*) _dfSingle, iterations(), _submodels.size()));
					}
				}
			}
		}
	
		/*// DEBUG: Print submodels
		unsigned int modelNum = 0;
		for (auto& model : submodels) {
			wcout << "Submodel " << modelNum++ << endl << endl;
			model->print();
			wcout << endl << endl;
		}

		// DEBUG: Flag for breakpoint
		bool gotHere = true;*/
	}
	
	//! Default destructor
	~drwnLearnPiecewise()
	{
		delete [] _dfSingle;
		delete [] _x;

		for (auto& model : _submodels)
			delete model;
	}

	double time() { return _lastTime; }

	void loadTruePars()
	{
		for (unsigned int i = 0; i < _n; i++)
			_truePars[i] = _ct.model().factor(i).getParameter();
		_hasTruePars = true;
	}

	// TODO: Instead of having to load learnt parameters each time, get directly from model
	double mse()
	{
		return (_truePars - _learntPars).squaredNorm() / (double) _truePars.size();
	}

	unsigned int numSubmodels() const { return _submodels.size(); }

	// Have to abstract how to access iteration, because different mechanism for piecewise learning
	unsigned int& iterations() { return _iter; }

	//! Learn model parameters
	// NOTE: You have to initialize parameters yourself, defaults to all zeros
	double learn(const double eps = 1e-6, wchar_t* solnOutput = L"solutions.txt", wchar_t* llOutput = L"loglikelihoods.txt", wchar_t* gradOutput = L"gradients.txt", bool showOutput = false, bool takeLog = false, bool showOuterIter = false, bool shuffleEachTime = false, bool stopOneSubproblemIters = false, unsigned int maxIters = 0, unsigned int fixedIters = 0)
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

		// DEBUG
		if (showOuterIter) {
			wcout << "Outer iteration 0";
			if (_hasTruePars)
				wcout << ", mse " << mse();
			wcout << endl;
		}

		// Output likelihood and parameters before first iteration
		//if (showOutput)
		//	monitor(0, objective(_x));

		// This function was really just a wrapper for the solve from parent class
		//double value = solve(100, eps, showOutput);	// maxIter

		clock_t t = clock();

		unsigned int numIters = _maxIters;
		if (fixedIters)
			numIters = fixedIters;
		else if (maxIters)
			numIters = maxIters;

		// Outer iteration
		for (unsigned int i = 0; i < numIters; i++) {
			unsigned int previousIters = iterations();

			// Shuffle components
			class R {
			public:
				int operator()(int n) {
					boost::random::uniform_int_distribution<> dist(0, n - 1);
					return dist(drwnCumulativeUtils::rng);
				}
			};

			if ((i == 0 && !shuffleEachTime) || shuffleEachTime)
				std::random_shuffle(_submodels.begin(), _submodels.end(), R());
		
			// Learn components
			unsigned int numActive = 0;
			for (auto& model : _submodels) {
				if (showOutput) {
					wcout << "Subproblem ";
					model->printOrder();
					wcout << endl;
				}

				model->learn(eps, showOutput, takeLog);

				if (model->isActive())
					numActive++;

				if (showOutput) {
					wcout << numActive << " active subproblems" << endl;
					wcout << endl;
				}
			}

			// Set the final solution to the model parameters (for calculating mse)
			for (unsigned int j = 0; j < _n; j++)
				_x[j] = _ct.model().factor(j).getParameter();

			if (showOuterIter) {
				wcout << "Outer iteration " << i + 1;
				if (_hasTruePars)
					wcout << ", mse " << mse();
				wcout << ", iterations " << iterations() - previousIters << ", active subproblems " << numActive << "/" << _submodels.size() << endl;				
			}

			// Termination criterion is quit when all submodels have become "active"
			if (!fixedIters && numActive == _submodels.size())
				break;

			// If flag is set, and consecutively doing as many iterations as number of submodels (i.e. 1 per model)
			if (stopOneSubproblemIters && (_lastIterations == iterations() - previousIters) && (_lastIterations <= _submodels.size()))
				break;

			_lastIterations = iterations() - previousIters;
		}

		t = clock() - t;
		_lastTime = (((double)t) / (double)CLOCKS_PER_SEC);

		/*// DEBUG: Print gradients
		wcout << endl;
		for (auto& model : _submodels) {
			model->printGradient();
		}
		wcout << endl;

		// DEBUG: Print submodel gradients
		for (auto& model : _submodels) {
			model->printSubgradient2();
		}
		wcout << endl;*/
		
		// Close files
		if (solnOutput)
			_solutions.close();
		if (llOutput)
			_likelihoods.close();
		if(gradOutput)
			_gradients.close();

		//return value;
		return 0.0;
	}

	//! Prints order of submodels
	// NOTE: This was for debugging shuffling
	void printOrder()
	{
		for (auto& model : _submodels)
			model->printOrder();
	}

	void print()
	{
		for (auto& model : _submodels)
			model->print();
	}
		
	//! Callback for each iteration during optimization (if \b bMonitor is \a true)
    void monitor(unsigned iter, double objValue)
	{
		// TODO: Output to files if open
		wcout << "iteration " << iter << ", objective " << objValue << endl;

		if (_solutions.is_open())
			_ct.model().printParameters(_solutions);
		if (_likelihoods.is_open())
			_likelihoods << objValue << endl;
	}
};