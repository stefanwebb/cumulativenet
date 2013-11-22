/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnSubset.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

class drwnSubset
{
public:
	// Up to 32 elements. 1 denotes presence of element, 0 absence.
	unsigned int _subset;

	// Constructors
	drwnSubset() { _subset = 0; }

	drwnSubset(unsigned int subset) { _subset = subset; }

	drwnSubset(const drwnSubset& A) { _subset = A._subset;}

	// Overloaded operators
	drwnSubset& operator=(const drwnSubset& A) { _subset = A._subset; return *this; }

	drwnSubset& operator=(unsigned int subset) { _subset = subset; return *this; }

	drwnSubset& operator++() { _subset++; }

	bool operator==(const drwnSubset& B) { return this->_subset == B._subset; }

	bool operator!=(const drwnSubset& B) { return this->_subset != B._subset; }

	// Set intersection A ^ B
	static drwnSubset setIntersection(const drwnSubset& A, const drwnSubset& B) { return drwnSubset(A._subset & B._subset);}

	// Set union A v B
	static drwnSubset setUnion(const drwnSubset& A, const drwnSubset& B) { return drwnSubset(A._subset | B._subset);}

	// Set difference A \ B
	static drwnSubset setMinus(const drwnSubset& A, const drwnSubset& B) { return drwnSubset(A._subset & ~B._subset); }

	// Test whether A is emptyset
	static bool isEmptyset(const drwnSubset& A) { return !A._subset; }

	// Test whether A is a subset of B
	static bool isSubset(const drwnSubset& A, const drwnSubset& B) { return !(A._subset & ~B._subset); }

	// Count number of elements (bits)
	static unsigned int countElements(const drwnSubset& A)
	{
		unsigned int i = A._subset - ((A._subset >> 1) & 0x55555555);
		i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
		return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}

	// Get next set with same number of bits. Used for iterating over all sets of same cardinality.
	/*static drwnSubset snoob(drwnSubset x) {
		unsigned int smallest, ripple, ones;
		smallest = x._subset & -x._subset;
		ripple = x._subset + smallest;
		ones = x._subset ^ ripple;
		ones = (ones >> 2) / smallest;
		return drwnSubset(ripple | ones);
	}*/

	// Get next subset of A with respect to B
	static drwnSubset nextSubset(const drwnSubset& A, const drwnSubset& B) { return drwnSubset((A._subset - B._subset) & B._subset); }

	// Convert subset A to an index with respect to B. Alternatively, collapse bits of A according to bitmask B.
	static unsigned int whatSubset(const drwnSubset& A, const drwnSubset& B)
	{
		unsigned int howManyBits = countElements(B), result = 0;
		for (unsigned int i = 0, k = 0; i < howManyBits; i++, k++) {
			// Loop to next set bit of B
			while(!(B._subset & (1 << k)))
				k++;

			// Set correct bit of result
			result |= (A._subset & (1 << k)) >> (k - i);
		}
		return result;
	}

	// Convert array (can be vector or set) to subset
	template<class S>
	static drwnSubset makeSubset(const S& set)
	{
		unsigned int result = 0;

		// Loop over elements in set
		for (S::const_iterator mi = set.begin(); mi != set.end(); mi++)
			result |= 1 << *mi;

		return drwnSubset(result);
	}

	// Convert array (can be vector or set) and index to subset
	template<class S>
	static drwnSubset makeSubset(const S& set, const map<int, int>& varIndex)
	{
		unsigned int result = 0;

		// Loop over elements in set
		for (S::const_iterator mi = set.begin(); mi != set.end(); mi++) {
			map<int, int>::const_iterator elem = varIndex.find(*mi);
			if (elem != varIndex.end())
				result |= 1 << elem->second;
		}

		return drwnSubset(result);
	}

	// Extract bits of A according to bitmask and map
	template<class S>
	static drwnSubset extractSubset(const drwnSubset& A, const S& set, const map<int, int>& varIndex) {
		unsigned int result = 0, i = 0;
		for (S::const_iterator mi = set.begin(); mi != set.end(); i++, mi++) {
			// If factor variable is set to be differentiated, then set bit
			map<int, int>::const_iterator elem = varIndex.find(*mi);
			if (elem != varIndex.end()) {
				if (!drwnSubset::isEmptyset(drwnSubset::setIntersection(A, drwnSubset(1 << elem->second))))
					result |= 1 << i;
			}
		}
		return drwnSubset(result);
	}

	// Print set as numbers
	// This doesn't strictly have to do with drwnSubsets
	template<class S>
	static void printSubset(const S& set)
	{
		// Take care of empty set
		if (!(set.size())) {
			wcout << "empty";
			return;
		}

		// Otherwise print out
		S::const_iterator element = set.begin();
		wcout << *(element++);
		for (; element != set.end(); element++)
			wcout << " " << *element;

	}

	// Print first n elements of set
	// Print set as numbers
	// This doesn't strictly have to do with drwnSubsets
	template<class S>
	static void printSubset(const S& set, unsigned int n)
	{
		// Take care of empty set
		if (!(set.size())) {
			wcout << "empty";
			return;
		}

		// Otherwise print out
		S::const_iterator element = set.begin();
		wcout << *(element++);
		for (unsigned int i = 0; (element != set.end()) && (i < n); element++, i++)
			wcout << " " << *element;

	}

	// Print variables with a set for converting between subset indices and universe indices
	template<class S>
	static void printSubset(const drwnSubset& A, const S& set, const drwnVarUniversePtr& u)
	{
		// Take care of empty subset
		if (drwnSubset::isEmptyset(A)) {
			wcout << "empty";
			return;
		}

		// If not empty subset then loop through bits
		unsigned int howManyBits = countElements(A);
		S::const_iterator mi = set.begin();
		for (unsigned int i = 0, k = 0; i < howManyBits; i++, k++, mi++) {
			// Loop to next set bit of B
			while(!(A._subset & (1 << k))) {
				k++;
				mi++;
			}

			// Print variable
			wcout << u->varName(*mi).c_str();
			if (i != howManyBits - 1)
				wcout << " ";
		}
	}

	// Print variables with a set for convert between subset indices and universe indices
	template<class S>
	static void printSubset(const drwnSubset& A, const S& set)
	{
		// Take care of empty subset
		if (drwnSubset::isEmptyset(A)) {
			wcout << "empty";
			return;
		}

		// If not empty subset then loop through bits
		unsigned int howManyBits = countElements(A);
		S::const_iterator mi = set.begin();
		for (unsigned int i = 0, k = 0; i < howManyBits; i++, k++, mi++) {
			// Loop to next set bit of B
			while(!(A._subset & (1 << k))) {
				k++;
				mi++;
			}

			// Print variable
			wcout << *mi;
			if (i != howManyBits - 1)
				wcout << " ";
		}
	}
};