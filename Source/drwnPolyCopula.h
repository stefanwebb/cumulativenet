/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnPolyCopula.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

class drwnPolyCopula : public drwnCdf	//: public drwnStdObjIface 
{
public:
	//! Domain type is double
	typedef double domain_type;

	//! Range type is double
	typedef double range_type;

	//! Whether copula produced by transformation method
	// TODO: Can make this better with a clever metaprogramming template trick?
	static bool isTransformCopula() { return false; }

	//! Identity function
	static domain_type transformArgument(domain_type value) { return value; }

	//! Identity function
	static domain_type derivativeArgument(domain_type value) { return value; }
	
	//! Return type
    virtual const char *type() const { return "drwnPolyCopula"; }

	//! Make copy
    virtual drwnPolyCopula *clone() const { return new drwnPolyCopula(*this); }
	
	//! Calculate a partial derivative of copula, evalued at parameters
	// TODO: Make return a type declared in class, so will be able to used with templates
	range_type partialDerivative(domain_type* sample, drwnSubset& isDiffed) const;
};