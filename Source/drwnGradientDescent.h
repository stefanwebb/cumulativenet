/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnGradientDescent.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

class drwnGradientDescent
{
public:
	//! Dimension of optimization problem
	unsigned int _n;

	//! Current feasible solution
	double* _x;

	//! Gradient at _x
	double* _df;

	//! Gradient at _x (for other iteration)
	double* _df2;

	//! Rate at which to decrease step size
	double _beta;
			
	//! Maximum number of iterations of backtracking in backtracking line search
	static const unsigned int maxStepIter = 200;

	//! Rate to adjust slope
	double _alpha;

	//! Roughly speaking, rate at which slope must decrease during backtracking line search
	double _gamma;

	bool _isSuccess;

public:
	// Have to abstract how to access iteration, because different mechanism for piecewise learning
	virtual unsigned int& iterations() = 0; // { return _numIter; }

	//! Construct a problem with dimension n 
	drwnGradientDescent(unsigned int n)
		: _beta(0.7), _alpha(1e-3), _gamma(0.9)
	{
		// Memory for parameters and gradients
		_x = new double[n];
		_df = new double[n];
		_df2 = new double[n];
		_n = n;

		// Initialize to zero
		for (unsigned int i = 0; i < n; i++)
			_x[i] = _df[i] = _df2[i] = 0;
	}

	//! Default destructor
	~drwnGradientDescent()
	{
		delete [] _x;
		delete [] _df;
		delete [] _df2;
	}

	//void initialize (unsigned n, const double* x = 0);

	bool isSuccess() { return _isSuccess; }

	//! Solve the optimization problem for up to maxiter iterations to precision tol calls monitor function after each iteration if bMonitor is true 
	double solve(unsigned maxiter, double tol = 1.0e-3, bool bMonitor = false, bool bStepMonitor = false)
	{
		Eigen::Map<VectorXd> g(_df, _n);
		Eigen::Map<VectorXd> g2(_df2, _n);
		VectorXd xold(_n);
		Eigen::Map<VectorXd> x(_x, _n);

		// Calculate initial gradient and log-likelihood
		double logLikelihood = objectiveAndGradient(_x, _df);

		// Gradient descent iterations
		for (unsigned int iter = 0; iter < maxiter; iter++) {
			iterations()++;

			// Call monitor if flag set
			if (bMonitor && iter > 0)
				monitor(iter, logLikelihood);

			// Choose gradient depending on iteration
			double* dfOld = (((iter % 2) == 0) ? _df: _df2);
			double* dfNew = (((iter % 2) == 1) ? _df: _df2);

			// Calculate squared norm of old gradient
			double squaredNormGradient = 0;
			for	(unsigned int i = 0; i < _n; i++)
				squaredNormGradient += dfOld[i] * dfOld[i];

			// Backtracking line search
			for (unsigned int stepIter = 0; stepIter < maxStepIter; stepIter++) {
				// Work out relative step size, given that previous steps in this direction have been taken
				double step = (stepIter ? ((_beta - 1) * pow(_beta, stepIter - 1)) : 1.0);

				// Record previous values of parameters
				xold = x;

				// Take a step in gradient direction
				for (unsigned int i = 0; i < _n; i++)
					_x[i] += step * -dfOld[i];

				// Calculate new log-likelihood and gradient
				double newLogLikelihood = objectiveAndGradient(_x, dfNew);

				if (newLogLikelihood == std::numeric_limits<double>::max())
					continue;

				// DEBUG: Print iteration info
				if (bStepMonitor)
					wcout << "backtracking line search iteration " << stepIter << ", log-likelihood " << newLogLikelihood << endl;

				// Calculate step size
				double stepSize;
				if (stepIter == 0)
					stepSize = 1.0;
				else if (stepIter)
					stepSize = 1.0 * pow(_beta, stepIter);

				// See if likelihood increased sufficiently
				// Calculate dot product of new and old gradients
				double lineIncrease = - stepSize * _alpha * squaredNormGradient, dotProductGradients = 0;
				for	(unsigned int i = 0; i < _n; i++)
					dotProductGradients += dfOld[i] * dfNew[i];
				
				// DEBUG
				//wcout << "line increase " << lineIncrease << endl;
				/*if (iter == 2 && (abs(dotProductGradients) <= abs(squaredNormGradient))) {
					bool gotHere = true;
				}*/

				// Break if decrease sufficient in log-likelihood and gradient has sufficiently decreased
				// NOTE: Second condition is "strong Wolfe condition on curvature." See Wikipedia article on Wolfe conditions.
				if ((newLogLikelihood <= logLikelihood + lineIncrease)) { // && (abs(dotProductGradients) <= _gamma * abs(squaredNormGradient))
					// DEBUG: Print number of steps taken
					if (bStepMonitor)
						wcout << stepIter + 1 << " steps taken during backtracking line search" << endl;

					// NOTE: Put these in three different if's, because I may want to return convergence type in future
					// Determine whether to terminate algorithm based on relative change in likelihood
					if ((logLikelihood - newLogLikelihood ) / abs(logLikelihood) <= tol)	{
						// DEBUG
						//wcout << "Converged due to relative change in LL < eps" << endl;

						/*if ((iter % 2) == 1)
							wcout << "gradient norm " << g.norm() << endl;
						else if ((iter % 2) == 0)
							wcout << "gradient norm " << g2.norm() << endl;*/

						// Call monitor if flag set
						if (bMonitor)
							monitor(iter + 1, newLogLikelihood);

						_isSuccess = true;

						return newLogLikelihood;
					}

					// Test whether step size is small enough
					else if ((x - xold).norm() <= tol) {
						// DEBUG
						//wcout << "Converged due to step size < eps" << endl;

						// Call monitor if flag set
						if (bMonitor)
							monitor(iter + 1, newLogLikelihood);

						_isSuccess = true;

						return newLogLikelihood;
					}

					// Test whether gradient is small enough
					if ((iter % 2) == 1) {
						if (g.norm() <= tol) {
							// DEBUG
							//wcout << "Converged due to gradient < eps" << endl;

							// Call monitor if flag set
							if (bMonitor)
								monitor(iter + 1, newLogLikelihood);

							_isSuccess = true;

							return newLogLikelihood;
						}
					}

					else if ((iter % 2) == 0) {
						if (g2.norm() <= tol) {
							// DEBUG
							//wcout << "Converged due to gradient < eps" << endl;

							// Call monitor if flag set
							if (bMonitor)
								monitor(iter + 1, newLogLikelihood);

							_isSuccess = true;

							return newLogLikelihood;
						}
					}
										

					logLikelihood = newLogLikelihood;

					// We should be taking smaller steps each time, hence the following
					//if (stepIter > 0)
					//	startRate = 0.5 * pow(beta, stepIter);

					break;
				}
			
				if (stepIter >= maxStepIter - 1) {
					//DRWN_LOG_MESSAGE("Maximum number of (backtracking line search) iterations reached.");
					_isSuccess = false;
					return logLikelihood;
				}
			}
		}

		//DRWN_LOG_MESSAGE("Maximum number of iterations reached.");
		_isSuccess = false;
		return logLikelihood;
	}

	//! Returns value of objective function at point x 
	virtual double objective(const double *x) const = 0;

	//! Populates gradient of objective function at point x 
	virtual void gradient(const double *x, double *df) const = 0;

	//! Returns value of objective function and populates gradient df at point x 
	// NOTE: You want to override this when it is efficient to calculate both at same time
	virtual double objectiveAndGradient(const double *x, double *df) const
	{
		gradient(x, df);
		return objective(x);
	}

	//! Dimension of optimization problem
    inline unsigned size() const { return _n; }

    //! Returns the \a i-th component of the current solution
    inline double operator[](unsigned i) const { return _x[i]; }

    //! Returns a reference to the \a i-th component of the current solution
    inline double& operator[](unsigned i) { return _x[i]; }

    //! Callback for each iteration during optimization (if \b bMonitor is \a true)
    virtual void monitor(unsigned iter, double objValue)
	{
		wcout << "iteration " << iter << ", objective " << objValue << endl;
	}
};