/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCliqueTree.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//#include "drwnCliqueParent.h"
//#include "drwnCumulativeNet.h"

#undef max

using namespace std;

struct derivTerm;

template <class G>
class drwnCliqueTree	// : public drwnStdObjIface
{	
protected:
	//! Reference to graphical model on which this clique tree is build
	G& _gm;

	//! Precalculate information about cliques
	vector<drwnCliqueInfo<G> > _cliqueInfo;

	//! Memory for final and temporary messages
	// TODO: Also, memory for temporary derivative values and factor values
	typename G::factor_type::range_type* _messages;

	//! Memory for keeping count of unused neighbours of cliques during message passing
	unsigned int* _unusedNeighbours;

	//! Number of entries in _messages
	unsigned int _numMessages;

	//! First entry in _messages for temporary messages
	unsigned int _tempIndex;

	//! First entry in _messages for temporary factor values
	unsigned int _valueIndex;

	//! First entry in _messages for temporary derivative values
	unsigned int _derivIndex;

	//! First entry in _message for root messages
	unsigned int _rootMessageIndex;

	typename G::marginal_type::domain_type margValue_;
	typename G::factor_type::domain_type margCopulaValue_;

	//! Leaves of tree. Used for message-passing
	vector<int> _leaves;

	//! Roots of tree and associated final message. Used for message-passing for calculating final derivative.
	vector<pair<int, typename G::factor_type::range_type> > _roots;

	//! Scopes and cumulative scopes for all factors in a clique, for all cliques
	pair<drwnSubset, drwnSubset>* _scopes;

	//! Masks for each clique when marginalizing variables
	vector<unsigned int> _margMasks;

	//! Flag to say whether to use _margMasks
	bool _isMarged;

	//! Create cliques from graphical model
	void build();

	//! Create cliques from subset of graphical model
	void build(set<int>& vars);

	//! Fill out clique information structures (is called after adding cliques and before inference)
	void initialize();

	//! Free memory
	void clear();

	//! Add a clique to the graph
	void addClique(vector<int>& factors, vector<int>& vars);

	//! Add parent to clique \b indx
	void addParent(int indx, int parent) { _cliqueInfo[indx]._parents.push_back(parent); }

	//! Set child of clique \b indx
	void setChild(int indx, int child); //const { _cliqueInfo[indx]->_child = child; }

	//! Return the set of separator variables between cliques \b clA and \b clB
	static vector<int> intersection(const vector<int>& clA, const vector<int>& clB);

	//! Work out and store list of leaves
	void determineLeaves();

	// Work out and store list of roots
	void determineRoots();

	//! Allocate memory for messages
	void createMessages();

	//! Create maps between universe variables and clique vars
	void createIndices();

	//! Calculate scopes and cumulative scopes of all factors (relative to clique variables)
	void createScopes();

	//! Fill subset information
	void createSubsets();

	//! Put a topological ordering on the cliques by shuffling them
	void makeTopologicalOrder();

public:
	typedef drwnCliqueParent<G> parent_iterator;

	//! Construct a clique tree for a given graphical model
    drwnCliqueTree(const G& graphicalModel);

	// Construct a clique tree for subgraph of a given graphical model
	drwnCliqueTree(const G& graphicalModel, set<int>& vars);

    //! Copy constructor
    drwnCliqueTree(const drwnCliqueTree& cliqueTree);

	//! Destructor
    virtual ~drwnCliqueTree();

    //! Return type
    virtual const char *type() const { return "drwnCliqueTree"; }

	//! Clone clique tree
    virtual drwnCliqueTree *clone() const { return new drwnCliqueTree(*this); }

	//! Return graphical model
	G& model() const { return _gm; }

    //! Return the number of cliques in the tree
    int numCliques() const { return (int) _cliqueInfo.size(); }

	//! Return the factors in clique at index \b indx
    const vector<int>& factors(int indx) const { return _cliqueInfo[indx]._factors; }

	//! Add factors \b factors to clique \b indx
	void addCliqueFactors(int indx, vector<int>& factors) { _cliqueInfo[indx]._factors.insert(_cliqueInfo[indx]._factors.end(), factors.begin(), factors.end());}

	//! Return the variables in clique at index \b indx
    const vector<int>& vars(int indx) const { return _cliqueInfo[indx]._vars; }

	//! Return child of clique \b indx
	int child(int indx) const { return _cliqueInfo[indx]._child; }

	//! Return parents of clique \b indx
	const vector<int>& parents(int indx) const { return _cliqueInfo[indx]._parents; }

	//! Return sepset between clique \b indx and child
	const vector<int>& sepset(int indx) const { return _cliqueInfo[indx]._sepset; }

	//! Return sepset between clique i and j
	const vector<int>& sepset(unsigned int i, unsigned int j) const { return child(i) == j ? _cliqueInfo[i]._sepset : _cliqueInfo[j]._sepset; }

	//! Return leaves
	const vector<int>& leaves() const { return _leaves; }

	//! Return roots
	vector<pair<int, typename G::factor_type::range_type> >& roots() { return _roots; }

	//! Return true if clique has a child
	bool hasChild(unsigned int i) const { return child(i) != -1; }

	//! Return number of unused neighbours in message passing
	unsigned int& numUnused(unsigned int i) const { return _unusedNeighbours[i]; }

	//! Return index of lowest unused parent
	unsigned int& lowestUnusedParent(unsigned int i) const { return _unusedNeighbours[numCliques() + i]; }

	//! Return number of neighbours of clique
	unsigned int neighbours(unsigned int i) const { return parents(i).size() + (int) hasChild(i); }

	//! Reference to final message
	//typename G::factor_type::range_type& message(unsigned int cliqueIndx, unsigned int msgIndx) const { return _messages[_cliqueInfo[cliqueIndx]._messageIndex + msgIndx]; }

	void generateMasks(typename G::marginal_type::domain_type* sample, bool isCopulaSample = false)
	{
		// Allocate memory for masks
		if (_margMasks.size() != numCliques()) {
			//_margMasks.clear();
			_margMasks.resize(numCliques());
		}

		// Loop over cliques, build masks
		for (unsigned int i = 0; i < numCliques(); i++) {
			unsigned int mask = 0;
			// Loop over variables
			for (unsigned int j = 0; j < vars(i).size(); j++) {
				// If not marginalized then add to mask
				if (!isCopulaSample) {
					if (sample[vars(i)[j]] != margValue_)
						mask |= (1 << j);
				}
				else {
					if (sample[vars(i)[j]] != margCopulaValue_)
						mask |= (1 << j);
				}
			}

			_margMasks[i] = mask;
		}

		// Set flag so the masks will be used during inference
		_isMarged = true;
	}

	bool isMarged() {return _isMarged; }

	void turnOffMasks() { _isMarged = false; }

	unsigned int mask(unsigned int i) { return _margMasks[i];}
	
	typename G::factor_type::range_type& message(int i, int j, unsigned int msgIndx) const {
		if (j == -1)
			return _messages[_rootMessageIndex + msgIndx];
		else if (child(i) == j)
			return _messages[_cliqueInfo[i]._forwardMessageIndex + msgIndx];
		else
			return _messages[_cliqueInfo[j]._backwardMessageIndex + msgIndx];
	}

	//! Reference to last final message for a given clique
	//typename G::factor_type::range_type& lastMessage(unsigned int i) const { return message(i, numSubsets(i) - 1); }
	typename G::factor_type::range_type& lastMessage(int i) const { return _messages[_rootMessageIndex + numSubsets(i) - 1]; }
	
	//! Reference to temporary message
	typename G::factor_type::range_type& tempMessage(unsigned int msgIndx) const 
	{
		// DEBUG: For now, make sure within bounds
		DRWN_ASSERT_MSG(msgIndx < _valueIndex - _tempIndex, "Went past end of temporary message structure!")

		return _messages[_tempIndex + msgIndx];
	}

	//! Reference to temporary value of k-th factor in clique i
	typename G::factor_type::range_type& tempValue(int i, unsigned int k, unsigned int msgIndx) const 
	{
		// DEBUG: For now, make sure within bounds
		DRWN_ASSERT_MSG((int) msgIndx < (1 << model().factor(factors(i)[k]).getVars().size()), "Went past end of temporary value structure!")
		
		return _messages[_valueIndex + _cliqueInfo[i]._factorIndices[k] + msgIndx];
	}

	//! Reference to temporary value
	typename G::factor_type::range_type& tempValue(unsigned int msgIndx) const 
	{
		// DEBUG: For now, make sure within bounds
		DRWN_ASSERT_MSG(msgIndx < _derivIndex - _valueIndex, "Went past end of temporary value structure!")
		
		return _messages[_valueIndex + msgIndx];
	}

	//! Reference to temporary value of derivative term of k-th factor in clique i
	typename G::factor_type::range_type& tempDeriv(int i, unsigned int k, unsigned int msgIndx) const 
	{
		// DEBUG: For now, make sure within bounds
		DRWN_ASSERT_MSG((int) msgIndx < (1 << drwnSubset::countElements(cumulativeScope(i, k + 1))), "Went past end of temporary derivative structure!")

		return _messages[_derivIndex + _cliqueInfo[i]._derivIndices[k] + msgIndx];
	}

	//! Reference to temporary value of derivative term
	typename G::factor_type::range_type& tempDeriv(unsigned int msgIndx) const 
	{
		// DEBUG: For now, make sure within bounds
		DRWN_ASSERT_MSG(msgIndx < _numMessages - _derivIndex, "Went past end of temporary derivative structure!")

		return _messages[_derivIndex + msgIndx];
	}

	//! Reset counts of unused neighbours of cliques for message passing
	void resetUnusedNeighbours();

	//! True if clique is a leaf
	bool isLeaf(unsigned int i) const { return !parents(i).size() || (parents(i).size() == 1 && child(i) == -1); }

	//! Return number of parents of clique
	unsigned int numParents(unsigned int i) const { return parents(i).size(); }

	//! Return number of parents when passing message from clique i to clique j
	unsigned int numParents(unsigned int i, unsigned int j) const { return numParents(i) + (j == -1 ? (hasChild(i) ? 1 : 0) : (!hasChild(i) ? -1 : 0)); }

	//! Return number of factors in clique
	unsigned int numFactors(unsigned int i) const { return factors(i).size(); }

	//! Return number of variables in clique
	unsigned int numVars(unsigned int i) const { return vars(i).size(); }

	//! Return number of variables in sepset between clique i and its child
	unsigned int numSepsetVars(unsigned int i) const { return sepset(i).size(); }

	//! Return number of variables in sepset between clique i and j
	unsigned int numSepsetVars(unsigned int i, unsigned int j) const { return sepset(i, j).size(); }

	//! Return scope of j-th factor in clique i relative to clique i vars
	const drwnSubset& scope(unsigned int i, unsigned int j) const { return _scopes[_cliqueInfo[i]._scopeIndex + j].first; }

	//! Return cumulative scope of j-th factor in clique i relative to clique i vars
	const drwnSubset& cumulativeScope(unsigned int i, unsigned int j) const { return _scopes[_cliqueInfo[i]._scopeIndex + j].second; }

	//! Return index between universe variables and clique variables for clique i
	map<int, int>& index(unsigned int i) const { return (map<int, int>&) _cliqueInfo[i]._index; }

	//! Return number of subsets of clique i
	unsigned int numSubsets(unsigned int i) const { return 1 << numVars(i); }

	//! Return clique subset (relative to clique variables) of clique i
	const drwnSubset& subset(unsigned int i) const { return _cliqueInfo[i]._subset; }

	//! Return sepset subset (relative to clique i) of sepset between i and its child
	const drwnSubset& sepsetSubset(unsigned int i) const { return _cliqueInfo[i]._sepsetSubset; }

	//! Return sepset subset (relative to clique i) of sepset between i and j
	const drwnSubset& sepsetSubset(unsigned int i, unsigned int j) const { return child(i) == j ? _cliqueInfo[i]._sepsetSubset : _cliqueInfo[j]._sepsetSubsetChild; }
	
	//! Return subset of clique i minus sepset between i and j (relative to clique i)
	const drwnSubset& cliqueMinusSepset(unsigned int i, unsigned int j) const { return child(i) == j ? _cliqueInfo[i]._minusSepset : _cliqueInfo[j]._minusSepsetChild; }

	//! Return subset of clique i minus sepset between i and its child (relative to clique i)
	const drwnSubset& cliqueMinusSepset(unsigned int i) const { return _cliqueInfo[i]._minusSepset; }

	//! Return number of subsets of sepset between clique i and its child
	unsigned int numSepsetSubsets(unsigned int i) const { return 1 << sepset(i).size(); }

	//! Return number of subsets of sepset between clique i and j
	unsigned int numSepsetSubsets(unsigned int i, unsigned int j) const { return child(i) == j ? 1 << sepset(i).size() : 1 << sepset(j).size(); }

	//! Reset messages to zero
	void resetMessages(bool takeLog = false)
	{
		if (!takeLog) {
			for (unsigned int i = 0; i < _tempIndex; i++)
				_messages[i] = 0;
		}
		else {
			for (unsigned int i = 0; i < _tempIndex; i++)
				_messages[i] = -std::numeric_limits<double>::infinity();
		}
	}

	//! Reset final messages
	void resetFinalMessages(int idx, bool takeLog = false)
	{
		if (!takeLog) {
			for (unsigned int i = 0; i < numSubsets(idx); i++)
				_messages[_rootMessageIndex + i] = 0;
		}
		else {
			for (unsigned int i = 0; i < numSubsets(idx); i++)
				_messages[_rootMessageIndex + i] = -std::numeric_limits<double>::infinity();
		}
	}

	//! Reset temporary message to zero
	void resetTempMsgs(bool takeLog)
	{
		if (!takeLog) {
			for (unsigned int i = _tempIndex; i < _valueIndex; i++)
				_messages[i] = 0;
		}
		else {
			for (unsigned int i = _tempIndex; i < _valueIndex; i++)
				_messages[i] = -std::numeric_limits<double>::infinity();
		}
	}

	//! Reset temporary derivative terms
	void resetDerivMsgs(unsigned int i) { for (unsigned int j = _derivIndex; j < _derivIndex + _cliqueInfo[i]._countDerivTerms; j++) _messages[j] = std::numeric_limits<G::factor_type::range_type>::quiet_NaN(); }

	//! Print the first k parents of clique i when passing message from i to j
	void printParents(int i, int j, unsigned k);

	//! Return iterator to first parent
	parent_iterator beginParents(int i, int j) { return parent_iterator(*this, i, j, 0); }

	//! Return iterator to last parent
	parent_iterator endParents(int i, int j) { return parent_iterator(*this, i, j, neighbours(i)); }

	//! Breaks clique tree along edge from i to j, and returns cliques on j side
	vector<unsigned int> previousCliques(int i, int j);

	//! Determine what variables have already been differentiated when we are passing a message from i to j
	set<unsigned int> diffVars(int i, int j);

	//! Stack for dynamic programming in inference algorithm
	vector<derivTerm> stack;

	unsigned int maxClique()
	{
		unsigned maxVal = 0;
		for (unsigned int i = 0; i < numCliques(); i++)
			maxVal = max(maxVal, numVars(i));
		return maxVal;
	}

	unsigned int maxSepset()
	{
		unsigned maxVal = 0;
		for (unsigned int i = 0; i < numCliques(); i++)
			maxVal = max(maxVal, numSepsetVars(i));
		return maxVal;
	}

	unsigned int totalSize()
	{
		unsigned sumVars = 0;
		for (unsigned int i = 0; i < numCliques(); i++)
			sumVars += numVars(i);
		return sumVars;
	}
};

typedef drwnCliqueTree<normalCumulativeNet> normalCliqueTree;

typedef drwnCliqueTree<debugCumulativeNet> debugCliqueTree;

typedef drwnCliqueTree<gumbelCumulativeNet> gumbelCliqueTree;

typedef drwnCliqueTree<frankCumulativeNet> frankCliqueTree;

typedef drwnCliqueTree<claytonCumulativeNet> claytonCliqueTree;

// Construct a clique tree for a given graphical model
template <class G>
drwnCliqueTree<G>::drwnCliqueTree(const G& graphicalModel) :
	_gm((G&) graphicalModel)
{
	// Initialize variables
	_messages = 0;
	_scopes = 0;
	_unusedNeighbours = 0;
	_numMessages = 0;
		
	// Build clique tree and initialize precalculated values
	build();
	initialize();
}

// Construct a clique tree for subgraph of a given graphical model
template <class G>
drwnCliqueTree<G>::drwnCliqueTree(const G& graphicalModel, set<int>& vars) :
	_gm((G&) graphicalModel)
{
	// Initialize variables
	_messages = 0;
	_scopes = 0;
	_unusedNeighbours = 0;
	_numMessages = 0;
		
	// Build clique tree and initialize precalculated values
	build(vars);
	initialize();
}

// Copy constructor
// TODO: Fix this up! (non-essential)
template <class G>
drwnCliqueTree<G>::drwnCliqueTree(const drwnCliqueTree& g) :
	_leaves(g._leaves), _gm(g._gm)
{
	// Resize vectors to required size
    //_cliques.resize(g._cliques.size(), NULL);
	//_childSepsets.resize(g._childSepsets.size(), NULL);

	// Populate with copies of entries from g
    /*for (unsigned i = 0; i < g._cliques.size(); i++) {
        if (g._cliques[i] != NULL)
            _cliques[i] = g._cliques[i];	// NOTE: Does this work for pairs?
		if (i < g._childSepsets.size()) {
			if (g._childSepsets[i] != NULL)
				_childSepsets[i] = g._childSepsets[i];
		}
    }*/

	// TODO: Copy clique info structures
}

// Destructor
template <class G>
drwnCliqueTree<G>::~drwnCliqueTree()
{
	clear();
}

// Initialize members
template <class G>
void drwnCliqueTree<G>::initialize()
{
	_isMarged = false;

	// Calculate leaves and roots
	determineLeaves();
	determineRoots();

	// Reset count of unused neighbours
	// NOTE: No need to initialize neighbour count, since it's done at start of message passing algorithm
	//resetUnusedNeighbours();

	// Create indices
	createIndices();

	// Create scopes
	createScopes();

	// Fill subsets information
	// NOTE: Must have created indices before calling this
	createSubsets();
	
	// Allocate memory for messages
	// NOTE: Must do this after subsets have been calculated
	createMessages();

	// Give stack some initial size
	stack.reserve(64);

	// NOTE: Assumes all marginals have same form
	margValue_ = model().marginal(0).quantile(1.0);
	if (G::factor_type::isTransformCopula())
		margCopulaValue_ = G::factor_type::transformArgument(1.0);
	else
		margCopulaValue_ = 1.0;
}

// Allocate memory for messages
template <class G>
void drwnCliqueTree<G>::createMessages()
{
	// Count how many final messages we require
	unsigned int numMessages = 0, maxSubsets = 0;
	for (int i = 0; i < numCliques(); i ++) {
		_cliqueInfo[i]._forwardMessageIndex = numMessages;
		numMessages += numSubsets(i);

		maxSubsets = max(maxSubsets, numSubsets(i));

		if (child(i) != -1) {
			_cliqueInfo[i]._backwardMessageIndex = numMessages;
			numMessages += numSubsets(child(i));
		}
	}

	// Allocate memory for root messages
	_rootMessageIndex = numMessages;
	numMessages += maxSubsets;

	// Count how many temporary messages we require
	unsigned int countEntries = 0;
	_tempIndex = numMessages;
	for (int i = 0; i < numCliques(); i ++)
		countEntries = max(countEntries, numParents(i, -1) * numSubsets(i));
	numMessages += countEntries;

	// Loop over factors, count how much memory required
	_valueIndex = numMessages;
	countEntries = 0;
	for (int i = 0; i < numCliques(); i++) {
		unsigned int cliqueEntries = 0;
		vector<int>::const_iterator jt = factors(i).begin();
		_cliqueInfo[i]._factorIndices.reserve(numFactors(i));
		for (unsigned int k = 0; k < numFactors(i); k++, jt++) {
			_cliqueInfo[i]._factorIndices.push_back(cliqueEntries);
			cliqueEntries += (1 << model().factor(*jt).getVars().size());
		}
		countEntries = max(countEntries, cliqueEntries);
	}
	numMessages += countEntries;

	// Allocate memory for temporary derivatives. We use NaN to keep track of which ones have been calculated already.
	// Loop over scopes (i.e. factors), count how much memory required
	// TODO: Add a derivIndices to drwnCliqueInfo, since only needed for some cliques
	_derivIndex = numMessages;
	countEntries = 0;
	for (int i = 0; i < numCliques(); i++) {
		_cliqueInfo[i]._countDerivTerms = 0;
		vector<int>::const_iterator jt = factors(i).begin();
		if (numFactors(i) > 2) {
			_cliqueInfo[i]._derivIndices.reserve(numFactors(i) - 2);
			for (unsigned int k = 0; k < numFactors(i) - 2; k++, jt++) {
				//_factorIndices[model().numFactors() + *jt] = countEntries;
				_cliqueInfo[i]._derivIndices.push_back(_cliqueInfo[i]._countDerivTerms);
				//countEntries += 1 << drwnSubset::countElements(ct.cumulativeScope(i, k + 1));
				_cliqueInfo[i]._countDerivTerms += 1 << drwnSubset::countElements(cumulativeScope(i, k + 1));
			}
		}
		countEntries = max(countEntries, _cliqueInfo[i]._countDerivTerms);
	}
	numMessages += countEntries;
	
	// Allocate memory for messages and temporary messages
	_numMessages = numMessages;	// So we know how much memory to zero
	_messages = new G::factor_type::range_type[numMessages];
}

// Create maps between universe variables and clique vars
template <class G>
void drwnCliqueTree<G>::createIndices()
{
	// Loop over cliques
	for (int i = 0; i < numCliques(); i++) {
		// Enter variables into index
		for (unsigned int k = 0; k < numVars(i); k++)
			_cliqueInfo[i]._index.insert(make_pair(vars(i)[k], k));
	}
}

// Calculate scopes and cumulative scopes of all factors (relative to clique variables)
template <class G>
void drwnCliqueTree<G>::createScopes()
{	
	// Allocate memory for scopes
	unsigned int scopeIndex = 0;
	_scopes = new pair<drwnSubset, drwnSubset>[model().numFactors()];

	// Loop over cliques
	for (int i = 0; i < numCliques(); i++) {
		_cliqueInfo[i]._scopeIndex = scopeIndex;

		// DEBUG: Output scopes
		//wcout << "Clique " << i << endl;
		
		// Loop over factors in clique
		for (unsigned int k = 0; k < numFactors(i); k++) {
			// NOTE: factorNeighbours(i) just returns the scope of factor i, since they are neighbours in factor graph
			_scopes[scopeIndex + k].first = drwnSubset::makeSubset(model().factorNeighbors(factors(i)[k]), index(i));
			
			if (k)
				_scopes[scopeIndex + k].second = drwnSubset::setUnion(_scopes[scopeIndex + k].first, _scopes[scopeIndex + k - 1].second);
			else
				_scopes[scopeIndex + k].second = _scopes[scopeIndex + k].first;

			// DEBUG: Output scopes
			/*wcout << "Scope of factor " << factors(i)[k] << ": ";
			drwnSubset::printSubset(_scopes[scopeIndex + k].first, vars(i), model().getUniverse());
			wcout << endl << "Scopes of factors ";
			drwnSubset::printSubset(factors(i), k);
			wcout << ": ";
			drwnSubset::printSubset(_scopes[scopeIndex + k].second, vars(i), model().getUniverse());
			wcout << endl;*/
		}
		
		// DEBUG: Extra line for pretty output!
		//wcout << endl;
		
		scopeIndex += numFactors(i);
	}
}

// Fill subset information
template <class G>
void drwnCliqueTree<G>::createSubsets()
{
	// Loop over cliques
	for (int i = 0; i < numCliques(); i++) {
		// Create clique subset
		_cliqueInfo[i]._subset = numSubsets(i) - 1;

		// Create sepset subset (relative to clique variables) for clique and child
		_cliqueInfo[i]._sepsetSubset = drwnSubset::makeSubset(sepset(i), index(i));
		if (hasChild(i))
			_cliqueInfo[i]._sepsetSubsetChild = drwnSubset::makeSubset(sepset(i), index(child(i)));
	}

	// Loop over cliques again (have to have calculated all clique / sepset subsets first
	for (int i = 0; i < numCliques(); i++) {
		// Create subset of clique variables minus sepset variables for clique and child
		_cliqueInfo[i]._minusSepset = drwnSubset::setMinus(subset(i), sepsetSubset(i));
		if (hasChild(i))
			// TODO: Is this correct?
			_cliqueInfo[i]._minusSepsetChild = drwnSubset::setMinus(subset(child(i)), _cliqueInfo[i]._sepsetSubsetChild);
	}
}

// Reset counts of unused neighbours of cliques for message passing
template <class G>
void drwnCliqueTree<G>::resetUnusedNeighbours()
{
	// Allocate memory if first time
	if (!_unusedNeighbours)
		_unusedNeighbours = new unsigned int[numCliques() * 2];

	// Calculate number of unused neighbours
	for (unsigned int i = 0; i < numCliques(); i++)
		_unusedNeighbours[i] = neighbours(i);
	for (unsigned int i = numCliques(); i < 2 * numCliques(); i++)
		_unusedNeighbours[i] = 0;
}

// Free memory
template <class G>
void drwnCliqueTree<G>::clear()
{
	// Free memory
    //for (vector<pair<drwnClique, drwnClique>* >::const_iterator it = _cliques.begin(); it != _cliques.end(); ++it)
    //    delete *it;

	if (_messages)
		delete [] _messages;
	if (_scopes)
		delete [] _scopes;
	if (_unusedNeighbours)
		delete [] _unusedNeighbours;
}

// Add a clique to the graph
template <class G>
void drwnCliqueTree<G>::addClique(vector<int>& factors, vector<int>& vars)
{
    // Temporary clique structure
	drwnCliqueInfo<G> _tempClique;

	// TODO: Need to do this after pushing?
	_tempClique._factors = factors;
	_tempClique._vars = vars;
	_tempClique._child = -1;
	_cliqueInfo.push_back(_tempClique);
}

// Set child of clique \b indx
template <class G>
void drwnCliqueTree<G>::setChild(int indx, int child)
{
	// Adjust child and sepset
	// NOTE: I can't remember why the following condition is necessary (if it actually is)...
	if (numCliques()) {
		_cliqueInfo[indx]._child = child;
		_cliqueInfo[indx]._sepset = intersection(_cliqueInfo[indx]._vars, _cliqueInfo[child]._vars);
	}
}

// Return the set of separator variables between cliques \b clA and \b clB
template <class G>
vector<int> drwnCliqueTree<G>::intersection(const vector<int>& clA, const vector<int>& clB)
{
    vector<int> sepset;

    set_intersection(clA.begin(), clA.end(), clB.begin(), clB.end(),
        insert_iterator<vector<int> >(sepset, sepset.begin()));

    return sepset;
}

/* NOTE: Outline of clique tree construction algorithm (so I can understand later!)
	1. create neighbour list, i.e., list of neighbours of each variable in graph
	2. select next variable to eliminate by min-fill criterion
		a. for each variable X, check how many neighbours of X are not connected to other neighbours of X
		   (these correspond to fill edges, that will be added if we eliminate this variable)
		b. keep track of lowest fill-weight, and all variables with that weight
		c. uniformly select variable to be eliminated from this list
	3. eliminate chosen variable, and create clique
		a. make list of factors in clique (these are unused factors that have chosen variable in scope)
		b. make list of variables in clique (these are uneliminated variable neighbours of chosen variable),
		   parents of clique (these are unused newly created factor neighbours of chosen variable),
		   and add fill edges to neighbour list (by going through all pairs of neighbours of chosen variable as previously)
		c. check if new clique is a subset of one of its parents (other way around is not necessary, since not possible)
		   if it is, record subset parent and most recently created parent (with highest index)
		d. add new clique as neighbour of all neighbours of eliminated variable (this is so we know how to connect cliques)
		e. etc ...
*/

// Work out and store list of leaves
template <class G>
void drwnCliqueTree<G>::determineLeaves()
{
	_leaves.clear();

	// Cliques without parents, or no children and no more than one parent, in default message passing order are leaves
	for (int i = 0; i < numCliques(); i++) {
		if (!parents(i).size() || (child(i) == -1 && parents(i).size() < 2))
			_leaves.push_back(i);
	}
}

// Work out and store list of roots
template <class G>
void drwnCliqueTree<G>::determineRoots()
{
	_roots.clear();

	for (int i = 0; i < numCliques(); i++) {
		if (child(i) == -1)
			_roots.push_back(pair<int, typename G::factor_type::range_type>(i, 1.0));
	}
}

// Create cliques from graphical model
template<class G>
void drwnCliqueTree<G>::build()
{
	// Flag to determine if we need to reorder in topological order (only if merged parents
	bool reorderTopologically = false;

	// Create neighbours lists
	// TODO: Move this into drwnCumulativeNet.h
	vector<set<int> > neighbours(_gm.numVariables() * 2);
	for (int i = 0; i < _gm.numFactors(); i++) {
		// For nonempty factors
		const G::factor_type* factor = &_gm.factor(i);
		if (factor) {
			// Loop over all pairs for variables in factor
			const vector<int> vars = factor->getVars();
			for (unsigned int i = 0; i < vars.size() - 1; i++) {
				for (unsigned int j = i + 1; j < vars.size(); j++) {
					neighbours[vars[i]].insert(vars[j]);
					neighbours[vars[j]].insert(vars[i]);
				}
			}
		}
	}

	// Simulate variable elimination with a min-fill criterion for selecting next variable to eliminate
	vector<bool> isVarAdded(_gm.numVariables() * 2, false);
	vector<bool> isFactorAdded(_gm.numFactors(), false);
	for (int i = 0; i < _gm.numVariables(); i++) {
		int minWeight = numeric_limits<int>::max();
		vector<int> minVars;
		
		// Create list of variables with the lowest min-fill score
		for (int j = 0; j < _gm.numVariables(); j++) {
			// Skip variable if already added
			if (isVarAdded[j])
				continue;

			// Count number of fill edges required if removing this variable
			int w = 0;
			for (set<int>::const_iterator it = neighbours[j].begin(); it != neighbours[j].end(); ++it) {
				// Don't consider variables already added and neighbours corresponding to new factors
				if (isVarAdded[*it] || *it >= _gm.numVariables())
					continue;

				// Loop through neighbours greater than current neighbour
				set<int>::const_iterator jt = it;
				for (++jt; jt != neighbours[j].end(); ++jt) {
					// Don't consider variables already added (and neighbours corresponding to newly created factors)
					if (isVarAdded[*jt] || *jt >= _gm.numVariables())
						continue;

					// Record fill edge if there is not an edge between these two neighbours
					if (neighbours[*it].find(*jt) == neighbours[*it].end())
						w++;
				}
			}

			// NOTE: There no advantage to randomly choosing amongst 0 fill edge variables, right?
			if (w == 0) {
				// DEBUG: Following line so minWeight prints nicely
				//minWeight = 0;
				minVars.clear();
				minVars.push_back(j);
				break; 
			}

			// If removing this variable creates less fill edges than any previous, start new list
			else if (w < minWeight) {
				minWeight = w;
				minVars.clear();
				minVars.push_back(j);
			}

			// If removing this variable creates as many fill edges as previous min, just add to list
			else if (w == minWeight)
				minVars.push_back(j);

		}

		// Select variable to be eliminated uniformly from list of variables with lowest min-fill score
		int minIndex;
		if (minVars.size() == 1)
			minIndex = minVars[0];
		else
			minIndex = minVars[rand() % minVars.size()];

		// DEBUG: Print variable eliminated and number of fill edges introduced
		//wcout << "Var: " << cn.getUniverse()->varName(minIndex).c_str() << " - Fill Edges: " << minWeight << endl;
		
		// Simulate variable elimination, adding clique to clique tree
		isVarAdded[minIndex] = true;

		// Loop through factors that contain eliminated variable in their scope
		//pair<drwnClique, drwnClique>* cliqueEntry = new pair<drwnClique, drwnClique>;
		vector<int> factors, vars, usesNewFactors;
		for (set<int>::const_iterator it = _gm.varNeighbors(minIndex).begin(); it != _gm.varNeighbors(minIndex).end(); it++) {
			// Add those that are not marked to clique and mark them
			if (!isFactorAdded[*it]) {
				isFactorAdded[*it] = true;
				factors.push_back(*it);
			}
		}

		// Recalculate fill edges and add to neighbours list
		// Also, calculate scope of factor clique
		vars.push_back(minIndex);
		for (set<int>::const_iterator it = neighbours[minIndex].begin(); it != neighbours[minIndex].end(); ++it) {
			// Don't consider variables already added
			if (isVarAdded[*it])
				continue;

			// Make record if neighbour is a newly created factor that hasn't been used
			else if (*it >= _gm.numVariables()) {
				usesNewFactors.push_back(*it - _gm.numVariables());
				continue;
			}

			// Add variable to clique scope
			vars.push_back(*it);

			// Add fill edges
			set<int>::const_iterator jt = it;
			for (++jt; jt != neighbours[minIndex].end(); ++jt) {
				// Don't consider variables already added
				if (isVarAdded[*jt] || *jt >= _gm.numVariables())
					continue;

				// Adjust neighbours for fill edge (i.e. add fill edge)
				if (neighbours[*it].find(*jt) == neighbours[*it].end()) {
					neighbours[*it].insert(*jt);
					neighbours[*jt].insert(*it);
				}
			}
		}

		// Have to sort vars, so can calculate intersection
		sort(vars.begin(), vars.end());

		// Loop through list of parent cliques to see if at least one is a subset of child
		int firstSubset = -1;
		int maxParent = numCliques();
		for (vector<int>::iterator it = usesNewFactors.begin(); it != usesNewFactors.end(); it++) {
			// Since a variable is eliminated from parent to child, size of child must be less than size of parent
			if (vars.size() < this->vars(*it).size()) {
				// Test if parent clique is subset of parent and calculate max parent for later
				if (includes(this->vars(*it).begin(), this->vars(*it).end(), vars.begin(), vars.end())) {
					firstSubset = *it;
					maxParent = *max_element(usesNewFactors.begin(), usesNewFactors.end());
					break;
				}
			}
		}

		// We need to add newly created factor (or childless parent if merging) as neighbour of all neighbours of current variable
		for (set<int>::const_iterator it = neighbours[minIndex].begin(); it != neighbours[minIndex].end(); ++it) {
			// If not added variables or new factor neighbour, then add correct clique as neighbour
			if (!isVarAdded[*it] && *it < _gm.numVariables()) {
				neighbours[*it].insert(_gm.numVariables() + ((firstSubset == -1) ? maxParent : firstSubset));	// maxParent

				// Record variable as neighbour of newly created factor
				neighbours[_gm.numVariables() + ((firstSubset == -1) ? maxParent : firstSubset)].insert(*it);	//maxParent
			}
		}

		// If a subset of any other clique...
		if (firstSubset >= 0) {
			// ... add factors to that clique
			if (factors.size()) {
				addCliqueFactors(firstSubset, factors);
			}

			// ... if more than one parent, mark and connect all parents to last parent (to preserve topological ordering)
			if (usesNewFactors.size() > 1) {
				if (firstSubset != maxParent)
					reorderTopologically = true;
				
				// Loop through parents, making adjustments
				for (vector<int>::iterator it = usesNewFactors.begin(); it != usesNewFactors.end(); it++) {
					// Skip first superset
					if (*it == firstSubset)
						continue;
					
					// Set child as most recent clique
					setChild(*it, firstSubset);

					// Add parent
					addParent(firstSubset, *it);

					// Mark as used
					isVarAdded[_gm.numVariables() + *it] = true;
				}
			}

			// ... on to next variable
			continue;
		}

		// (Clique must have at least one factor in it, and 31 variables or less since using 32-bit int to index subsets)
		DRWN_ASSERT(vars.size() < 32);	// factors.size() && 
		
		// Otherwise, add clique to clique tree
		addClique(factors, vars);

		// Loop through list of newly created factors that this clique uses
		for (vector<int>::iterator it = usesNewFactors.begin(); it != usesNewFactors.end(); it++) {
			// Set child as most recent clique
			setChild(*it, numCliques() - 1);

			// Add parent
			addParent(numCliques() - 1, *it);

			// Mark as used
			isVarAdded[_gm.numVariables() + *it] = true;
		}
	}

	if (reorderTopologically)
		makeTopologicalOrder();
}

// Create cliques from subgraph of graphical model
template<class G>
void drwnCliqueTree<G>::build(set<int>& varSet)
{
	// Flag to determine if we need to reorder in topological order (only if merged parents
	bool reorderTopologically = false;

	// Need to be able to convert from vars order to universe order
	vector<int> vars(varSet.begin(), varSet.end());

	// Extract list of factors in subgraph, initialize neighbours map
	map<int, bool> isFactorUsed;

	for (unsigned int i = 0; i < vars.size(); i++) {
		const set<int>& varNeighbours = _gm.varNeighbors(vars[i]);
		for (set<int>::const_iterator jt = varNeighbours.begin(); jt != varNeighbours.end(); jt++)
			isFactorUsed.insert(pair<int, bool>(*jt, false));
	}

	// Create map between variable numbers and integers
	// TODO: Remove vars and replace with a map<int, bool> isVarUsed
	map<int, int> varMap;
	for (unsigned int i = 0; i < vars.size(); i++)
		varMap.insert(pair<int, int>(vars[i], i));

	// Create neighbours lists
	vector<set<int> > neighbours(vars.size());
	for (map<int, bool>::iterator it = isFactorUsed.begin(); it != isFactorUsed.end(); it++) {
		// Loop over all pairs for variables in factor
		const vector<int> fVars = _gm.factor(it->first).getVars();
		for (unsigned int j = 0; j < fVars.size() - 1; j++) {
			for (unsigned int k = j + 1; k < fVars.size(); k++) {
				map<int, int>::iterator jIndx = varMap.find(fVars[j]);
				map<int, int>::iterator kIndx = varMap.find(fVars[k]);
				if (jIndx != varMap.end() && kIndx != varMap.end()) {
					neighbours[jIndx->second].insert(kIndx->second);
					neighbours[kIndx->second].insert(jIndx->second);
				}
			}
		}
	}

	//vector<bool> isNewFactorAdded(vars.size(), false);
	//vector<bool> isFactorAdded(factors.size(), false);

	// Simulate variable elimination with a min-fill criterion for selecting next variable to eliminate
	vector<bool> isVarAdded(vars.size() * 2, false);
	
	//for (set<int>::iterator it = vars.begin(); it != vars.end(); it++) {
	for (unsigned int i = 0; i < vars.size(); i++) {
		int minWeight = std::numeric_limits<int>::max();
		vector<int> minVars;
		
		// Create list of variables with the lowest min-fill score
		//for (set<int>::iterator jt = vars.begin(); jt != vars.end(); jt++) {
		for (unsigned j = 0; j < vars.size(); j++) {
			// Skip variable if already added
			if (isVarAdded[j])
				continue;

			// Count number of fill edges required if removing this variable
			int w = 0;
			for (set<int>::const_iterator it = neighbours[j].begin(); it != neighbours[j].end(); ++it) {
				// Don't consider variables already added and neighbours corresponding to new factors
				if (isVarAdded[*it] || *it >= (int) vars.size())
					continue;

				// Loop through neighbours greater than current neighbour
				set<int>::const_iterator jt = it;
				for (jt++; jt != neighbours[j].end(); ++jt) {
					// Don't consider variables already added (and neighbours corresponding to newly created factors)
					if (isVarAdded[*jt] || *jt >= (int) vars.size())
						continue;

					// Record fill edge if there is not an edge between these two neighbours
					if (neighbours[*it].find(*jt) == neighbours[*it].end())
						w++;
				}
			}

			// NOTE: There no advantage to randomly choosing amongst 0 fill edge variables, right?
			if (w == 0) {
				// DEBUG: Following line so minWeight prints nicely
				//minWeight = 0;
				minVars.clear();
				minVars.push_back(j);
				break; 
			}

			// If removing this variable creates less fill edges than any previous, start new list
			else if (w < minWeight) {
				minWeight = w;
				minVars.clear();
				minVars.push_back(j);
			}

			// If removing this variable creates as many fill edges as previous min, just add to list
			else if (w == minWeight)
				minVars.push_back(j);

		}

		// Select variable to be eliminated uniformly from list of variables with lowest min-fill score
		int minIndex;
		if (minVars.size() == 1)
			minIndex = minVars[0];
		else
			minIndex = minVars[rand() % minVars.size()];

		// DEBUG: Catch case where fails
		//if (varSet.size() == 8 && minIndex == 2)	
		//	bool gotHere = true;

		// DEBUG: Print variable eliminated and number of fill edges introduced
		//wcout << "Var: " << cn.getUniverse()->varName(minIndex).c_str() << " - Fill Edges: " << minWeight << endl;
		
		// Simulate variable elimination, adding clique to clique tree
		isVarAdded[minIndex] = true;

		// Loop through factors that contain eliminated variable in their scope
		vector<int> cliqueFactors, cliqueVars, usesNewFactors;
		for (set<int>::const_iterator it = _gm.varNeighbors(vars[minIndex]).begin(); it != _gm.varNeighbors(vars[minIndex]).end(); it++) {
			// Add those that are not marked to clique and mark them
			map<int, bool>::iterator factorIndx = isFactorUsed.find(*it);
			if (!factorIndx->second) {
				factorIndx->second = true;
				cliqueFactors.push_back(factorIndx->first);
			}
		}

		// Recalculate fill edges and add to neighbours list
		// Also, calculate scope of factor clique
		cliqueVars.push_back(vars[minIndex]);
		for (set<int>::const_iterator it = neighbours[minIndex].begin(); it != neighbours[minIndex].end(); ++it) {
			// Don't consider variables already added
			if (isVarAdded[*it])
				continue;

			// Make record if neighbour is a newly created factor that hasn't been used
			else if (*it >= (int) vars.size()) {
				usesNewFactors.push_back(*it - vars.size());
				continue;
			}

			// Add variable to clique scope
			cliqueVars.push_back(vars[*it]);

			// Add fill edges
			set<int>::const_iterator jt = it;
			for (++jt; jt != neighbours[minIndex].end(); ++jt) {
				// Don't consider variables already added
				if (isVarAdded[*jt] || *jt >= (int) vars.size())
					continue;

				// Adjust neighbours for fill edge (i.e. add fill edge)
				if (neighbours[*it].find(*jt) == neighbours[*it].end()) {
					neighbours[*it].insert(*jt);
					neighbours[*jt].insert(*it);
				}
			}
		}

		// Have to sort vars, so can calculate intersection
		sort(cliqueVars.begin(), cliqueVars.end());

		// Loop through list of parent cliques to see if at least one is a subset of child
		int firstSubset = -1;
		int maxParent = numCliques();
		for (vector<int>::iterator it = usesNewFactors.begin(); it != usesNewFactors.end(); it++) {
			// Since a variable is eliminated from parent to child, size of child must be less than size of parent
			if (cliqueVars.size() < this->vars(*it).size()) {
				// Test if parent clique is subset of parent and calculate max parent for later
				if (includes(this->vars(*it).begin(), this->vars(*it).end(), cliqueVars.begin(), cliqueVars.end())) {
					firstSubset = *it;
					maxParent = *max_element(usesNewFactors.begin(), usesNewFactors.end());
					break;
				}
			}
		}

		// We need to add newly created factor (or childless parent if merging) as neighbour of all neighbours of current variable
		for (set<int>::const_iterator it = neighbours[minIndex].begin(); it != neighbours[minIndex].end(); ++it) {
			// If not added variables or new factor neighbour, then add correct clique as neighbour
			if (!isVarAdded[*it] && *it < (int) vars.size()) {
				neighbours[*it].insert(vars.size() + maxParent);
			}
		}

		// If a subset of any other clique...
		if (firstSubset >= 0) {
			// ... add factors to that clique
			if (cliqueFactors.size())
				addCliqueFactors(firstSubset, cliqueFactors);

			// ... if more than one parent, mark and connect all parents to last parent (to preserve topological ordering)
			if (usesNewFactors.size() > 1) {
				if (firstSubset != maxParent)
					reorderTopologically = true;

				// Loop through parents, making adjustments
				for (vector<int>::iterator it = usesNewFactors.begin(); it != usesNewFactors.end(); it++) {
					// Skip maximum parent
					if (*it == firstSubset)
						continue;

					// Set child as most recent clique
					setChild(*it, firstSubset);

					// Add parent
					addParent(firstSubset, *it);

					// Mark as used
					isVarAdded[vars.size() + *it] = true;
				}
			}

			// ... on to next variable
			continue;
		}

		// DEBUG: Catch case where fails
		//if (varSet.size() == 8)	
		//	bool gotHere = true;

		// (Clique must have at least one factor in it, and 31 variables or less since using 32-bit int to index subsets)
		// DEBUG: Seeing what happens when remove requirement that clique contain factors
		DRWN_ASSERT(cliqueVars.size() < 32);	// cliqueFactors.size() && 

		// Otherwise, add clique to clique tree
		addClique(cliqueFactors, cliqueVars);
		
		// Loop through list of newly created factors that this clique uses
		for (vector<int>::iterator it = usesNewFactors.begin(); it != usesNewFactors.end(); it++) {
			// Set child as most recent clique
			setChild(*it, numCliques() - 1);

			// Add parent
			addParent(numCliques() - 1, *it);

			// Mark as used
			isVarAdded[vars.size() + *it] = true;
		}
	}

	if (reorderTopologically)
		makeTopologicalOrder();
}

// Put a topological ordering on the cliques by shuffling them
template <class G>
void drwnCliqueTree<G>::makeTopologicalOrder()
{
	// Keeps track of whether we have received message from child for each clique (doesn't always happen)
	//vector<bool> receivedChild(numCliques(), false);

	// Records passing order
	vector<pair<int, int> > passingOrder(numCliques());
	
	// Initialize number of unused neighbours for each clique
	//ct.resetUnusedNeighbours();

	// Make list of sets of unused neighbours
	vector<set<int> > unusedNeighbours(numCliques());
	for (int i = 0; i < numCliques(); i++) {
		if (hasChild(i))
			unusedNeighbours[i].insert(child(i));
		if (parents(i).size())
			unusedNeighbours[i].insert(parents(i).begin(), parents(i).end());
	}

	// Initialize priority queue with all leaves that aren't the root
	unsigned int root = numCliques() - 1;
	vector<int> leaves;
	determineLeaves();
	leaves.reserve(this->leaves().size());
	for (vector<int>::const_iterator it = this->leaves().begin(); it != this->leaves().end(); it++) {
		//if (*it != root && (child(*it) != -1))
		if (!parents(*it).size())
			leaves.push_back(*it);
	}

	// If the root clique is disjoint (no parents or children) then we have to add it as well
	// NOTE: Also add root clique if it has children
	//if ((child(root) == -1 && !parents(root).size()) || (child(root) != -1 &&))
	//if (!parents(root).size())
	//	leaves.push_back(root);

	priority_queue< int, vector<int>, std::greater<int> > nextMsg(std::greater<int>(), leaves);

	// Pass messages
	unsigned int msgNum = 0;
	while (nextMsg.size()) {
		int i = nextMsg.top(), j = -1;
		
		// Work out to whom to pass message
		if (unusedNeighbours[i].size()) {
			j = *unusedNeighbours[i].begin();
			unusedNeighbours[j].erase(i);
		}

		passingOrder[msgNum].first = i;
		passingOrder[msgNum++].second = j;

		nextMsg.pop();

		// If this message wasn't to itself...
		if (j != -1) {
			// ... push it if it is root and has 0 unused neighbours, or has 1 unused neighbour otherwise
			//if (((j == root || !ct.isLeaf(j)) && !ct.numUnused(j)) || (j != root && ct.numUnused(j) == 1))
			//if ((j == root) && !unusedNeighbours[j].size())
			if ((child(j) == -1) && !unusedNeighbours[j].size())
				nextMsg.push(j);
			//else if ((j != root) && (unusedNeighbours[j].size() <= 1))
			else if ((child(j) != -1) && (unusedNeighbours[j].size() <= 1))
				nextMsg.push(j);
		}
	}

	// Swap cliques
	for (int i = 0; i < numCliques() - 1; i++) {
		// If this is not clique i...
		if (passingOrder[i].first != i) {
			// ... find location of clique i in passing order
			int j = i + 1;
			for (; j < numCliques(); j++) {
				if (passingOrder[j].first == i)
					break;
			}

			// ... swap cliques passingOrder[i] and i
			// Swap parents
			unsigned int childI = child(i), childJ = child(passingOrder[i].first);
			for (vector<int>::const_iterator it = parents(passingOrder[i].first).begin(); it != parents(passingOrder[i].first).end(); it++)
				_cliqueInfo[*it]._child = i;
			for (vector<int>::const_iterator it = parents(i).begin(); it != parents(i).end(); it++)
				_cliqueInfo[*it]._child = passingOrder[i].first;

			// Adjust parents of children
			if (childI != -1) {
				for (vector<int>::iterator it = ((vector<int>&) parents(childI)).begin(); it != ((vector<int>&) parents(childI)).end(); it++) {
					if (*it == i) {
						*it = passingOrder[i].first;
						sort(((vector<int>&) parents(childI)).begin(), ((vector<int>&) parents(childI)).end());
						break;
					}
				}
			}
			if (childJ != -1) {
				for (vector<int>::iterator it = ((vector<int>&) parents(childJ)).begin(); it != ((vector<int>&) parents(childJ)).end(); it++) {
					if (*it == passingOrder[i].first) {
						*it = i;
						sort(((vector<int>&) parents(childJ)).begin(), ((vector<int>&) parents(childJ)).end());
						break;
					}
				}
			}

			// Swap clique data
			drwnCliqueInfo<G> tempClique = _cliqueInfo[passingOrder[i].first];
			_cliqueInfo[passingOrder[i].first] = _cliqueInfo[i];
			_cliqueInfo[i] = tempClique;
			
			// ... swap passing order
			for (int k = 0; k < numCliques(); k++) {
				if (passingOrder[k].second == passingOrder[i].first)
					passingOrder[k].second = i;
				else if (passingOrder[k].second == i)
					passingOrder[k].second = passingOrder[i].first;
			}

			passingOrder[j].first = passingOrder[i].first;
			passingOrder[i].first = i;
		}
	}

	// Change direction of edges, so that always passing to child
	for (int i = 0; i < numCliques(); i++) {
		// If not passing to child ...
		if (passingOrder[i].second != child(i)) {
			// TODO: Continue work from here tomorrow 13/1/2013
			// Add i as parent of j
			((vector<int>&) parents(passingOrder[i].second)).push_back(i);
			sort(((vector<int>&) parents(passingOrder[i].second)).begin(), ((vector<int>&) parents(passingOrder[i].second)).end());

			// Remove j from parents of i
			set<int> newParents(parents(i).begin(), parents(i).end());
			newParents.erase(passingOrder[i].second);
			((vector<int>&) parents(i)).assign(newParents.begin(), newParents.end());

			// Set j as child of i
			setChild(i, passingOrder[i].second);

			// Set -1 as child of j
			_cliqueInfo[passingOrder[i].second]._child = -1;
			_cliqueInfo[passingOrder[i].second]._sepset.clear();
		}
	}
}

// Print the first k parents of clique i when passing message from i to j
template <class G>
void drwnCliqueTree<G>::printParents(int i, int j, unsigned k)
{
	vector<int>::const_iterator jt = parents(i).begin();
	for (unsigned int l = 1; l <= k; l++, jt++) {
		// Work out true parent (see comments above for same procedure)
		// TODO: Replace with iterator
		unsigned int parent;
		if (jt == parents(i).end())
			parent = child(i);
		else {
			if (*jt == j) {
				l--;
				continue;
			}
			else
				parent = *jt;
		}

		// Print parent
		if (l == 1)
			wcout << parent;
		else
			wcout << " " << parent;

		// Make sure doesn't go past end
		if (jt == parents(i).end())
			break;
	}
}

// Breaks clique tree along edge from i to j, and returns cliques on j side
template <class G>
vector<unsigned int> drwnCliqueTree<G>::previousCliques(int i, int j)
{
	// Do depth-wise search on tree (this works because it is a tree, so we know we won't visit nodes twice etc.)
	vector<pair<int, int> > stack;
	vector<unsigned int> cliques;
	
	stack.push_back(pair<int, int>(j, i));
	while (stack.size()) {
		int current = stack.back().first, from = stack.back().second;
		stack.pop_back();
		cliques.push_back(current);
		for (drwnCliqueTree<G>::parent_iterator it = beginParents(current, from); it != endParents(current, from); it++) {
			stack.push_back(pair<int, int>(*it, current));
		}
	}

	return cliques;
}

// Determine what variables are differentiated when we are passing a message from i to j
template <class G>
set<unsigned int> drwnCliqueTree<G>::diffVars(int i, int j)
{
	// Do depth-wise search on tree (this works because it is a tree, so we know we won't visit nodes twice etc.)
	vector<pair<int, int> > stack;
	set<unsigned int> diffedVars;
	
	stack.push_back(pair<int, int>(i, j));
	while (stack.size()) {
		int current = stack.back().first, from = stack.back().second;
		stack.pop_back();

		// Push variables in the clique minus the sepset
		drwnSubset currentDiffedVars = (from != -1) ? cliqueMinusSepset(current, from) : subset(current);
		unsigned int howManyBits = drwnSubset::countElements(currentDiffedVars);
		for (unsigned int k = 0, l = 0; k < howManyBits; k++, l++) {
			// Loop to next set bit to be considered
			while(!(currentDiffedVars._subset & (1 << l)))
				l++;

			// Add variable to set
			diffedVars.insert(vars(current)[l]);
		}
		
		for (drwnCliqueTree<G>::parent_iterator it = beginParents(current, from); it != endParents(current, from); it++) {
			stack.push_back(pair<int, int>(*it, current));
		}
	}

	return diffedVars;

}