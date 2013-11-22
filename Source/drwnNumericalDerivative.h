/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnNumericalDerivative.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

class drwnNumericalDerivative
{
private:
	//! Starting offset for central difference formula
	double h_;

	//! Maximum number of Richardson extrapolation steps to take
	unsigned int M_;

	//! Enough memory to store the central difference values from M_ extrapolation steps
	double* R_;

	//! Tolerance for stopping criterion
	double eps_;

	//! Function to access an intermediate value in Richardson extrapolation
	// TODO: Better indexing that doesn't require multiplications (this is probably not worth the trouble)
	double& R(unsigned int i, unsigned int j) const
	{
		return R_[j * (M_ + 1) - j * (j + 1) / 2 + i];
	}

public:
	//! Construct with given algorithm metaparameters
	drwnNumericalDerivative(const double& h = 0.01, const double& eps = 1.0e-6, unsigned int M = 10)
		: h_(h), eps_(eps), M_(M)
	{
		DRWN_ASSERT_MSG(M_, "drwnNumericalDerivative: Maximum number of Richardson extrapolation steps must be positive. ")

		// Allocate enough memory for the upper diagonal of an (M + 1) * (M + 1) matrix
		// TODO: More flexible options about where to place memory for this. Shared between classes? (One for each thread.)
		R_ = new double[(M_ + 1) * (M_ + 2) / 2];
	}

	//! Default destructor
	~drwnNumericalDerivative()
	{
		delete [] R_;
	}

	//! Returns value at x of univariate function to be differentiated
	virtual double function(const double& x) const = 0;

	//! Returns the centered difference of function
	double centeredDifference(const double& x, const double& h) const
	{
		return (function(x + h) - function(x - h)) / (2.0 * h);
	}

	//! Calculate numerical derivative of f at x using Richardson extrapolation
	const double& operator()(const double& x)
	{
		// Initialize first centered difference
		double h = h_;
		R_[0] = centeredDifference(x, h);

		// DEBUG
		//wcout << "R(0,0) = " << R(0, 0) << endl;

		// Loop over extrapolation steps
		for (unsigned int n = 1; n <= M_; n++) {
			h *= 2.0;
			R(n, 0) = centeredDifference(x, h);

			// DEBUG
			//wcout << "R(" << n << ",0) = " << R(n, 0) << endl;

			// Loop over terms that need to be calculated for next extrapolation step
			for (unsigned int k = 1; k <= n; k++) {
				R(n - k, k) = (pow(4.0, k) * R(n - k, k - 1) - R(n - k + 1, k - 1)) / (pow(4.0, k) - 1);

				// DEBUG
				//wcout << "R(" << n - k << "," << k << ") = " << R(n - k, k) << endl;
			}

			// Check termination criterion
			if (abs(R(0, n) - R(0, n - 1)) <= eps_)
				return R(0, n);
		}

		// If got to here, then didn't satisfy termination criterion before maximum number of steps
		return R(0, M_);
	}
};