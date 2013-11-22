/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnNodes.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

template <class T> class drwnNodes : public vector<T>
{
private:
	priority_queue<int, vector<int>, std::greater<int> > _freeNodes;

public:
	// Default constructor
	drwnNodes();

	// Insert element
	void insert(const T& t);

	// Remove element
	void remove(int idx);

	// Count how many elements
	//int size() const;
};

// Default constructor
template<class T>
drwnNodes<T>::drwnNodes() : vector<T>()
{
}

// Insert element
template<class T>
void drwnNodes<T>::insert(const T& t)
{
	// If no free nodes then push to back
	if (!_freeNodes.size())
		vector<T>::push_back(t);

	// Otherwise use a free node
	else {
		(*this)[_freeNodes.top()] = t;
		_freeNodes.pop();
	}
}

// Remove element
template<class T>
void drwnNodes<T>::remove(int idx)
{
	// If it's last element then no need to add to free nodes
	if (idx + 1 == vector<T>::size()) {
		pop_back();
	}

	// Otherwise, add to free nodes and zero
	else {
		_freeNodes.push(idx);
		(*this)[idx] = 0;
	}
}

// Count how many elements
/*template<class T>
int drwnNodes<T>::size() const
{
	return vector<T>::size() - _freeNodes.size();
}*/