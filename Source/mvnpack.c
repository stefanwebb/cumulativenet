/* mvnpack.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Table of constant values */

static doublereal c_b3 = 5e-5;
static integer c__1 = 1;
static integer c__5 = 5;
static integer c_b9 = 625000;
static real c_b17 = 1.f;
static doublereal c_b47 = 0.;
static integer c__0 = 0;
static integer c_b96 = 100000;
static doublereal c_b103 = 1.;
static integer c__2 = 2;
static integer c__4 = 4;


/* This file contains a short test program, software for MVN distribution */
/* subroutines SADMVN, RANMVN, KROMVN and SPHMVN, plus supporting software. */
/* The file is self contained and should compile without errors on (77) */
/* standard Fortran compilers. The test program demonstrates the use of */
/* four different methods for computing MVN distribution values for a */
/* five dimensional example problem, with three different integration limit */
/* combinations. */

/*          Alan Genz */
/*          Department of Mathematics */
/*          Washington State University */
/*          Pullman, WA 99164-3113 */
/*          Email : alangenz@wsu.edu */

/* Copyright (C) 2010, Alan Genz,  All rights reserved. */

/* Redistribution and use in source and binary forms, with or without */
/* modification, are permitted provided the following conditions are met: */
/*   1. Redistributions of source code must retain the above copyright */
/*      notice, this list of conditions and the following disclaimer. */
/*   2. Redistributions in binary form must reproduce the above copyright */
/*      notice, this list of conditions and the following disclaimer in */
/*      the documentation and/or other materials provided with the */
/*      distribution. */
/*   3. The contributor name(s) may not be used to endorse or promote */
/*      products derived from this software without specific prior */
/*      written permission. */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE */
/* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, */
/* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS */
/* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR */
/* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

/* Main program */ int MAIN__(void)
{
    /* Initialized data */

    static doublereal up[5] = { 0.,1.5198,1.7817,1.4755,1.5949 };
    static doublereal low[5] = { 0.,0.,1.7817,1.4755,1.5949 };
    static integer infin[5] = { 1,2,1,1,0 };
    static doublereal correl[10] = { -.707107,0.,.5,0.,.5,.5,0.,.5,.5,.5 };

    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe(void), do_fio(integer *, char *, ftnlen);

    /* Local variables */
    static integer i__, j, k, ij;
    static doublereal vald;
    static integer iftd;
    static doublereal errd, valk;
    static integer iftk;
    static doublereal errk, valr, vals;
    static integer iftr, ifts;
    static doublereal errr, errs;
    extern doublereal condit_(integer *, doublereal *);
    extern /* Subroutine */ int sadmvn_(integer *, doublereal *, doublereal *,
	     integer *, doublereal *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *), ranmvn_(integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *)
	    , sphmvn_(integer *, doublereal *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *), kromvn_(integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *);

    /* Fortran I/O blocks */
    static cilist io___5 = { 0, 6, 0, "('        Test of RANMVN, SADMVN, KRO"
	    "MVN and SPHMVN')", 0 };
    static cilist io___6 = { 0, 6, 0, "(12X, 'Requested Accuracy ',F8.5)", 0 }
	    ;
    static cilist io___7 = { 0, 6, 0, "('           Number of Dimensions is "
	    "',I2)", 0 };
    static cilist io___8 = { 0, 6, 0, "('     Maximum # of Function Values i"
	    "s ',I7)", 0 };
    static cilist io___10 = { 0, 6, 0, "(/' I     Limits')", 0 };
    static cilist io___11 = { 0, 6, 0, "(4X,'Lower  Upper  Lower Left of Cor"
	    "relation Matrix')", 0 };
    static cilist io___14 = { 0, 6, 0, "(I2, ' -infin  infin ', 7F9.5)", 0 };
    static cilist io___16 = { 0, 6, 0, "(I2, ' -infin', F7.4, 1X, 7F9.5)", 0 }
	    ;
    static cilist io___17 = { 0, 6, 0, "(I2, F7.4, '  infin ', 7F9.5)", 0 };
    static cilist io___18 = { 0, 6, 0, "(I2, 2F7.4, 1X, 7F9.5)", 0 };
    static cilist io___19 = { 0, 6, 0, "(20X, 'Condition Number is', F8.1)", 
	    0 };
    static cilist io___32 = { 0, 6, 0, "(' Results for:  RANMVN',9X,'SADMVN'"
	    ",9X,'KROMVN',                       9X, 'SPHMVN')", 0 };
    static cilist io___33 = { 0, 6, 0, "(' Values:   ',4(F11.6,I4))", 0 };
    static cilist io___34 = { 0, 6, 0, "('Errests:   ',4(2X,'(',F8.6,')',3X))"
	    , 0 };



/*     Test program for SADMVN, RANMVN, KROMVN, SPHMVN */

/*          Chen Problem */
    s_wsfe(&io___5);
    e_wsfe();
    s_wsfe(&io___6);
    do_fio(&c__1, (char *)&c_b3, (ftnlen)sizeof(doublereal));
    e_wsfe();
    s_wsfe(&io___7);
    do_fio(&c__1, (char *)&c__5, (ftnlen)sizeof(integer));
    e_wsfe();
    s_wsfe(&io___8);
    do_fio(&c__1, (char *)&c_b9, (ftnlen)sizeof(integer));
    e_wsfe();

    for (k = 1; k <= 3; ++k) {
	s_wsfe(&io___10);
	e_wsfe();
	s_wsfe(&io___11);
	e_wsfe();
	ij = 0;
	for (i__ = 1; i__ <= 5; ++i__) {
	    if (infin[i__ - 1] < 0) {
		s_wsfe(&io___14);
		do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
		i__1 = i__ - 1;
		for (j = 1; j <= i__1; ++j) {
		    do_fio(&c__1, (char *)&correl[ij + j - 1], (ftnlen)sizeof(
			    doublereal));
		}
		do_fio(&c__1, (char *)&c_b17, (ftnlen)sizeof(real));
		e_wsfe();
	    } else if (infin[i__ - 1] == 0) {
		s_wsfe(&io___16);
		do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&up[i__ - 1], (ftnlen)sizeof(doublereal)
			);
		i__1 = i__ - 1;
		for (j = 1; j <= i__1; ++j) {
		    do_fio(&c__1, (char *)&correl[ij + j - 1], (ftnlen)sizeof(
			    doublereal));
		}
		do_fio(&c__1, (char *)&c_b17, (ftnlen)sizeof(real));
		e_wsfe();
	    } else if (infin[i__ - 1] == 1) {
		s_wsfe(&io___17);
		do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&low[i__ - 1], (ftnlen)sizeof(
			doublereal));
		i__1 = i__ - 1;
		for (j = 1; j <= i__1; ++j) {
		    do_fio(&c__1, (char *)&correl[ij + j - 1], (ftnlen)sizeof(
			    doublereal));
		}
		do_fio(&c__1, (char *)&c_b17, (ftnlen)sizeof(real));
		e_wsfe();
	    } else {
		s_wsfe(&io___18);
		do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&low[i__ - 1], (ftnlen)sizeof(
			doublereal));
		do_fio(&c__1, (char *)&up[i__ - 1], (ftnlen)sizeof(doublereal)
			);
		i__1 = i__ - 1;
		for (j = 1; j <= i__1; ++j) {
		    do_fio(&c__1, (char *)&correl[ij + j - 1], (ftnlen)sizeof(
			    doublereal));
		}
		do_fio(&c__1, (char *)&c_b17, (ftnlen)sizeof(real));
		e_wsfe();
	    }
	    ij = ij + i__ - 1;
	}
	s_wsfe(&io___19);
	d__1 = condit_(&c__5, correl);
	do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	e_wsfe();
	sadmvn_(&c__5, low, up, infin, correl, &c_b9, &c_b3, &c_b47, &errs, &
		vals, &ifts);
	kromvn_(&c__5, low, up, infin, correl, &c_b9, &c_b3, &c_b47, &errk, &
		valk, &iftk);
	ranmvn_(&c__5, low, up, infin, correl, &c_b9, &c_b3, &c_b47, &errr, &
		valr, &iftr);
	sphmvn_(&c__5, low, up, infin, correl, &c_b9, &c_b3, &c_b47, &errd, &
		vald, &iftd);
	s_wsfe(&io___32);
	e_wsfe();
	s_wsfe(&io___33);
	do_fio(&c__1, (char *)&valr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&iftr, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&vals, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ifts, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&valk, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&iftk, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&vald, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&iftd, (ftnlen)sizeof(integer));
	e_wsfe();
	s_wsfe(&io___34);
	do_fio(&c__1, (char *)&errr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&errs, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&errk, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&errd, (ftnlen)sizeof(doublereal));
	e_wsfe();
	--infin[0];
    }
    return 0;
} /* MAIN__ */

/* Subroutine */ int ranmvn_(integer *n, doublereal *lower, doublereal *upper,
	 integer *infin, doublereal *correl, integer *maxpts, doublereal *
	abseps, doublereal *releps, doublereal *error, doublereal *value, 
	integer *inform__)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal d__, e, eps;
    static integer mpt, ivls, infis;
    extern /* Subroutine */ int rcrude_(integer *, integer *, D_fp, 
	    doublereal *, doublereal *, integer *);
    extern doublereal mvnfnc_(), mvnnit_(integer *, doublereal *, doublereal *
	    , doublereal *, integer *, integer *, doublereal *, doublereal *);


/*     A subroutine for computing multivariate normal probabilities. */
/*     This subroutine uses the Monte-Carlo algorithm given in the paper */
/*     "Numerical Computation of Multivariate Normal Probabilities", in */
/*     J. of Computational and Graphical Stat., 1(1992), pp. 141-149, by */
/*          Alan Genz */
/*          Department of Mathematics */
/*          Washington State University */
/*          Pullman, WA 99164-3113 */
/*          Email : alangenz@wsu.edu */

/*  Parameters */

/*     N      INTEGER, the number of variables. */
/*     LOWER  REAL, array of lower integration limits. */
/*     UPPER  REAL, array of upper integration limits. */
/*     INFIN  INTEGER, array of integration limits flags: */
/*            if INFIN(I) < 0, Ith limits are (-infinity, infinity); */
/*            if INFIN(I) = 0, Ith limits are (-infinity, UPPER(I)]; */
/*            if INFIN(I) = 1, Ith limits are [LOWER(I), infinity); */
/*            if INFIN(I) = 2, Ith limits are [LOWER(I), UPPER(I)]. */
/*     CORREL REAL, array of correlation coefficients; the correlation */
/*            coefficient in row I column J of the correlation matrix */
/*            should be stored in CORREL( J + ((I-2)*(I-1))/2 ), for J < I. */
/*     MAXPTS INTEGER, maximum number of function values allowed. This */
/*            parameter can be used to limit the time taken. A */
/*            sensible strategy is to start with MAXPTS = 1000*N, and then */
/*            increase MAXPTS if ERROR is too large. */
/*     ABSEPS REAL absolute error tolerance. */
/*     RELEPS REAL relative error tolerance. */
/*     ERROR  REAL estimated absolute error, with 99% confidence level. */
/*     VALUE  REAL estimated value for the integral */
/*     INFORM INTEGER, termination status parameter: */
/*            if INFORM = 0, normal completion with ERROR < EPS; */
/*            if INFORM = 1, completion with ERROR > EPS and MAXPTS */
/*                           function vaules used; increase MAXPTS to */
/*                           decrease ERROR; */
/*            if INFORM = 2, N > 100 or N < 1. */

    /* Parameter adjustments */
    --correl;
    --infin;
    --upper;
    --lower;

    /* Function Body */
    if (*n > 100 || *n < 1) {
	*inform__ = 2;
	*value = 0.;
	*error = 1.;
	return 0;
    }
    *inform__ = (integer) mvnnit_(n, &correl[1], &lower[1], &upper[1], &infin[
	    1], &infis, &d__, &e);
    if (*n - infis == 0) {
	*value = 1.;
	*error = 0.;
    } else if (*n - infis == 1) {
	*value = e - d__;
	*error = 2e-16f;
    } else {

/*        Call then Monte-Carlo integration subroutine */

	mpt = *n * 10 + 25;
	i__1 = *n - infis - 1;
	rcrude_(&i__1, &mpt, (D_fp)mvnfnc_, error, value, &c__0);
	ivls = mpt;
L10:
/* Computing MAX */
	d__1 = *abseps, d__2 = *releps * abs(*value);
	eps = max(d__1,d__2);
	if (*error > eps && ivls < *maxpts) {
/* Computing MAX */
/* Computing MIN */
/* Computing 2nd power */
	    d__1 = *error / eps;
	    i__2 = (integer) (mpt * (d__1 * d__1)), i__3 = *maxpts - ivls;
	    i__1 = min(i__2,i__3);
	    mpt = max(i__1,10);
	    i__1 = *n - infis - 1;
	    rcrude_(&i__1, &mpt, (D_fp)mvnfnc_, error, value, &c__1);
	    ivls += mpt;
	    goto L10;
	}
	if (*error > eps && ivls >= *maxpts) {
	    *inform__ = 1;
	}
    }
    return 0;
} /* ranmvn_ */

/* Subroutine */ int rcrude_(integer *ndim, integer *maxpts, D_fp functn, 
	doublereal *absest, doublereal *finest, integer *ir)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer k, m;
    static doublereal x[100], fun;
    extern doublereal uni_(void);
    static integer npts;
    static doublereal findif, finval, varprd, varest, varsqr;


/*     Crude Monte-Carlo Algorithm with simple antithetic variates */
/*      and weighted results on restart */

    if (*ir <= 0) {
	varest = 0.;
	*finest = 0.;
    }
    finval = 0.;
    varsqr = 0.;
    npts = *maxpts / 2;
    i__1 = npts;
    for (m = 1; m <= i__1; ++m) {
	i__2 = *ndim;
	for (k = 1; k <= i__2; ++k) {
	    x[k - 1] = uni_();
	}
	fun = (*functn)(ndim, x);
	i__2 = *ndim;
	for (k = 1; k <= i__2; ++k) {
	    x[k - 1] = 1 - x[k - 1];
	}
	fun = ((*functn)(ndim, x) + fun) / 2;
	findif = (fun - finval) / m;
/* Computing 2nd power */
	d__1 = findif;
	varsqr = (m - 2) * varsqr / m + d__1 * d__1;
	finval += findif;
    }
    varprd = varest * varsqr;
    *finest += (finval - *finest) / (varprd + 1);
    if (varsqr > 0.) {
	varest = (varprd + 1) / varsqr;
    }
    *absest = sqrt(varsqr / (varprd + 1)) * 3;
    return 0;
} /* rcrude_ */

/* Subroutine */ int sphmvn_(integer *n, doublereal *lower, doublereal *upper,
	 integer *infin, doublereal *correl, integer *maxpts, doublereal *
	abseps, doublereal *releps, doublereal *error, doublereal *value, 
	integer *inform__)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal d__, e;
    static integer ns;
    static doublereal eps;
    static integer mpt, ivls, infis;
    extern /* Subroutine */ int scrude_(integer *, integer *, doublereal *, 
	    doublereal *, integer *);
    extern doublereal spnrnt_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *, 
	    integer *);


/*     A subroutine for computing multivariate normal probabilities. */
/*     This subroutine uses a Mont-Carlo algorithm given in the paper */
/*       "Three Digit Accurate Multiple Normal Probabilities", */
/*          pp. 369-380, Numer. Math. 35(1980), by I. Deak */


/*  Parameters */

/*     N      INTEGER, the number of variables. */
/*     LOWER  REAL, array of lower integration limits. */
/*     UPPER  REAL, array of upper integration limits. */
/*     INFIN  INTEGER, array of integration limits flags: */
/*            if INFIN(I) < 0, Ith limits are (-infinity, infinity); */
/*            if INFIN(I) = 0, Ith limits are (-infinity, UPPER(I)]; */
/*            if INFIN(I) = 1, Ith limits are [LOWER(I), infinity); */
/*            if INFIN(I) = 2, Ith limits are [LOWER(I), UPPER(I)]. */
/*     CORREL REAL, array of correlation coefficients; the correlation */
/*            coefficient in row I column J of the correlation matrix */
/*            should be stored in CORREL( J + ((I-2)*(I-1))/2 ), for J < I. */
/*     MAXPTS INTEGER, maximum number of function values allowed. This */
/*            parameter can be used to limit the time. A sensible */
/*            strategy is to start with MAXPTS = 1000*N, and then */
/*            increase MAXPTS if ERROR is too large. */
/*     ABSEPS REAL absolute error tolerance. */
/*     RELEPS REAL relative error tolerance. */
/*     ERROR  REAL, estimated absolute error, with 99% confidence level. */
/*     VALUE  REAL, estimated value for the integral */
/*     INFORM INTEGER, termination status parameter: */
/*            if INFORM = 0, normal completion with ERROR < EPS; */
/*            if INFORM = 1, completion with ERROR > EPS and MAXPTS */
/*                           function vaules used; increase MAXPTS to */
/*                           decrease ERROR; */
/*            if INFORM = 2, N > 100. */

    /* Parameter adjustments */
    --correl;
    --infin;
    --upper;
    --lower;

    /* Function Body */
    if (*n > 100) {
	*inform__ = 2;
	*value = 0.;
	*error = 1.;
	return 0;
    }
    *inform__ = (integer) spnrnt_(n, &correl[1], &lower[1], &upper[1], &infin[
	    1], &infis, &d__, &e, &ns);
    if (*n - infis == 0) {
	*value = 1.;
	*error = 0.;
    } else if (*n - infis == 1) {
	*value = e - d__;
	*error = 2e-16f;
    } else {

/*        Call then Monte-Carlo integration subroutine */

/* Computing 3rd power */
	i__1 = *n;
	mpt = ns / (i__1 * (i__1 * i__1)) + 25;
	i__1 = *n - infis;
	scrude_(&i__1, &mpt, error, value, &c__0);
	ivls = mpt * ns;
L10:
/* Computing MAX */
	d__1 = *abseps, d__2 = *releps * abs(*value);
	eps = max(d__1,d__2);
	if (*error > eps && ivls < *maxpts) {
/* Computing MAX */
/* Computing MIN */
/* Computing 2nd power */
	    d__1 = *error / eps;
	    i__2 = (integer) (mpt * (d__1 * d__1)), i__3 = (*maxpts - ivls) / 
		    ns;
	    i__1 = min(i__2,i__3);
	    mpt = max(i__1,10);
	    i__1 = *n - infis;
	    scrude_(&i__1, &mpt, error, value, &c__1);
	    ivls += mpt * ns;
	    goto L10;
	}
	if (*error > eps && ivls >= *maxpts) {
	    *inform__ = 1;
	}
    }
    return 0;
} /* sphmvn_ */

doublereal spnrml_0_(int n__, integer *n, doublereal *correl, doublereal *
	lower, doublereal *upper, integer *infin, integer *infis, doublereal *
	d__, doublereal *e, integer *nso)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal ret_val, d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal a[100], b[100];
    static integer i__, j, k;
    static doublereal u[10000]	/* was [100][100] */, y[100];
    static integer ic[100], ij;
    static doublereal bt;
    static integer is[100], ns;
    static doublereal rs, tmp;
    static integer infi[100];
    extern doublereal rnor_(void), sphlim_(integer *, doublereal *, 
	    doublereal *, integer *, doublereal *);
    extern /* Subroutine */ int limits_(doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *);


/*     Integrand subroutine */


/*    First generate U = COV*(random orthogonal matrix) */

    /* Parameter adjustments */
    if (correl) {
	--correl;
	}
    if (lower) {
	--lower;
	}
    if (upper) {
	--upper;
	}
    if (infin) {
	--infin;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_spnrnt;
	}

    for (k = *n - 1; k >= 1; --k) {
	tmp = 0.;
	i__1 = *n;
	for (j = k; j <= i__1; ++j) {
	    y[j - 1] = rnor_();
/* Computing 2nd power */
	    d__1 = y[j - 1];
	    tmp += d__1 * d__1;
	}
	tmp = -sqrt(tmp);
	bt = 1 / (tmp * (y[k - 1] + tmp));
	y[k - 1] += tmp;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    tmp = 0.;
	    i__2 = *n;
	    for (j = k; j <= i__2; ++j) {
		tmp += u[i__ + j * 100 - 101] * y[j - 1];
	    }
	    tmp = bt * tmp;
	    i__2 = *n;
	    for (j = k; j <= i__2; ++j) {
		u[i__ + j * 100 - 101] -= tmp * y[j - 1];
	    }
	}
    }

/*     Compute integrand average */

    rs = sqrt(3.);
    for (i__ = 1; i__ <= 3; ++i__) {
	ic[i__ - 1] = i__;
    }
    ic[3] = *n + 1;
    ret_val = 0.;
    ns = 0;
L10:
    for (i__ = 1; i__ <= 3; ++i__) {
	is[i__ - 1] = -1;
    }
L20:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	tmp = 0.;
	for (j = 1; j <= 3; ++j) {
	    tmp += is[j - 1] * u[i__ + ic[j - 1] * 100 - 101];
	}
	y[i__ - 1] = tmp / rs;
    }
    ++ns;
    ret_val += (sphlim_(n, a, b, infi, y) - ret_val) / ns;
    for (i__ = 1; i__ <= 3; ++i__) {
	is[i__ - 1] += 2;
	if (is[i__ - 1] < 2) {
	    goto L20;
	}
	is[i__ - 1] = -1;
    }
    for (i__ = 1; i__ <= 3; ++i__) {
	++ic[i__ - 1];
	if (ic[i__ - 1] < ic[i__]) {
	    goto L10;
	}
	ic[i__ - 1] = i__;
    }
    ret_val /= 2;
    return ret_val;

L_spnrnt:
    ret_val = 0.;

/*     Initialisation */

    ij = 0;
    *infis = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	infi[i__ - 1] = infin[i__];
	if (infi[i__ - 1] < 0) {
	    ++(*infis);
	} else {
	    a[i__ - 1] = 0.;
	    b[i__ - 1] = 0.;
	    if (infi[i__ - 1] != 0) {
		a[i__ - 1] = lower[i__];
	    }
	    if (infi[i__ - 1] != 1) {
		b[i__ - 1] = upper[i__];
	    }
	}
	i__2 = i__ - 1;
	for (j = 1; j <= i__2; ++j) {
	    ++ij;
	    u[i__ + j * 100 - 101] = correl[ij];
	    u[j + i__ * 100 - 101] = 0.;
	}
	u[i__ + i__ * 100 - 101] = 1.;
    }
    *nso = 1;
    for (i__ = 1; i__ <= 3; ++i__) {
	*nso = (*nso << 1) * (*n - *infis - i__ + 1) / i__;
    }

/*     First move any doubly infinite limits to innermost positions */

    if (*infis < *n) {
	i__1 = *n - *infis + 1;
	for (i__ = *n; i__ >= i__1; --i__) {
	    if (infi[i__ - 1] >= 0) {
		i__2 = i__ - 1;
		for (j = 1; j <= i__2; ++j) {
		    if (infi[j - 1] < 0) {
			i__3 = j - 1;
			for (k = 1; k <= i__3; ++k) {
			    tmp = u[j + k * 100 - 101];
			    u[j + k * 100 - 101] = u[i__ + k * 100 - 101];
			    u[i__ + k * 100 - 101] = tmp;
			}
			i__3 = i__ - 1;
			for (k = j + 1; k <= i__3; ++k) {
			    tmp = u[i__ + k * 100 - 101];
			    u[i__ + k * 100 - 101] = u[k + j * 100 - 101];
			    u[k + j * 100 - 101] = tmp;
			}
			i__3 = *n;
			for (k = i__ + 1; k <= i__3; ++k) {
			    tmp = u[k + j * 100 - 101];
			    u[k + j * 100 - 101] = u[k + i__ * 100 - 101];
			    u[k + i__ * 100 - 101] = tmp;
			}
			tmp = a[j - 1];
			a[j - 1] = a[i__ - 1];
			a[i__ - 1] = tmp;
			tmp = b[j - 1];
			b[j - 1] = b[i__ - 1];
			b[i__ - 1] = tmp;
			tmp = (doublereal) infi[j - 1];
			infi[j - 1] = infi[i__ - 1];
			infi[i__ - 1] = (integer) tmp;
			goto L30;
		    }
		}
	    }
L30:
	    ;
	}
    }

/*     Determine Cholesky decomposition */

    i__1 = *n - *infis;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n - *infis;
	for (i__ = j; i__ <= i__2; ++i__) {
	    tmp = u[i__ + j * 100 - 101];
	    i__3 = j - 1;
	    for (k = 1; k <= i__3; ++k) {
		tmp -= u[i__ + k * 100 - 101] * u[j + k * 100 - 101];
	    }
	    if (i__ == j) {
		u[j + j * 100 - 101] = sqrt((max(tmp,0.)));
	    } else if (u[i__ + i__ * 100 - 101] > 0.) {
		u[i__ + j * 100 - 101] = tmp / u[j + j * 100 - 101];
	    } else {
		u[i__ + j * 100 - 101] = 0.;
	    }
	}
    }
    i__1 = *n - *infis;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (u[i__ + i__ * 100 - 101] > 0.) {
	    if (infi[i__ - 1] != 0) {
		a[i__ - 1] /= u[i__ + i__ * 100 - 101];
	    }
	    if (infi[i__ - 1] != 1) {
		b[i__ - 1] /= u[i__ + i__ * 100 - 101];
	    }
	    i__2 = i__;
	    for (j = 1; j <= i__2; ++j) {
		u[i__ + j * 100 - 101] /= u[i__ + i__ * 100 - 101];
	    }
	}
    }
    limits_(a, b, infi, d__, e);
    return ret_val;
} /* spnrml_ */

doublereal spnrml_(integer *n)
{
    return spnrml_0_(0, n, (doublereal *)0, (doublereal *)0, (doublereal *)0, 
	    (integer *)0, (integer *)0, (doublereal *)0, (doublereal *)0, (
	    integer *)0);
    }

doublereal spnrnt_(integer *n, doublereal *correl, doublereal *lower, 
	doublereal *upper, integer *infin, integer *infis, doublereal *d__, 
	doublereal *e, integer *nso)
{
    return spnrml_0_(1, n, correl, lower, upper, infin, infis, d__, e, nso);
    }

doublereal sphlim_(integer *n, doublereal *a, doublereal *b, integer *infi, 
	doublereal *y)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val, d__1, d__2;

    /* Local variables */
    static integer i__;
    static doublereal cmn, cmx;
    extern doublereal sphinc_(integer *, doublereal *);

    /* Parameter adjustments */
    --y;
    --infi;
    --b;
    --a;

    /* Function Body */
    cmn = (doublereal) (*n * -10);
    cmx = (doublereal) (*n * 10);
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (y[i__] > 0.) {
	    if (infi[i__] != 1) {
/* Computing MIN */
		d__1 = cmx, d__2 = b[i__] / y[i__];
		cmx = min(d__1,d__2);
	    }
	    if (infi[i__] != 0) {
/* Computing MAX */
		d__1 = cmn, d__2 = a[i__] / y[i__];
		cmn = max(d__1,d__2);
	    }
	} else {
	    if (infi[i__] != 1) {
/* Computing MAX */
		d__1 = cmn, d__2 = b[i__] / y[i__];
		cmn = max(d__1,d__2);
	    }
	    if (infi[i__] != 0) {
/* Computing MIN */
		d__1 = cmx, d__2 = a[i__] / y[i__];
		cmx = min(d__1,d__2);
	    }
	}
    }
    if (cmn < cmx) {
	if (cmn >= 0. && cmx >= 0.) {
	    ret_val = sphinc_(n, &cmx) - sphinc_(n, &cmn);
	} else if (cmn < 0. && cmx >= 0.) {
	    d__1 = -cmn;
	    ret_val = sphinc_(n, &d__1) + sphinc_(n, &cmx);
	} else {
	    d__1 = -cmn;
	    d__2 = -cmx;
	    ret_val = sphinc_(n, &d__1) - sphinc_(n, &d__2);
	}
    } else {
	ret_val = 0.;
    }
    return ret_val;
} /* sphlim_ */

/* Subroutine */ int scrude_(integer *ndim, integer *maxpts, doublereal *
	absest, doublereal *finest, integer *ir)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer m;
    static doublereal findif, finval, varprd, varest;
    extern doublereal spnrml_(integer *);
    static doublereal varsqr;


/*     Crude Monte-Carlo Algorithm for Deak method with */
/*      weighted results on restart */

    if (*ir <= 0) {
	varest = 0.;
	*finest = 0.;
    }
    finval = 0.;
    varsqr = 0.;
    i__1 = *maxpts;
    for (m = 1; m <= i__1; ++m) {
	findif = (spnrml_(ndim) - finval) / m;
	finval += findif;
/* Computing 2nd power */
	d__1 = findif;
	varsqr = (m - 2) * varsqr / m + d__1 * d__1;
    }
    varprd = varest * varsqr;
    *finest += (finval - *finest) / (varprd + 1);
    if (varsqr > 0.) {
	varest = (varprd + 1) / varsqr;
    }
    *absest = sqrt(varsqr / (varprd + 1)) * 3;
    return 0;
} /* scrude_ */

doublereal sphinc_(integer *n, doublereal *r__)
{
    /* System generated locals */
    doublereal ret_val, d__1;

    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal pf, et, rr;
    extern doublereal phi_(doublereal *);


/*                   R */
/*     SPHINC =  K  I  exp(-t*t/2) t**(N-1) dt, for N > 1. */
/*                N  0 */

    if (*r__ > 0.) {
	rr = *r__ * *r__;
	pf = 1.;
	for (i__ = *n - 2; i__ >= 2; i__ += -2) {
	    pf = rr * pf / i__ + 1;
	}
	if (*n % 2 == 0) {
	    et = log(pf) - rr / 2;
	    if (et > -40.) {
		ret_val = 1 - exp(et);
	    } else {
		ret_val = 1.;
	    }
	} else {
	    d__1 = -(*r__);
	    ret_val = 1 - phi_(&d__1) * 2;
	    et = log(*r__ * pf) - rr / 2;
	    if (et > -40.) {
		ret_val -= exp(et) * 2 / 2.5066282746310004;
	    }
	}
    } else {
	ret_val = 0.;
    }
    return ret_val;
} /* sphinc_ */

doublereal rnor_(void)
{
    /* Initialized data */

    static doublereal x[65] = { .3409450287039653,.4573145918669259,
	    .5397792816116612,.6062426796530441,.6631690627645207,
	    .7136974590560222,.7596124749339174,.8020356003555283,
	    .8417226679789527,.8792102232083114,.9148948043867484,
	    .9490791137530882,.9820004812398864,1.01384923802994,
	    1.044781036740172,1.074925382028552,1.104391702268125,
	    1.13327377624394,1.161653030133931,1.189601040838737,
	    1.21718147070087,1.244451587898246,1.271463480572119,
	    1.298265041883197,1.32490078218086,1.351412509933371,
	    1.377839912870011,1.404221063559975,1.430592868502691,
	    1.456991476137671,1.483452656603219,1.510012164318519,
	    1.53670609335952,1.563571235037691,1.590645447014253,
	    1.617968043674446,1.645580218369081,1.673525509567038,
	    1.70185032506274,1.730604541317782,1.7598421990383,
	    1.789622321566574,1.820009890130691,1.851077020230275,
	    1.882904397592872,1.915583051943031,1.94921657491636,
	    1.983923928905685,2.019843052906235,2.057135559990095,
	    2.095992956249391,2.136645022544389,2.179371340398135,
	    2.224517507216017,2.272518554850147,2.323933820094302,
	    2.379500774082828,2.440221797979943,2.507511701865317,
	    2.583465835225429,2.671391590320836,2.776994269662875,
	    2.776994269662875,2.776994269662875,2.776994269662875 };

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    double exp(doublereal), d_sign(doublereal *, doublereal *), log(
	    doublereal);

    /* Local variables */
    static integer j;
    static doublereal y, xt, xx;
    extern doublereal uni_(void);


/*     RNOR generates normal random numbers with zero mean and unit */
/*     standard deviation, often denoted N(0,1),adapted from G. Marsaglia */
/*     and W. W. Tsang: "A Fast, Easily Implemented Method for Sampling */
/*     from Decreasing or Symmetric Unimodal Density Functions" */
/*      SIAM J. Sci. Stat. Comput. 5(1984), pp. 349-359. */

    y = uni_();
    j = (integer) (uni_() * 128) % 64;
    xt = x[j + 1];
    ret_val = (y + y - 1) * xt;
    if (abs(ret_val) > x[j]) {
	xx = (xt - abs(ret_val)) * .487899177760394 / (xt - x[j]);
	y = uni_();
/* Computing 2nd power */
	d__1 = xx;
	if (y > 12.6770580788656 - exp(-(d__1 * d__1) / 2) * 
		12.37586029917064) {
	    ret_val = d_sign(&xx, &ret_val);
	} else {
/* Computing 2nd power */
	    d__1 = xt;
/* Computing 2nd power */
	    d__2 = ret_val;
	    if (exp(-(d__1 * d__1) / 2) + y / (xt * 51.0646118913834) > exp(-(
		    d__2 * d__2) / 2)) {
L10:
		xx = log(uni_()) * .3601015713011893;
/* Computing 2nd power */
		d__1 = xx;
		if (log(uni_()) * -2 <= d__1 * d__1) {
		    goto L10;
		}
		d__1 = x[64] - xx;
		ret_val = d_sign(&d__1, &ret_val);
	    }
	}
    }
    return ret_val;
} /* rnor_ */


doublereal uni_(void)
{
    /* Initialized data */

    static integer x10 = 11111111;
    static integer x11 = 22222223;
    static integer x12 = 33333335;
    static integer x20 = 44444447;
    static integer x21 = 55555559;
    static integer x22 = 66666661;

    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    static integer h__, z__, p12, p13, p21, p23;


/*     Uniform (0, 1) random number generator */

/*     Reference: */
/*     L'Ecuyer, Pierre (1996), */
/*     "Combined Multiple Recursive Random Number Generators" */
/*     Operations Research 44, pp. 816-822. */


/*                 INVMP1 = 1/(M1+1) */

/*     Component 1 */

    h__ = x10 / 11714;
    p13 = (x10 - h__ * 11714) * 183326 - h__ * 2883;
    h__ = x11 / 33921;
    p12 = (x11 - h__ * 33921) * 63308 - h__ * 12979;
    if (p13 < 0) {
	p13 += 2147483647;
    }
    if (p12 < 0) {
	p12 += 2147483647;
    }
    x10 = x11;
    x11 = x12;
    x12 = p12 - p13;
    if (x12 < 0) {
	x12 += 2147483647;
    }

/*     Component 2 */

    h__ = x20 / 3976;
    p23 = (x20 - h__ * 3976) * 539608 - h__ * 2071;
    h__ = x22 / 24919;
    p21 = (x22 - h__ * 24919) * 86098 - h__ * 7417;
    if (p23 < 0) {
	p23 += 2145483479;
    }
    if (p21 < 0) {
	p21 += 2145483479;
    }
    x20 = x21;
    x21 = x22;
    x22 = p21 - p23;
    if (x22 < 0) {
	x22 += 2145483479;
    }

/*     Combination */

    z__ = x12 - x22;
    if (z__ <= 0) {
	z__ += 2147483647;
    }
    ret_val = z__ * 4.656612873077392578125e-10;
    return ret_val;
} /* uni_ */

/* Subroutine */ int sadmvn_(integer *n, doublereal *lower, doublereal *upper,
	 integer *infin, doublereal *correl, integer *maxpts, doublereal *
	abseps, doublereal *releps, doublereal *error, doublereal *value, 
	integer *inform__)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal d__, e;
    static integer m;
    static doublereal work[100000];
    extern /* Subroutine */ int adapt_(integer *, integer *, integer *, D_fp, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *, integer *);
    static integer infis;
    static doublereal oldval;
    extern doublereal mvnfnc_();
    static integer maxcls, newcls, rulcls, totcls;
    extern doublereal mvnnit_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *);


/*     A subroutine for computing multivariate normal probabilities. */
/*     This subroutine uses an algorithm given in the paper */
/*     "Numerical Computation of Multivariate Normal Probabilities", in */
/*     J. of Computational and Graphical Stat., 1(1992), pp. 141-149, by */
/*          Alan Genz */
/*          Department of Mathematics */
/*          Washington State University */
/*          Pullman, WA 99164-3113 */
/*          Email : alangenz@wsu.edu */

/*  Parameters */

/*     N      INTEGER, the number of variables. */
/*     LOWER  REAL, array of lower integration limits. */
/*     UPPER  REAL, array of upper integration limits. */
/*     INFIN  INTEGER, array of integration limits flags: */
/*            if INFIN(I) < 0, Ith limits are (-infinity, infinity); */
/*            if INFIN(I) = 0, Ith limits are (-infinity, UPPER(I)]; */
/*            if INFIN(I) = 1, Ith limits are [LOWER(I), infinity); */
/*            if INFIN(I) = 2, Ith limits are [LOWER(I), UPPER(I)]. */
/*     CORREL REAL, array of correlation coefficients; the correlation */
/*            coefficient in row I column J of the correlation matrix */
/*            should be stored in CORREL( J + ((I-2)*(I-1))/2 ), for J < I. */
/*     MAXPTS INTEGER, maximum number of function values allowed. This */
/*            parameter can be used to limit the time taken. A */
/*            sensible strategy is to start with MAXPTS = 1000*N, and then */
/*            increase MAXPTS if ERROR is too large. */
/*     ABSEPS REAL absolute error tolerance. */
/*     RELEPS REAL relative error tolerance. */
/*     ERROR  REAL estimated absolute error, with 99% confidence level. */
/*     VALUE  REAL estimated value for the integral */
/*     INFORM INTEGER, termination status parameter: */
/*            if INFORM = 0, normal completion with ERROR < EPS; */
/*            if INFORM = 1, completion with ERROR > EPS and MAXPTS */
/*                           function vaules used; increase MAXPTS to */
/*                           decrease ERROR; */
/*            if INFORM = 2, N > 20 or N < 1. */

    /* Parameter adjustments */
    --correl;
    --infin;
    --upper;
    --lower;

    /* Function Body */
    if (*n > 20 || *n < 1) {
	*inform__ = 2;
	*value = 0.;
	*error = 1.;
	return 0;
    }
    *inform__ = (integer) mvnnit_(n, &correl[1], &lower[1], &upper[1], &infin[
	    1], &infis, &d__, &e);
    m = *n - infis;
    if (m == 0) {
	*value = 1.;
	*error = 0.;
    } else if (m == 1) {
	*value = e - d__;
	*error = 2e-16f;
    } else {

/*        Call the subregion adaptive integration subroutine */

	--m;
	rulcls = 1;
	adapt_(&m, &rulcls, &c__0, (D_fp)mvnfnc_, abseps, releps, &c_b96, 
		work, error, value, inform__);
/* Computing MIN */
	i__1 = rulcls * 10;
	maxcls = min(i__1,*maxpts);
	totcls = 0;
	adapt_(&m, &totcls, &maxcls, (D_fp)mvnfnc_, abseps, releps, &c_b96, 
		work, error, value, inform__);
/* Computing MAX */
	d__1 = *abseps, d__2 = *releps * abs(*value);
	if (*error > max(d__1,d__2)) {
L10:
	    oldval = *value;
/* Computing MAX */
/* Computing MIN */
	    i__3 = maxcls * 3 / 2, i__4 = *maxpts - totcls;
	    i__1 = rulcls << 1, i__2 = min(i__3,i__4);
	    maxcls = max(i__1,i__2);
	    newcls = -1;
	    adapt_(&m, &newcls, &maxcls, (D_fp)mvnfnc_, abseps, releps, &
		    c_b96, work, error, value, inform__);
	    totcls += newcls;
/* Computing 2nd power */
	    d__2 = *error;
	    *error = (d__1 = *value - oldval, abs(d__1)) + sqrt(rulcls * (
		    d__2 * d__2) / totcls);
/* Computing MAX */
	    d__1 = *abseps, d__2 = *releps * abs(*value);
	    if (*error > max(d__1,d__2)) {
		if (*maxpts - totcls > rulcls << 1) {
		    goto L10;
		}
	    } else {
		*inform__ = 0;
	    }
	}
    }
    return 0;
} /* sadmvn_ */


/* Subroutine */ int kromvn_(integer *n, doublereal *lower, doublereal *upper,
	 integer *infin, doublereal *correl, integer *maxpts, doublereal *
	abseps, doublereal *releps, doublereal *error, doublereal *value, 
	integer *inform__)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal d__, e;
    static integer ivls, infis;
    extern doublereal mvnfnc_();
    extern /* Subroutine */ int krobov_(integer *, integer *, integer *, D_fp,
	     doublereal *, doublereal *, doublereal *, doublereal *, integer *
	    );
    extern doublereal mvnnit_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *);


/*     A subroutine for computing multivariate normal probabilities. */
/*     This subroutine uses an algorithm given in the paper */
/*     "Numerical Computation of Multivariate Normal Probabilities", in */
/*     J. of Computational and Graphical Stat., 1(1992), pp. 141-149, by */
/*          Alan Genz */
/*          Department of Mathematics */
/*          Washington State University */
/*          Pullman, WA 99164-3113 */
/*          Email : AlanGenz@wsu.edu */

/*  Parameters */

/*     N      INTEGER, the number of variables. */
/*     LOWER  REAL, array of lower integration limits. */
/*     UPPER  REAL, array of upper integration limits. */
/*     INFIN  INTEGER, array of integration limits flags: */
/*            if INFIN(I) < 0, Ith limits are (-infinity, infinity); */
/*            if INFIN(I) = 0, Ith limits are (-infinity, UPPER(I)]; */
/*            if INFIN(I) = 1, Ith limits are [LOWER(I), infinity); */
/*            if INFIN(I) = 2, Ith limits are [LOWER(I), UPPER(I)]. */
/*     CORREL REAL, array of correlation coefficients; the correlation */
/*            coefficient in row I column J of the correlation matrix */
/*            should be stored in CORREL( J + ((I-2)*(I-1))/2 ), for J < I. */
/*     MAXPTS INTEGER, maximum number of function values allowed. This */
/*            parameter can be used to limit the time. A sensible */
/*            strategy is to start with MAXPTS = 1000*N, and then */
/*            increase MAXPTS if ERROR is too large. */
/*     ABSEPS REAL absolute error tolerance. */
/*     RELEPS REAL relative error tolerance. */
/*     ERROR  REAL estimated absolute error, with 99% confidence level. */
/*     VALUE  REAL estimated value for the integral */
/*     INFORM INTEGER, termination status parameter: */
/*            if INFORM = 0, normal completion with ERROR < EPS; */
/*            if INFORM = 1, completion with ERROR > EPS and MAXPTS */
/*                           function vaules used; increase MAXPTS to */
/*                           decrease ERROR; */
/*            if INFORM = 2, N > 100 or N < 1. */

    /* Parameter adjustments */
    --correl;
    --infin;
    --upper;
    --lower;

    /* Function Body */
    if (*n > 100 || *n < 1) {
	*inform__ = 2;
	*value = 0.;
	*error = 1.;
    } else {
	*inform__ = (integer) mvnnit_(n, &correl[1], &lower[1], &upper[1], &
		infin[1], &infis, &d__, &e);
	if (*n - infis == 0) {
	    *value = 1.;
	    *error = 0.;
	} else if (*n - infis == 1) {
	    *value = e - d__;
	    *error = 2e-16f;
	} else {

/*        Call the lattice rule integration subroutine */

	    ivls = 0;
	    i__1 = *n - infis - 1;
	    krobov_(&i__1, &ivls, maxpts, (D_fp)mvnfnc_, abseps, releps, 
		    error, value, inform__);
	}
    }
    return 0;
} /* kromvn_ */

/* Subroutine */ int krobov_(integer *ndim, integer *minvls, integer *maxvls, 
	D_fp functn, doublereal *abseps, doublereal *releps, doublereal *
	abserr, doublereal *finest, integer *inform__)
{
    /* Initialized data */

    static integer p[20] = { 113,173,263,397,593,907,1361,2053,3079,4621,6947,
	    10427,15641,23473,35221,52837,79259,118891,178349,267523 };
    static integer c__[2000]	/* was [20][100] */ = { 42,64,111,151,229,264,
	    505,468,1189,1764,2872,4309,6610,9861,13482,13482,34566,31929,
	    73726,103650,54,34,67,168,40,402,220,635,1423,1349,1238,2339,1658,
	    7101,5629,5629,38838,40295,16352,50089,55,57,98,46,268,406,195,
	    849,287,1859,387,4154,3022,6257,6068,6068,23965,2610,16297,70223,
	    32,9,36,197,42,147,410,687,186,693,2135,4480,2603,7878,11974,
	    11974,17279,5177,74268,41805,13,72,48,69,153,452,199,948,341,78,
	    235,4967,5211,11170,4732,4732,35325,17271,60788,74847,26,86,110,
	    64,294,153,248,37,77,438,1565,630,265,11638,14946,14946,33471,
	    23770,8555,112775,26,16,2,2,71,224,460,1014,733,531,221,5212,4985,
	    7542,12097,12097,330,9140,1077,40889,13,75,131,198,2,2,471,513,
	    733,68,1515,2592,3,2592,17609,17609,36050,952,25486,64866,26,75,2,
	    191,130,2,2,2,1116,2234,2950,4715,4971,2591,11740,11740,26419,
	    39631,86595,44053,14,70,2,134,199,224,331,2,2,2310,486,1808,2127,
	    6074,15170,15170,3012,3,59450,1754,13,42,124,134,199,224,662,2,
	    1539,2310,3473,1808,1877,1428,10478,10478,38428,11424,19958,
	    129471,26,2,124,167,199,449,547,2,2,2310,2,5213,1877,8925,10478,
	    10478,36430,49719,62205,13630,35,86,48,124,149,101,209,2,2,2,2950,
	    2,2,11736,17610,17610,36430,38267,62205,53467,2,62,2,16,199,182,
	    547,1026,2,2310,982,216,2925,8925,2,2,36755,25172,4825,53467,2,62,
	    2,124,149,449,547,2,2,2310,2950,4014,3175,5623,2,2,39629,2,4825,
	    61378,2,30,124,124,153,101,209,2,2,2102,3122,3499,3878,5623,7064,
	    7064,5749,2,89174,133761,2,30,124,124,130,451,2,1026,1116,2102,
	    2950,3499,1940,1535,7064,7064,5749,59445,89174,2,56,5,70,124,149,
	    181,680,201,847,178,3172,4204,1940,6759,7064,7064,36755,2,62205,
	    133761,28,42,70,141,149,181,680,201,1174,314,2091,2701,1940,9953,
	    5665,5665,5749,59445,19958,2,7,70,48,134,15,101,629,2,2,921,2091,
	    2701,5117,9953,1771,1771,14353,38267,62205,133761,7,70,126,128,
	    119,101,370,1026,827,1074,9,5213,5117,11459,2947,2947,14353,44358,
	    19958,133761,28,70,48,2,294,377,574,413,713,1074,3449,4157,5771,
	    9953,4453,4453,14353,14673,27626,65531,4,53,126,2,31,85,63,1026,
	    910,1074,3122,1209,5117,7615,12323,12323,32395,53892,63080,65531,
	    49,70,56,32,82,453,63,1026,944,2147,2846,4157,5117,7615,17610,
	    17610,32395,14674,62205,65531,4,70,65,32,260,453,259,2,139,314,
	    3122,4460,5117,11377,14809,14809,32395,14673,62205,38080,40,53,48,
	    32,122,453,268,2,1174,1869,3122,335,5117,11377,14809,14809,32395,
	    14674,62205,133761,48,42,48,31,209,85,259,703,1174,178,1947,4460,
	    5117,2762,5665,5665,32396,41368,19958,133761,5,62,70,31,209,197,
	    547,703,1539,178,2846,1533,5771,11734,5665,5665,32396,17875,8914,
	    131061,35,53,2,64,122,451,209,2,1397,1324,3122,4575,5771,11459,
	    2947,2947,32396,17875,83856,5431,27,53,92,64,296,2,209,2,1397,
	    1324,772,4013,5117,6892,2947,2947,32396,30190,30760,65531,16,53,
	    124,99,130,2,209,393,1174,510,1387,4460,3658,1535,2947,2947,27739,
	    20444,47774,78250,16,69,92,4,130,101,547,393,370,2309,2895,1881,
	    3658,6759,2947,2947,14353,55869,47774,11397,2,75,126,4,260,449,
	    547,678,33,1541,1387,2701,3658,4695,12323,12323,36430,15644,19958,
	    38841,2,5,131,167,260,449,209,413,1210,1541,3,4030,3658,1535,
	    12323,12323,36430,25499,62205,38841,7,53,124,124,30,449,209,1026,
	    2,1541,3,4030,3658,6892,4453,4453,36430,15644,39865,107233,28,86,
	    70,124,206,173,547,2,370,1541,3,1881,3658,2,4453,4453,15727,20983,
	    39865,107233,4,2,70,124,94,173,547,2,1423,342,1320,4030,5255,2,
	    2026,2026,38428,44358,74988,111286,49,5,70,124,209,2,108,1026,370,
	    1324,1320,1738,2925,6892,11772,11772,28987,15644,75715,19065,4,30,
	    20,124,94,453,63,1026,370,1324,2963,249,2619,6892,2026,2026,28987,
	    15644,75715,38841,56,75,105,124,122,453,63,2,1423,1324,2963,335,
	    1714,4177,11665,11665,27739,485,74988,19065,8,59,70,107,209,2,108,
	    405,1423,1324,1320,57,4100,4177,12323,12323,38428,41428,34522,
	    19065,2,2,2,85,209,426,63,953,1423,510,1320,2561,6718,6339,12323,
	    12323,27739,485,74988,16099,2,69,2,79,122,66,63,2,434,570,2380,
	    2561,6718,6950,3582,3582,18786,485,74988,127638,56,5,27,85,122,
	    367,108,1026,1423,570,108,2561,4100,1226,2940,2940,14353,485,
	    25101,82411,7,5,108,111,209,426,259,123,901,2197,1284,1533,2322,
	    1226,2940,2940,15727,41428,44621,96659,16,63,27,85,130,101,268,
	    123,139,173,702,1533,842,1226,6654,6654,28987,53798,44621,96659,
	    28,62,39,128,2,453,268,953,1174,1202,1429,1533,4100,4177,4449,
	    4449,19151,50230,44621,82411,7,5,2,31,130,2,547,953,427,998,907,
	    4013,6718,6892,9254,9254,19757,53798,25101,96659,7,69,131,31,130,
	    32,209,123,427,1324,3220,4013,5119,6890,11470,11470,19757,50253,
	    25101,82411,28,30,131,31,38,32,209,405,200,1324,3125,4013,4728,
	    3640,304,304,19757,50253,25101,51986,4,44,92,31,38,32,209,794,
	    1247,178,1320,4013,5255,3640,304,304,14353,35677,44621,101677,49,
	    30,92,64,79,101,209,123,114,1324,2963,4013,5771,1226,11470,11470,
	    22876,35677,47768,39264,4,86,48,167,82,2,547,647,114,1324,1320,
	    1533,5771,10590,304,304,19151,17474,41547,39264,37,86,2,4,94,2,
	    209,613,1441,1541,1320,856,5771,10590,11470,11470,24737,7592,
	    44621,101677,55,2,126,107,82,453,209,1026,139,1541,2963,856,5117,
	    6950,6156,6156,24737,4098,10273,39264,21,69,20,167,122,223,209,
	    647,728,1541,1320,468,5771,6950,9254,9254,4412,17474,74988,39264,
	    33,5,126,124,122,147,547,768,1116,342,1639,468,5117,6950,11772,
	    11772,30567,485,74988,47996,40,5,2,124,209,449,108,953,1174,1541,
	    3168,468,5117,1226,6654,6654,30537,41428,74988,96659,16,2,2,124,
	    209,290,63,405,139,886,1660,2561,5117,6950,11772,11772,19757,485,
	    74988,82411,16,2,131,124,122,2,63,953,113,178,2895,468,5117,6950,
	    6156,6156,30537,41428,74988,47996,28,61,38,124,122,453,63,405,113,
	    1324,2895,2022,5117,7586,11470,11470,19757,485,74988,10971,7,69,
	    117,124,168,2,405,918,113,1324,2895,2022,5117,7586,11470,11470,
	    30537,41428,34522,10004,16,17,2,107,220,83,285,918,1406,1324,2895,
	    2434,5771,7565,11772,11772,30537,485,34522,82411,28,2,131,183,62,
	    223,234,123,1247,510,1639,138,5771,7565,11772,11772,4412,41428,
	    67796,96659,4,2,68,2,60,101,259,953,200,784,1297,4605,1868,3640,
	    11772,11772,24737,41428,67796,82411,49,2,58,2,168,453,259,953,200,
	    784,1639,1100,4483,3640,11470,11470,28987,41428,30208,82411,4,53,
	    38,2,282,2,259,918,200,501,404,2561,4728,6950,11470,11470,19757,
	    41428,2,82411,56,69,90,62,282,83,259,953,200,652,3168,2561,3658,
	    7565,304,304,19757,41428,67062,96659,35,2,38,32,82,83,209,536,
	    1247,1541,2963,57,5255,6950,11470,11470,19757,9020,18500,96659,2,
	    2,108,31,209,147,209,405,1247,1541,2943,57,3658,3599,11470,11470,
	    30537,22816,29251,96659,2,86,38,31,122,2,209,70,27,1324,2943,3249,
	    5255,3599,304,304,30537,4098,29251,82411,2,69,2,31,94,453,209,124,
	    427,178,550,468,3658,3599,11470,11470,33186,4098,2,96659,16,13,
	    131,31,209,147,209,1005,427,1324,1387,468,3658,2441,304,304,4010,
	    4098,67796,51986,16,2,131,31,122,147,209,529,1122,178,1387,468,
	    5255,4885,11470,11470,4010,7592,67062,110913,28,2,131,167,122,147,
	    209,207,1122,1324,2895,57,5255,4885,304,304,4010,42517,38649,
	    51986,4,37,68,4,122,147,209,405,696,1541,2895,468,3658,4885,304,
	    304,17307,485,59302,51986,16,43,14,107,122,147,547,405,696,342,
	    2895,1738,6718,7565,304,304,15217,50006,6225,110913,28,65,94,167,
	    258,147,289,953,427,1541,1387,313,6718,7565,6654,6654,32789,50006,
	    67062,82411,4,2,131,124,148,147,289,953,1539,2144,2895,856,842,
	    1226,11508,11508,37709,22816,6475,54713,49,2,131,124,286,453,234,
	    123,435,784,1387,6,2322,1226,304,304,4010,22816,6225,54713,4,30,
	    131,124,256,153,285,918,1122,2132,2895,3877,6718,1226,304,304,
	    4010,9020,46772,22360,40,86,108,124,256,153,316,918,758,1324,1320,
	    468,4100,6950,6156,6156,4010,485,38649,117652,40,45,18,124,62,147,
	    2,953,1247,1324,1320,557,6718,7586,3582,3582,33186,41428,67062,
	    22360,5,16,131,124,62,2,410,405,1247,1324,2963,468,4100,1346,
	    11470,11470,33186,41428,46772,78250,42,32,56,107,82,224,259,918,
	    1247,1324,1320,57,4100,2441,11470,11470,4010,41428,46772,78250,27,
	    18,85,142,122,290,259,953,200,510,1320,468,5117,6339,11470,11470,
	    11057,41428,67062,91996,16,86,117,184,82,320,259,468,200,652,1320,
	    4605,5771,3640,17274,17274,39388,50006,46772,22360,16,86,117,184,
	    82,453,268,405,200,1804,2963,2022,5771,6950,6654,6654,33186,485,
	    25372,91996,28,86,9,65,122,147,209,794,1247,1541,1320,2,5117,
	    10590,6654,6654,1122,41428,67062,97781,4,9,131,65,122,431,209,794,
	    114,1541,2,4605,5771,6339,6744,6744,15089,41428,6475,91996,16,63,
	    131,183,122,383,209,647,27,1541,3473,138,5771,6950,6711,6711,
	    39629,41428,25372,97781,28,63,55,31,209,290,209,613,118,2132,2,
	    1100,5771,6950,6654,6654,2,41428,67062,91996,4,11,92,31,122,290,
	    547,548,118,1324,3473,57,5771,6950,6156,6156,2,22816,67062,97781,
	    49,76,92,31,15,2,547,405,113,1324,772,2561,5117,1226,3370,3370,
	    23899,41428,67062,97781,4,76,92,31,79,162,209,953,118,1324,2550,
	    57,5117,1226,6654,6654,16466,41428,6225,91996,8,76,131,31,79,162,
	    209,405,453,178,9,57,5117,6950,12134,12134,16466,485,67062,97781,
	    8,63,131,167,79,147,209,953,453,510,1320,2022,5771,836,3370,3370,
	    17038,485,67062,97781,2,60,48,4,79,2,285,123,1084,1541,2963,5213,
	    5771,6891,6654,6654,9477,485,68247,36249,2,70,48,107,168,162,316,
	    918,1406,652,1320,3249,1868,7565,3582,3582,9260,9020,80676,39779 }
	    ;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double d_mod(doublereal *, doublereal *), sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal x[100];
    static integer np;
    static doublereal vk[100], alpha[100], value, difint, finval, varprd;
    static integer sampls;
    static doublereal varest, varsqr;
    static integer intvls;
    extern /* Subroutine */ int krosum_(integer *, doublereal *, integer *, 
	    doublereal *, D_fp, doublereal *, doublereal *);


/*  Automatic Multidimensional Integration Subroutine */

/*         AUTHOR: Alan Genz */
/*                 Department of Mathematics */
/*                 Washington State University */
/*                 Pulman, WA 99164-3113 */
/*                 Email: AlanGenz@wsu.edu */

/*         Last Change: 4/15/98 */

/*  KROBOV computes an approximation to the integral */

/*      1  1     1 */
/*     I  I ... I       F(X)  dx(NDIM)...dx(2)dx(1) */
/*      0  0     0 */


/*  KROBOV uses randomized Korobov rules. The primary references are */
/*  "Randomization of Number Theoretic Methods for Multiple Integration" */
/*   R. Cranley and T.N.L. Patterson, SIAM J Numer Anal, 13, pp. 904-14, */
/*  and */
/*   "Optimal Parameters for Multidimensional Integration", */
/*    P. Keast, SIAM J Numer Anal, 10, pp.831-838. */

/* **************  Parameters ******************************************** */
/* ***** Input parameters */
/*  NDIM    Number of variables, must exceed 1, but not exceed 40 */
/*  MINVLS  Integer minimum number of function evaluations allowed. */
/*          MINVLS must not exceed MAXVLS.  If MINVLS < 0 then the */
/*          routine assumes a previous call has been made with */
/*          the same integrand and continues that calculation. */
/*  MAXVLS  Integer maximum number of function evaluations allowed. */
/*  FUNCTN  EXTERNALly declared user defined function to be integrated. */
/*          It must have parameters (NDIM,Z), where Z is a real array */
/*          of dimension NDIM. */
/*  ABSEPS  Required absolute accuracy. */
/*  RELEPS  Required relative accuracy. */
/* ***** Output parameters */
/*  MINVLS  Actual number of function evaluations used. */
/*  ABSERR  Estimated absolute accuracy of FINEST. */
/*  FINEST  Estimated value of integral. */
/*  INFORM  INFORM = 0 for normal exit, when */
/*                     ABSERR <= MAX(ABSEPS, RELEPS*ABS(FINEST)) */
/*                  and */
/*                     INTVLS <= MAXCLS. */
/*          INFORM = 1 If MAXVLS was too small to obtain the required */
/*          accuracy. In this case a value FINEST is returned with */
/*          estimated absolute accuracy ABSERR. */
/* *********************************************************************** */
    *inform__ = 1;
    intvls = 0;
    if (*minvls >= 0) {
	*finest = 0.;
	varest = 0.;
	sampls = 6;
	for (i__ = 1; i__ <= 20; ++i__) {
	    np = i__;
	    if (*minvls < (sampls << 1) * p[i__ - 1]) {
		goto L10;
	    }
	}
/* Computing MAX */
	i__1 = 6, i__2 = *minvls / (p[np - 1] << 1);
	sampls = max(i__1,i__2);
    }
L10:
    vk[0] = 1. / p[np - 1];
    i__1 = *ndim;
    for (i__ = 2; i__ <= i__1; ++i__) {
	d__1 = c__[np + (*ndim - 1) * 20 - 21] * vk[i__ - 2];
	vk[i__ - 1] = d_mod(&d__1, &c_b103);
    }
    finval = 0.;
    varsqr = 0.;
    i__1 = sampls;
    for (i__ = 1; i__ <= i__1; ++i__) {
	krosum_(ndim, &value, &p[np - 1], vk, (D_fp)functn, alpha, x);
	difint = (value - finval) / i__;
	finval += difint;
/* Computing 2nd power */
	d__1 = difint;
	varsqr = (i__ - 2) * varsqr / i__ + d__1 * d__1;
    }
    intvls += (sampls << 1) * p[np - 1];
    varprd = varest * varsqr;
    *finest += (finval - *finest) / (varprd + 1);
    if (varsqr > 0.) {
	varest = (varprd + 1) / varsqr;
    }
    *abserr = sqrt(varsqr / (varprd + 1)) * 3;
/* Computing MAX */
    d__1 = *abseps, d__2 = abs(*finest) * *releps;
    if (*abserr > max(d__1,d__2)) {
	if (np < 20) {
	    ++np;
	} else {
/* Computing MIN */
	    i__1 = sampls * 3 / 2, i__2 = (*maxvls - intvls) / (p[np - 1] << 
		    1);
	    sampls = min(i__1,i__2);
	    sampls = max(6,sampls);
	}
	if (intvls + (sampls << 1) * p[np - 1] <= *maxvls) {
	    goto L10;
	}
    } else {
	*inform__ = 0;
    }
    *minvls = intvls;
    return 0;
} /* krobov_ */


/* Subroutine */ int krosum_(integer *ndim, doublereal *sumkro, integer *
	prime, doublereal *vk, D_fp functn, doublereal *alpha, doublereal *x)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double d_mod(doublereal *, doublereal *);

    /* Local variables */
    static integer j, k;
    extern doublereal uni_(void);

    /* Parameter adjustments */
    --x;
    --alpha;
    --vk;

    /* Function Body */
    *sumkro = 0.;
    i__1 = *ndim;
    for (j = 1; j <= i__1; ++j) {
	alpha[j] = uni_();
    }
    i__1 = *prime;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ndim;
	for (j = 1; j <= i__2; ++j) {
	    d__1 = k * vk[j] + alpha[j];
	    x[j] = d_mod(&d__1, &c_b103);
	    x[j] = (d__1 = x[j] * 2 - 1, abs(d__1));
	}
	*sumkro += ((*functn)(ndim, &x[1]) - *sumkro) / ((k << 1) - 1);
	i__2 = *ndim;
	for (j = 1; j <= i__2; ++j) {
	    x[j] = 1 - x[j];
	}
	*sumkro += ((*functn)(ndim, &x[1]) - *sumkro) / (k << 1);
    }
    return 0;
} /* krosum_ */


doublereal mvnfnc_0_(int n__, integer *n, doublereal *w, doublereal *correl, 
	doublereal *lower, doublereal *upper, integer *infin, integer *infis, 
	doublereal *d__, doublereal *e)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    double d_sign(doublereal *, doublereal *), sqrt(doublereal);

    /* Local variables */
    static doublereal a[100], b[100];
    static integer i__, j;
    static doublereal y[100], d1, e1, di, ei;
    static integer ij;
    extern doublereal bvn_(doublereal *, doublereal *, integer *, doublereal *
	    );
    static doublereal cov[5050], sum;
    static integer infi[100];
    static doublereal prod;
    extern doublereal phinv_(doublereal *);
    extern /* Subroutine */ int limits_(doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *), ncvsrt_(integer *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *);


/*     Integrand subroutine */

    /* Parameter adjustments */
    if (w) {
	--w;
	}
    if (correl) {
	--correl;
	}
    if (lower) {
	--lower;
	}
    if (upper) {
	--upper;
	}
    if (infin) {
	--infin;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_mvnnit;
	}

    di = d1;
    ei = e1;
    prod = e1 - d1;
    ij = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	d__1 = di + w[i__] * (ei - di);
	y[i__ - 1] = phinv_(&d__1);
	sum = 0.;
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    ++ij;
	    sum += cov[ij - 1] * y[j - 1];
	}
	++ij;
	if (cov[ij - 1] > 0.) {
	    d__1 = a[i__] - sum;
	    d__2 = b[i__] - sum;
	    limits_(&d__1, &d__2, &infi[i__], &di, &ei);
	} else {
	    d__1 = a[i__] - sum;
	    di = (d_sign(&c_b103, &d__1) + 1) / 2;
	    d__1 = b[i__] - sum;
	    ei = (d_sign(&c_b103, &d__1) + 1) / 2;
	}
	prod *= ei - di;
    }
    ret_val = prod;
    return ret_val;

/*     Entry point for intialization. */


L_mvnnit:
    ret_val = 0.;

/*     Initialization and computation of covariance Cholesky factor. */

    ncvsrt_(n, &lower[1], &upper[1], &correl[1], &infin[1], y, infis, a, b, 
	    infi, cov, d__, e);
    d1 = *d__;
    e1 = *e;
    if (*n - *infis == 2) {
/* Computing 2nd power */
	d__1 = cov[1];
	*d__ = sqrt(d__1 * d__1 + 1);
	a[1] /= *d__;
	b[1] /= *d__;
	d__1 = cov[1] / *d__;
	*e = bvn_(a, b, infi, &d__1);
	*d__ = 0.;
	++(*infis);
    }
    return ret_val;
} /* mvnfnc_ */

doublereal mvnfnc_(integer *n, doublereal *w)
{
    return mvnfnc_0_(0, n, w, (doublereal *)0, (doublereal *)0, (doublereal *)
	    0, (integer *)0, (integer *)0, (doublereal *)0, (doublereal *)0);
    }

doublereal mvnnit_(integer *n, doublereal *correl, doublereal *lower, 
	doublereal *upper, integer *infin, integer *infis, doublereal *d__, 
	doublereal *e)
{
    return mvnfnc_0_(1, n, (doublereal *)0, correl, lower, upper, infin, 
	    infis, d__, e);
    }

/* Subroutine */ int limits_(doublereal *a, doublereal *b, integer *infin, 
	doublereal *lower, doublereal *upper)
{
    extern doublereal phi_(doublereal *);

    *lower = 0.;
    *upper = 1.;
    if (*infin >= 0) {
	if (*infin != 0) {
	    *lower = phi_(a);
	}
	if (*infin != 1) {
	    *upper = phi_(b);
	}
    }
    return 0;
} /* limits_ */

/* Subroutine */ int ncvsrt_(integer *n, doublereal *lower, doublereal *upper,
	 doublereal *correl, integer *infin, doublereal *y, integer *infis, 
	doublereal *a, doublereal *b, integer *infi, doublereal *cov, 
	doublereal *d__, doublereal *e)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), exp(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal aj, bj;
    static integer ii, ij;
    static doublereal yl, yu, sum, amin, bmin, dmin__, emin;
    static integer jmin;
    static doublereal sumsq, cvdiag;
    extern /* Subroutine */ int rcswap_(integer *, integer *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *), limits_(
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *)
	    ;


/*     Subroutine to sort integration limits. */

    /* Parameter adjustments */
    --cov;
    --infi;
    --b;
    --a;
    --y;
    --infin;
    --correl;
    --upper;
    --lower;

    /* Function Body */
    ij = 0;
    ii = 0;
    *infis = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	infi[i__] = infin[i__];
	if (infi[i__] < 0) {
	    ++(*infis);
	} else {
	    a[i__] = 0.;
	    b[i__] = 0.;
	    if (infi[i__] != 0) {
		a[i__] = lower[i__];
	    }
	    if (infi[i__] != 1) {
		b[i__] = upper[i__];
	    }
	}
	i__2 = i__ - 1;
	for (j = 1; j <= i__2; ++j) {
	    ++ij;
	    ++ii;
	    cov[ij] = correl[ii];
	}
	++ij;
	cov[ij] = 1.;
    }

/*     First move any doubly infinite limits to innermost positions */

    if (*infis < *n) {
	i__1 = *n - *infis + 1;
	for (i__ = *n; i__ >= i__1; --i__) {
	    if (infi[i__] >= 0) {
		i__2 = i__ - 1;
		for (j = 1; j <= i__2; ++j) {
		    if (infi[j] < 0) {
			rcswap_(&j, &i__, &a[1], &b[1], &infi[1], n, &cov[1]);
			goto L10;
		    }
		}
	    }
L10:
	    ;
	}

/*     Sort remaining limits and determine Cholesky decomposition */

	ii = 0;
	i__1 = *n - *infis;
	for (i__ = 1; i__ <= i__1; ++i__) {

/*     Determine the integration limits for variable with minimum */
/*      expected probability and interchange that variable with Ith. */

	    emin = 1.;
	    dmin__ = 0.;
	    jmin = i__;
	    cvdiag = 0.;
	    ij = ii;
	    i__2 = *n - *infis;
	    for (j = i__; j <= i__2; ++j) {
		sum = 0.;
		sumsq = 0.;
		i__3 = i__ - 1;
		for (k = 1; k <= i__3; ++k) {
		    sum += cov[ij + k] * y[k];
/* Computing 2nd power */
		    d__1 = cov[ij + k];
		    sumsq += d__1 * d__1;
		}
		ij += j;
/* Computing MAX */
		d__1 = cov[ij] - sumsq;
		sumsq = sqrt((max(d__1,0.)));
		if (sumsq > 0.) {
		    if (infi[j] != 0) {
			aj = (a[j] - sum) / sumsq;
		    }
		    if (infi[j] != 1) {
			bj = (b[j] - sum) / sumsq;
		    }
		    limits_(&aj, &bj, &infi[j], d__, e);
		    if (emin - dmin__ >= *e - *d__) {
			jmin = j;
			if (infi[j] != 0) {
			    amin = aj;
			}
			if (infi[j] != 1) {
			    bmin = bj;
			}
			dmin__ = *d__;
			emin = *e;
			cvdiag = sumsq;
		    }
		}
	    }
	    if (jmin != i__) {
		rcswap_(&i__, &jmin, &a[1], &b[1], &infi[1], n, &cov[1]);
	    }

/*     Compute Ith column of Cholesky factor. */

	    ij = ii + i__;
	    cov[ij] = cvdiag;
	    i__2 = *n - *infis;
	    for (j = i__ + 1; j <= i__2; ++j) {
		if (cvdiag > 0.) {
		    sum = cov[ij + i__];
		    i__3 = i__ - 1;
		    for (k = 1; k <= i__3; ++k) {
			sum -= cov[ii + k] * cov[ij + k];
		    }
		    cov[ij + i__] = sum / cvdiag;
		} else {
		    cov[ij + i__] = 0.;
		}
		ij += j;
	    }

/*     Compute expected value for Ith integration variable and */
/*     scale Ith covariance matrix row and limits. */

	    if (cvdiag > 0.) {
		if (emin > dmin__ + 1e-8) {
		    yl = 0.;
		    yu = 0.;
		    if (infi[i__] != 0) {
/* Computing 2nd power */
			d__1 = amin;
			yl = -exp(-(d__1 * d__1) / 2) / 2.5066282746310005024;
		    }
		    if (infi[i__] != 1) {
/* Computing 2nd power */
			d__1 = bmin;
			yu = -exp(-(d__1 * d__1) / 2) / 2.5066282746310005024;
		    }
		    y[i__] = (yu - yl) / (emin - dmin__);
		} else {
		    if (infi[i__] == 0) {
			y[i__] = bmin;
		    }
		    if (infi[i__] == 1) {
			y[i__] = amin;
		    }
		    if (infi[i__] == 2) {
			y[i__] = (amin + bmin) / 2;
		    }
		}
		i__2 = i__;
		for (j = 1; j <= i__2; ++j) {
		    ++ii;
		    cov[ii] /= cvdiag;
		}
		if (infi[i__] != 0) {
		    a[i__] /= cvdiag;
		}
		if (infi[i__] != 1) {
		    b[i__] /= cvdiag;
		}
	    } else {
		y[i__] = 0.;
		ii += i__;
	    }
	}
	limits_(&a[1], &b[1], &infi[1], d__, e);
    }
    return 0;
} /* ncvsrt_ */

doublereal condit_(integer *n, doublereal *symin)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal ret_val, d__1;

    /* Local variables */
    static integer i__, j, ii, ij, im;
    static doublereal det, sum, sym[5050], rowmx, rowmxi;
    extern /* Subroutine */ int syminv_(integer *, doublereal *, doublereal *)
	    ;


/*     Computes condition number of symmetric matix in situ */

    /* Parameter adjustments */
    --symin;

    /* Function Body */
    rowmx = 0.;
    ij = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sum = 0.;
	im = (i__ - 2) * (i__ - 1) / 2;
	i__2 = i__ - 1;
	for (j = 1; j <= i__2; ++j) {
	    ++im;
	    sum += (d__1 = symin[im], abs(d__1));
	    ++ij;
	    sym[ij - 1] = symin[im];
	}
	sum += 1;
	++ij;
	sym[ij - 1] = 1.;
	im += i__;
	i__2 = *n - 1;
	for (j = i__; j <= i__2; ++j) {
	    sum += (d__1 = symin[im], abs(d__1));
	    im += j;
	}
	rowmx = max(sum,rowmx);
    }
    syminv_(n, sym, &det);
    rowmxi = 0.;
    ii = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sum = 0.;
	ij = ii;
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    ++ij;
	    sum += (d__1 = sym[ij - 1], abs(d__1));
	}
	i__2 = *n - 1;
	for (j = i__; j <= i__2; ++j) {
	    ij += j;
	    sum += (d__1 = sym[ij - 1], abs(d__1));
	}
	rowmxi = max(sum,rowmxi);
	ii += i__;
    }
    ret_val = rowmx * rowmxi;
    return ret_val;
} /* condit_ */

/* Subroutine */ int syminv_(integer *n, doublereal *lowinv, doublereal *det)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, ii;
    extern /* Subroutine */ int cholpi_(integer *, doublereal *), cholsk_(
	    integer *, doublereal *), cholnv_(integer *, doublereal *);


/*     Computes lower symmetric inverse and determinant in situ */

    /* Parameter adjustments */
    --lowinv;

    /* Function Body */
    cholsk_(n, &lowinv[1]);
    *det = 1.;
    ii = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ii += i__;
	*det *= lowinv[ii];
    }
    *det *= *det;
    cholnv_(n, &lowinv[1]);
    cholpi_(n, &lowinv[1]);
    return 0;
} /* syminv_ */

/* Subroutine */ int cholsk_(integer *n, doublereal *chofac)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal s, t;
    static integer ii, jj;


/*     Computes Choleski factor in situ */

    /* Parameter adjustments */
    --chofac;

    /* Function Body */
    jj = 0;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	ii = jj;
	i__2 = *n;
	for (i__ = j; i__ <= i__2; ++i__) {
	    s = chofac[ii + j];
	    i__3 = j - 1;
	    for (k = 1; k <= i__3; ++k) {
		s -= chofac[ii + k] * chofac[jj + k];
	    }
	    if (i__ == j) {
		t = sqrt((max(s,0.)));
		chofac[ii + j] = t;
	    } else {
		chofac[ii + j] = s / t;
	    }
	    ii += i__;
	}
	jj += j;
    }
    return 0;
} /* cholsk_ */

/* Subroutine */ int cholnv_(integer *n, doublereal *choinv)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k;
    static doublereal s, t;
    static integer ii, jj, kk;


/*     Inverts a lower triangular matrix in situ */

    /* Parameter adjustments */
    --choinv;

    /* Function Body */
    ii = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = 1 / choinv[ii + i__];
	jj = 0;
	i__2 = i__ - 1;
	for (j = 1; j <= i__2; ++j) {
	    s = 0.;
	    jj += j;
	    kk = jj;
	    i__3 = i__ - 1;
	    for (k = j; k <= i__3; ++k) {
		s += choinv[ii + k] * choinv[kk];
		kk += k;
	    }
	    choinv[ii + j] = -s * t;
	}
	ii += i__;
	choinv[ii] = t;
    }
    return 0;
} /* cholnv_ */

/* Subroutine */ int cholpi_(integer *n, doublereal *chopdi)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k;
    static doublereal s;
    static integer ii, jj, kk;


/*     Multiplies Choleski inverse factors in situ */

    /* Parameter adjustments */
    --chopdi;

    /* Function Body */
    ii = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    s = 0.;
	    jj = ii + i__;
	    kk = ii + j;
	    i__3 = *n;
	    for (k = i__; k <= i__3; ++k) {
		s += chopdi[kk] * chopdi[jj];
		jj += k;
		kk += k;
	    }
	    chopdi[ii + j] = s;
	}
	ii += i__;
    }
    return 0;
} /* cholpi_ */

/* Subroutine */ int rcswap_(integer *p, integer *q, doublereal *a, 
	doublereal *b, integer *infin, integer *n, doublereal *c__)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;
    static doublereal t;
    static integer ii, jj;


/*     Swaps rows and columns P and Q in situ. */

    /* Parameter adjustments */
    --c__;
    --infin;
    --b;
    --a;

    /* Function Body */
    t = a[*p];
    a[*p] = a[*q];
    a[*q] = t;
    t = b[*p];
    b[*p] = b[*q];
    b[*q] = t;
    j = infin[*p];
    infin[*p] = infin[*q];
    infin[*q] = j;
    jj = *p * (*p - 1) / 2;
    ii = *q * (*q - 1) / 2;
    t = c__[jj + *p];
    c__[jj + *p] = c__[ii + *q];
    c__[ii + *q] = t;
    i__1 = *p - 1;
    for (j = 1; j <= i__1; ++j) {
	t = c__[jj + j];
	c__[jj + j] = c__[ii + j];
	c__[ii + j] = t;
    }
    jj += *p;
    i__1 = *q - 1;
    for (i__ = *p + 1; i__ <= i__1; ++i__) {
	t = c__[jj + *p];
	c__[jj + *p] = c__[ii + i__];
	c__[ii + i__] = t;
	jj += i__;
    }
    ii += *q;
    i__1 = *n;
    for (i__ = *q + 1; i__ <= i__1; ++i__) {
	t = c__[ii + *p];
	c__[ii + *p] = c__[ii + *q];
	c__[ii + *q] = t;
	ii += i__;
    }
    return 0;
} /* rcswap_ */


doublereal phi_(doublereal *z__)
{
    /* Initialized data */

    static doublereal a[44] = { .610143081923200417926465815756,
	    -.434841272712577471828182820888,.176351193643605501125840298123,
	    -.060710795609249414860051215825,.017712068995694114486147141191,
	    -.004321119385567293818599864968,8.54216676887098678819832055e-4,
	    -1.2715509060916274262889394e-4,1.1248167243671189468847072e-5,
	    3.13063885421820972630152e-7,-2.70988068537762022009086e-7,
	    3.0737622701407688440959e-8,2.515620384817622937314e-9,
	    -1.02892992132031912759e-9,2.9944052119949939363e-11,
	    2.605178968726693629e-11,-2.634839924171969386e-12,
	    -6.43404509890636443e-13,1.12457401801663447e-13,
	    1.7281533389986098e-14,-4.264101694942375e-15,
	    -5.45371977880191e-16,1.58697607761671e-16,2.0899837844334e-17,
	    -5.900526869409e-18,-9.41893387554e-19,2.1497735647e-19,
	    4.6660985008e-20,-7.243011862e-21,-2.387966824e-21,1.91177535e-22,
	    1.20482568e-22,-6.72377e-25,-5.747997e-24,-4.28493e-25,
	    2.44856e-25,4.3793e-26,-8.151e-27,-3.089e-27,9.3e-29,1.74e-28,
	    1.6e-29,-8e-30,-2e-30 };

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    static doublereal b;
    static integer i__;
    static doublereal p, t, bm, bp, xa;


/*     Normal distribution probabilities accurate to 1d-15. */
/*     Reference: J.L. Schonfelder, Math Comp 32(1978), pp 1232-1240. */


    xa = abs(*z__) / 1.414213562373095048801688724209;
    if (xa > 100.) {
	p = 0.;
    } else {
	t = (xa * 8 - 30) / (xa * 4 + 15);
	bm = 0.;
	b = 0.;
	for (i__ = 24; i__ >= 0; --i__) {
	    bp = b;
	    b = bm;
	    bm = t * b - bp + a[i__];
	}
	p = exp(-xa * xa) * (bm - bp) / 4;
	if (*z__ > 0.) {
	    p = 1 - p;
	}
    }
    ret_val = p;
    return ret_val;
} /* phi_ */


doublereal phinv_(doublereal *p)
{
    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);

    /* Local variables */
    static doublereal q, r__;


/* 	ALGORITHM AS241  APPL. STATIST. (1988) VOL. 37, NO. 3 */

/* 	Produces the normal deviate Z corresponding to a given lower */
/* 	tail area of P. */

/* 	The hash sums below are the sums of the mantissas of the */
/* 	coefficients.   They are included for use in checking */
/* 	transcription. */


/*     Coefficients for P close to 0.5 */

/*     HASH SUM AB    55.88319 28806 14901 4439 */

/*     Coefficients for P not close to 0, 0.5 or 1. */

/*     HASH SUM CD    49.33206 50330 16102 89036 */

/* 	Coefficients for P near 0 or 1. */

/*     HASH SUM EF    47.52583 31754 92896 71629 */

    q = (*p * 2 - 1) / 2;
    if (abs(q) <= .425) {
	r__ = .180625 - q * q;
	ret_val = q * (((((((r__ * 2509.0809287301226727 + 
		33430.575583588128105) * r__ + 67265.770927008700853) * r__ + 
		45921.953931549871457) * r__ + 13731.693765509461125) * r__ + 
		1971.5909503065514427) * r__ + 133.14166789178437745) * r__ + 
		3.387132872796366608) / (((((((r__ * 5226.495278852854561 + 
		28729.085735721942674) * r__ + 39307.89580009271061) * r__ + 
		21213.794301586595867) * r__ + 5394.1960214247511077) * r__ + 
		687.1870074920579083) * r__ + 42.313330701600911252) * r__ + 
		1);
    } else {
/* Computing MIN */
	d__1 = *p, d__2 = 1 - *p;
	r__ = min(d__1,d__2);
	if (r__ > 0.) {
	    r__ = sqrt(-log(r__));
	    if (r__ <= 5.) {
		r__ += -1.6;
		ret_val = (((((((r__ * 7.7454501427834140764e-4 + 
			.0227238449892691845833) * r__ + 
			.24178072517745061177) * r__ + 1.27045825245236838258)
			 * r__ + 3.64784832476320460504) * r__ + 
			5.7694972214606914055) * r__ + 4.6303378461565452959) 
			* r__ + 1.42343711074968357734) / (((((((r__ * 
			1.05075007164441684324e-9 + 5.475938084995344946e-4) *
			 r__ + .0151986665636164571966) * r__ + 
			.14810397642748007459) * r__ + .68976733498510000455) 
			* r__ + 1.6763848301838038494) * r__ + 
			2.05319162663775882187) * r__ + 1);
	    } else {
		r__ += -5.;
		ret_val = (((((((r__ * 2.01033439929228813265e-7 + 
			2.71155556874348757815e-5) * r__ + 
			.0012426609473880784386) * r__ + 
			.026532189526576123093) * r__ + .29656057182850489123)
			 * r__ + 1.7848265399172913358) * r__ + 
			5.4637849111641143699) * r__ + 6.6579046435011037772) 
			/ (((((((r__ * 2.04426310338993978564e-15 + 
			1.4215117583164458887e-7) * r__ + 
			1.8463183175100546818e-5) * r__ + 
			7.868691311456132591e-4) * r__ + 
			.0148753612908506148525) * r__ + 
			.13692988092273580531) * r__ + .59983220655588793769) 
			* r__ + 1);
	    }
	} else {
	    ret_val = 9.;
	}
	if (q < 0.) {
	    ret_val = -ret_val;
	}
    }
    return ret_val;
} /* phinv_ */

doublereal bvn_(doublereal *lower, doublereal *upper, integer *infin, 
	doublereal *correl)
{
    /* System generated locals */
    doublereal ret_val, d__1, d__2, d__3, d__4;

    /* Local variables */
    extern doublereal bvnu_(doublereal *, doublereal *, doublereal *);


/*     A function for computing bivariate normal probabilities. */

/*  Parameters */

/*     LOWER  REAL, array of lower integration limits. */
/*     UPPER  REAL, array of upper integration limits. */
/*     INFIN  INTEGER, array of integration limits flags: */
/*            if INFIN(I) = 0, Ith limits are (-infinity, UPPER(I)]; */
/*            if INFIN(I) = 1, Ith limits are [LOWER(I), infinity); */
/*            if INFIN(I) = 2, Ith limits are [LOWER(I), UPPER(I)]. */
/*     CORREL REAL, correlation coefficient. */

    /* Parameter adjustments */
    --infin;
    --upper;
    --lower;

    /* Function Body */
    if (infin[1] == 2 && infin[2] == 2) {
	ret_val = bvnu_(&lower[1], &lower[2], correl) - bvnu_(&upper[1], &
		lower[2], correl) - bvnu_(&lower[1], &upper[2], correl) + 
		bvnu_(&upper[1], &upper[2], correl);
    } else if (infin[1] == 2 && infin[2] == 1) {
	ret_val = bvnu_(&lower[1], &lower[2], correl) - bvnu_(&upper[1], &
		lower[2], correl);
    } else if (infin[1] == 1 && infin[2] == 2) {
	ret_val = bvnu_(&lower[1], &lower[2], correl) - bvnu_(&lower[1], &
		upper[2], correl);
    } else if (infin[1] == 2 && infin[2] == 0) {
	d__1 = -upper[1];
	d__2 = -upper[2];
	d__3 = -lower[1];
	d__4 = -upper[2];
	ret_val = bvnu_(&d__1, &d__2, correl) - bvnu_(&d__3, &d__4, correl);
    } else if (infin[1] == 0 && infin[2] == 2) {
	d__1 = -upper[1];
	d__2 = -upper[2];
	d__3 = -upper[1];
	d__4 = -lower[2];
	ret_val = bvnu_(&d__1, &d__2, correl) - bvnu_(&d__3, &d__4, correl);
    } else if (infin[1] == 1 && infin[2] == 0) {
	d__1 = -upper[2];
	d__2 = -(*correl);
	ret_val = bvnu_(&lower[1], &d__1, &d__2);
    } else if (infin[1] == 0 && infin[2] == 1) {
	d__1 = -upper[1];
	d__2 = -(*correl);
	ret_val = bvnu_(&d__1, &lower[2], &d__2);
    } else if (infin[1] == 1 && infin[2] == 1) {
	ret_val = bvnu_(&lower[1], &lower[2], correl);
    } else if (infin[1] == 0 && infin[2] == 0) {
	d__1 = -upper[1];
	d__2 = -upper[2];
	ret_val = bvnu_(&d__1, &d__2, correl);
    }
    return ret_val;
} /* bvn_ */


doublereal bvnu_(doublereal *dh, doublereal *dk, doublereal *r__)
{
    /* Initialized data */

    static struct {
	doublereal e_1[3];
	doublereal fill_2[7];
	doublereal e_3[6];
	doublereal fill_4[4];
	doublereal e_5[10];
	} equiv_235 = { .1713244923791705, .3607615730481384, 
		.4679139345726904, {0}, .04717533638651177, .1069393259953183,
		 .1600783285433464, .2031674267230659, .2334925365383547, 
		.2491470458134029, {0}, .01761400713915212, 
		.04060142980038694, .06267204833410906, .08327674157670475, 
		.1019301198172404, .1181945319615184, .1316886384491766, 
		.1420961093183821, .1491729864726037, .1527533871307259 };

#define w ((doublereal *)&equiv_235)

    static struct {
	doublereal e_1[3];
	doublereal fill_2[7];
	doublereal e_3[6];
	doublereal fill_4[4];
	doublereal e_5[10];
	} equiv_236 = { -.9324695142031522, -.6612093864662647, 
		-.238619186083197, {0}, -.9815606342467191, -.904117256370475,
		 -.769902674194305, -.5873179542866171, -.3678314989981802, 
		-.1252334085114692, {0}, -.9931285991850949, 
		-.9639719272779138, -.9122344282513259, -.8391169718222188, 
		-.7463319064601508, -.636053680726515, -.5108670019508271, 
		-.3737060887154196, -.2277858511416451, -.07652652113349733 };

#define x ((doublereal *)&equiv_236)


    /* System generated locals */
    integer i__1;
    doublereal ret_val, d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double asin(doublereal), sin(doublereal), exp(doublereal), sqrt(
	    doublereal);

    /* Local variables */
    static doublereal a, b, c__, d__, h__;
    static integer i__;
    static doublereal k;
    static integer lg;
    static doublereal as;
    static integer ng;
    static doublereal bs, hk, hs;
    static integer is;
    static doublereal sn, rs, xs;
    extern doublereal phi_(doublereal *);
    static doublereal bvn, asr;


/*     A function for computing bivariate normal probabilities. */

/*       Alan Genz */
/*       Department of Mathematics */
/*       Washington State University */
/*       Pullman, WA 99164-3113 */
/*       Email : alangenz@wsu.edu */

/*    This function is based on the method described by */
/*        Drezner, Z and G.O. Wesolowsky, (1989), */
/*        On the computation of the bivariate normal inegral, */
/*        Journal of Statist. Comput. Simul. 35, pp. 101-107, */
/*    with major modifications for double precision, and for |R| close to 1. */

/* BVNU - calculate the probability that X is larger than DH and Y is */
/*       larger than DK. */

/* Parameters */

/*   DH  DOUBLE PRECISION, integration limit */
/*   DK  DOUBLE PRECISION, integration limit */
/*   R   DOUBLE PRECISION, correlation coefficient */

/*     Gauss Legendre Points and Weights, N =  6 */
/*     Gauss Legendre Points and Weights, N = 12 */
/*     Gauss Legendre Points and Weights, N = 20 */
    if (abs(*r__) < .3f) {
	ng = 1;
	lg = 3;
    } else if (abs(*r__) < .75f) {
	ng = 2;
	lg = 6;
    } else {
	ng = 3;
	lg = 10;
    }
    h__ = *dh;
    k = *dk;
    hk = h__ * k;
    bvn = 0.;
    if (abs(*r__) < .925f) {
	hs = (h__ * h__ + k * k) / 2;
	asr = asin(*r__);
	i__1 = lg;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    for (is = -1; is <= 1; is += 2) {
		sn = sin(asr * (is * x[i__ + ng * 10 - 11] + 1) / 2);
		bvn += w[i__ + ng * 10 - 11] * exp((sn * hk - hs) / (1 - sn * 
			sn));
	    }
	}
	d__1 = -h__;
	d__2 = -k;
	bvn = bvn * asr / 12.566370614359172 + phi_(&d__1) * phi_(&d__2);
    } else {
	if (*r__ < 0.) {
	    k = -k;
	    hk = -hk;
	}
	if (abs(*r__) < 1.) {
	    as = (1 - *r__) * (*r__ + 1);
	    a = sqrt(as);
/* Computing 2nd power */
	    d__1 = h__ - k;
	    bs = d__1 * d__1;
	    c__ = (4 - hk) / 8;
	    d__ = (12 - hk) / 16;
	    asr = -(bs / as + hk) / 2;
	    if (asr > -100.) {
		bvn = a * exp(asr) * (1 - c__ * (bs - as) * (1 - d__ * bs / 5)
			 / 3 + c__ * d__ * as * as / 5);
	    }
	    if (-hk < 100.) {
		b = sqrt(bs);
		d__1 = -b / a;
		bvn -= exp(-hk / 2) * sqrt(6.283185307179586) * phi_(&d__1) * 
			b * (1 - c__ * bs * (1 - d__ * bs / 5) / 3);
	    }
	    a /= 2;
	    i__1 = lg;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		for (is = -1; is <= 1; is += 2) {
/* Computing 2nd power */
		    d__1 = a * (is * x[i__ + ng * 10 - 11] + 1);
		    xs = d__1 * d__1;
		    rs = sqrt(1 - xs);
		    asr = -(bs / xs + hk) / 2;
		    if (asr > -100.) {
			bvn += a * w[i__ + ng * 10 - 11] * exp(asr) * (exp(
				-hk * (1 - rs) / ((rs + 1) * 2)) / rs - (c__ *
				 xs * (d__ * xs + 1) + 1));
		    }
		}
	    }
	    bvn = -bvn / 6.283185307179586;
	}
	if (*r__ > 0.) {
	    d__1 = -max(h__,k);
	    bvn += phi_(&d__1);
	}
	if (*r__ < 0.) {
/* Computing MAX */
	    d__3 = -h__;
	    d__4 = -k;
	    d__1 = 0., d__2 = phi_(&d__3) - phi_(&d__4);
	    bvn = -bvn + max(d__1,d__2);
	}
    }
    ret_val = bvn;
    return ret_val;
} /* bvnu_ */

#undef x
#undef w



/* Subroutine */ int adapt_(integer *ndim, integer *mincls, integer *maxcls, 
	D_fp functn, doublereal *absreq, doublereal *relreq, integer *lenwrk, 
	doublereal *work, doublereal *absest, doublereal *finest, integer *
	inform__)
{
    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    /* Local variables */
    extern /* Subroutine */ int adbase_(integer *, integer *, integer *, D_fp,
	     doublereal *, doublereal *, doublereal *, doublereal *, integer *
	    , integer *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *);
    static integer inmesh, invals, sbrgns, inwdth, lenrul, inerrs, rulcls, 
	    inmshs, inwork, inlowr, inpnts, inwgts, inuppr, mxrgns, inptrs, 
	    inlwrs, inuprs;


/*   Adaptive Multidimensional Integration Subroutine */

/*   Author: Alan Genz */
/*           Department of Mathematics */
/*           Washington State University */
/*           Pullman, WA 99164-3113 USA */

/*  This subroutine computes an approximation to the integral */

/*      1 1     1 */
/*     I I ... I       FUNCTN(NDIM,X)  dx(NDIM)...dx(2)dx(1) */
/*      0 0     0 */

/* **************  Parameters for ADAPT  ******************************** */

/* ***** Input Parameters */

/*  NDIM    Integer number of integration variables. */
/*  MINCLS  Integer minimum number of FUNCTN calls to be allowed; MINCLS */
/*          must not exceed MAXCLS. If MINCLS < 0, then ADAPT assumes */
/*          that a previous call of ADAPT has been made with the same */
/*          integrand and continues that calculation. */
/*  MAXCLS  Integer maximum number of FUNCTN calls to be used; MAXCLS */
/*          must be >= RULCLS, the number of function calls required for */
/*          one application of the basic integration rule. */
/*           IF ( NDIM .EQ. 1 ) THEN */
/*              RULCLS = 11 */
/*           ELSE IF ( NDIM .LT. 15 ) THEN */
/*              RULCLS = 2**NDIM + 2*NDIM*(NDIM+3) + 1 */
/*           ELSE */
/*              RULCLS = 1 + NDIM*(24-NDIM*(6-NDIM*4))/3 */
/*           ENDIF */
/*  FUNCTN  Externally declared real user defined integrand. Its */
/*          parameters must be (NDIM, Z), where Z is a real array of */
/*          length NDIM. */
/*  ABSREQ  Real required absolute accuracy. */
/*  RELREQ  Real required relative accuracy. */
/*  LENWRK  Integer length of real array WORK (working storage); ADAPT */
/*          needs LENWRK >= 16*NDIM + 27. For maximum efficiency LENWRK */
/*          should be about 2*NDIM*MAXCLS/RULCLS if MAXCLS FUNCTN */
/*          calls are needed. If LENWRK is significantly less than this, */
/*          ADAPT may be less efficient. */

/* ***** Output Parameters */

/*  MINCLS  Actual number of FUNCTN calls used by ADAPT. */
/*  WORK    Real array (length LENWRK) of working storage. This contains */
/*          information that is needed for additional calls of ADAPT */
/*          using the same integrand (input MINCLS < 0). */
/*  ABSEST  Real estimated absolute accuracy. */
/*  FINEST  Real estimated value of integral. */
/*  INFORM  INFORM = 0 for normal exit, when ABSEST <= ABSREQ or */
/*                     ABSEST <= |FINEST|*RELREQ with MINCLS <= MAXCLS. */
/*          INFORM = 1 if MAXCLS was too small for ADAPT to obtain the */
/*                     result FINEST to within the requested accuracy. */
/*          INFORM = 2 if MINCLS > MAXCLS, LENWRK < 16*NDIM + 27 or */
/*                     RULCLS > MAXCLS. */

/* *********************************************************************** */

/*     Begin driver routine. This routine partitions the working storage */
/*      array and then calls the main subroutine ADBASE. */

    /* Parameter adjustments */
    --work;

    /* Function Body */
    if (*ndim == 1) {
	lenrul = 5;
	rulcls = 9;
    } else if (*ndim < 12) {
	lenrul = 6;
	rulcls = pow_ii(&c__2, ndim) + (*ndim << 1) * (*ndim + 2) + 1;
    } else {
	lenrul = 6;
	rulcls = (*ndim << 1) * ((*ndim << 1) + 1) + 1;
    }
    if (*lenwrk >= lenrul * (*ndim + 4) + *ndim * 10 + 3 && rulcls <= *maxcls 
	    && *mincls <= *maxcls) {
	mxrgns = (*lenwrk - lenrul * (*ndim + 4) - *ndim * 7) / (*ndim * 3 + 
		3);
	inerrs = 1;
	invals = inerrs + mxrgns;
	inptrs = invals + mxrgns;
	inlwrs = inptrs + mxrgns;
	inuprs = inlwrs + mxrgns * *ndim;
	inmshs = inuprs + mxrgns * *ndim;
	inwgts = inmshs + mxrgns * *ndim;
	inpnts = inwgts + (lenrul << 2);
	inlowr = inpnts + lenrul * *ndim;
	inuppr = inlowr + *ndim;
	inwdth = inuppr + *ndim;
	inmesh = inwdth + *ndim;
	inwork = inmesh + *ndim;
	if (*mincls < 0) {
	    sbrgns = (integer) work[*lenwrk];
	}
	adbase_(ndim, mincls, maxcls, (D_fp)functn, absreq, relreq, absest, 
		finest, &sbrgns, &mxrgns, &rulcls, &lenrul, &work[inerrs], &
		work[invals], &work[inptrs], &work[inlwrs], &work[inuprs], &
		work[inmshs], &work[inwgts], &work[inpnts], &work[inlowr], &
		work[inuppr], &work[inwdth], &work[inmesh], &work[inwork], 
		inform__);
	work[*lenwrk] = (doublereal) sbrgns;
    } else {
	*inform__ = 2;
	*mincls = rulcls;
    }
    return 0;
} /* adapt_ */

/* Subroutine */ int bsinit_(integer *ndim, doublereal *w, integer *lenrul, 
	doublereal *g)
{
    /* System generated locals */
    integer w_dim1, w_offset, g_dim1, g_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j;
    static doublereal lam1, lam2, lam3, lamp, rulcon;
    extern /* Subroutine */ int rulnrm_(integer *, integer *, integer *, 
	    doublereal *, doublereal *);
    static integer rulpts[6];


/*     For initializing basic rule weights and symmetric sum parameters. */


/*     The following code determines rule parameters and weights for a */
/*      degree 7 rule (W(1,1),...,W(5,1)), two degree 5 comparison rules */
/*      (W(1,2),...,W(5,2) and W(1,3),...,W(5,3)) and a degree 3 */
/*      comparison rule (W(1,4),...W(5,4)). */

/*       If NDIM = 1, then LENRUL = 5 and total points = 9. */
/*       If NDIM < SDIM, then LENRUL = 6 and */
/*                      total points = 1+2*NDIM*(NDIM+2)+2**NDIM. */
/*       If NDIM > = SDIM, then LENRUL = 6 and */
/*                      total points = 1+2*NDIM*(1+2*NDIM). */

    /* Parameter adjustments */
    g_dim1 = *ndim;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    w_dim1 = *lenrul;
    w_offset = 1 + w_dim1;
    w -= w_offset;

    /* Function Body */
    i__1 = *lenrul;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *ndim;
	for (j = 1; j <= i__2; ++j) {
	    g[j + i__ * g_dim1] = 0.;
	}
	for (j = 1; j <= 4; ++j) {
	    w[i__ + j * w_dim1] = 0.;
	}
    }
    rulpts[4] = (*ndim << 1) * (*ndim - 1);
    rulpts[3] = *ndim << 1;
    rulpts[2] = *ndim << 1;
    rulpts[1] = *ndim << 1;
    rulpts[0] = 1;
    lamp = .85f;
    lam3 = .4707f;
    lam2 = 4 / (15 - 5 / lam3);
/* Computing 2nd power */
    d__1 = lam2;
    w[w_dim1 + 5] = (3 - lam3 * 5) / ((lam2 - lam3) * 180 * (d__1 * d__1));
    if (*ndim < 12) {
	lam1 = lam3 * 8 * (lam3 * 31 - 15) / ((lam3 * 3 - 1) * (lam3 * 5 - 3) 
		* 35);
/* Computing 3rd power */
	d__1 = lam3 * 3;
	w[*lenrul + w_dim1] = 1 / (d__1 * (d__1 * d__1)) / pow_ii(&c__2, ndim)
		;
    } else {
	lam1 = (lam3 * (15 - lam2 * 21) + (*ndim - 1) * 35 * (lam2 - lam3) / 
		9) / (lam3 * (21 - lam2 * 35) + (*ndim - 1) * 35 * (lam2 / 
		lam3 - 1) / 9);
/* Computing 3rd power */
	d__1 = lam3 * 3;
	w[w_dim1 + 6] = 1 / (d__1 * (d__1 * d__1) * 4);
    }
    w[w_dim1 + 3] = (15 - (lam3 + lam1) * 21 + lam3 * 35 * lam1) / (lam2 * 210 * (lam2 - lam3) * (lam2 - lam1)) - ((*ndim - 1) << 1) * w[w_dim1 + 5];
    w[w_dim1 + 2] = (15 - (lam3 + lam2) * 21 + lam3 * 35 * lam2) / (lam1 * 210 * (lam1 - lam3) * (lam1 - lam2));
	if (*ndim < 12) {
	rulpts[*lenrul - 1] = pow_ii(&c__2, ndim);
	lam3 = sqrt(lam3);
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    g[i__ + *lenrul * g_dim1] = lam3;
	}
    } else {
/* Computing 3rd power */
	d__1 = lam3 * 3;
	w[w_dim1 + 6] = 1 / (d__1 * (d__1 * d__1) * 4);
	rulpts[5] = (*ndim << 1) * (*ndim - 1);
	lam3 = sqrt(lam3);
	for (i__ = 1; i__ <= 2; ++i__) {
	    g[i__ + g_dim1 * 6] = lam3;
	}
    }
    if (*ndim > 1) {
/* Computing 2nd power */
	d__1 = lam2 * 6;
	w[(w_dim1 << 1) + 5] = 1 / (d__1 * d__1);
/* Computing 2nd power */
	d__1 = lam2 * 6;
	w[w_dim1 * 3 + 5] = 1 / (d__1 * d__1);
    }
    w[(w_dim1 << 1) + 3] = (3 - lam1 * 5) / (lam2 * 30 * (lam2 - lam1)) - ((*ndim - 1) << 1) * w[(w_dim1 << 1) + 5];
    w[(w_dim1 << 1) + 2] = (3 - lam2 * 5) / (lam1 * 30 * (lam1 - lam2));
    w[w_dim1 * 3 + 4] = (3 - lam2 * 5) / (lamp * 30 * (lamp - lam2));
    w[w_dim1 * 3 + 3] = (3 - lamp * 5) / (lam2 * 30 * (lam2 - lamp)) - ((*ndim - 1) << 1) * w[w_dim1 * 3 + 5];
    w[(w_dim1 << 2) + 2] = 1 / (lam1 * 6);
    lamp = sqrt(lamp);
    lam2 = sqrt(lam2);
    lam1 = sqrt(lam1);
    g[(g_dim1 << 1) + 1] = lam1;
    g[g_dim1 * 3 + 1] = lam2;
    g[(g_dim1 << 2) + 1] = lamp;
    if (*ndim > 1) {
	g[g_dim1 * 5 + 1] = lam2;
	g[g_dim1 * 5 + 2] = lam2;
    }
    for (j = 1; j <= 4; ++j) {
	w[j * w_dim1 + 1] = 1.;
	i__1 = *lenrul;
	for (i__ = 2; i__ <= i__1; ++i__) {
	    w[j * w_dim1 + 1] -= rulpts[i__ - 1] * w[i__ + j * w_dim1];
	}
    }
    rulcon = 2.;
    rulnrm_(lenrul, &c__4, rulpts, &w[w_offset], &rulcon);
    return 0;
} /* bsinit_ */

/* Subroutine */ int rulnrm_(integer *lenrul, integer *numnul, integer *
	rulpts, doublereal *w, doublereal *rulcon)
{
    /* System generated locals */
    integer w_dim1, w_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal alpha, normcf, normnl;


/*     Compute orthonormalized null rules. */

    /* Parameter adjustments */
    w_dim1 = *lenrul;
    w_offset = 1 + w_dim1;
    w -= w_offset;
    --rulpts;

    /* Function Body */
    normcf = 0.;
    i__1 = *lenrul;
    for (i__ = 1; i__ <= i__1; ++i__) {
	normcf += rulpts[i__] * w[i__ + w_dim1] * w[i__ + w_dim1];
    }
    i__1 = *numnul;
    for (k = 2; k <= i__1; ++k) {
	i__2 = *lenrul;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    w[i__ + k * w_dim1] -= w[i__ + w_dim1];
	}
	i__2 = k - 1;
	for (j = 2; j <= i__2; ++j) {
	    alpha = 0.;
	    i__3 = *lenrul;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		alpha += rulpts[i__] * w[i__ + j * w_dim1] * w[i__ + k * 
			w_dim1];
	    }
	    alpha = -alpha / normcf;
	    i__3 = *lenrul;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		w[i__ + k * w_dim1] += alpha * w[i__ + j * w_dim1];
	    }
	}
	normnl = 0.;
	i__2 = *lenrul;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    normnl += rulpts[i__] * w[i__ + k * w_dim1] * w[i__ + k * w_dim1];
	}
	alpha = sqrt(normcf / normnl);
	i__2 = *lenrul;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    w[i__ + k * w_dim1] = alpha * w[i__ + k * w_dim1];
	}
    }
    i__1 = *numnul;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *lenrul;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    w[i__ + j * w_dim1] /= *rulcon;
	}
    }
    return 0;
} /* rulnrm_ */

/* Subroutine */ int adbase_(integer *ndim, integer *mincls, integer *maxcls, 
	D_fp functn, doublereal *absreq, doublereal *relreq, doublereal *
	absest, doublereal *finest, integer *sbrgns, integer *mxrgns, integer 
	*rulcls, integer *lenrul, doublereal *errors, doublereal *values, 
	doublereal *pontrs, doublereal *lowers, doublereal *uppers, 
	doublereal *meshes, doublereal *weghts, doublereal *points, 
	doublereal *lower, doublereal *upper, doublereal *width, doublereal *
	mesh, doublereal *work, integer *inform__)
{
    /* System generated locals */
    integer lowers_dim1, lowers_offset, uppers_dim1, uppers_offset, 
	    meshes_dim1, meshes_offset, i__1, i__2;
    doublereal d__1, d__2;

    /* Local variables */
    static integer i__, j, top;
    extern /* Subroutine */ int differ_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, D_fp, integer *, 
	    integer *);
    static integer difcls, divaxn, rgncls, funcls;
    extern /* Subroutine */ int bsinit_(integer *, doublereal *, integer *, 
	    doublereal *), basrul_(integer *, doublereal *, doublereal *, 
	    doublereal *, D_fp, doublereal *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static integer nwrgns;
    extern /* Subroutine */ int trestr_(integer *, integer *, doublereal *, 
	    doublereal *);


/*        Main adaptive integration subroutine */


/*     Initialization of subroutine */

    /* Parameter adjustments */
    meshes_dim1 = *ndim;
    meshes_offset = 1 + meshes_dim1;
    meshes -= meshes_offset;
    uppers_dim1 = *ndim;
    uppers_offset = 1 + uppers_dim1;
    uppers -= uppers_offset;
    lowers_dim1 = *ndim;
    lowers_offset = 1 + lowers_dim1;
    lowers -= lowers_offset;
    --errors;
    --values;
    --pontrs;
    --weghts;
    --points;
    --lower;
    --upper;
    --width;
    --mesh;
    --work;

    /* Function Body */
    *inform__ = 2;
    funcls = 0;
    bsinit_(ndim, &weghts[1], lenrul, &points[1]);
    if (*mincls >= 0) {

/*       When MINCLS >= 0 determine initial subdivision of the */
/*       integration region and apply basic rule to each subregion. */

	*sbrgns = 0;
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    lower[i__] = 0.;
	    mesh[i__] = 1.;
	    width[i__] = 1 / (mesh[i__] * 2);
	    upper[i__] = 1.;
	}
	divaxn = 0;
	rgncls = *rulcls;
	nwrgns = 1;
L10:
	differ_(ndim, &lower[1], &upper[1], &width[1], &work[1], &work[*ndim 
		+ 1], (D_fp)functn, &divaxn, &difcls);
	funcls += difcls;
	if (funcls + rgncls * (mesh[divaxn] + 1) / mesh[divaxn] <= (
		doublereal) (*mincls)) {
	    rgncls = (integer) (rgncls * (mesh[divaxn] + 1) / mesh[divaxn]);
	    nwrgns = (integer) (nwrgns * (mesh[divaxn] + 1) / mesh[divaxn]);
	    ++mesh[divaxn];
	    width[divaxn] = 1 / (mesh[divaxn] * 2);
	    goto L10;
	}
	if (nwrgns <= *mxrgns) {
	    i__1 = *ndim;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		upper[i__] = lower[i__] + width[i__] * 2;
		mesh[i__] = 1.;
	    }
	}

/*     Apply basic rule to subregions and store results in heap. */

L20:
	++(*sbrgns);
	basrul_(ndim, &lower[1], &upper[1], &width[1], (D_fp)functn, &weghts[
		1], lenrul, &points[1], &work[1], &work[*ndim + 1], &errors[*
		sbrgns], &values[*sbrgns]);
	trestr_(sbrgns, sbrgns, &pontrs[1], &errors[1]);
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    lowers[i__ + *sbrgns * lowers_dim1] = lower[i__];
	    uppers[i__ + *sbrgns * uppers_dim1] = upper[i__];
	    meshes[i__ + *sbrgns * meshes_dim1] = mesh[i__];
	}
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    lower[i__] = upper[i__];
	    upper[i__] = lower[i__] + width[i__] * 2;
	    if (lower[i__] + width[i__] < 1.) {
		goto L20;
	    }
	    lower[i__] = 0.;
	    upper[i__] = lower[i__] + width[i__] * 2;
	}
	funcls += *sbrgns * *rulcls;
    }

/*     Check for termination */

L30:
    *finest = 0.;
    *absest = 0.;
    i__1 = *sbrgns;
    for (i__ = 1; i__ <= i__1; ++i__) {
	*finest += values[i__];
	*absest += errors[i__];
    }
/* Computing MAX */
    d__1 = *absreq, d__2 = *relreq * abs(*finest);
    if (*absest > max(d__1,d__2) || funcls < *mincls) {

/*     Prepare to apply basic rule in (parts of) subregion with */
/*     largest error. */

	top = (integer) pontrs[1];
	rgncls = *rulcls;
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    lower[i__] = lowers[i__ + top * lowers_dim1];
	    upper[i__] = uppers[i__ + top * uppers_dim1];
	    mesh[i__] = meshes[i__ + top * meshes_dim1];
	    width[i__] = (upper[i__] - lower[i__]) / (mesh[i__] * 2);
	    rgncls = (integer) (rgncls * mesh[i__]);
	}
	differ_(ndim, &lower[1], &upper[1], &width[1], &work[1], &work[*ndim 
		+ 1], (D_fp)functn, &divaxn, &difcls);
	funcls += difcls;
	rgncls = (integer) (rgncls * (mesh[divaxn] + 1) / mesh[divaxn]);
	if (funcls + rgncls <= *maxcls) {
	    if (*sbrgns + 1 <= *mxrgns) {

/*     Prepare to subdivide into two pieces. */

		nwrgns = 1;
		width[divaxn] /= 2;
	    } else {
		nwrgns = 0;
		width[divaxn] = width[divaxn] * mesh[divaxn] / (mesh[divaxn] 
			+ 1);
		meshes[divaxn + top * meshes_dim1] = mesh[divaxn] + 1;
	    }
	    if (nwrgns > 0) {

/*     Only allow local subdivision when space is available. */

		i__1 = *sbrgns + nwrgns;
		for (j = *sbrgns + 1; j <= i__1; ++j) {
		    i__2 = *ndim;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			lowers[i__ + j * lowers_dim1] = lower[i__];
			uppers[i__ + j * uppers_dim1] = upper[i__];
			meshes[i__ + j * meshes_dim1] = mesh[i__];
		    }
		}
		uppers[divaxn + top * uppers_dim1] = lower[divaxn] + width[
			divaxn] * 2;
		lowers[divaxn + (*sbrgns + 1) * lowers_dim1] = uppers[divaxn 
			+ top * uppers_dim1];
	    }
	    funcls += rgncls;
	    basrul_(ndim, &lowers[top * lowers_dim1 + 1], &uppers[top * 
		    uppers_dim1 + 1], &width[1], (D_fp)functn, &weghts[1], 
		    lenrul, &points[1], &work[1], &work[*ndim + 1], &errors[
		    top], &values[top]);
	    trestr_(&top, sbrgns, &pontrs[1], &errors[1]);
	    i__1 = *sbrgns + nwrgns;
	    for (i__ = *sbrgns + 1; i__ <= i__1; ++i__) {

/*     Apply basic rule and store results in heap. */

		basrul_(ndim, &lowers[i__ * lowers_dim1 + 1], &uppers[i__ * 
			uppers_dim1 + 1], &width[1], (D_fp)functn, &weghts[1],
			 lenrul, &points[1], &work[1], &work[*ndim + 1], &
			errors[i__], &values[i__]);
		trestr_(&i__, &i__, &pontrs[1], &errors[1]);
	    }
	    *sbrgns += nwrgns;
	    goto L30;
	} else {
	    *inform__ = 1;
	}
    } else {
	*inform__ = 0;
    }
    *mincls = funcls;
    return 0;
} /* adbase_ */

/* Subroutine */ int basrul_(integer *ndim, doublereal *a, doublereal *b, 
	doublereal *width, D_fp functn, doublereal *w, integer *lenrul, 
	doublereal *g, doublereal *center, doublereal *z__, doublereal *
	rgnert, doublereal *basest)
{
    /* System generated locals */
    integer w_dim1, w_offset, g_dim1, g_offset, i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal rgncmp, rgnval, rgncpt, rgnerr, rgnvol;
    extern doublereal fulsum_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, D_fp);
    static doublereal fsymsm;


/*     For application of basic integration rule */


/*     Compute Volume and Center of Subregion */

    /* Parameter adjustments */
    --z__;
    --center;
    --width;
    --b;
    --a;
    g_dim1 = *ndim;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    w_dim1 = *lenrul;
    w_offset = 1 + w_dim1;
    w -= w_offset;

    /* Function Body */
    rgnvol = 1.;
    i__1 = *ndim;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rgnvol = rgnvol * 2 * width[i__];
	center[i__] = a[i__] + width[i__];
    }
    *basest = 0.;
    *rgnert = 0.;

/*     Compute basic rule and error */

L10:
    rgnval = 0.;
    rgnerr = 0.;
    rgncmp = 0.;
    rgncpt = 0.;
    i__1 = *lenrul;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fsymsm = fulsum_(ndim, &center[1], &width[1], &z__[1], &g[i__ * 
		g_dim1 + 1], (D_fp)functn);
/*     Basic Rule */
	rgnval += w[i__ + w_dim1] * fsymsm;
/*     First comparison rule */
	rgnerr += w[i__ + (w_dim1 << 1)] * fsymsm;
/*     Second comparison rule */
	rgncmp += w[i__ + w_dim1 * 3] * fsymsm;
/*     Third Comparison rule */
	rgncpt += w[i__ + (w_dim1 << 2)] * fsymsm;
    }

/*     Error estimation */

/* Computing 2nd power */
    d__1 = rgncmp;
/* Computing 2nd power */
    d__2 = rgnerr;
    rgnerr = sqrt(d__1 * d__1 + d__2 * d__2);
/* Computing 2nd power */
    d__1 = rgncpt;
/* Computing 2nd power */
    d__2 = rgncmp;
    rgncmp = sqrt(d__1 * d__1 + d__2 * d__2);
    if (rgnerr * 4 < rgncmp) {
	rgnerr /= 2;
    }
    if (rgnerr * 2 > rgncmp) {
	rgnerr = max(rgnerr,rgncmp);
    }
    *rgnert += rgnvol * rgnerr;
    *basest += rgnvol * rgnval;

/*     When subregion has more than one piece, determine next piece and */
/*      loop back to apply basic rule. */

    i__1 = *ndim;
    for (i__ = 1; i__ <= i__1; ++i__) {
	center[i__] += width[i__] * 2;
	if (center[i__] < b[i__]) {
	    goto L10;
	}
	center[i__] = a[i__] + width[i__];
    }
    return 0;
} /* basrul_ */

doublereal fulsum_(integer *s, doublereal *center, doublereal *hwidth, 
	doublereal *x, doublereal *g, D_fp f)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal ret_val;

    /* Local variables */
    static integer i__, l;
    static doublereal gi, gl;
    static integer ixchng, lxchng;
    static doublereal intsum;


/* ***  To compute fully symmetric basic rule sum */

    /* Parameter adjustments */
    --g;
    --x;
    --hwidth;
    --center;

    /* Function Body */
    ret_val = 0.;

/*     Compute centrally symmetric sum for permutation of G */

L10:
    intsum = 0.;
    i__1 = *s;
    for (i__ = 1; i__ <= i__1; ++i__) {
	x[i__] = center[i__] + g[i__] * hwidth[i__];
    }
L20:
    intsum += (*f)(s, &x[1]);
    i__1 = *s;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__] = -g[i__];
	x[i__] = center[i__] + g[i__] * hwidth[i__];
	if (g[i__] < 0.) {
	    goto L20;
	}
    }
    ret_val += intsum;

/*     Find next distinct permuation of G and loop back for next sum */

    i__1 = *s;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (g[i__ - 1] > g[i__]) {
	    gi = g[i__];
	    ixchng = i__ - 1;
	    i__2 = (i__ - 1) / 2;
	    for (l = 1; l <= i__2; ++l) {
		gl = g[l];
		g[l] = g[i__ - l];
		g[i__ - l] = gl;
		if (gl <= gi) {
		    --ixchng;
		}
		if (g[l] > gi) {
		    lxchng = l;
		}
	    }
	    if (g[ixchng] <= gi) {
		ixchng = lxchng;
	    }
	    g[i__] = g[ixchng];
	    g[ixchng] = gi;
	    goto L10;
	}
    }

/*     End loop for permutations of G and associated sums */

/*     Restore original order to G's */

    i__1 = *s / 2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	gi = g[i__];
	g[i__] = g[*s + 1 - i__];
	g[*s + 1 - i__] = gi;
    }
    return ret_val;
} /* fulsum_ */

/* Subroutine */ int differ_(integer *ndim, doublereal *a, doublereal *b, 
	doublereal *width, doublereal *z__, doublereal *dif, D_fp functn, 
	integer *divaxn, integer *difcls)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    static doublereal frthdf, funcen, widthi;


/*     Compute fourth differences and subdivision axes */

    /* Parameter adjustments */
    --dif;
    --z__;
    --width;
    --b;
    --a;

    /* Function Body */
    *difcls = 0;
    *divaxn = *divaxn % *ndim + 1;
    if (*ndim > 1) {
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    dif[i__] = 0.;
	    z__[i__] = a[i__] + width[i__];
	}
L10:
	funcen = (*functn)(ndim, &z__[1]);
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    widthi = width[i__] / 5;
	    frthdf = funcen * 6;
	    z__[i__] -= widthi * 4;
	    frthdf += (*functn)(ndim, &z__[1]);
	    z__[i__] += widthi * 2;
	    frthdf -= (*functn)(ndim, &z__[1]) * 4;
	    z__[i__] += widthi * 4;
	    frthdf -= (*functn)(ndim, &z__[1]) * 4;
	    z__[i__] += widthi * 2;
	    frthdf += (*functn)(ndim, &z__[1]);
/*     Do not include differences below roundoff */
	    if (funcen + frthdf / 8 != funcen) {
		dif[i__] += abs(frthdf) * width[i__];
	    }
	    z__[i__] -= widthi * 4;
	}
	*difcls = *difcls + (*ndim << 2) + 1;
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    z__[i__] += width[i__] * 2;
	    if (z__[i__] < b[i__]) {
		goto L10;
	    }
	    z__[i__] = a[i__] + width[i__];
	}
	i__1 = *ndim;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (dif[*divaxn] < dif[i__]) {
		*divaxn = i__;
	    }
	}
    }
    return 0;
} /* differ_ */

/* Subroutine */ int trestr_(integer *pointr, integer *sbrgns, doublereal *
	pontrs, doublereal *rgners)
{
    static doublereal rgnerr;
    static integer subrgn, subtmp;

/* ***BEGIN PROLOGUE TRESTR */
/* ***PURPOSE TRESTR maintains a heap for subregions. */
/* ***DESCRIPTION TRESTR maintains a heap for subregions. */
/*            The subregions are ordered according to the size of the */
/*            greatest error estimates of each subregion (RGNERS). */

/*   PARAMETERS */

/*     POINTR Integer. */
/*            The index for the subregion to be inserted in the heap. */
/*     SBRGNS Integer. */
/*            Number of subregions in the heap. */
/*     PONTRS Real array of dimension SBRGNS. */
/*            Used to store the indices for the greatest estimated errors */
/*            for each subregion. */
/*     RGNERS Real array of dimension SBRGNS. */
/*            Used to store the greatest estimated errors for each */
/*            subregion. */

/* ***ROUTINES CALLED NONE */
/* ***END PROLOGUE TRESTR */

/*   Global variables. */


/*   Local variables. */

/*   RGNERR Intermediate storage for the greatest error of a subregion. */
/*   SUBRGN Position of child/parent subregion in the heap. */
/*   SUBTMP Position of parent/child subregion in the heap. */


/* ***FIRST PROCESSING STATEMENT TRESTR */

    /* Parameter adjustments */
    --rgners;
    --pontrs;

    /* Function Body */
    rgnerr = rgners[*pointr];
    if ((doublereal) (*pointr) == pontrs[1]) {

/*        Move the new subregion inserted at the top of the heap */
/*        to its correct position in the heap. */

	subrgn = 1;
L10:
	subtmp = subrgn << 1;
	if (subtmp <= *sbrgns) {
	    if (subtmp != *sbrgns) {

/*              Find maximum of left and right child. */

		if (rgners[(integer) pontrs[subtmp]] < rgners[(integer) 
			pontrs[subtmp + 1]]) {
		    ++subtmp;
		}
	    }

/*           Compare maximum child with parent. */
/*           If parent is maximum, then done. */

	    if (rgnerr < rgners[(integer) pontrs[subtmp]]) {

/*              Move the pointer at position subtmp up the heap. */

		pontrs[subrgn] = pontrs[subtmp];
		subrgn = subtmp;
		goto L10;
	    }
	}
    } else {

/*        Insert new subregion in the heap. */

	subrgn = *sbrgns;
L20:
	subtmp = subrgn / 2;
	if (subtmp >= 1) {

/*           Compare child with parent. If parent is maximum, then done. */

	    if (rgnerr > rgners[(integer) pontrs[subtmp]]) {

/*              Move the pointer at position subtmp down the heap. */

		pontrs[subrgn] = pontrs[subtmp];
		subrgn = subtmp;
		goto L20;
	    }
	}
    }
    pontrs[subrgn] = (doublereal) (*pointr);

/* ***END TRESTR */

    return 0;
} /* trestr_ */

/* Main program alias */ int tstnrm_ () { MAIN__ (); return 0; }
