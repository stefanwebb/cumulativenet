/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCumulativeDebug.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//#include "drwnCumulativeNet.h"

using namespace std;

enum ShapemodelType { SQUARE, DIAGONAL, FOUR, OVERLAP, MISTAKE};

//! Functions to create debug cumulative networks
namespace drwnCumulativeDebug
{
	//! Create two factor network with two variable scopes that completely overlap
	template <class G>
	G& createOneCliqueNet();

	//! Create two factor network with two variable scopes that overlap in one variable
	template <class G>
	G& createTwoFactorNet();

	//! Create student example (pg. ?) for arbitrary network type
	template <class G>
	G& createStudentNet();

	//! Create extended student example for debugging
	template <class G>
	G& createExtendedStudentNet();

	//! Create a network with two disjoint cliques
	template <class G>
	G& createDisjointFourCliques();

	//! Create mini student example (pg. ?) for arbitrary network type
	template <class G>
	G& createMiniStudentNet();

	//! Create one factor network with 2 variables
	template <class G>
	G& createOneFactorNet();

	//! Create one factor network with n variables
	template <class G>
	G& createOneFactorNet(unsigned int n);

	//! Create chain of bivariate factors
	template <class G>
	G& createBivariateChain(unsigned int n);

	//! Create binary tree with n levels
	template <class G>
	G& createBinaryTree(unsigned int n);

	//! Create loop of bivariate factors
	template <class G>
	G& createBivariateLoop(unsigned int n);

	//! Create network over n x m grid
	template <class G>
	G& createGrid(unsigned int n, unsigned int m);

	//! Create shape model over grid of size 2^n x 2^n
	template <class G>
	G& createShapeModel(unsigned int n, ShapemodelType type);

	//! Create network with a clique with n parents
	template <class G>
	G& createNParents(unsigned int n);

	//! Load shape model from factors and marginals files
	template <class G>
	G& loadShapeModel(unsigned int n, wchar_t* marginalsFile, wchar_t* factorsFile);

	//! Function that waits until user has pressed a key. Stops program from exiting before we have examined output
	void pressAnyKey();
};

// Create two factor network with two variable scopes that completely overlap
template <class G>
G& drwnCumulativeDebug::createOneCliqueNet()
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "X1");
    universe->addVariable(2, "X2");
		
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[2];
	for (unsigned int i = 0; i < 2; i++)
		fs[i] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	c.insert(1);
	fs[0]->setVariables(c);

	fs[1]->setVariables(c);
	
	// Add factors to cumulative network
	for (unsigned int i = 0; i < 2; i++)
		cn.addFactor(fs[i]);

	// Return network
	return cn;
}

// Create one factor network
template <class G>
G& drwnCumulativeDebug::createOneFactorNet()
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "X1");
    universe->addVariable(2, "X2");
	//universe->addVariable(2, "X3");
	//universe->addVariable(2, "X4");
		
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[1];
	fs[0] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	c.insert(1);
	//c.insert(2);
	//c.insert(3);
	fs[0]->setVariables(c);

	// Add factors to cumulative network
	cn.addFactor(fs[0]);

	// Return network
	return cn;
}

// Create network with a clique with n parents
template <class G>
G& drwnCumulativeDebug::createNParents(unsigned int n)
{
	DRWN_ASSERT(n)

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "X1");

	for (unsigned int i = 0; i < n; i++) {
		universe->addVariable(2, (string("X") + std::to_string(i + 2)).c_str());
	}

	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	for (unsigned int i = 0; i < n; i++) {
		G::factor_type* fs = new G::factor_type;
		drwnClique c;
		c.insert(0);
		c.insert(i + 1);
		fs->setVariables(c);
		cn.addFactor(fs);
	}

	// Return network
	return cn;
}

// Create one factor network
template <class G>
G& drwnCumulativeDebug::createOneFactorNet(unsigned int n)
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	for (unsigned int i = 0; i < n; i++)
		universe->addVariable(2, (string("X") + std::to_string(i + 1)).c_str());
    		
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[1];
	fs[0] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	for (unsigned int i = 0; i < n; i++)
		c.insert(i);
	
	fs[0]->setVariables(c);

	// Add factors to cumulative network
	cn.addFactor(fs[0]);

	// Return network
	return cn;
}

// Create chain of bivariate factors
template <class G>
G& drwnCumulativeDebug::createBivariateChain(unsigned int n)
{
	// Note: can't have zero factors
	DRWN_ASSERT_MSG(n > 0, "Cannot create a bivariate chain with zero factors.")

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	for (unsigned int i = 0; i < n + 1; i++)
		universe->addVariable(2, (string("X") + std::to_string(i + 1)).c_str());
    		
	// Create empty network
	G& cn = *(new G(universe));

	cn.name().assign("Chain");
	cn.size() = n + 1;
	
	drwnClique c;
	for (unsigned int i = 0; i < n; i++) {
		// Allocate factor
		G::factor_type* fs = new G::factor_type;

		// Fill variables for factors
		c.clear();
		c.insert(i);
		c.insert(i + 1);
		fs->setVariables(c);

		// Add factor to cumulative network
		cn.addFactor(fs);
	}
	
	// Return network
	return cn;
}

// Create loop of bivariate factors (with n variables / factors)
template <class G>
G& drwnCumulativeDebug::createBivariateLoop(unsigned int n)
{
	// Note: Don't allow this factor to create a loop with 2 variables / factors. Use one clique function instead.
	DRWN_ASSERT_MSG(n > 2, "Bivariate loop requires 3 or more variables / factors.")

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	for (unsigned int i = 0; i < n; i++)
		universe->addVariable(2, (string("X") + std::to_string(i + 1)).c_str());
    		
	// Create empty network
	G& cn = *(new G(universe));

	cn.name().assign("Loop");
	cn.size() = n;
	
	// Create
	drwnClique c;
	for (unsigned int i = 0; i < n; i++) {
		// Allocate factor
		G::factor_type* fs = new G::factor_type;

		// Fill variables for factors
		c.clear();
		c.insert(i);
		c.insert((i < n - 1) ? (i + 1) : 0);
		fs->setVariables(c);

		// Add factor to cumulative network
		cn.addFactor(fs);
	}
	
	// Return network
	return cn;
}

// Load shape model from factors and marginals files
template <class G>
G& drwnCumulativeDebug::loadShapeModel(unsigned int n, wchar_t* marginalsFile, wchar_t* factorsFile)
{
	// *** CONTINUE FROM HERE AFTER MCDONALDS! ***

	DRWN_ASSERT_MSG(n > 0, "Invalid size for shape model.")

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	// Create variables and label by X-Y coordinates
	for (unsigned int j = 0; j < (1 << n); j++) {
		for (unsigned int i = 0; i < (1 << n); i++)
			universe->addVariable(2, (string("X") + std::to_string(i + 1) + string("Y") + std::to_string(j + 1)).c_str());
	}

	// Create empty network
	G& cn = *(new G(universe));

	// Read factors
	wifstream factors;
	factors.open(factorsFile);
	factors.precision(std::numeric_limits<double>::digits10);
	unsigned int numFactors;
	factors >> numFactors;

	for (unsigned int i = 0; i < numFactors; i++) {
		G::factor_type* fs = new G::factor_type;
		drwnClique vars;

		unsigned int numVars;
		factors >> numVars;
		for (unsigned int j = 0; j < numVars; j++) {
			unsigned int newVar;
			factors >> newVar;
			vars.insert(newVar);
		}
		
		fs->setVariables(vars);
		cn.addFactor(fs);
	}

	// Set parameters
	for (unsigned int i = 0; i < numFactors; i++) {
		double newPar;
		factors >> newPar;
		cn.factor(i).setParameter(newPar);
	}

	// Set levels
	for (unsigned int i = 0; i < numFactors; i++) {
		unsigned int level;
		factors >> level;
		cn.factor(i).level() = level;
	}
	factors.close();

	// Set marginals
	wifstream marginals;
	marginals.open(marginalsFile);
	marginals.precision(std::numeric_limits<double>::digits10);
	unsigned int numVars;
	marginals >> numVars;
	DRWN_ASSERT_MSG(numVars == (1 << n) * (1 << n), "Invalid number of marginals")
	for (unsigned int i = 0; i < numVars; i++) {
		double newMean, newSd;
		marginals >> newMean;
		marginals >> newSd;
		cn.marginal(i).setMean(newMean);
		cn.marginal(i).setStandardDeviation(newSd);
	}

	return cn;
}

// Create shape model over grid of size 2^n x 2^n
template <class G>
G& drwnCumulativeDebug::createShapeModel(unsigned int n, ShapemodelType type)
{
	DRWN_ASSERT_MSG(n > 0, "Invalid size for shape model.")

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	// Create variables and label by X-Y coordinates
	for (unsigned int j = 0; j < (1 << n); j++) {
		for (unsigned int i = 0; i < (1 << n); i++)
			universe->addVariable(2, (string("X") + std::to_string(i + 1) + string("Y") + std::to_string(j + 1)).c_str());
	}

	// Create empty network
	G& cn = *(new G(universe));

	// Structure for stack
	struct ShapeRegion
	{
		// Region and a point within region are specified
		unsigned int xStart, xEnd, yStart, yEnd, xPoint, yPoint, level;

		ShapeRegion() {}
		ShapeRegion(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e, unsigned int f, unsigned int l) : xStart(a), xEnd(b), yStart(c), yEnd(d), xPoint(e), yPoint(f), level(l) {}
	};

	// Push first element onto stack
	vector<ShapeRegion> stack;
	stack.push_back(ShapeRegion(0, (1 << n) - 1, 0, (1 << n) - 1, rand() % (1 << n), rand() % (1 << n), 0));

	// Loop until stack is empty
	while (stack.size())
	{
		ShapeRegion thisRegion = stack.back();
		stack.pop_back();

		// Put the point within region into factor
		vector<int> c;
		//c.insert(thisRegion.xPoint + thisRegion.yPoint * (2 << n));
		
		// We need to divide this region into 4 parts
		for (unsigned int i = 0; i < 2; i++) {
			for (unsigned int j = 0; j < 2; j++) {
				ShapeRegion newRegion;

				// Calculate the dimensions of this region
				// NOTE: xWidth == yWidth, but I leave in anyway...
				unsigned int xWidth = (thisRegion.xEnd - thisRegion.xStart + 1) / 2, yWidth = (thisRegion.yEnd - thisRegion.yStart + 1) / 2;
				newRegion.xStart = thisRegion.xStart + i * xWidth, newRegion.xEnd = thisRegion.xStart + (i + 1) * xWidth - 1;
				newRegion.yStart = thisRegion.yStart + j * yWidth, newRegion.yEnd = thisRegion.yStart + (j + 1) * yWidth - 1;

				// If random point was in this region then no need to choose new point
				if (newRegion.xStart <= thisRegion.xPoint && thisRegion.xPoint <= newRegion.xEnd && newRegion.yStart <= thisRegion.yPoint && thisRegion.yPoint <= newRegion.yEnd) {
					newRegion.xPoint = thisRegion.xPoint;
					newRegion.yPoint = thisRegion.yPoint;
				}

				// Otherwise choose a random point and push new region to stack
				else {
					newRegion.xPoint = newRegion.xStart + rand() % xWidth;
					newRegion.yPoint = newRegion.yStart + rand() % yWidth;
				}

				c.push_back(newRegion.xPoint + newRegion.yPoint * (1 << n));

				newRegion.level = thisRegion.level + 1;
				
				// Stop recursion when reach 4 pixels next to each other
				if (xWidth > 1 && yWidth > 1)
					stack.push_back(newRegion);
			}
		}

		// This was how it was originally, it wasn't what I intended however...
		if (type == MISTAKE) {
			// Pairwise factor excluding diagonal
			for (unsigned int i = 0; i < c.size() - 1; i++) {
				G::factor_type* fs = new G::factor_type;
				drwnClique cPairwise;
				cPairwise.insert(c[i]);
				cPairwise.insert(c[i + 1]);
				fs->setVariables(cPairwise);
				fs->level() = thisRegion.level;
				cn.addFactor(fs);
			}
		}

		// Add these four variables to the model
		else if (type == FOUR) {
			G::factor_type* fs = new G::factor_type;
			drwnClique vars;
			vars.insert(c[0]);
			vars.insert(c[1]);
			vars.insert(c[2]);
			vars.insert(c[3]);
			fs->setVariables(vars);
			fs->level() = thisRegion.level;
			cn.addFactor(fs);
		}

		else if (type == SQUARE || type == DIAGONAL || type == OVERLAP) {
			G::factor_type* fs[4];
			for (unsigned int i = 0; i < 4; i++)
				fs[i] = new G::factor_type;
			drwnClique vars;

			vars.insert(c[0]);
			vars.insert(c[1]);
			fs[0]->setVariables(vars);
			fs[0]->level() = thisRegion.level;
			cn.addFactor(fs[0]);

			vars.clear();
			vars.insert(c[1]);
			vars.insert(c[3]);
			fs[1]->setVariables(vars);
			fs[1]->level() = thisRegion.level;
			cn.addFactor(fs[1]);

			vars.clear();
			vars.insert(c[2]);
			vars.insert(c[3]);
			fs[2]->setVariables(vars);
			fs[2]->level() = thisRegion.level;
			cn.addFactor(fs[2]);

			vars.clear();
			vars.insert(c[0]);
			vars.insert(c[2]);
			fs[3]->setVariables(vars);
			fs[3]->level() = thisRegion.level;
			cn.addFactor(fs[3]);
		}

		// Add in diagonals
		if (type == DIAGONAL || type == OVERLAP) {
			G::factor_type* fs[2];
			for (unsigned int i = 0; i < 2; i++)
				fs[i] = new G::factor_type;
			drwnClique vars;

			vars.insert(c[0]);
			vars.insert(c[3]);
			fs[0]->setVariables(vars);
			fs[0]->level() = thisRegion.level;
			cn.addFactor(fs[0]);

			vars.clear();
			vars.insert(c[1]);
			vars.insert(c[2]);
			fs[1]->setVariables(vars);
			fs[1]->level() = thisRegion.level;
			cn.addFactor(fs[1]);
		}

		// Add in overlap
		if (type == OVERLAP) {
			unsigned int width = 1 << n;
			// Only add extra factors for highest level (adjacent pixels)
			if (c[2] == c[0] + 1) {

			// Don't want to add in variables outside image
			if (((c[1] / width) < (width - 1)) && ((c[2] % width) < (width - 1))) {
				G::factor_type* fs[8];
				for (unsigned int i = 0; i < 8; i++)
					fs[i] = new G::factor_type;
				drwnClique vars;

				vars.insert(c[2]);
				vars.insert(c[2] + 1);
				fs[0]->setVariables(vars);
				fs[0]->level() = thisRegion.level;
				cn.addFactor(fs[0]);

				vars.clear();
				vars.insert(c[3]);
				vars.insert(c[3] + 1);
				fs[1]->setVariables(vars);
				fs[1]->level() = thisRegion.level;
				cn.addFactor(fs[1]);

				vars.clear();
				vars.insert(c[1] + width);
				vars.insert(c[1] + width + 1);
				fs[2]->setVariables(vars);
				fs[2]->level() = thisRegion.level;
				cn.addFactor(fs[2]);

				vars.clear();
				vars.insert(c[3] + width);
				vars.insert(c[3] + width + 1);
				fs[3]->setVariables(vars);
				fs[3]->level() = thisRegion.level;
				cn.addFactor(fs[3]);

				vars.clear();
				vars.insert(c[1]);
				vars.insert(c[1] + width);
				fs[4]->setVariables(vars);
				fs[4]->level() = thisRegion.level;
				cn.addFactor(fs[4]);

				vars.clear();
				vars.insert(c[3]);
				vars.insert(c[3] + width);
				fs[5]->setVariables(vars);
				fs[5]->level() = thisRegion.level;
				cn.addFactor(fs[5]);

				vars.clear();
				vars.insert(c[2] + 1);
				vars.insert(c[3] + 1);
				fs[6]->setVariables(vars);
				fs[6]->level() = thisRegion.level;
				cn.addFactor(fs[6]);

				vars.clear();
				vars.insert(c[3] + 1);
				vars.insert(c[3] + width + 1);
				fs[7]->setVariables(vars);
				fs[7]->level() = thisRegion.level;
				cn.addFactor(fs[7]);
			}

			}
		}
	};

	return cn;
}

// Create network over n x m grid
template <class G>
G& drwnCumulativeDebug::createGrid(unsigned int n, unsigned int m)
{
	// Note: can't have zero factors
	DRWN_ASSERT_MSG((n > 0 && m > 0) && (n > 1 || m > 1), "Invalid dimensions for creating grid.")

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	for (unsigned int j = 0; j < m; j++) {
		for (unsigned int i = 0; i < n; i++)
			universe->addVariable(2, (string("X") + std::to_string(i + 1) + string("Y") + std::to_string(j + 1)).c_str());
	}
    		
	// Create empty network
	G& cn = *(new G(universe));

	cn.name().assign("Grid");
	if (n == m)
		cn.size() = n;
	
	// Loop over each variable, work out factors
	drwnClique c;
	for (unsigned int j = 0; j < m; j++) {
		for (unsigned int i = 0; i < n; i++) {
			// For all but last column, need to add factor going left to right across grid
			if (i < n - 1) {
				G::factor_type* fs = new G::factor_type;

				c.clear();
				c.insert(i + j * n);
				c.insert((i + 1) + j * n);
				fs->setVariables(c);

				// Add factor to cumulative network
				cn.addFactor(fs);
			}

			// For all but last row, need to add factor going top to bottom across grid
			if (j < m - 1) {
				G::factor_type* fs = new G::factor_type;

				c.clear();
				c.insert(i + j * n);
				c.insert(i + (j + 1) * n);
				fs->setVariables(c);

				// Add factor to cumulative network
				cn.addFactor(fs);
			}
		}
	}
	
	// Return network
	return cn;
}

// Create binary tree with n levels
template <class G>
G& drwnCumulativeDebug::createBinaryTree(unsigned int n)
{
	DRWN_ASSERT_MSG(n > 1, "Binary tree must have more than 1 level!")

	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());

	for (unsigned int i = 0; i < (1 << n) - 1; i++)
		universe->addVariable(2, (string("X") + std::to_string(i + 1)).c_str());
    		
	// Create empty network
	G& cn = *(new G(universe));

	cn.name().assign("Tree");
	cn.size() = n;

	// Loop over levels of factors (not variables)
	drwnClique c;
	unsigned int factorNum = 0;
	for (unsigned int i = 0; i < n - 1; i++) {
		// Loop over variables above factors and connect to variable below
		for (unsigned int j = (1 << i) - 1; j <=  2 * ((1 << i) - 1); j++) {
			// Fill variables for factors
			G::factor_type* fs = new G::factor_type;
			c.clear();
			c.insert(j);
			c.insert(2 * j + 1);
			fs->setVariables(c);
			cn.addFactor(fs);

			fs = new G::factor_type;
			c.clear();
			c.insert(j);
			c.insert(2 * j + 2);
			fs->setVariables(c);
			cn.addFactor(fs);
		}
	}

	// Return network
	return cn;
}

// Create two factor network with two variable scopes that overlap in one variable 
template <class G>
G& drwnCumulativeDebug::createTwoFactorNet()
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "X1");
    universe->addVariable(2, "X2");
	universe->addVariable(2, "X3");
	
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[2];
	for (unsigned int i = 0; i < 2; i++)
		fs[i] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	c.insert(1);
	fs[0]->setVariables(c);

	c.clear();
	c.insert(1);
	c.insert(2);
	fs[1]->setVariables(c);
	
	// Add factors to cumulative network
	for (unsigned int i = 0; i < 2; i++)
		cn.addFactor(fs[i]);

	// Return network
	return cn;
}

// Create student example (pg. ?) for arbitrary network type
template <class G>
G& drwnCumulativeDebug::createStudentNet()
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "C");	// 0
    universe->addVariable(2, "D");	// 1
	universe->addVariable(2, "I");	// 2
	universe->addVariable(2, "G");	// 3
	universe->addVariable(2, "S");	// 4
	universe->addVariable(2, "L");	// 5
	universe->addVariable(2, "J");	// 6
	universe->addVariable(2, "H");	// 7

	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[6];
	for (unsigned int i = 0; i < 6; i++)
		fs[i] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	c.insert(1);
	fs[0]->setVariables(c);
	c.clear();
	c.insert(1);
	c.insert(2);
	c.insert(3);
	fs[1]->setVariables(c);
	c.clear();
	c.insert(4);
	c.insert(2);
	fs[2]->setVariables(c);
	c.clear();
	c.insert(3);
	c.insert(7);
	fs[3]->setVariables(c);
	c.clear();
	c.insert(5);
	c.insert(3);
	fs[4]->setVariables(c);
	c.clear();
	c.insert(6);
	c.insert(5);
	c.insert(4);
	fs[5]->setVariables(c);

	// Add factors to cumulative network
	for (unsigned int i = 0; i < 6; i++)
		cn.addFactor(fs[i]);

	// Return network
	return cn;
}

// Create extended student example for debugging
template <class G>
G& drwnCumulativeDebug::createExtendedStudentNet()
{
	// Start off with standard student example
	G& cn = createStudentNet<G>();

	// For now, add one factor so there is a clique with three factors
	drwnClique c;
	c.insert(3);
	c.insert(2);
	c.insert(5);
	G::factor_type* f = new G::factor_type;
	f->setVariables(c);
	cn.addFactor(f);

	// Return modified student network
	return cn;
}

// Create mini student example (pg. ?) for arbitrary network type
template <class G>
G& drwnCumulativeDebug::createMiniStudentNet()
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "D");	// D: 0
	universe->addVariable(2, "I");	// I: 1
	universe->addVariable(2, "G");	// G: 2
	universe->addVariable(2, "S");	// S: 3
	universe->addVariable(2, "L");	// L: 4
	
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[8];
	for (unsigned int i = 0; i < 5; i++)
		fs[i] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	fs[0]->setVariables(c);

	c.insert(1);
	c.insert(2);
	fs[1]->setVariables(c);

	c.clear();
	c.insert(1);
	fs[2]->setVariables(c);

	c.insert(3);
	fs[3]->setVariables(c);

	c.clear();
	c.insert(2);
	c.insert(4);
	fs[4]->setVariables(c);
		
	// Add factors to cumulative network
	for (unsigned int i = 0; i < 5; i++)
		cn.addFactor(fs[i]);

	// Return network
	return cn;
}

// Create a network with two disjoint cliques
template <class G>
G& drwnCumulativeDebug::createDisjointFourCliques()
{
	// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "A1");
    universe->addVariable(2, "A2");
	universe->addVariable(2, "A3");
	universe->addVariable(2, "B1");
    universe->addVariable(2, "B2");
	universe->addVariable(2, "B3");
	
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	G::factor_type* fs[4];
	for (unsigned int i = 0; i < 4; i++)
		fs[i] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	c.insert(1);
	fs[0]->setVariables(c);

	c.clear();
	c.insert(1);
	c.insert(2);
	fs[1]->setVariables(c);

	c.clear();
	c.insert(3);
	c.insert(4);
	fs[2]->setVariables(c);

	c.clear();
	c.insert(4);
	c.insert(5);
	fs[3]->setVariables(c);
	
	// Add factors to cumulative network
	for (unsigned int i = 0; i < 4; i++)
		cn.addFactor(fs[i]);

	// Return network
	return cn;
}