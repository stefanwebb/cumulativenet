/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCdf.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

// Create an empty copula
drwnCdf::drwnCdf()
{
	// Do nothing
}

// Create copula with given variables
drwnCdf::drwnCdf(const drwnClique& c)
{
	setVariables(c);
}

// Copy constructor
drwnCdf::drwnCdf(const drwnCdf& f)
	: _variables(f._variables), _varIndex(f._varIndex)
{
	// Do nothing
}

// Default destructor
drwnCdf::~drwnCdf()
{
	// Do nothing
}

// Add multiple variables
void drwnCdf::setVariables(const drwnClique& c)
{
	// Copy variables
	_variables.reserve(c.size());
	_variables.assign(c.begin(), c.end());
	
	// Build variable index map
	for (unsigned int i = 0; i < _variables.size(); i++) {
		_varIndex.insert(pair<int, int>(_variables[i], i));
	}
}