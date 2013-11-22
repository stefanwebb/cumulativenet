/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCdf.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

class drwnCdf	//: public drwnStdObjIface 
{
protected:
	//! List of variables in factor (converts from factor index to universe index)
    vector<int> _variables;

	//! Index of variable in factor (converts from universe index to factor index)
    map<int, int> _varIndex;

	unsigned int _level;

public:
    //! Create an empty copula
    drwnCdf();

	//! Create copula with given variables
	drwnCdf(const drwnClique& c);

    //! Copy constructor
    drwnCdf(const drwnCdf& f);

	//! Default destructor
    virtual ~drwnCdf();

	//! Return type
    virtual const char *type() const { return "drwnCdf"; }

    //! Make copy
    virtual drwnCdf *clone() const { return new drwnCdf(*this); }

    //! True if the factor contains no variables
    bool empty() const { return _variables.empty(); }

    //! Number of variables in this factor
    size_t size() const { return _variables.size(); }

	unsigned int& level() { return _level; }

    //! Number of bytes comsumed by the factor data (excludes variables lists, etc)
    //virtual size_t memory() const { return 0; }

    //! Returns true if the variable is included in this factor
    bool hasVariable(int var) const { return (_varIndex.find(var) != _varIndex.end()); }

    //! Returns variable at given index
    int varId(int indx) const { return _variables[indx]; }

	//! Add multiple variables
    void setVariables(const drwnClique& c);
    
    //! Returns the ordered set of variables over which this factor is defined
	// TODO: Change this to vars() to make consistent with rest of interface
    const vector<int>& getVars() const { return _variables; }

    //! Returns the set of variables over which this factor is defined
    drwnClique getClique() const { return drwnClique(_variables.begin(), _variables.end()); }

};