/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCumulativeNet.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

#undef max

// DEBUG: Forward declaration of files to output sampling algorithm values
/*namespace drwnCumulativeUtils {
	extern wofstream parameters;
}*/

template<class C, class M>
class drwnCumulativeNet //: public drwnStdObjIface
{
protected:
	//! Variable universe
	drwnVarUniversePtr _pUniverse;

	//! Array of copula factors.
	drwnNodes<C *> _factors;

	//! Array of marginal densities
	vector<M> _marginals;
	
	//! Neighbors of variables. Used to find what/how many factors a variable is in the scope of.
	vector<set<int> > _neighbors;

	string _modelName;

	unsigned int _size;
	
public:
	//! Type of factor
	typedef C factor_type;

	//! Type of marginal
	typedef M marginal_type;

	//! Default constructor
	drwnCumulativeNet();

	//! Construct an empty Cumulative Distribution Network with a given variable universe
	drwnCumulativeNet(const drwnVarUniversePtr& u);

	//! Construct a copy of a Cumulative Distribution Network from an existing one
	drwnCumulativeNet(const drwnCumulativeNet<C, M>& g);

	//! Destructor, will delete factors
	~drwnCumulativeNet();

	string& name() { return _modelName; }

	unsigned int& size() { return _size; }

	//! Return type
    virtual const char *type() const { return "drwnCumulativeNet"; }

	//! Make copy of cumulative network from existing one
    virtual drwnCumulativeNet *clone() const { return new drwnCumulativeNet(*this); }

	//! Resize vectors to size of variable universe
	void initialize();

	//! Get copula factor
	C& factor(int i) const { return *_factors[i]; }

	//! Get marginal density
	M& marginal(int i) const { return  (M&) _marginals[i]; }

	//! Get the neighbors of a factor
	const vector<int>& factorNeighbors(int factorIndex) const { return _factors[factorIndex]->getVars(); }

	//! Get the neighbors of a variable (i.e. factors of which variable is in scope)
	const set<int>& varNeighbors(int varIndex) const { return _neighbors[varIndex]; }

	//! Determine how many factors of which a variable is in the scope.
	int howManyScopes(int varIndex) const { return ((unsigned int) varIndex >= _neighbors.size()) ? 0 : _neighbors[varIndex].size(); }

	//! Add factor to graph and take ownership
	void addFactor(C* factor);

	//! Delete factor from graph and free memory
	void removeFactor(int factorIndex);

	//! Get max level
	unsigned int maxLevel()
	{
		unsigned int maxL = 0;
		for (unsigned int i = 0; i < numFactors(); i++) {
			if (factor(i).level() > maxL)
				maxL = factor(i).level();
		}
		return maxL;
	}

	//! Get min level
	unsigned int minLevel()
	{
		unsigned int minL = std::numeric_limits<unsigned int>::max();
		for (unsigned int i = 0; i < numFactors(); i++) {
			if (factor(i).level() < minL)
				minL = factor(i).level();
		}
		return minL;
	}

	//! Set all levels to given value
	void setLevel(unsigned int level, typename C::parameter_type par)
	{
		for (unsigned int i = 0; i < numFactors(); i++) {
			if (factor(i).level() == level)
				factor(i).setParameter(par);
		}
	}

	void saveFactors(const wchar_t* filename, bool saveLevels = false)
	{
		// Open file for serializing shape model
		wofstream model;
		model.open(filename);
		model.precision(std::numeric_limits<double>::digits10);

		// Save number of factors / parameters
		model << numFactors() << L" ";

		// Save factors
		for (unsigned int i = 0; i < numFactors(); i++) {
			model << factor(i).size() << L" ";
			for	(auto& v : factor(i).getVars())
				model << v << L" ";
		}
	
		// Save parameters
		for (unsigned int i = 0; i < numFactors(); i++) {
			model << factor(i).getParameter() << L" ";
		}

		// Save levels
		if (saveLevels) {
			for (unsigned int i = 0; i < numFactors(); i++)
				model << factor(i).level() << L" ";
		}

		model.close();
	}

	void saveMarginals(wchar_t* filename)
	{
		// Open file for serializing shape model
		wofstream model;
		model.open(filename);
		model.precision(std::numeric_limits<double>::digits10);

		// Save number of variables
		model << numVariables() << L" ";
	
		// Save parameters
		for (unsigned int i = 0; i < numVariables(); i++) {
			// NOTE: Only works if normal/pixel marginals
			model << marginal(i).getMean() << L" " << marginal(i).getStandardDeviation() << L" ";
		}

		model.close();
	}

	//! Count number of elements in factor list (include null factors)
	int numFactors() const { return _factors.size(); }

	//! Return the universe of variables for this factor graph
	// TODO: Replace with universe() to make consistent with rest of interface!
    const drwnVarUniversePtr& getUniverse() const { return _pUniverse; }

    //! Return the number of variables in the universe
    int numVariables() const { return (_pUniverse == NULL) ? 0 : _pUniverse->numVariables(); }

	//! Set parameters of factors as random values
	void randomizeParameters()
	{
		for (unsigned int i = 0; i < numFactors(); i++)
			factor(i).randomizeParameter();
	}

	//! Print out current parameters
	void printParameters()
	{
		// DEBUG: Print to file so we can use in R
		/*wofstream parameters;
		parameters.open(L"parameters.txt");
		parameters.precision(std::numeric_limits<double>::digits10);
		for (unsigned int i = 0; i < numFactors(); i++)
			parameters << "rho" << i << " ";
		parameters << endl;*/

		wcout << "current parameters ";
		for (unsigned int i = 0; i < numFactors(); i++) {
			wcout << factor(i).getParameter() << " ";
			//parameters << factor(i).getParameter() << " ";
		}
		wcout << endl;
		//parameters << endl;
		//parameters.close();
	}

	//! Print out current parameters to pre-opened file
	void printParameters(wofstream& file, bool printEndL = true)
	{
		for (unsigned int i = 0; i < numFactors(); i++)
			file << factor(i).getParameter() << " ";
		if (printEndL)
			file << endl;
	}

	//! Print out current parameters to string
	void printParameters(string& pars)
	{
		//parameters.precision(std::numeric_limits<double>::digits10);
		stringstream ss;
		ss.precision(std::numeric_limits<double>::digits10);
		for (unsigned int i = 0; i < numFactors(); i++) {
			ss << factor(i).getParameter();
			if (i < numFactors() - 1)
				ss << " ";
			//pars.append(std::to_string(factor(i).getParameter()) + string(" "));
		}
		pars.append(ss.str());
	}

	//! Resets parameters of marginals to defaults
	void resetMarginals()
	{
		for (unsigned int i = 0; i < numVariables(); i++)
			marginal(i).reset();
	}

	//! Print the factors and what variables are in their scope. Useful to debug functions to construct types of test networks.
	void printFactors()
	{
		for (unsigned int i = 0; i < numFactors(); i++) {
			wcout << "factor " << i << endl;
			C& f = factor(i);
			const vector<int>& factorVars = f.getVars();

			for (vector<int>::const_iterator it = factorVars.begin(); it < factorVars.end(); it++)
				wcout << getUniverse()->varName(*it).c_str() << " ";
			wcout << endl;
		}
	}

	//! Cache all factors
	void cacheFactors(drwnSamples<drwnCumulativeNet<C, M>>& samples, bool logSpace = false, bool gradAlso = false, bool printDerivatives = false) {
		// Cache values of all factors
		for (unsigned int j = 0; j < numFactors(); j++) {
			if (!C::isTransformCopula())
				factor(j).cacheDerivatives(samples.copula(), gradAlso, logSpace, printDerivatives);
			else
				factor(j).cacheDerivatives(samples.temp(), gradAlso, logSpace, printDerivatives);
		}
	}

	////! Save to XML node
	//bool save(drwnXMLNode& xml) const;

	////! Load from XML node
    //bool load(drwnXMLNode& xml);

};

//! A cumulative network with normal copulas and normal marginals
typedef drwnCumulativeNet<drwnNormCopula, drwnNormMarginal> normalCumulativeNet;

//! The debug cumulative network type, which has a polynomial function for the "copula"
typedef drwnCumulativeNet<drwnPolyCopula, drwnNormMarginal> debugCumulativeNet;

//! A cumulative network with normal copulas and bernoulli marginals
typedef drwnCumulativeNet<drwnNormCopula, drwnBernoulliMarginal> bernoulliCumulativeNet;

//! A cumulative network with normal copulas and exponential marginals
typedef drwnCumulativeNet<drwnNormCopula, drwnExponentialMarginal> exponentialCumulativeNet;

//! A cumulative network with gumbel copulas and normal marginals
typedef drwnCumulativeNet<drwnGumbelCopula, drwnNormMarginal> gumbelCumulativeNet;

//! A cumulative network with gumbel copulas and normal marginals
typedef drwnCumulativeNet<drwnClaytonCopula, drwnNormMarginal> claytonCumulativeNet;

//! A cumulative network with frank copulas and normal marginals
typedef drwnCumulativeNet<drwnFrankCopula, drwnNormMarginal> frankCumulativeNet;

// Default constructor
template<class C, class M>
drwnCumulativeNet<C, M>::drwnCumulativeNet() : _pUniverse(NULL)
{
	initialize();
}

// Construct an empty Cumulative Distribution Network with a given variable universe
template<class C, class M>
drwnCumulativeNet<C, M>::drwnCumulativeNet(const drwnVarUniversePtr& u) : _pUniverse(u)
{
	initialize();
}

// Construct a copy of a Cumulative Distribution Network from an existing one
template<class C, class M>
drwnCumulativeNet<C, M>::drwnCumulativeNet(const drwnCumulativeNet<C, M>& g) :
_pUniverse(g._pUniverse), _factors(g._factors), _marginals(g._marginals), _neighbors(g._neighbors)
{
	// Make copy of factors
    _factors.resize(g._factors.size(), NULL);
    for (unsigned i = 0; i < g._factors.size(); i++) {
		// We don't want to clone empty factors
        if (g._factors[i] != NULL) {
            _factors[i] = g._factors[i]->clone();
        }
    }

	// Make copy of marginals
    /*_marginals.resize(g._marginals.size(), NULL);
    for (unsigned i = 0; i < g._marginals.size(); i++) {
		// We don't want to clone empty factors
        if (g._marginals[i] != NULL) {
            _marginals[i] = g._marginals[i]->clone();
        }
    }*/
}

// Destructor, will delete factors
template<class C, class M>
drwnCumulativeNet<C, M>::~drwnCumulativeNet()
{
	// Loop through factors
	for (drwnNodes<C *>::const_iterator it = _factors.begin(); it != _factors.end(); ++it) {
		// Only delete nonempty factors
		if (*it)
			delete *it;
    }
	_factors.clear();

	// Loop through marginals
	/*for (drwnNodes<F *>::const_iterator it = _marginals.begin(); it != _marginals.end(); ++it) {
		// Only delete nonempty factors
		if (*it)
			delete *it;
    }
	_marginals.clear();*/
}

// Resize vectors to size of variable universe
template<class C, class M>
void drwnCumulativeNet<C, M>::initialize()
{
	// Create empty marginals
	unsigned int size = &(*_pUniverse) != 0 ? _pUniverse->numVariables() : 0;
	_marginals.resize(size);

	// Loop through marginals
	//for (drwnNodes<F *>::const_iterator it = _marginals.begin(); it != _marginals.end(); ++it) {
	//	(*it) = new M();
    //}

	_neighbors.resize(size);
}

// Add factor to graph and take ownership
template<class C, class M>
void drwnCumulativeNet<C, M>::addFactor(C* factor)
{
	// Iterate through variable in factor
	for (vector<int>::const_iterator p = factor->getVars().begin(); p != factor->getVars().end(); p++) {
		// Add record of variable being in scope of this factor
		_neighbors[*p].insert(_factors.size());
	}

	// Add factor to network
	_factors.insert(factor);
}

// Delete factor from graph and free memory
template<class C, class M>
void drwnCumulativeNet<C, M>::removeFactor(int factorIndex)
{
	// Iterate through variable in factor
	for (vector<int>::iterator p = factor.getVars().begin(); p != factor.getVars().end(); p++) {
		// Add record of variable being in scope of this factor
		_neighbors[*p].erase(_factors.size());
	}

	// Delete factor
	delete _factors[factorIndex];
	_factors.remove(factorIndex);
}