/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnSampleCliques.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace std;

//! This class constructs an array of clique trees of increasing subsets of the graphical model, required for sampling.
template <class G>
class drwnSampleCliques
{
private:
	//! Array of clique trees for variables up to and including variable (to calculate f(x1, ..., xi)
	vector<drwnCliqueTree<G>* > _cliqueTrees;

	//! Array of clique trees for variables up to but not including variable (to calculate P(x1 = c1, x2 = c2, ..., xi <= ci)
	vector<drwnCliqueTree<G>* > _cliqueTreeMinuses;

	//! Factors that contain variable but are not in the minus clique tree. We need to add these in manually when doing inference
	vector<vector<int> > _extraFactors;

	//! Order in which to perform sampling
	vector<int> _samplingOrder;

	//! List of factors containing variables that are conditioned on
	vector<unsigned int> _condFactors;

	//! List of variables to condition on
	vector<unsigned int> _condVars;

public:
	//! Construct array of clique trees for graphical model.
	drwnSampleCliques(const G& g);

	//! Construct array of sampling clique trees given subset of variables for graphical model.
	drwnSampleCliques(const G& g, const set<int>& vars);

	//! Default destructor
	~drwnSampleCliques();

	//! Return graphical model over which cliques are defined
	G& model() { return _cliqueTrees[_samplingOrder[0]]->model(); }

	//! Return clique tree for variable i
	drwnCliqueTree<G>& cliqueTree(unsigned int i) const { return *_cliqueTrees[i];}

	//! Return minus clique tree for variable i
	drwnCliqueTree<G>& cliqueTreeMinus(unsigned int i) const { return *_cliqueTreeMinuses[i];}

	const vector<unsigned int>& condFactors() const { return _condFactors; }

	const vector<unsigned int>& condVars() const { return _condVars; }

	//! Return i-th variable in sampling order
	int samplingOrder(unsigned int i) { return _samplingOrder[i]; } 

	//! Return number of variables to sample (this may be less than number of model variables when second constructor is called)
	unsigned int numVars() { return _samplingOrder.size(); }

	//! Return extra factors for variable
	const vector<int>& extraFactors(unsigned int i) const { return _extraFactors[i]; }
};

typedef drwnSampleCliques<normalCumulativeNet> normalSampleCliques;

typedef drwnSampleCliques<debugCumulativeNet> debugSampleCliques;

// Construct array of clique trees for graphical model.
template <class G>
drwnSampleCliques<G>::drwnSampleCliques(const G& g)
{
	_cliqueTrees.resize(g.numVariables());
	_cliqueTreeMinuses.resize(g.numVariables());
	_extraFactors.resize(g.numVariables());
	_samplingOrder.resize(g.numVariables());

	// Create neighbours lists
	// TODO: Move this into drwnCumulativeNet.h
	vector<set<int> > neighbours(g.numVariables());
	for (int i = 0; i < g.numFactors(); i++) {
		// For nonempty factors
		const G::factor_type* factor = &g.factor(i);
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

	// Insert leaves into to-do list
	set<int> todo;
	for (int i = 0; i < g.numVariables(); i++) {
		if (neighbours[i].size() < 2)
			todo.insert(i);
	}

	// If no leaf, then insert one variable at random
	if (!todo.size())
		todo.insert(rand() % g.numVariables());

	// Loop over to-do list, and create cliques for subgraphs, greedily adding variables
	unsigned int numRemaining = g.numVariables();
	vector<bool> isUsed(g.numVariables(), false);
	vector<set<int>* > vars(g.numVariables(), 0);

	// TODO: Can rewrite some logic of following now that have branchVar??
	map<set<int>*, int> branchVar;	// maps between branch and variable containing most recent clique tree on branch

	// DEBUG: Used to fix ordering
	//unsigned int thisVar = 0;
	while(numRemaining) {
		set<set<int>*> currentBranches;
		while(todo.size()) {
			// Determine variables with minimum weight
			unsigned int minWeight = numeric_limits<int>::max();
			vector<int> minVars;
			set<set<int>*> branches;

			//int lastNeighbour = 0;	<- this didn't work for finding minus clique, so used branchVar instead
			for (set<int>::iterator it = todo.begin(); it != todo.end(); it++) {
				// Loop over neighbours of variable
				unsigned int weight = 0;
				for (set<int>::iterator jt = neighbours[*it].begin(); jt != neighbours[*it].end(); jt++) {
					// Calculate unique branches on neighbours
					if (isUsed[*jt]) {
						//weight += vars[*jt]->size();
						branches.insert(vars[*jt]);
						//lastNeighbour = *jt;
					}
				}

				// Sum weight of unique branches
				for (set<set<int>*>::iterator jt = branches.begin(); jt != branches.end(); jt++)
					weight += (*jt)->size();
				branches.clear();

				// Adjust min weight
				if (weight < minWeight) {
					minVars.clear();
					minVars.push_back(*it);
					minWeight = weight;

					// Break immediately if zero weight
					if (!weight)
						break;
				}
				else if (weight == minWeight)
					minVars.push_back(*it);
			}

			// Choose minimum weight variable
			int minVar;
			if (minVars.size() == 1)
				minVar = minVars[0];
			else
				minVar = minVars[rand() % minVars.size()];

			// DEBUG: Sample in order 0, 1, 2, ...
			//minVar = thisVar++;

			// Create clique tree for that variable and add non-used neighbours to to-do list
			set<int>* maxBranch = 0;
			unsigned int maxBranchSize = 0;
			for (set<int>::iterator jt = neighbours[minVar].begin(); jt != neighbours[minVar].end(); jt++) {
				if (isUsed[*jt]) {
					// Add branch to set (we are counting *unique* branches)
					branches.insert(vars[*jt]);
					if (vars[*jt]->size() > maxBranchSize) {
						maxBranchSize = vars[*jt]->size();
						maxBranch = vars[*jt];
					}
				}
				else
					todo.insert(*jt);
			}

			// If no branches (no used neighbours), then create new branch
			if (!branches.size()) {
				vars[minVar] = new set<int>();
				currentBranches.insert(vars[minVar]);
				_cliqueTreeMinuses[minVar] = 0;
				branchVar.insert(pair<set<int>*, int>(vars[minVar], minVar));
			}

			// Else if we have more than one unique branch, we will need to merge them
			else if (branches.size() > 1) {
				// Loop over branches
				for (set<set<int>*>::iterator it = branches.begin(); it != branches.end(); it++) {
					// Skip maximum branch
					if (*it == maxBranch)
						continue;

					// Loop over variables in branch and merge branches
					for (set<int>::iterator jt = (*it)->begin(); jt != (*it)->end(); jt++)
						// Associate other variables with the maximum branch
						vars[*jt] = maxBranch;

					// Insert variables in these branches with maximum branch
					maxBranch->insert((*it)->begin(), (*it)->end());

					// Free branch
					currentBranches.erase(*it);
					branchVar.erase(*it);
					delete *it;
				}

				// Set branch for current variable
				vars[minVar] = maxBranch;

				// DEBUG: Trying to work out what variable it fails on
				//if (numRemaining == 22)
				//	bool gotHere = true;

				// Create minus clique tree
				_cliqueTreeMinuses[minVar] = new drwnCliqueTree<G>(g, *vars[minVar]);
			}

			// Else if just one variable
			else if (branches.size() == 1) {
				// Set branch for current variable
				vars[minVar] = *branches.begin();

				// Set minus clique tree to previous clique tree
				_cliqueTreeMinuses[minVar] = _cliqueTrees[branchVar.find(*branches.begin())->second];


			}

			// Work out what factors are missing
			if (branches.size()) {
				// Associate current variable with maximum branch
				branchVar.find(maxBranch)->second = minVar;

				// Fetch factors that contain current variable
				set<int> factors = g.varNeighbors(minVar);

				// Loop through factors in clique tree, remove from set of factors that contain current variable
				for (int i = 0; i < _cliqueTreeMinuses[minVar]->numCliques(); i++) {
					drwnCliqueTree<G>& tempCt = *_cliqueTreeMinuses[minVar];
					for (vector<int>::const_iterator jt = tempCt.factors(i).begin(); jt != tempCt.factors(i).end(); jt++)
						factors.erase(*jt);
				}

				// Store remaining factors
				_extraFactors[minVar].insert(_extraFactors[minVar].begin(), factors.begin(), factors.end());
			}
			
			// Insert variable into branch, create clique tree, etc.
			vars[minVar]->insert(minVar);
			if (minVar == 1)
				bool gotHere = true;
			_cliqueTrees[minVar] = new drwnCliqueTree<G>(g, *vars[minVar]);
			isUsed[minVar] = true;
			todo.erase(minVar);
			_samplingOrder[g.numVariables() - numRemaining--] = minVar;
			//branches.clear();
		}

		// Free branches
		for (set<set<int>*>::iterator it = currentBranches.begin(); it != currentBranches.end(); it++)
			delete *it;

		// If variables left, we must have disjoint graph, so find a variable that hasn't been used and start over
		if (numRemaining) {
			int nextVar = 0;
			while(isUsed[nextVar])
				nextVar++;
			todo.insert(nextVar);
		}
	}
}

// Construct array of sampling clique trees given subset of variables for graphical model.
template <class G>
drwnSampleCliques<G>::drwnSampleCliques(const G& g, const set<int>& vars)
{
	_cliqueTrees.resize(g.numVariables());
	_cliqueTreeMinuses.resize(g.numVariables());
	_extraFactors.resize(g.numVariables());
	_samplingOrder.resize(g. numVariables() - vars.size());

	// Create list of factors that contain vars that are conditioned on
	set<unsigned int> uniqueFactors;
	for (auto& v: vars)
		uniqueFactors.insert(g.varNeighbors(v).begin(), g.varNeighbors(v).end());
	_condFactors.reserve(uniqueFactors.size());
	_condFactors.insert(_condFactors.begin(), uniqueFactors.begin(), uniqueFactors.end());
	uniqueFactors.clear();
	_condVars.insert(_condVars.begin(), vars.begin(), vars.end());
	
	// Create neighbours lists
	// TODO: Move this into drwnCumulativeNet.h
	vector<set<int> > neighbours(g.numVariables());
	for (unsigned int i = 0; i < g.numFactors(); i++) {
		// For nonempty factors
		const G::factor_type* factor = &g.factor(i);
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

	// Loop over to-do list, and create cliques for subgraphs, greedily adding variables
	vector<bool> isUsed(g.numVariables(), false);
	vector<set<int>* > varBranch(g.numVariables(), 0);	// for each variable, marks which branch it is associated with
	map<set<int>*, int> branchVar;	// maps between branch and variable containing most recent clique tree on branch
	set<set<int>*> currentBranches;
	set<set<int>*> branches;

	// Loop over vars that have been conditioned on
	// NOTE: Trying a range based "for" for the first time!
	set<int> todo;
	for (auto& v: vars) {
		// Determine adjacent branches and add non-used neighbours to to-do list
		set<int>* bMax = 0; unsigned int bMaxSize = 0;
		for (auto& u : neighbours[v]) {
			if (isUsed[u]) {
				// Add branch to set (we are counting *unique* branches)
				branches.insert(varBranch[u]);
				if (varBranch[u]->size() > bMaxSize) {
					bMaxSize = varBranch[u]->size();
					bMax = varBranch[u];
				}
			}
			else if (vars.find(u) == vars.end())
				todo.insert(u);
		}

		// If no branches (no used neighbours), then create new branch
		if (!branches.size()) {
			varBranch[v] = new set<int>();
			currentBranches.insert(varBranch[v]);
			branchVar.insert(pair<set<int>*, int>(varBranch[v], v));
		}

		// Else if we have more than one unique branch, we will need to merge them
		else if (branches.size() > 1) {
			// Loop over branches
			for (auto& b: branches) {
				// Skip maximum branch
				if (b == bMax)
					continue;

				// Associate other variables with the maximum branch
				for (auto u: *b)
					varBranch[u] = bMax;

				// Insert variables in these branches with maximum branch
				bMax->insert(b->begin(), b->end());

				// Free branch
				currentBranches.erase(b);
				branchVar.erase(b);
				delete b;
			}

			// Set branch for current variable
			varBranch[v] = bMax;
		}

		// Else if just one variable
		else if (branches.size() == 1)
			// Set branch for current variable
			varBranch[v] = *branches.begin();

		// Associate current variable with maximum branch
		if (branches.size())
			branchVar.find(bMax)->second = v;

		// Insert variable into branch, etc.
		varBranch[v]->insert(v);
		_cliqueTrees[v] = 0;
		_cliqueTreeMinuses[v] = 0;
		isUsed[v] = true;
		branches.clear();
	}

	// Create clique trees over branches
	//for (auto& b : branchVar)
	//	_cliqueTrees[b.second] = new drwnCliqueTree<G>(g, *b.first);

	// Add unused leaves
	for (unsigned int i = 0; i < g.numVariables(); i++) {
		if ((neighbours[i].size() < 2) && !isUsed[i])
			todo.insert(i);
	}

	// Loop over variables that weren't conditioned on
	unsigned int numRemaining = g.numVariables() - vars.size();		
	while(numRemaining) {
		while(todo.size()) {
			// Determine variables with minimum weight
			unsigned int minWeight = numeric_limits<int>::max();
			vector<int> minVars;
			
			//int lastNeighbour = 0;	<- this didn't work for finding minus clique, so used branchVar instead
			for (set<int>::iterator it = todo.begin(); it != todo.end(); it++) {
				// Loop over neighbours of variable
				unsigned int weight = 0;
				for (set<int>::iterator jt = neighbours[*it].begin(); jt != neighbours[*it].end(); jt++) {
					// Calculate unique branches on neighbours
					if (isUsed[*jt]) {
						//weight += vars[*jt]->size();
						branches.insert(varBranch[*jt]);
						//lastNeighbour = *jt;
					}
				}

				// Sum weight of unique branches
				for (set<set<int>*>::iterator jt = branches.begin(); jt != branches.end(); jt++)
					weight += (*jt)->size();
				branches.clear();

				// Adjust min weight
				if (weight < minWeight) {
					minVars.clear();
					minVars.push_back(*it);
					minWeight = weight;

					// Break immediately if zero weight
					if (!weight)
						break;
				}
				else if (weight == minWeight)
					minVars.push_back(*it);
			}

			// Choose minimum weight variable
			int minVar;
			if (minVars.size() == 1)
				minVar = minVars[0];
			else
				minVar = minVars[rand() % minVars.size()];

			// DEBUG: Sample in order 0, 1, 2, ...
			//minVar = thisVar++;

			// Determine adjacent branches and add non-used neighbours to to-do list
			set<int>* maxBranch = 0;
			unsigned int maxBranchSize = 0;
			for (set<int>::iterator jt = neighbours[minVar].begin(); jt != neighbours[minVar].end(); jt++) {
				if (isUsed[*jt]) {
					// Add branch to set (we are counting *unique* branches)
					branches.insert(varBranch[*jt]);
					if (varBranch[*jt]->size() > maxBranchSize) {
						maxBranchSize = varBranch[*jt]->size();
						maxBranch = varBranch[*jt];
					}
				}
				else
					todo.insert(*jt);
			}

			// If no branches (no used neighbours), then create new branch
			if (!branches.size()) {
				varBranch[minVar] = new set<int>();
				currentBranches.insert(varBranch[minVar]);
				_cliqueTreeMinuses[minVar] = 0;
				branchVar.insert(pair<set<int>*, int>(varBranch[minVar], minVar));
			}

			// Else if we have more than one unique branch, we will need to merge them
			else if (branches.size() > 1) {
				// Loop over branches
				for (set<set<int>*>::iterator it = branches.begin(); it != branches.end(); it++) {
					// Skip maximum branch
					if (*it == maxBranch)
						continue;

					// Loop over variables in branch and merge branches
					for (set<int>::iterator jt = (*it)->begin(); jt != (*it)->end(); jt++)
						// Associate other variables with the maximum branch
							varBranch[*jt] = maxBranch;

					// Insert variables in these branches with maximum branch
					maxBranch->insert((*it)->begin(), (*it)->end());

					// Free branch
					currentBranches.erase(*it);
					branchVar.erase(*it);
					delete *it;
				}

				// Set branch for current variable
				varBranch[minVar] = maxBranch;

				// Create minus clique tree
				_cliqueTreeMinuses[minVar] = new drwnCliqueTree<G>(g, *varBranch[minVar]);
			}

			// Else if just one variable
			else if (branches.size() == 1) {
				// Set branch for current variable
				varBranch[minVar] = *branches.begin();

				// Set minus clique tree to previous clique tree (if exists)
				if (_cliqueTrees[branchVar.find(*branches.begin())->second])
					_cliqueTreeMinuses[minVar] = _cliqueTrees[branchVar.find(*branches.begin())->second];
				else
					_cliqueTreeMinuses[minVar] = new drwnCliqueTree<G>(g, *varBranch[minVar]);
			}

			// Work out what factors are missing
			if (branches.size()) {
				// Associate current variable with maximum branch
				branchVar.find(maxBranch)->second = minVar;

				// Fetch factors that contain current variable
				set<int> factors = g.varNeighbors(minVar);

				// Loop through factors in clique tree, remove from set of factors that contain current variable
				for (unsigned int i = 0; i < _cliqueTreeMinuses[minVar]->numCliques(); i++) {
					drwnCliqueTree<G>& tempCt = *_cliqueTreeMinuses[minVar];
					for (vector<int>::const_iterator jt = tempCt.factors(i).begin(); jt != tempCt.factors(i).end(); jt++)
						factors.erase(*jt);
				}

				// Store remaining factors
				_extraFactors[minVar].insert(_extraFactors[minVar].begin(), factors.begin(), factors.end());
			}

			// Insert variable into branch, create clique tree, etc.
			varBranch[minVar]->insert(minVar);
			if (minVar == 1)
				bool gotHere = true;
			_cliqueTrees[minVar] = new drwnCliqueTree<G>(g, *varBranch[minVar]);
			isUsed[minVar] = true;
			todo.erase(minVar);
			_samplingOrder[g.numVariables() - vars.size() - numRemaining--] = minVar;
			branches.clear();
		}

		// Free branches
		for (set<set<int>*>::iterator it = currentBranches.begin(); it != currentBranches.end(); it++)
			delete *it;

		// If variables left, we must have disjoint graph, so find a variable that hasn't been used and start over
		if (numRemaining) {
			int nextVar = 0;
			while(isUsed[nextVar])
				nextVar++;
			todo.insert(nextVar);
		}
	}
}

// Default destructor
template <class G>
drwnSampleCliques<G>::~drwnSampleCliques()
{
	// TODO: Replace with smart pointers
	// Calculate all unique clique trees
	set<drwnCliqueTree<G>* > cliqueTrees;
	for (auto& ct : _cliqueTrees) {
		if (ct)
			cliqueTrees.insert(ct);
	}
	for (auto& ct : _cliqueTreeMinuses) {
		if (ct)
			cliqueTrees.insert(ct);
	}

	// Free memory
	for (auto& ct : cliqueTrees)
		delete ct;
}