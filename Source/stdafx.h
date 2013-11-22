/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    stdafx.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#define EIGEN2_SUPPORT

// Need to define Gdi+ version so we have advanced features
#define GDIPVER 0x0110

// DEBUG: Disable asserts since drwnOptimizer doesn't work properly
//#define NDEBUG
//#define EIGEN_NO_STATIC_ASSERT 

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <array>

#include <windows.h>
#include <ddraw.h>
#include <boost/math/distributions/normal.hpp>
#include <boost/type_traits.hpp>
#include <boost/random.hpp>
#include <cmath>
//#include <Core>

#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <queue>
#include <gdiplus.h>
#include <limits>
#include <math.h>

typedef std::set<int> drwnClique;

using namespace boost::math::policies;

// Define a policy:
typedef policy<
	boost::math::policies::domain_error<errno_on_error>, 
	boost::math::policies::pole_error<errno_on_error>,
	boost::math::policies::overflow_error<errno_on_error>,
	boost::math::policies::evaluation_error<errno_on_error> 
      > my_policy;

typedef boost::math::normal_distribution<double, my_policy> mynormal;

//#include "../src/lib/base/drwnTriplet.h"
//#include "../src/lib/pgm/drwnVarUniverse.h"
#include "drwnVarUniverse.h"

#define DRWN_ASSERT_MSG(a, b) { if (!(a)) {cout << b; exit(0); } }
#define DRWN_ASSERT(a) { if (!(a)) {cout << "Error!"; exit(0); } }
#define DRWN_LOG_MESSAGE(a) { cout << a << endl; }
#define DRWN_LOG_ERROR(a) { cout << a << endl; }
#define DRWN_LOG(a, b) { cout << b; exit(0); }
#define DRWN_TODO { cout << "not implemented yet!"; exit(0); }

#include "drwnGdiPlus.h"
#include "drwnNumericalDerivative.h"
#include "drwnAgent.h"
#include "drwnNodes.h"
#include "drwnSubset.h"
#include "drwnCdf.h"
#include "drwnNormCopula.h"
#include "drwnPolyCopula.h"
#include "drwnGumbelCopula.h"
#include "drwnFrankCopula.h"
#include "drwnClaytonCopula.h"
#include "drwnNormMarginal.h"
#include "drwnBernoulliMarginal.h"
#include "drwnExponentialMarginal.h"
#include "drwnPixelMarginal.h"
#include "drwnCumulativeNet.h"

#include "drwnSamples.h"
#include "drwnCopulaSamples.h"
#include "drwnCliqueInfo.h"
#include "drwnCliqueParent.h"

#include "drwnCliqueTree.h"
#include "drwnSampleCliques.h"

#include "drwnCumulativeUtils.h"
#include "drwnCumulativeDebug.h"

#include "drwnBinaryImageDataset.h"
#include "drwnWindow.h"