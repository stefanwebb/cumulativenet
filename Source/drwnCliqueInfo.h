/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCliqueInfo.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

//! Precalculated information about each clique that is used by inference algorithm
// (this way only call new once for all members, and decrease cache mishits)
template <class G>
class drwnCliqueInfo
{
public:
	//! Factors in clique
	vector<int> _factors;

	//! Variables in clique
	vector<int> _vars;

	//! Child of clique, when passing messages in order that tree was built
	int _child;

	//! Variables in sepset
	vector<int> _sepset;

	//! Parents of clique, when passing messages in order that tree was built
	vector<int> _parents;

	//! First entry in _messages for final messages from i to j for this clique i
	unsigned int _forwardMessageIndex;

	//! First entry in _messages for final messages from j to i for this clique i
	unsigned int _backwardMessageIndex;

	//! First entry in _messages for temporary messages for this clique
	// Remove once only having 1 temporary messages structure
	//unsigned int _tempIndex;

	//! First entry in _scopes for this clique
	unsigned int _scopeIndex;

	//! Clique subset (relative to clique variables)
	drwnSubset _subset;

	//! Sepset subset (relative to clique variables) for clique and child
	drwnSubset _sepsetSubset, _sepsetSubsetChild;
	
	//! Subset of clique variables minus sepset variables for clique and child
	drwnSubset _minusSepset, _minusSepsetChild;

	//! Number of sepset subsets
	//unsigned int _countSepsetSubsets;

	//! Map for converting from universe variables to clique variables
	map<int, int> _index;

	//! Number of temporary derivative terms
	unsigned int _countDerivTerms;

	vector<int> _factorIndices;
	vector<int> _derivIndices;

	//! Map from parents to parent number
	//map<int, int> _parentIndex;
};
