/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    mvnpack.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include "f2c.h"

extern "C" int sadmvn_(integer *n, doublereal *lower, doublereal *upper, integer *infin, doublereal *correl, integer *maxpts, doublereal *abseps, doublereal *releps, doublereal *error, doublereal *value, integer *inform__);