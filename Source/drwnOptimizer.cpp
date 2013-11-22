/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Distributed under the terms of the BSD license (see the LICENSE file)
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnOptimizer.cpp
** AUTHOR(S):   Stephen Gould <stephen.gould@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

#undef min
#undef max

// C++ standard library
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <string.h>
#include <assert.h>
#include <limits>
#include <iomanip>

// Eigen library
#include <Eigen/Core>

// Darwin headers
//#include "../base/drwnCompatibility.h"
//#include "../base/drwnLogger.h"
#include "drwnOptimizer.h"

using namespace std;
using namespace Eigen;

// Constructors/Destructors -------------------------------------------------

drwnOptimizer2::drwnOptimizer2() :
    _n(0), _x(NULL), _df(NULL)
{
    // do nothing
}

drwnOptimizer2::drwnOptimizer2(unsigned n) :
    _n(n)
{
    _x = new double[_n];
    memset(_x, 0, _n * sizeof(double));
    _df = new double[_n];
}

drwnOptimizer2::drwnOptimizer2(const drwnOptimizer2& o) :
    _n(o._n)
{
    if (_n == 0) {
	_x = NULL;
	_df = NULL;
	return;
    }

    _x = new double[_n];
    memcpy(_x, o._x, _n * sizeof(double));
    _df = new double[_n];
    memcpy(_df, o._df, _n * sizeof(double));
}

drwnOptimizer2::~drwnOptimizer2()
{
    if (_x != NULL) {
	delete[] _x;
	delete[] _df;
    }
}

// Public Member Functions ---------------------------------------------------

void drwnOptimizer2::initialize(unsigned n, const double *x)
{
    DRWN_ASSERT(n != 0);
    if (_x != NULL) {
	delete[] _x;
	delete[] _df;
    }

    _x = new double[_n = n];
    _df = new double[_n];

    initialize(x);
}

void drwnOptimizer2::initialize(const double *x)
{
    DRWN_ASSERT(_n != 0);
    if (x == NULL) {
	memset(_x, 0, _n * sizeof(double));
    } else {
	memcpy(_x, x, _n * sizeof(double));
    }
}

double drwnOptimizer2::solve(unsigned maxiter, double tol, bool bMonitor)
{
    DRWN_ASSERT(_x != NULL);

    // constants
    const int m = (_n < 7 ? _n : 7);
    //const double eps = 1.0e-6;
	
	// DEBUG
	/*_debug.open(L"debugLearning.txt");
	_debug.precision(std::numeric_limits<double>::digits10);
	_search.open(L"debugSearch.txt");
	_search.precision(std::numeric_limits<double>::digits10);
	for (unsigned int i = 0; i < _n; i++)
		_debug << "rho" << i + 1 << " ";
	for (unsigned int i = 0; i < _n; i++)
		_debug << "dRho" << i + 1 << " ";
	_debug << "obj where" << endl;
	for (unsigned int i = 0; i < _n; i++)
		_search << "rho" << i + 1 << " ";
	_search << endl;

	_wOutput.open(L"wBeforeLineSearch.txt");
	_wOutput2.open(L"wEndFunction.txt");*/
	
    drwnLBFGSResult info = lbfgsMinimize(m, maxiter, tol, tol, tol, bMonitor);
    switch (info) {
    case DRWN_LBFGS_CONVERGED_F:
		if (bMonitor)
			DRWN_LOG_MESSAGE("Optimization converged (relative function decrease <= tol).");
		_isSuccess = true;
        break;
    case DRWN_LBFGS_CONVERGED_G:
		if (bMonitor)
			DRWN_LOG_MESSAGE("Optimization converged (step size <= eps)");
		_isSuccess = true;
        break;
    case DRWN_LBFGS_CONVERGED_X:
		if (bMonitor)
			DRWN_LOG_MESSAGE("Optimization converged (gradient norm <= eps)");
		_isSuccess = true;
        break;
    case DRWN_LBFGS_MAX_ITERS:
		if (bMonitor)
			DRWN_LOG_MESSAGE("Maximum number of iterations reached.");
		_isSuccess = false;
        break;
    default:
	if (bMonitor)
		DRWN_LOG_ERROR("Parameters were: _n = " << _n << ", m = " << m
            << ", bMonitor = " << (bMonitor ? 1 : 0)
            << ", tol = " << tol << ", eps = "  << tol
            << ", maxiter = " << maxiter << ", info = " << info);
		_isSuccess = false;
    }

	/*_debug.close();
	_search.close();
	_wOutput.close();
	_wOutput2.close();*/

    return objective(_x);
}

void drwnOptimizer2::monitor(unsigned iter, double objValue)
{
    char buffer[32];
    sprintf_s(buffer, 32, "%5d %10.5f", iter, objValue);
    DRWN_LOG_MESSAGE(buffer);
    cout.flush();
}

// Protected Member Functions ------------------------------------------------

drwnOptimizer2::drwnLBFGSResult drwnOptimizer2::lbfgsMinimize(int m, unsigned maxiter,
    double epsg, double epsf, double epsx, bool bMonitor)
{
    DRWN_ASSERT((m > 0) && (m <= (int)_n));
    DRWN_ASSERT((epsg >= 0.0) && (epsf >= 0.0) && (epsx >= 0.0));

    Eigen::Map<VectorXd> x(_x, _n);

    VectorXd w(_n*(2*m+1) + 2*m);
    Eigen::Map<VectorXd> g(_df, _n);
    VectorXd xold(_n);
    VectorXd diag(VectorXd::Ones(_n));

    double f = objectiveAndGradient(_x, _df);

	// DEBUG
	/*wcout << "In lbfgsMinimize" << endl;
	wcout << "objective = " << f << endl;
	wcout << "parameters" << endl;
	for (unsigned int i = 0; i < _n; i++)
		wcout << _x[i] << " ";
	wcout << endl;
	wcout << "gradient" << endl;
	for (unsigned int i = 0; i < _n; i++)
		wcout << _df[i] << " ";
	wcout << endl << endl;*/

	/*for (unsigned int i = 0; i < _n; i++)
		_debug << _x[i] << " ";
	for (unsigned int i = 0; i < _n; i++)
		_debug << _df[i] << " ";
	_debug << f << " lbfgsMinimize" << endl;*/

	double fold;

    int point = 0;
    int ispt = _n + 2*m;
    int iypt = ispt + _n*m;
    int npt = 0;

    w.setZero();
    w.segment(ispt, _n) = -g;
    double stp1 = 1.0 / g.norm();

    for (int iter = 0; iter < (int)maxiter; iter++) {
		iterations()++;

        xold = x;
        fold = f;

        int bound = std::min(iter, m);
        if (iter != 0) {
            double ys = w.segment(iypt + npt, _n).dot(w.segment(ispt + npt, _n));
            double yy = w.segment(iypt + npt, _n).squaredNorm();
            diag.setConstant(ys / yy);
            w[_n + ((point == 0) ? m - 1 : point - 1)] = 1.0 / ys;
            w.head(_n) = -g;

            int cp = point;
            for (int i = 0; i < bound; i++) {
                cp -= 1;
                if (cp == -1) {
                    cp = m - 1;
                }
                double sq = w.segment(ispt + cp * _n, _n).dot(w.head(_n));
                int inmc = _n + m + cp;
                int iycn = iypt + cp * _n;
                w[inmc] = sq * w[_n + cp];

                w.head(_n) -= w[inmc] * w.segment(iycn, _n);
            }

            w.head(_n) *= diag;		// *** ABORTS ON THIS STATEMENT!

            for (int i = 0; i < bound; i++) {
                double yr = w.segment(iypt + cp * _n, _n).dot(w.head(_n));
                int inmc = _n + m + cp;
                int iscn = ispt + cp * _n;
                double beta = w[inmc] - w[_n + cp] * yr;

                w.head(_n) += beta * w.segment(iscn, _n);
                cp += 1;
                if (cp == m) {
                    cp = 0;
                }
            }

            w.segment(ispt + point * _n, _n) = w.head(_n);
        }
        
        double stp = (iter == 0) ? stp1 : 1.0;
        w.head(_n) = g;

		//wcout << "w" << endl;
		//cout << w << endl;

		/*for (unsigned int i = 0; i < w.size(); i++)
			_wOutput << w[i] << L" ";
		_wOutput << endl;*/

        bool success = lbfgsSearch(f, w.segment(ispt + point * _n, _n), stp, diag);
        if (!success) {
            return DRWN_LBFGS_ERROR;
        }

        npt = point * _n;
        w.segment(ispt + npt, _n) *= stp;
        w.segment(iypt + npt, _n) = g - w.head(_n);

		/*for (unsigned int i = 0; i < w.size(); i++)
			_wOutput2 << w[i] << L" ";
		_wOutput2 << endl;*/

        point += 1;
        if (point == m) {
            point = 0;
        }

        if (bMonitor) {
            this->monitor(iter + 1, f);
        }

        if (g.norm() <= epsg) {
            return DRWN_LBFGS_CONVERGED_G;
        }

        double tf = std::max(fabs(fold), std::max(fabs(f), 1.0));
        if (fold - f <= epsf * tf) {
            return DRWN_LBFGS_CONVERGED_F;
        }
        if ((x - xold).norm() <= epsx) {
            return DRWN_LBFGS_CONVERGED_X;
        }
    }

    return DRWN_LBFGS_MAX_ITERS;
}

bool drwnOptimizer2::lbfgsSearch(double &f, const VectorXd &s, double& stp, VectorXd& diag)
{
    static const int MAXFEV = 20;
    static const double STPMIN = pow(10.0, -20.0);
    static const double STPMAX = pow(10.0, 20.0);
    static const double XTOL = 100.0 * numeric_limits<double>::min();
    static const double GTOL = 0.9;
    static const double FTOL = 0.0001;
    static const double XTRAPF = 4.0;

	/*// DEBUG: Output search direction to file
	for (unsigned int i = 0; i < _n; i++)
		_search << s[i] << " ";
	_search << endl;*/
	
	// NOTE: Is this evaluation necessary, since it was calculated previously?
    f = objectiveAndGradient(_x, _df);
	
	// DEBUG
	/*wcout << "In lbfgsSearch" << endl;
	wcout << "objective = " << f << endl;
	wcout << "parameters" << endl;
	for (unsigned int i = 0; i < _n; i++)
		wcout << _x[i] << " ";
	wcout << endl;
	wcout << "gradient" << endl;
	for (unsigned int i = 0; i < _n; i++)
		wcout << _df[i] << " ";
	wcout << endl << endl;*/

	/*for (unsigned int i = 0; i < _n; i++)
		_debug << _x[i] << " ";
	for (unsigned int i = 0; i < _n; i++)
		_debug << _df[i] << " ";
	_debug << f << " lbfgsSearch" << endl;*/

    if (stp <= 0) {
        return false;
    }

    Eigen::Map<VectorXd> x(_x, _n);
    Eigen::Map<VectorXd> g(_df, _n);

    double dginit = g.dot(s);
    if (dginit >= 0.0) {
        return false;
    }

    bool brackt = false;
    bool stage1 = true;
    double finit = f;
    double dgtest = FTOL * dginit;
    double width = STPMAX - STPMIN;
    double width1 = 2.0 * width;

    diag = x;

    double stx = 0.0;
    double fx = finit;
    double dgx = dginit;
    double sty = 0.0;
    double fy = finit;
    double dgy = dginit;
    bool infoc = true;


    double fm, fxm, fym;
    double dgm, dgxm, dgym;
    double stmin, stmax;

    for (int nfev = 0; nfev < MAXFEV; nfev++) {
        if (brackt) {
            if (stx < sty) {
                stmin = stx;
                stmax = sty;
            } else {
                stmin = sty;
                stmax = stx;
            }
        } else {
            stmin = stx;
            stmax = stp + XTRAPF * (stp - stx);
        }

        if (stp > STPMAX) {
            stp = STPMAX;
        }
        if (stp < STPMIN) {
            stp = STPMIN;
        }
        if ((brackt && ((stp <= stmin) || (stp >= stmax))) || (nfev == MAXFEV - 1) ||
            (!infoc) || (brackt && ((stmax - stmin) <= XTOL * stmax))) {
            stp = stx;
        }

		x = diag + stp * s;
        f = objectiveAndGradient(_x, _df);

		// DEBUG
		// While objective is Inf, scale by 0.9
		// NOTE: If this works, then replace the previous two lines and make this a do { ... } while (...)
		/*while (f == std::numeric_limits<double>::infinity()) {
			stp *= 0.9;
			x = diag + stp * s;
			f = objectiveAndGradient(_x, _df);
		}*/

		// DEBUG
		/*wcout << "objective = " << f << endl;
		wcout << "parameters" << endl;
		for (unsigned int i = 0; i < _n; i++)
			wcout << _x[i] << " ";
		wcout << endl;
		wcout << "gradient" << endl;
		for (unsigned int i = 0; i < _n; i++)
			wcout << _df[i] << " ";
		wcout << endl << endl;*/

		/*for (unsigned int i = 0; i < _n; i++)
			_debug << _x[i] << " ";
		for (unsigned int i = 0; i < _n; i++)
			_debug << _df[i] << " ";
		_debug << f << " lbfgsSearch" << endl;*/

        double dg = g.dot(s);
        double ftest1 = finit + stp * dgtest;

        if ((brackt && ((stp <= stmin) || (stp >= stmax))) || (!infoc)) {
            return true;
        }
        if ((stp == STPMAX) && (f <= ftest1) && (dg <= dgtest)) {
            return true;
        }
        if ((stp == STPMIN) && ((f >= ftest1) || (dg >= dgtest))) {
            return true;
        }
        if (brackt && (stmax - stmin <= XTOL * stmax)) {
            return true;
        }
        if ((f <= ftest1) && (fabs(dg) <= -GTOL * dginit)) {
            return true;
        }

        stage1 = stage1 && ((f > ftest1) || (dg < std::min(FTOL, GTOL) * dginit));

        if (stage1) {
            fm = f - stp * dgtest;
            fxm = fx - stx * dgtest;
            fym = fy - sty * dgtest;
            dgm = dg - dgtest;
            dgxm = dgx - dgtest;
            dgym = dgy - dgtest;
            infoc = lbfgsStep(stx, fxm, dgxm, sty, fym, dgym, stp, fm, dgm, brackt, stmin, stmax);
            fx = fxm + stx * dgtest;
            fy = fym + sty * dgtest;
            dgx = dgxm + dgtest;
            dgy = dgym + dgtest;
        } else {
            infoc = lbfgsStep(stx, fx, dgx, sty, fy, dgy, stp, f, dg, brackt, stmin, stmax);
        }

        if (brackt) {
            if (fabs(sty - stx) >= 0.66 * width1) {
                stp = stx + 0.5 * (sty - stx);
            }
            width1 = width;
            width = fabs(sty - stx);
        }
    }

    return true;
}

bool drwnOptimizer2::lbfgsStep(double& stx, double& fx, double& dx, 
    double& sty, double& fy, double& dy,  
    double& stp, const double& fp, const double& dp,
    bool& brackt, const double& stmin, const double& stmax)
{
    bool bound;
    double gamma;
    double p;
    double q;
    double r;
    double s;
    double stpc;
    double stpf;
    double stpq;
    double theta;

    if ((brackt && ((stp <= std::min(stx, sty)) || (stp >= std::max(stx, sty)))) ||
        (dx * (stp - stx) >= 0) || (stmax < stmin)) {
        return false;
    }

    double sgnd = dp*(dx/fabs(dx));
    if (fp > fx) {
        bound = true;
        theta = 3.0 * (fx - fp) / (stp - stx) + dx + dp;
        s = std::max(fabs(theta), std::max(fabs(dx), fabs(dp)));
        gamma = s * sqrt((theta / s) * (theta / s) - (dx / s) * (dp / s));
        if (stp < stx) {
            gamma = -gamma;
        }
        p = gamma - dx + theta;
        q = gamma - dx + gamma + dp;
        r = p / q;
        stpc = stx + r * (stp - stx);
        stpq = stx + dx/((fx - fp)/(stp - stx) + dx)/2 * (stp - stx);
        if (fabs(stpc - stx) < fabs(stpq - stx)) {
            stpf = stpc;
        } else {
            stpf = stpc + (stpq - stpc)/2;
        }
        brackt = true;

    } else {

        if (sgnd < 0.0) {
            bound = false;
            theta = 3.0 * (fx - fp) / (stp - stx) + dx + dp;
            s = std::max(fabs(theta), std::max(fabs(dx), fabs(dp)));
            gamma = s * sqrt((theta / s) * (theta / s) - (dx / s) * (dp / s));
            if (stp > stx) {
                gamma = -gamma;
            }
            p = gamma - dp + theta;
            q = gamma - dp + gamma + dx;
            r = p / q;
            stpc = stp + r * (stx - stp);
            stpq = stp + dp / (dp - dx) * (stx - stp);
            stpf = (fabs(stpc - stp) > fabs(stpq - stp)) ? stpc : stpq;
            brackt = true;

        } else {

            if (fabs(dp) < fabs(dx)) {
                bound = true;
                theta = 3.0 * (fx - fp) / (stp - stx) + dx + dp;
                s = std::max(fabs(theta), std::max(fabs(dx), fabs(dp)));
                gamma = s * sqrt(std::max(0.0, (theta/s)*(theta/s) - (dx/s)*(dp/s)));
                if (stp > stx) {
                    gamma = -gamma;
                }
                p = gamma - dp + theta;
                q = gamma + (dx - dp) + gamma;
                r = p / q;
                if ((r < 0.0) && (gamma != 0.0)) {
                    stpc = stp + r * (stx - stp);
                } else {
                    stpc = (stp > stx) ? stmax : stmin;
                }

                stpq = stp + dp / (dp - dx) * (stx - stp);
                if (brackt) {
                    stpf = (fabs(stp - stpc) < fabs(stp - stpq)) ? stpc : stpq;
                } else {
                    stpf = (fabs(stp - stpc) > fabs(stp - stpq)) ? stpc : stpq;
                }

            } else {
                bound = false;
                if (brackt) {
                    theta = 3.0 * (fp - fy) / (sty - stp) + dy + dp;
                    s = std::max(fabs(theta), std::max(fabs(dy), fabs(dp)));
                    gamma = s * sqrt((theta/s)*(theta/s) - (dy/s)*(dp/s));
                    if (stp > sty) {
                        gamma = -gamma;
                    }
                    p = gamma - dp + theta;
                    q = gamma - dp + gamma + dy;
                    r = p / q;
                    stpc = stp + r * (sty - stp);
                    stpf = stpc;
                } else {
                    stpf = (stp > stx) ? stmax : stmin;
                }
            }
        }
    }

    if (fp > fx) {
        sty = stp;
        fy = fp;
        dy = dp;
    } else {
        if (sgnd < 0.0) {
            sty = stx;
            fy = fx;
            dy = dx;
        }
        stx = stp;
        fx = fp;
        dx = dp;
    }

    stp = std::max(stmin, std::min(stmax, stpf));
    if (brackt && bound) {
        if (sty > stx) {
            stp = std::min(stx + 0.66*(sty - stx), stp);
        } else {
            stp = std::max(stx + 0.66*(sty - stx), stp);
        }
    }

    return true;
}