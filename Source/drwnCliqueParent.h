/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCliqueParent.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

template <class G> class drwnCliqueTree;

template <class G>
class drwnCliqueParent
{
protected:
	drwnCliqueTree<G>& _ct;

	int _i;

	int _j;

	unsigned int _k;

	unsigned int _numNeighbours;

public:
	drwnCliqueParent(drwnCliqueTree<G>& ct, int i, int j, unsigned int k);

	drwnCliqueParent<G>& operator++ ();

	drwnCliqueParent<G>& operator++ (int);

	bool operator==(const drwnCliqueParent<G>&) const;

	bool operator!=(const drwnCliqueParent<G>&) const;

	int operator*();

	void skipChild();
	
};

template <class G>
drwnCliqueParent<G>::drwnCliqueParent(drwnCliqueTree<G>& ct, int i, int j, unsigned int k)
	: _ct(ct)
{
	_i = i;
	_j = j;
	_k = k;
	_numNeighbours = _ct.parents(_i).size() + (_ct.hasChild(_i) ? 1 : 0);
	skipChild();
	DRWN_ASSERT_MSG(_k <= _numNeighbours, "Starting point past last parent!");
}

template <class G>
drwnCliqueParent<G>& drwnCliqueParent<G>::operator++ () 
{ 
	if (_k < _numNeighbours)
		_k++;
	else
		DRWN_ASSERT_MSG(false, "Can't increment parent iterator past end.");
	skipChild();

	return *this; 
}

template <class G>
drwnCliqueParent<G>& drwnCliqueParent<G>::operator++ (int op)
{ 
	return operator++();
}

template <class G>
int drwnCliqueParent<G>::operator*()
{
	int parent;
	if (_k < _ct.parents(_i).size()) {
		parent = _ct.parents(_i)[_k];
		if (parent == _j) {
			if (_k + 1 == _ct.parents(_i).size())
				parent = _ct.child(_i);
			else
				parent = _ct.parents(_i)[_k + 1];
		}
		else
			parent = _ct.parents(_i)[_k];
	}
	else {
		DRWN_ASSERT_MSG(_k < _numNeighbours, "Can't dereference parent iterator past end.");
		parent = _ct.child(_i);
	}
	return parent;
}

template <class G>
bool drwnCliqueParent<G>::operator==(const drwnCliqueParent<G>& op) const
{
	return &op._ct == &_ct && op._k == _k;
}

template <class G>
bool drwnCliqueParent<G>::operator!=(const drwnCliqueParent<G>& op) const
{
	return !operator==(op);
}

template <class G>
void drwnCliqueParent<G>::skipChild()
{
	if (_k < _ct.parents(_i).size()) {
		if (_ct.parents(_i)[_k] == _j) {
			_k++;
		}
	}
	else {
		if (_k < _numNeighbours && _ct.child(_i) == _j)
			_k++;
	}
}