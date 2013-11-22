/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnTestCases.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

#include <boost/filesystem.hpp>
//using namespace boost::filesystem;

//#include "drwnCumulativeDebug.h"
#include "WinHttpClient.h"

#include "drwnCliqueTree.h"

// DEBUG: Use these to start tracing in dspCalculateMessage
/*namespace drwnCumulativeUtils {
	extern bool stopYet;
	extern bool stopYet2;
}*/

//! Functions to perform test cases
namespace drwnTestCases
{
	// Compare speeds of two different root finding methods
	template <class G>
	void compareRootFinding();

	// Try learning a network with two factors and two cliques from 3 starts with known parameters
	template <class G>
	void learnTwoFactors();

	// Try learning a network with two factors and a single clique with known parameters
	template <class G>
	void learnSingleClique(unsigned int i);

	// Compare both learning methods (will use as a template for later)
	template <class G>
	void compareLearningMethods();

	// Learn shape model over horse data set (for now)
	template <class G>
	void learnShapeModel();

	// Learn a single copulae of scope n from known parameters
	template <class G>
	void learnSingleCopula(unsigned int n);

	// Try learning a chain of length n with two factors and two cliques from 3 starts with known parameters
	template <class G>
	void learnChain(unsigned int n);

	// Compare running inference over a part of the model to inference over the full clique tree with variables marginalized
	template <class G>
	void testInferenceSubmodel();

	// Test samples produced using normal space to those produced in log-space
	template <class G>
	void testLogSpaceSampling();

	// Test log-space version of inference
	template <class G>
	void testLogSpaceInference();

	// Test sampling over a subset of model conditioned on remaining variables
	template <class G>
	void testCondSampling();

	// Write density and CDF of bivariate copula over a grid to file
	template <class F>
	void outputCopulaGrid(double p);

	// Output some values of normal copula, its subsets and gradients
	// NOTE: Use this one to test formula against numerical derivatives calculated in R
	void evalNormalCopula();

	// Output some values of Clayton copula, its subsets and gradients
	void evalClaytonCopula();

	// Example testing numerical differentiation
	void testNumericalDerivative();

	// Output the maximum clique size vs. model size for the test models (used to create figure for experiments chapter)
	template <class G>
	void recordModelSizes();

	// Sample a single copula
	template <class G>
	void sampleCopula(unsigned int n, double parameter, const wchar_t* fileName);

	// Code that generates samples for figure that shows stable sampling
	void stableSampleFigure();

	// Compare calculating gradient over a part of the model to gradient over the full clique tree with variables marginalized
	template <class G>
	void testGradientSubmodel();

	//! Run inference experiment
	template <class G>
	void inferenceExperiment(G& model, unsigned int numSamples, unsigned int numRepeats, bool offLineResults = true, bool printTime = false, bool logSpace = false);
	
	//! Run learning experiment
	template <class G>
	void learningExperiment(G& model, unsigned int numSamples, unsigned int numRepeats, bool offLineResults = true, bool printTime = false, bool logSpaceSampling = false, double eps = 1e-8);
	
	//! Run CMAR learning experiment
	template <class G>
	void cmarExperiment(G& model, unsigned int numSamples, unsigned int numRepeats, bool offLineResults = true, double missing = 0.0, bool printTime = false, bool logSpaceSampling = false, double eps = 1e-8);

	//! Upload learning results
	template <class G>
	bool uploadLearningResults(WinHttpClient& client, G& model, bool isSuccess, unsigned int numSamples, string& truePars, char* method, unsigned int numIterations, double mse, double time, double eps);

	//! Upload learning results
	template <class G>
	bool uploadCmarResults(WinHttpClient& client, G& model, bool isSuccess, unsigned int numSamples, string& truePars, char* method, unsigned int numIterations, double mse, double missing, double time, double eps);

	template <class G>
	void testLogSpaceGradient();

	//! Test new learning method
	template <class G>
	void testPiecewiseLearning(G& cn, unsigned int numSamples, unsigned int numRepeats);

	//! Learn a shape model, save factors, draw samples, save samples
	template <class G>
	void learnSampleShapeModel(ShapemodelType type, unsigned int numSamples, double eps, unsigned int maxIters, unsigned int fixedIters, bool stopOne, bool shuffleEachTime, unsigned int seed);

	//! Print first eight samples of shape model dataset, for use as a figure in thesis (after converted to PDF in R)
	template <class G>
	void printShapeModelDataset();

	//! Test conclusions for three variable case in experiments chapter
	template <class G>
	void testModelLimitations();
};

//! Test conclusions for three variable case in experiments chapter
template <class G>
void drwnTestCases::testModelLimitations()
{
	// Two samples representing the classes
	drwnSamples<G> dataset(3, 2);
	for (unsigned int i = 0; i < 3; i++)
		dataset.sample(0)[i] = 1.0;
	for (unsigned int i = 0; i < 3; i++)
		dataset.sample(1)[i] = -1.0;

	// Create models
	G& cnChain = drwnCumulativeDebug::createBivariateChain<G>(2);
	//G& cnLoop = drwnCumulativeDebug::createBivariateLoop<G>(3);
	G& cnLoop = drwnCumulativeDebug::createOneFactorNet<G>(3);

	cnChain.randomizeParameters();
	cnLoop.randomizeParameters();

	// Create clique tree over model
	drwnCliqueTree<G> ctChain = drwnCliqueTree<G>(cnChain);
	drwnCliqueTree<G> ctLoop = drwnCliqueTree<G>(cnLoop);
	//drwnCumulativeUtils::printCliqueTree(ct);

	//drwnCopulaSamples<G> copulaSamples(samples, cn, true);

	drwnLearnRestartOnError<G> learnChain(ctChain, dataset);
	//drwnLearnRestartOnError<G> learnLoop(ctLoop, dataset);
	drwnLearnModel<G, drwnGradientDescent> learnLoop(ctLoop, dataset);
	learnChain.learn(1e-8, 0, 0, 0, false);
	learnLoop.learn(1e-8, 0, 0, 0, false);

	cnChain.saveMarginals(L"limitsChainMarginals.txt");
	cnLoop.saveMarginals(L"limitsLoopMarginals.txt");
	cnChain.saveFactors(L"limitsChainFactors.txt", true);
	cnLoop.saveFactors(L"limitsLoopFactors.txt", true);

	/*cnLoop.factor(0).setParameter(0.9999999999);
	cnLoop.factor(1).setParameter(-0.9999999999);
	cnLoop.factor(2).setParameter(0.9999999999);*/

	const unsigned int numSamples = 10;
	drwnSamples<G> samplesChain(cnChain, numSamples, true, false, false);
	drwnSamples<G> samplesLoop(cnLoop, numSamples, true, false, false);
	
	samplesChain.print(cnChain, L"limitsChainSamples.txt");
	samplesLoop.print(cnLoop, L"limitsLoopSamples.txt");

	// Print samples to screen (makes it easier to see patterns)
	wcout << "Chain samples" << endl << endl;
	for (unsigned int i = 0; i < numSamples; i++)
		samplesChain.print(i, 3, 1);
	wcout << endl << endl << "Loop samples" << endl << endl;
	for (unsigned int i = 0; i < numSamples; i++)
		samplesLoop.print(i, 3, 1);

	// Count how many samples X1 agrees with X2
	double numAgree = 0;
	for (unsigned int i = 0; i < numSamples; i++) {
		if ((samplesChain.sample(i)[0] > 0 && samplesChain.sample(i)[2] > 0) || (samplesChain.sample(i)[0] < 0 && samplesChain.sample(i)[2] < 0))
			numAgree++;
	}
	wcout << "Chain: " << numAgree / (double) numSamples << endl;
	numAgree = 0;
	for (unsigned int i = 0; i < numSamples; i++) {
		if ((samplesLoop.sample(i)[0] > 0 && samplesLoop.sample(i)[2] > 0) || (samplesLoop.sample(i)[0] < 0 && samplesLoop.sample(i)[2] < 0))
			numAgree++;
	}
	wcout << "Loop: " << numAgree / (double) numSamples << endl;
	
	delete &cnChain;
	delete &cnLoop;
}

//! Output the maximum clique size vs. model size for the test models (used to create figure for experiments chapter)
template <class G>
void drwnTestCases::recordModelSizes()
{
	G* cn;

	// Open files
	wofstream chainCliques, chainSepsets, chainTotal, loopCliques, loopSepsets, loopTotal, treeCliques, treeSepsets, treeTotal, gridCliques, gridSepsets, gridTotal;
	chainCliques.open(L"chainCliques.txt");
	chainSepsets.open(L"chainSepsets.txt");
	chainTotal.open(L"chainTotal.txt");
	loopCliques.open(L"loopCliques.txt");
	loopSepsets.open(L"loopSepsets.txt");
	loopTotal.open(L"loopTotal.txt");
	treeCliques.open(L"treeCliques.txt");
	treeSepsets.open(L"treeSepsets.txt");
	treeTotal.open(L"treeTotal.txt");
	gridCliques.open(L"gridCliques.txt");
	gridSepsets.open(L"gridSepsets.txt");
	gridTotal.open(L"gridTotal.txt");

	// Save values for chains
	wcout << "Calculating values for chains" << endl;
	for (unsigned int i = 1; i < 16; i++) {
		wcout << i << " ";
		cn = &drwnCumulativeDebug::createBivariateChain<G>(i);
		drwnCliqueTree<G> ct(*cn);
		chainCliques << ct.maxClique() << " ";
		chainSepsets << ct.maxSepset() << " ";
		chainTotal << ct.totalSize() << " ";
		delete cn;
	}

	// Save values for loops
	wcout << endl << "Calculating values for loops" << endl;
	for (unsigned int i = 3; i < 16; i++) {
		wcout << i << " ";
		cn = &drwnCumulativeDebug::createBivariateLoop<G>(i);
		drwnCliqueTree<G> ct(*cn);
		loopCliques << ct.maxClique() << " ";
		loopSepsets << ct.maxSepset() << " ";
		loopTotal << ct.totalSize() << " ";
		delete cn;
	}

	// Save values for trees
	wcout << endl << "Calculating values for trees" << endl;
	for (unsigned int i = 2; i < 14; i++) {
		wcout << i << " ";
		cn = &drwnCumulativeDebug::createBinaryTree<G>(i);
		drwnCliqueTree<G> ct(*cn);
		treeCliques << ct.maxClique() << " ";
		treeSepsets << ct.maxSepset() << " ";
		treeTotal << ct.totalSize() << " ";
		delete cn;
	}

	// Save values for grids
	wcout << endl << "Calculating values for grids" << endl;
	for (unsigned int i = 2; i < 11; i++) {
		wcout << i << " ";
		cn = &drwnCumulativeDebug::createGrid<G>(i, i);
		drwnCliqueTree<G> ct(*cn);
		gridCliques << ct.maxClique() << " ";
		gridSepsets << ct.maxSepset() << " ";
		gridTotal << ct.totalSize() << " ";
		delete cn;
	}

	// Close files
	chainCliques.close();
	chainSepsets.close();
	chainTotal.close();
	loopCliques.close();
	loopSepsets.close();
	loopTotal.close();
	treeCliques.close();
	treeSepsets.close();
	treeTotal.close();
	gridCliques.close();
	gridSepsets.close();
	gridTotal.close();
}

//! Learn a shape model, save factors, draw samples, save samples
template <class G>
void drwnTestCases::learnSampleShapeModel(ShapemodelType type, unsigned int numSamples, double eps, unsigned int maxIters, unsigned int fixedIters, bool stopOne, bool shuffleEachTime, unsigned int seed)
{
	// Print what we're doing
	wcout << "Constructing a ";
	if (type == MISTAKE)
		wcout << "mistake";
	else if (type == SQUARE)
		wcout << "square";
	else if (type == DIAGONAL)
		wcout << "diagonal";
	else if (type == FOUR)
		wcout << "four";
	wcout << " shape model with ";
	if (!strcmp(G::factor_type::name(), "Normal"))
		wcout << "normal";
	else
		wcout << "clayton";
	if (seed)
		wcout << ", fixing random seed to " << seed;
	wcout << endl;

	// Load horse data set as specified in xml file
	//drwnSamples<G> samples(drwnBinaryImageDataset(L"horses.xml"));
	drwnSamples<G> samples(drwnBinaryImageDataset(L"simpleShapes.xml"));

	// Check that size of data set is the square of a power of 2
	double n = log((double) samples.numVars()) / log(2.0) / 2.0;
	DRWN_ASSERT_MSG(n == floor(n), "The width of images in data set must be a power of 2.")

	// Fix seed if specified
	if (seed)
		drwnCumulativeUtils::rng.seed(seed);

	// Create shape model
	G& cn = drwnCumulativeDebug::createShapeModel<G>(n, type);
	cn.randomizeParameters();

	wcout << cn.numFactors() << " factors, " << cn.numFactors() + (1 << (unsigned int) n) * (1 << (unsigned int) n) << " parameters in shape model" << endl << endl;

	// Create clique tree over model
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	//drwnCumulativeUtils::printCliqueTree(ct);

	drwnCopulaSamples<G> copulaSamples(samples, cn, true);
	drwnLearnPiecewise<G, drwnOptimizer2> learnModel(ct, copulaSamples, false, SINGLES);

	wcout << "Learning with eps = " << eps << ", ";
	if (stopOne)
		wcout << "stopping after consecutive #iters = #subproblems, ";
	else
		wcout << "running until convergence, ";
	if (shuffleEachTime)
		wcout << "shuffling subproblems each iteration";
	else
		wcout << "shuffling subproblems once";
	if (fixedIters)
		wcout << ", " << fixedIters << " fixed iterations";
	else if (maxIters)
		wcout << ", " << maxIters << " maximum iterations";
	wcout << ", " << learnModel.numSubmodels() << " submodels in piecewise learning" << endl << endl;
	
	learnModel.learn(eps, 0, 0, 0, false, false, true, true, shuffleEachTime, maxIters, fixedIters);		// last two are showOuterIter, and shuffleEachTime
	wcout << "Piecewise learning: iterations " << learnModel.iterations() << ", time " << learnModel.time() << endl << endl;

	// Save model so we can load later if need be
	unsigned int uniqueNumber = 1;
	string fileName(string("shapeFactors"));
	if (type == MISTAKE)
		fileName.append("Mistake");
	else if (type == SQUARE)
		fileName.append("Square");
	else if (type == DIAGONAL)
		fileName.append("Diagonal");
	else if (type == FOUR)
		fileName.append("Four");
	fileName.append(G::factor_type::name());
	boost::filesystem::path p((fileName + string(".txt")).c_str());
	while (boost::filesystem::exists(p) || boost::filesystem::is_directory(p)) {
		p = (fileName + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
	}
	cn.saveFactors(p.c_str(), true);

	wcout << "Sampling " << numSamples << " samples";
	// Time it

	clock_t t = clock();
	drwnSamples<G> samples2(cn, numSamples, true, false, true);
	t = clock() - t;

	wcout << "Took " << (((double)t) / ((double)CLOCKS_PER_SEC)) << " seconds" << endl;

	uniqueNumber = 1;
	string fileName2(string("shapeSamples"));
	if (type == MISTAKE)
		fileName2.append("Mistake");
	else if (type == SQUARE)
		fileName2.append("Square");
	else if (type == DIAGONAL)
		fileName2.append("Diagonal");
	else if (type == FOUR)
		fileName2.append("Four");
	fileName2.append(G::factor_type::name());
	boost::filesystem::path p2((fileName2 + string(".txt")).c_str());
	while (boost::filesystem::exists(p2) || boost::filesystem::is_directory(p2)) {
		p2 = (fileName2 + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
	}

	samples2.print(cn, p2.c_str());
}

//! Print first eight samples of shape model dataset, for use as a figure in thesis (after converted to PDF in R)
template <class G>
void drwnTestCases::printShapeModelDataset()
{
	// Load horse data set as specified in xml file
	drwnSamples<G> samples(drwnBinaryImageDataset(L"horses.xml"));

	// Check that size of data set is the square of a power of 2
	double n = log((double) samples.numVars()) / log(2.0) / 2.0;
	DRWN_ASSERT_MSG(n == floor(n), "The width of images in data set must be a power of 2.")

	G& cn = drwnCumulativeDebug::createShapeModel<G>(5, MISTAKE);
	samples.print(cn, L"shapeSamplesDataset.txt");
	
	delete &cn;
}

//! Learn shape model over horse data set (for now)
template <class G>
void drwnTestCases::learnShapeModel()
{
	// Load horse data set as specified in xml file
	drwnSamples<G> samples(drwnBinaryImageDataset(L"horses.xml"));

	// Check that size of data set is the square of a power of 2
	double n = log((double) samples.numVars()) / log(2.0) / 2.0;
	DRWN_ASSERT_MSG(n == floor(n), "The width of images in data set must be a power of 2.")

	// Create proposed shape model
	// DEBUG: Fix seed
	//drwnCumulativeUtils::rng.seed(1355985858);
	G& cn = drwnCumulativeDebug::createShapeModel<G>(5, FOUR);
	//G& cn = drwnCumulativeDebug::loadShapeModel<G>(5, L"shapeMarginals.txt", L"shapeFactors.txt");

	cn.randomizeParameters();
	//cn.saveFactors(L"shapeFactorsOverlap.txt", true);

	// A "random start"
	
	//cn.printParameters();

	// Create clique tree over model
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	//drwnCumulativeUtils::printCliqueTree(ct);

	// DEBUG
	//samples.print(20, 32, 32);

	wcout << cn.numFactors() << " factors in shape model" << endl;

	//drwnCopulaSamples<G> copulaSamples(samples, cn, true);
	/*drwnSamples<G> samples2(cn, 16, true, false, true);
	samples2.print(cn, L"shapeIndSamples.txt");*/

	/*samples2.save(0, 1 << (unsigned int) n, 1 << (unsigned int) n, L"randomSample.bmp");
	samples.save(0, 1 << (unsigned int) n, 1 << (unsigned int) n, L"dataSample.bmp");*/

	drwnCopulaSamples<G> copulaSamples(samples, cn, true);

	//cn.saveMarginals(L"shapeMarginals.txt");
	//cn.saveFactors(L"shapeFactors.txt", true);*/

	// DEBUG: Draw samples from random parameters
	//drwnSamples<G> samples2(cn, 1, true, false, true);
	//samples2.print(0, 1 << 5, 1 << 5);
	//samples2.print(cn, L"shapeSamples.txt");

	drwnLearnPiecewise<G, drwnOptimizer2> learnModel(ct, copulaSamples, false, SINGLES);

	wcout << learnModel.numSubmodels() << " submodels in piecewise learning" << endl << endl;

	learnModel.learn(1e-6, 0, 0, 0, false, false, true, true, true);		// last two are showOuterIter, and shuffleEachTime
	wcout << "Piecewise learning: iterations " << learnModel.iterations() << ", time " << learnModel.time() << endl;

	cn.saveFactors(L"shapeFactorsFour.txt", true);

	wcout << endl;

	drwnSamples<G> samples2(cn, 16, true, false, true);
	samples2.print(0, 1 << 5, 1 << 5);
	samples2.print(cn, L"shapeSamplesFour.txt");

	delete &cn;
}

//! Compare both learning methods (will use as a template for later)
template <class G>
void drwnTestCases::compareLearningMethods()
{
	normalCumulativeNet& cn = drwnCumulativeDebug::createTwoFactorNet<normalCumulativeNet>();

	// Create clique tree
	normalCliqueTree ct = normalCliqueTree(cn);
	drwnCumulativeUtils::printCliqueTree(ct);
	
	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	cn.randomizeParameters();
	cn.printParameters();
		
	// Draw samples and print to file so we can use in R
	normalSamples samples(cn, 10000, false);

	cn.randomizeParameters();

	drwnLearnModel<normalCumulativeNet, drwnGradientDescent> learnModel(ct, samples);	// drwnOptimizer
	learnModel.learn();

	drwnLearnModel<normalCumulativeNet, drwnOptimizer> learnModel2(ct, samples);
	learnModel2.learn(1e-6, L"solutions2.txt", L"loglikelihoods2.txt");

	delete &cn;
}

//! Compare speeds of two different root finding methods
template <class G>
void drwnTestCases::compareRootFinding()
{
	G* cn;

	// Number of samples to use for each model
	const unsigned int numSamples = 10000, numTimes = 1000;

	// Open file
	wofstream brentOutput, safeNewtonOutput;
	brentOutput.open(L"brentsMethod.txt");
	brentOutput.precision(std::numeric_limits<double>::digits10);
	safeNewtonOutput.open(L"safeNewtonsMethod.txt");
	safeNewtonOutput.precision(std::numeric_limits<double>::digits10);

	//myfile << "safeNewtons Brents" << endl;

	// Do sampling over bivariate chain with 1 to 15 links	
	for (unsigned int i = 1; i < 16; i++) {
		cn = &drwnCumulativeDebug::createBivariateChain<G>(i);

		clock_t t;

		// DEBUG: Fix seed
		//unsigned int seed = 1355985858;
		drwnCumulativeUtils::rng.seed(std::time(0));
		
		for (unsigned int j = 0; j < numTimes; j++) {
			if (j % 100 == 0)
				wcout << "Model size " << i << "/15, Safe Newton's " << "j = " << j << "/1000" << endl;

			cn->randomizeParameters();

			// Time sampling for safe Newton's method
			t = clock();
			normalSamples samples(*cn, numSamples, false, false);
			t = clock() - t;
			safeNewtonOutput << numSamples / (((float)t) / CLOCKS_PER_SEC) << " ";
		}
		safeNewtonOutput << endl;

		// Seed must be identical to make a fair comparison
		//drwnCumulativeUtils::rng.seed(std::time(0));

		for (unsigned int j = 0; j < numTimes; j++) {
			if (j % 100 == 0)
				wcout << "Model size " << i << "/15, Brent's " << "j = " << j << "/1000" << endl;

			cn->randomizeParameters();

			// Time sampling for Brent's method
			t = clock();
			normalSamples samples2(*cn, numSamples, true, false);
			t = clock() - t;
			brentOutput << numSamples / (((float)t) / CLOCKS_PER_SEC) << " ";
		}
		brentOutput << endl;

		delete cn;
	}

	// Close file
	//myfile.close();
	brentOutput.close();
	safeNewtonOutput.close();
}

//! Write density and CDF of bivariate copula over a grid to file
template <class F>
void drwnTestCases::outputCopulaGrid(double p)
{
	// Create copula
	drwnClique c;
	c.insert(0);
	c.insert(1);
	F copula;
	copula.setVariables(c);
	copula.setParameter(p);

	// Open file
	wofstream myfile, myfile2;
	myfile.open(L"copulaValues.txt");
	myfile2.open(L"copulaDenValues.txt");
	myfile.precision(std::numeric_limits<double>::digits10);
	myfile2.precision(std::numeric_limits<double>::digits10);

	// Evaluate copula over grid
	double sample[2], step = 0.01;
	for (unsigned int i = 0; i < (1.0 / step + 1.0); i++) {
		sample[1] = step * i;
		for (unsigned int j = 0; j < (1.0 / step + 1.0); j++) {
			sample[0] = step * j;
			myfile << copula.evaluateCdf(sample) << " ";
			myfile2 << copula.partialDerivative(sample, drwnSubset(3)) << " ";
		}
	}
	
	myfile.close();
	myfile2.close();
}

//! Tests log-space version of inference
template <class G>
void drwnTestCases::testLogSpaceSampling()
{
	// Create test case CDN
	//G& cn = drwnCumulativeDebug::createOneFactorNet<G>();
	G& cn = drwnCumulativeDebug::createBivariateChain<G>(2);

	// Create clique tree
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	//drwnCumulativeUtils::printCliqueTree(ct);
	
	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	cn.randomizeParameters();
	
	// DEBUG: Fix parameter temporarily
	//cn.factor(0).setParameter(3);
	//cn.factor(0).setParameter(0.99999);

	cn.printParameters();
	
	// Draw samples and print to file so we can use in R
	drwnCumulativeUtils::rng.seed(seed);
	drwnSamples<G> samples(cn, 2000, true, false, false);
	samples.print(cn, L"samplesNormal.txt");

	wcout << "Successfully drawn samples in normal-space!" << endl;

	drwnCumulativeUtils::rng.seed(seed);
	drwnSamples<G> samples2(cn, 2000, true, false, true);
	samples2.print(cn, L"samplesLog.txt");

	wcout << "Successfully drawn samples in log-space!" << endl;

	//drwnLearnModel<G, drwnGradientDescent> learnModel(ct, samples2);
	//drwnLearnRestartOnError<G> learnModel(ct, samples2);

	//learnModel.learn(1e-6, 0, 0, 0, true);

	delete &cn;
}

//! Test sampling over a subset of model conditioned on remaining variables
template <class G>
void drwnTestCases::testCondSampling()
{
	// Create test case CDN
	//G& cn = drwnCumulativeDebug::createStudentNet<G>();
	G& cn = drwnCumulativeDebug::createOneFactorNet<G>(4);

	// Define variables being conditioned on
	drwnClique vars;
	//vars.insert(0);
	vars.insert(1);
	vars.insert(2);
	/*vars.insert(6);
	vars.insert(4);
	vars.insert(5);*/

	cn.factor(0).setParameter(0.75);

	double cond[4] = {0, 0.5, -0.2, 0};

	cn.printParameters();

	// Create clique tree
	//drwnSampleCliques<G> sc(cn, c);
	//drwnCumulativeUtils::printSampleCliques(sc);

	drwnSamples<G> samples(cn, 10000, vars, cond);
	samples.print(cn, L"samplesCond.txt");
}

//! Tests log-space version of inference
template <class G>
void drwnTestCases::testLogSpaceInference()
{
	/*// Create variable universe
	drwnVarUniversePtr universe(new drwnVarUniverse());
	universe->addVariable(2, "A");
    universe->addVariable(2, "B");
	universe->addVariable(2, "C");
	universe->addVariable(2, "D");
	
	// Create empty network
	G& cn = *(new G(universe));

	// Allocate factors
	const unsigned int n = 4;
	G::factor_type* fs[n];
	for (unsigned int i = 0; i < n; i++)
		fs[i] = new G::factor_type;

	// Fill variables for factors
	drwnClique c;
	c.insert(0);
	c.insert(1);
	c.insert(2);
	//c.insert(3);
	fs[0]->setVariables(c);

	c.clear();
	c.insert(0);
	c.insert(1);
	fs[1]->setVariables(c);

	c.clear();
	c.insert(1);
	c.insert(2);
	fs[2]->setVariables(c);

	c.insert(0);
	c.insert(1);
	c.insert(2);
	c.insert(3);
	fs[3]->setVariables(c);
	
	// Add factors to cumulative network
	for (unsigned int i = 0; i < n; i++)
		cn.addFactor(fs[i]);*/

	//G& cn = drwnCumulativeDebug::createStudentNet<G>();
	//G& cn = drwnCumulativeDebug::createTwoFactorNet<G>();
	G& cn = drwnCumulativeDebug::createBivariateChain<G>(2);

	// Create and print clique tree
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	drwnCumulativeUtils::printCliqueTree(ct);

	// DEBUG: Fix seed
	drwnCumulativeUtils::rng.seed(1355985858);
	
	// Randomize parameters before drawing samples
	cn.randomizeParameters();
	cn.printParameters();
		
	// Draw a few samples for purpose of inference
	drwnSamples<G> samples(cn, 10);
	drwnCopulaSamples<G> copulaSamples(samples, ct);

	// Differentiate variables
	ct.resetMessages();

	// Load sample in drwnSamples and make ready for copula, caching factors etc.
	samples.print(0, cn);

	// Loop over samples and calculate the density of each
	bool takeLog = false;
	wcout << "Log of Copula Densities" << endl;
	for (unsigned int i = 0; i < samples.numSamples(); i++) {
		cn.cacheFactors(samples.load(cn, i), takeLog);
		wcout << log(drwnCumulativeUtils::copulaDensity(ct, samples, true, takeLog)) << endl;
		//drwnCumulativeUtils::diffClique(ct, 0, -1, &ct.message(0, -1, 0), copulaSamples[0], true, false);

		/*ct.resetMessages(takeLog);
		//drwnCumulativeUtils::stopYet = true;
		drwnCumulativeUtils::dspCalculateMsg(ct, 0, 1, samples.temp(), true, takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 2, 5, samples.temp(), true, takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 3, 4, samples.temp(), true, takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 1, 4, samples.temp(), true, takeLog);
		drwnCumulativeUtils::stopYet = true;
		drwnCumulativeUtils::dspCalculateMsg(ct, 4, 5, samples.temp(), true, takeLog);
		drwnCumulativeUtils::stopYet = false;*/
	}
	wcout << endl;

	takeLog = true;
	wcout << "Copula Log Densities" << endl;
	for (unsigned int i = 0; i < samples.numSamples(); i++) {
		cn.cacheFactors(samples.load(cn, i), takeLog);
		wcout << drwnCumulativeUtils::copulaDensity(ct, samples, true, takeLog) << endl;
		//ct.resetMessages(true);
		//drwnCumulativeUtils::diffClique(ct, 0, -1, &ct.message(0, -1, 0), copulaSamples[0], true, true);

		/*ct.resetMessages(takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 0, 1, samples.temp(), true, takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 2, 5, samples.temp(), true, takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 3, 4, samples.temp(), true, takeLog);
		drwnCumulativeUtils::dspCalculateMsg(ct, 1, 4, samples.temp(), true, takeLog);
		drwnCumulativeUtils::stopYet = true;
		drwnCumulativeUtils::dspCalculateMsg(ct, 4, 5, samples.temp(), true, takeLog);
		drwnCumulativeUtils::stopYet = false;*/
	}
	wcout << endl;
	
	// Test differentiating clique in normal space
	/*drwnCumulativeUtils::stopYet = true;
	drwnCumulativeUtils::diffClique(ct, 0, -1, &ct.message(0, -1, 0), copulaSamples[0], true, false);

	// Test differentiating clique in log space
	ct.resetMessages(true);
	wcout << endl;
	cn.cacheFactors(samples.load(cn, 0), true, false, false);
	drwnCumulativeUtils::diffClique(ct, 0, -1, &ct.message(0, -1, 0), copulaSamples[0], true, true);*/
}

// NOTE: Used for Example 6.1 in thesis
//! Try learning a network with two factors and two cliques from 3 starts with known parameters
template <class G>
void drwnTestCases::learnTwoFactors()
{
	G& cn = drwnCumulativeDebug::createTwoFactorNet<G>();

	// Create clique tree
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	drwnCumulativeUtils::printCliqueTree(ct);
	
	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	//cn.randomizeParameters();
	cn.factor(0).setParameter(0.6);
	cn.factor(1).setParameter(-0.2);

	cn.printParameters();
		
	// Draw samples and print to file so we can use in R
	normalSamples samples(cn, 10000);
	samples.print(cn, L"samplesLearnEx1.txt");

	// We are comparing gradient descent and L-BFGS
	drwnLearnModel<normalCumulativeNet, drwnGradientDescent> learnModel(ct, samples);
	drwnLearnRestartOnError<normalCumulativeNet> learnModel2(ct, samples);

	cn.factor(0).setParameter(-0.7);
	cn.factor(1).setParameter(0.7);
	//cn.randomizeParameters();
	learnModel.learn(1e-6, L"solutionsLearnEx1GradDesc1.txt", L"loglikeLearnEx1GradDesc1.txt", 0);

	cn.factor(0).setParameter(-0.7);
	cn.factor(1).setParameter(0.7);
	learnModel2.learn(1e-6, L"solutionsLearnEx1BFGS1.txt", L"loglikeLearnEx1BFGS1.txt", 0);

	cn.factor(0).setParameter(-0.5);
	cn.factor(1).setParameter(-0.7);
	//cn.randomizeParameters();
	learnModel.learn(1e-6, L"solutionsLearnEx1GradDesc2.txt", L"loglikeLearnEx1GradDesc2.txt", 0);

	cn.factor(0).setParameter(-0.5);
	cn.factor(1).setParameter(-0.7);
	learnModel2.learn(1e-6, L"solutionsLearnEx1BFGS2.txt", L"loglikeLearnEx1BFGS2.txt", 0);

	cn.factor(0).setParameter(0.7);
	cn.factor(1).setParameter(0.7);
	//cn.randomizeParameters();
	learnModel.learn(1e-6, L"solutionsLearnEx1GradDesc3.txt", L"loglikeLearnEx1GradDesc3.txt", 0);

	cn.factor(0).setParameter(0.7);
	cn.factor(1).setParameter(0.7);
	learnModel2.learn(1e-6, L"solutionsLearnEx1BFGS3.txt", L"loglikeLearnEx1BFGS3.txt", 0);
}

//! Try learning a chain of length n with two factors and two cliques from 3 starts with known parameters
template <class G>
void drwnTestCases::learnChain(unsigned int n)
{
	//G& cn = drwnCumulativeDebug::createBivariateChain<G>(n);
	G& cn = drwnCumulativeDebug::createTwoFactorNet<G>();

	// Create clique tree
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	drwnCumulativeUtils::printCliqueTree(ct);
	
	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	cn.randomizeParameters();
	/*cn.factor(0).setParameter(0.6);
	cn.factor(1).setParameter(-0.2);*/

	cn.printParameters();
		
	// Draw samples and print to file so we can use in R
	normalSamples samples(cn, 10000, true, false);
	samples.print(cn, L"samplesChain.txt");

	// DEBUG: Output values on grid
	/*drwnCopulaSamples<G> copulaSamples(samples, ct, true);
	wofstream grid;
	grid.open(L"gridDebug.txt");
	grid.precision(std::numeric_limits<double>::digits10);
	const unsigned int fineness = 80;
	for (unsigned int i = 0; i < fineness; i++) {
		wcout << "calculating grid row " << i << endl;
		for (unsigned int j = 0; j < fineness; j++) {
			double xCoord = -0.99 + j * (2.0 * 0.99 / ((double) fineness - 1));
			double yCoord = -0.99 + i * (2.0 * 0.99 / ((double) fineness - 1));

			// We need to copy parameters from memory into the respective factors
			cn.factor(0).setParameter(xCoord);
			cn.factor(1).setParameter(yCoord);

			// Calculate objective
			double negLogLikelihood = 0.0;
			for (unsigned int k = 0; k < copulaSamples.numSamples(); k++) {
				// A case where it fails
				if (k == 42)
					bool gotHere = true;

				double sampleLL = drwnCumulativeUtils::calculateGradient(ct, copulaSamples[k], (typename G::factor_type::domain_type*) 0) / (double) copulaSamples.numSamples();

				// DEBUG
				if (_isnan(sampleLL))
					bool gotHere = true;
				else if (sampleLL == std::numeric_limits<double>::max())
					bool gotHere = true;

				negLogLikelihood -= sampleLL;
			}
			
			if (negLogLikelihood == std::numeric_limits<double>::infinity())
				grid << "Inf" << " ";
			else
				grid << negLogLikelihood << " ";
		}
		wcout << endl;
	}
	grid.close();*/

	// We are comparing gradient descent and L-BFGS
	drwnLearnModel<normalCumulativeNet, drwnGradientDescent> learnModel(ct, samples);
	//drwnLearnModel<normalCumulativeNet, drwnOptimizer2> learnModel2(ct, samples);
	//drwnLearnBarrierMethod<> learnModel2(ct, samples);
	drwnLearnRestartOnError<> learnModel2(ct, samples);

	/*wcout << "Gradient descent" << endl;
	cn.randomizeParameters();
	learnModel.learn(1e-8, 0, 0, 0, false);
	cn.printParameters();*/

	wcout << "L-BFGS" << endl;
	cn.randomizeParameters();
	/*cn.randomizeParameters();
	cn.randomizeParameters();
	cn.randomizeParameters();
	cn.randomizeParameters();*/
	//cn.factor(0).setParameter(-0.430278);
	cn.printParameters();

	learnModel2.learn(1e-8, L"solnChainBFGS.txt", L"loglikeChainBFGS.txt", L"gradChainBFGS.txt", true);
	cn.printParameters();

	//learnModel2.learn(1e-8, L"solnChainBFGS2.txt", 0, 0, true);
	//cn.printParameters();
}

//! Compare running inference over a part of the model to inference over the full clique tree with variables marginalized
template <class G>
void drwnTestCases::testInferenceSubmodel()
{
	//G& cn = drwnCumulativeDebug::createStudentNet<G>();
	//G& cn = drwnCumulativeDebug::createTwoFactorNet<G>();
	
	G& cn = drwnCumulativeDebug::createBivariateChain<G>(3);

	// Create clique tree over full model
	drwnCliqueTree<G> ctFull = drwnCliqueTree<G>(cn);
	drwnCumulativeUtils::printCliqueTree(ctFull);
	wcout << endl;

	// Create clique tree over model minus G and S
	drwnClique c;
	c.insert(0);
	c.insert(1);
	//c.insert(2);
	//c.insert(3);
	//c.insert(4);
	//c.insert(5);
	//c.insert(6);
	//c.insert(7);
	drwnCliqueTree<G> ctSub = drwnCliqueTree<G>(cn, c);
	drwnCumulativeUtils::printCliqueTree(ctSub);

	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	cn.randomizeParameters();
	cn.printParameters();

	// Draw samples and print to file so we can use in R
	normalSamples samples(cn, 10);
	
	//drwnCopulaSamples<G> copulaSamples(samples, cn, false);

	// Marginalize 2 variables
	//samples.marginalizeVariable(0);
	//samples.marginalizeVariable(1);
	samples.marginalizeVariable(2);
	samples.marginalizeVariable(3);
	//samples.marginalizeVariable(4);
	//samples.marginalizeVariable(5);
	
	samples.print(cn, L"margSamples.txt");

	// Test inference
	for (unsigned int i = 0; i < samples.numSamples(); i++) {
		// Load sample in drwnSamples and make ready for copula, caching factors etc.
		cn.cacheFactors(samples.load(cn, i));

		// Load masks for inference
		ctFull.generateMasks(samples[i]);
		
		// Compare value of density over full network and part of network
		//drwnCumulativeUtils::stopYet = true;
		wcout << drwnCumulativeUtils::copulaDensity(ctFull, samples, true) << " " << drwnCumulativeUtils::copulaDensity(ctSub, samples, true) << endl;
		
	}
}

//! Compare calculating gradient over a part of the model to gradient over the full clique tree with variables marginalized
// NOTE: This is to test a step during CMAR learning.
template <class G>
void drwnTestCases::testGradientSubmodel()
{
	//G& cn = drwnCumulativeDebug::createStudentNet<G>();
	//G& cn = drwnCumulativeDebug::createTwoFactorNet<G>();

	G& cn = drwnCumulativeDebug::createBivariateChain<G>(3);

	// Create clique tree over full model
	drwnCliqueTree<G> ctFull = drwnCliqueTree<G>(cn);
	drwnCumulativeUtils::printCliqueTree(ctFull);
	wcout << endl;

	// Create clique tree over model minus G and S
	drwnClique c;
	c.insert(0);
	c.insert(1);
	c.insert(2);
	//c.insert(3);
	//c.insert(4);
	//c.insert(5);
	//c.insert(6);
	//c.insert(7);
	drwnCliqueTree<G> ctSub = drwnCliqueTree<G>(cn, c);
	drwnCumulativeUtils::printCliqueTree(ctSub);

	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	cn.randomizeParameters();
	cn.printParameters();

	// Draw samples and print to file so we can use in R
	drwnSamples<G> samples(cn, 10);
	drwnCopulaSamples<G> copulaSamples(samples, cn, false);

	// Marginalize 2 variables
	//copulaSamples.marginalizeVariable(0);
	//copulaSamples.marginalizeVariable(1);
	//copulaSamples.marginalizeVariable(2);
	copulaSamples.marginalizeVariable(3);
	//copulaSamples.marginalizeVariable(4);
	//copulaSamples.marginalizeVariable(5);

	copulaSamples.print(cn, L"margSamples.txt");

	// Memory to store gradient (1 parameter per factor)
	G::factor_type::domain_type gradient[3];

	// Test inference
	for (unsigned int i = 0; i < samples.numSamples(); i++) {
		// Load sample in drwnSamples and make ready for copula, caching factors etc.
		//cn.cacheFactors(samples.load(cn, i));

		// Load masks for inference
		ctFull.generateMasks(copulaSamples[i], true);

		// Compare value of density over full network and part of network
		//drwnCumulativeUtils::stopYet = true;
		wcout << "Sample " << i << " over full clique tree" << endl;
		wcout << "log-like " << drwnCumulativeUtils::calculateGradient(ctFull, copulaSamples[i], gradient) << " gradient";
		for (unsigned int j = 0; j < 3; j++)
			wcout << " " << gradient[j];
		wcout << endl << "Sample " << i << " over partial clique tree" << endl;
		wcout << "log-like " << drwnCumulativeUtils::calculateGradient(ctSub, copulaSamples[i], gradient) << " gradient";
		for (unsigned int j = 0; j < 3; j++)
			wcout << " " << gradient[j];
		wcout << endl << endl;
	}
}

//! Test new learning method
template <class G>
void drwnTestCases::testPiecewiseLearning(G& cn, unsigned int numSamples, unsigned int numRepeats)
{
	// Create model and full clique tree
	//G& cn = drwnCumulativeDebug::createBinaryTree<G>(6);
	//G& cn = drwnCumulativeDebug::createBivariateChain<G>(3);
	//G& cnLoop = drwnCumulativeDebug::createBivariateLoop<G>(3);
	//G& cn = drwnCumulativeDebug::createGrid<G>(6, 6);
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);

	// Fix parameters
	//drwnCumulativeUtils::rng.seed(1355985858);
	
	wofstream resultsPiecewise;
	unsigned int uniqueNumber = 1;
	string fileName(string("piecewiseCl") + string(G::factor_type::name()) + cn.name() + std::to_string(cn.size()) + string("Samples") + std::to_string(numSamples));	// + + string("Repeats") + std::to_string(numRepeats)
	boost::filesystem::path p((fileName + string(".txt")).c_str());
	while (boost::filesystem::exists(p) || boost::filesystem::is_directory(p)) {
		p = (fileName + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
	}
	resultsPiecewise.open(p.c_str());
	resultsPiecewise.precision(std::numeric_limits<double>::digits10);

	wofstream resultsLBFGS;
	uniqueNumber = 1;
	string fileName2(string("piecewiseLBFGS") + string(G::factor_type::name()) + cn.name() + std::to_string(cn.size()) + string("Samples") + std::to_string(numSamples));	//  + + string("Repeats") + std::to_string(numRepeats)
	boost::filesystem::path p2((fileName2 + string(".txt")).c_str());
	while (boost::filesystem::exists(p2) || boost::filesystem::is_directory(p2)) {
		p2 = (fileName2 + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
	}
	resultsLBFGS.open(p2.c_str());
	resultsLBFGS.precision(std::numeric_limits<double>::digits10);

	resultsPiecewise << L"mse iters time" << endl;
	resultsLBFGS << L"mse iters time" << endl;

	wcout << "model " << cn.name().c_str() << ", size " << cn.size() << ", samples " << numSamples << endl;

	// Draw samples and learn numRepeats times
	for (unsigned int i = 0; i < numRepeats; i++) {
		cn.resetMarginals();

		wcout << "Repeat number " << i + 1 << endl;

		// Draw samples
		cn.randomizeParameters();
		drwnSamples<G> samples(cn, numSamples, true, false, true);
		drwnCopulaSamples<G> copulaSamples(samples, cn, true);

		// Construct learning object and attempt to recover parameters
		drwnLearnPiecewise<G, drwnOptimizer2> learnModel(ct, copulaSamples, false, SINGLES);
		//drwnLearnPiecewise<G, drwnGradientDescent> learnModel2(ct, copulaSamples, false, PAIRS);
		drwnLearnRestartOnError<G> learnModel3(ct, copulaSamples, false);
		learnModel.loadTruePars();
		//learnModel2.loadTruePars();
		learnModel3.loadTruePars();

		// Piecewise learning with singles
		//drwnCumulativeUtils::rng.seed(135585);
		//drwnCumulativeUtils::rng.seed(time(0));
		cn.randomizeParameters();
		learnModel.learn(1e-8, 0, 0, 0, false, false, false, true);		// last two are showOuterIter, and shuffleEachTime
		wcout << "Piecewise learning with singles: mse " << learnModel.mse() << ", iterations " << learnModel.iterations() << ", time " << learnModel.time() << endl;
		resultsPiecewise << learnModel.mse() << L" " << learnModel.iterations() << L" " << learnModel.time() << endl;
	
		// Piecewise learning with pairs
		//drwnCumulativeUtils::rng.seed(135585);
		//cn.randomizeParameters();
		//learnModel2.learn(1e-6, 0, 0, 0, false, false, false, true);		// last two are showOuterIter, and shuffleEachTime
		//wcout << "L-BFGS w/barrier method: mse " << learnModel2.mse() << ", iterations " << learnModel2.iterations() << ", time " << learnModel2.time() << endl;

		// L-BFGS
		//drwnCumulativeUtils::rng.seed(135585);
		cn.randomizeParameters();
		learnModel3.learn(1e-8, 0, 0, 0, false, false, false);		// last is showOuterIter
		wcout << "L-BFGS w/barrier method: mse " << learnModel3.mse() << ", iterations " << learnModel3.iterations() << ", time " << learnModel3.time() << endl << endl;
		resultsLBFGS << learnModel3.mse() << L" " << learnModel3.iterations() << L" " << learnModel3.time() << endl;
	}

	resultsPiecewise.close();
	resultsLBFGS.close();

	delete &cn;
}

//! Test calculating gradient in log space
template <class G>
void drwnTestCases::testLogSpaceGradient()
{
	//G& cn = drwnCumulativeDebug::createStudentNet<G>();
	//G& cn = drwnCumulativeDebug::createTwoFactorNet<G>();

	// Number of factors
	const unsigned int numFactors = 5;

	G& cn = drwnCumulativeDebug::createBivariateLoop<G>(numFactors);
	//G& cn = drwnCumulativeDebug::createBinaryTree<G>(5);
	//G& cn = drwnCumulativeDebug::createGrid<G>(5, 5);

	// Create clique tree over full model
	drwnCliqueTree<G> ctFull = drwnCliqueTree<G>(cn);
	//drwnCumulativeUtils::printCliqueTree(ctFull);
	//wcout << endl;

	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	cn.randomizeParameters();
	cn.printParameters();

	// Draw samples and print to file so we can use in R
	drwnSamples<G> samples(cn, 1000, true, false , true);
	drwnCopulaSamples<G> copulaSamples(samples, cn, true);

	// Memory to store gradient (1 parameter per factor)
	G::factor_type::domain_type* gradient = new G::factor_type::domain_type[cn.numFactors()];
	G::factor_type::domain_type* gradientSum = new G::factor_type::domain_type[cn.numFactors()];

	/*// Test inference
	//for (unsigned int i = 0; i < samples.numSamples(); i++) {
		unsigned int i = 0;
		// Load sample in drwnSamples and make ready for copula, caching factors etc.
		//cn.cacheFactors(samples.load(cn, i));
		
		// Compare value of density over full network and part of network
		//drwnCumulativeUtils::stopYet = true;
		wcout << "Calculating " << i << " in normal space" << endl;
		//wcout << "log-like " << drwnCumulativeUtils::calculateGradient(ctFull, copulaSamples[i], gradient, false) << " gradient";
		wcout << "log-like " << drwnCumulativeUtils::likelihoodGradient(ctFull, copulaSamples, gradient, gradientSum, false) << " gradient";
		for (unsigned int j = 0; j < cn.numFactors(); j++)
			wcout << " " << gradient[j];

		wcout << endl << "Calculating " << i << " in log space" << endl;
		//wcout << "log-like " << drwnCumulativeUtils::calculateGradient(ctFull, copulaSamples[i], gradient, true) << " gradient";
		wcout << "log-like " << drwnCumulativeUtils::likelihoodGradient(ctFull, copulaSamples, gradient, gradientSum, true) << " gradient";
		for (unsigned int j = 0; j < cn.numFactors(); j++)
			wcout << " " << gradient[j];
		wcout << endl << endl;
	//}*/

	
	drwnLearnModel<G, drwnGradientDescent> learnModel(ctFull, copulaSamples, false);
	drwnLearnBarrierMethod<G> learnModel2(ctFull, copulaSamples, false);

	// DEBUG: Fix seed
	seed = 13559;
	drwnCumulativeUtils::rng.seed(seed);
	cn.randomizeParameters();
	cn.printParameters();
	learnModel.learn(1e-6, 0, 0, 0, true, false);

	wcout << endl;

	// DEBUG: Fix seed
	seed = 13559;
	drwnCumulativeUtils::rng.seed(seed);
	cn.randomizeParameters();
	cn.printParameters();
	learnModel.learn(1e-6, 0, 0, 0, true, true);
	//learnModel2.learn(1e-6, 0, 0, 0, true);

	delete [] gradient;
	delete [] gradientSum;
}

//! Try learning a network with two factors and a single cliques with known parameters
template <class G>
void drwnTestCases::learnSingleClique(unsigned int j)
{
	G& cn = drwnCumulativeDebug::createOneCliqueNet<G>();

	// Create clique tree
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	drwnCumulativeUtils::printCliqueTree(ct);
	
	// DEBUG: Fix seed
	unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;

	//cn.randomizeParameters();
	cn.factor(0).setParameter(0.5);
	cn.factor(1).setParameter(-0.5);

	cn.printParameters();
		
	// Draw samples and print to file so we can use in R
	normalSamples samples(cn, 10000);
	//samples.print(cn, L"samplesLearnEx2.txt");
	drwnCopulaSamples<normalCumulativeNet> copulaSamples(samples, cn, true);

	// DEBUG: Output values on grid
	/*drwnCopulaSamples<G> copulaSamples(samples, ct, true);
	wofstream grid;
	grid.open(L"gridLearnEx2.txt");
	grid.precision(std::numeric_limits<double>::digits10);
	const unsigned int fineness = 40;
	for (unsigned int i = 0; i < fineness; i++) {
		wcout << "calculating grid row " << i << endl;
		for (unsigned int j = 0; j < fineness; j++) {
			double xCoord = -0.99 + j * (2.0 * 0.99 / ((double) fineness - 1));
			double yCoord = -0.99 + i * (2.0 * 0.99 / ((double) fineness - 1));

			// We need to copy parameters from memory into the respective factors
			cn.factor(0).setParameter(xCoord);
			cn.factor(1).setParameter(yCoord);

			// Calculate objective
			double negLogLikelihood = 0.0;
			for (unsigned int k = 0; k < copulaSamples.numSamples(); k++) {
				// A case where it fails
				if (k == 42)
					bool gotHere = true;

				double sampleLL = drwnCumulativeUtils::calculateGradient(ct, copulaSamples[k], (typename G::factor_type::domain_type*) 0) / (double) copulaSamples.numSamples();

				// DEBUG
				if (_isnan(sampleLL))
					bool gotHere = true;
				else if (sampleLL == std::numeric_limits<double>::max())
					bool gotHere = true;

				negLogLikelihood -= sampleLL;
			}
			
			grid << negLogLikelihood << " ";
		}
		wcout << endl;
	}
	grid.close();*/

	// We are comparing gradient descent and L-BFGS
	drwnLearnModel<normalCumulativeNet, drwnGradientDescent> learnModel(ct, copulaSamples, false);
	drwnLearnBarrierMethod<normalCumulativeNet> learnModel2(ct, copulaSamples, false);

	wofstream results1, results2;
	results1.open((wstring(L"resultsLearnEx2graddescent") + std::to_wstring(j) + wstring(L".txt")).c_str());
	results1.precision(std::numeric_limits<double>::digits10);
	results2.open((wstring(L"resultsLearnEx2bfgs") + std::to_wstring(j) + wstring(L".txt")).c_str());
	results2.precision(std::numeric_limits<double>::digits10);
	for (unsigned int i = 0; i < cn.numFactors(); i++) {
		results1 << "rho" << i << " ";
		results2 << "rho" << i << " ";
	}
	results1 << "type" << endl;
	results2 << "type" << endl;

	drwnCumulativeUtils::rng.seed(std::time(0));

	// See where converges to with different random starts
	for (unsigned int i = 0 ; i < 72; i++) {
		if (i % 10 == 0)
			wcout << "Learning iteration #" << i << endl;

		cn.randomizeParameters();
		cn.printParameters(results1, false);
		//cn.printParameters();
		learnModel.learn(1e-8, 0, 0, 0, false);
		//cn.printParameters();

		double mse1 = 0.5 * (pow(cn.factor(0).getParameter() - 0.5, 2.0) + pow(cn.factor(1).getParameter() + 0.5, 2.0));
		double mse2 = 0.5 * (pow(cn.factor(1).getParameter() - 0.5, 2.0) + pow(cn.factor(0).getParameter() + 0.5, 2.0));

		if (mse1 < 0.1)
			results1 << "L" << endl;
		else if (mse2 < 0.1)
			results1 << "R" << endl;
		else
			results1 << "N" << endl;
				
		cn.randomizeParameters();

		// DEBUG: Initial values for which gets caught on ridge!
		/*cn.factor(0).setParameter(-0.677495281677693);
		cn.factor(1).setParameter(-0.678901470266283);*/

		cn.printParameters(results2, false);
		learnModel2.learn(1e-8, 0, 0, 0, false);

		mse1 = 0.5 * (pow(cn.factor(0).getParameter() - 0.5, 2.0) + pow(cn.factor(1).getParameter() + 0.5, 2.0));
		mse2 = 0.5 * (pow(cn.factor(1).getParameter() - 0.5, 2.0) + pow(cn.factor(0).getParameter() + 0.5, 2.0));

		wcout << learnModel2.iterations() << " iterations" << endl;
		//cn.printParameters();
		if (mse1 < 0.1)
			results2 << "L" << endl;
		else if (mse2 < 0.1)
			results2 << "R" << endl;
		else
			results2 << "N" << endl;
	}

	results1.close();
	results2.close();
}

//! Sample a single copula
template <class G>
void drwnTestCases::sampleCopula(unsigned int n, double parameter, const wchar_t* fileName)
{
	drwnCumulativeNet<G,drwnNormMarginal>& cn = drwnCumulativeDebug::createOneFactorNet<drwnCumulativeNet<G,drwnNormMarginal> >(n);
	
	// Create clique tree
	drwnCliqueTree<drwnCumulativeNet<G,drwnNormMarginal> > ct = drwnCliqueTree<drwnCumulativeNet<G,drwnNormMarginal> >(cn);
		
	// DEBUG: Fix seed
	/*unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;*/

	cn.factor(0).setParameter(parameter);

	// Draw samples and print to file so we can use in R
	drwnSamples<drwnCumulativeNet<G,drwnNormMarginal> > samples(cn, 2000, true, true);
	drwnCopulaSamples<drwnCumulativeNet<G,drwnNormMarginal> > copulaSamples(samples, cn, false);
	copulaSamples.print(cn, fileName);

	delete &cn;
}

//! Learn a single copulae of scope n from known parameters
template <class G>
void drwnTestCases::learnSingleCopula(unsigned int n)
{
	//G& cn = drwnCumulativeDebug::createOneFactorNet<G>(n);
	G& cn = drwnCumulativeDebug::createBivariateChain<G>(3);
	
	// DEBUG: Using this function to test learning with all test models
	
	//G& cn = drwnCumulativeDebug::createBivariateLoop<G>(15);
	//G& cn = drwnCumulativeDebug::createBinaryTree<G>(6);
	//G& cn = drwnCumulativeDebug::createGrid<G>(5, 5);

	// Create clique tree
	drwnCliqueTree<G> ct = drwnCliqueTree<G>(cn);
	//drwnCumulativeUtils::printCliqueTree(ct);
	
	// DEBUG: Fix seed
	/*unsigned int seed = 1355985858;
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;*/

	for (unsigned int i = 0 ; i < 2; i++) {
		cn.resetMarginals();
		cn.randomizeParameters();

		wcout << "True parameters" << endl;
		cn.printParameters();
		
		// Draw samples and print to file so we can use in R
		drwnSamples<G> samples(cn, 100000, true, true);
		wcout << "Making fraction of data missing" << endl;
		clock_t t = clock();
		samples.makeMissing(0.1);
		t = clock() - t;
		wcout << (((float)t) / CLOCKS_PER_SEC) << " seconds" << endl;
		wcout << "Finished making fraction of data missing" << endl;
		//samples.print(cn, L"samplesSingleCopula.txt");

		cn.randomizeParameters();

		wcout << "Starting values" << endl;
		cn.printParameters();

		// We are comparing gradient descent to L-BFGS
		drwnLearnModel<G, drwnGradientDescent> learnModel(ct, samples, true, true);
		//drwnLearnModel<G, drwnOptimizer2> learnModel(ct, samples);
		//drwnLearnRestartOnError<G> learnModel(ct, samples);
		//learnModel.learn(1e-8, L"solutionsSingleCopula.txt", L"loglikeSingleCopula.txt");
		learnModel.learn(1e-6, 0, 0, 0, true);
		if (learnModel.isSuccess())
			wcout << "converged successfully :)" << endl;
		else
			wcout << "convergence failed :(" << endl;

		wcout << "Learnt parameters" << endl;
		cn.printParameters();
	}
}

//! Run inference experiment
template <class G>
void drwnTestCases::inferenceExperiment(G& model, unsigned int numSamples, unsigned int numRepeats, bool offLineResults, bool printTime, bool logSpace)
{
	clock_t t;
	if (printTime)
		t = clock();
	
	// Create clique tree
	drwnCliqueTree<G> ct(model);
	//drwnCumulativeUtils::printCliqueTree(ct);

	// Open output file (try to give it a unique name)
	wofstream times;
	if (offLineResults) {
		unsigned int uniqueNumber = 1;
		string fileName(string("inference") + string(G::factor_type::name()) + model.name() + std::to_string(model.size()));	// string("Samples") + std::to_string(numSamples) + + string("Repeats") + std::to_string(numRepeats)
		boost::filesystem::path p((fileName + string(".txt")).c_str());
		while (boost::filesystem::exists(p) || boost::filesystem::is_directory(p)) {
			p = (fileName + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
		}
		times.open(p.c_str());
		times.precision(std::numeric_limits<double>::digits10);
	}

	// Set up uploading online results
	else {
		DRWN_TODO;
	}

	// Print type of experiment
	/*if (offLineResults)
		wcout << "Offline";
	else
		wcout << "Online";*/
	wcout << "Inference with " << G::factor_type::name() << " " << model.name().c_str() << " of size n = " << model.size() << ", and " << numRepeats << " repeats of " << numSamples << " samples" << endl;

	// Randomize seed
	//drwnCumulativeUtils::rng.seed(std::time(0));
	
	// Loop over repeats
	for (unsigned int i = 0; i < numRepeats; i++) {
		clock_t t;
		if (numRepeats > 1)
			wcout << "Repeat " << i + 1 << endl;

		// Draw sample
		model.randomizeParameters();
		drwnSamples<G> samples(model, numSamples, true, false, logSpace);
		//samples.print(model, L"testSamples.txt");
		
		// Loop over samples, calculating density
		t = clock();
		for (unsigned int j = 0; j < numSamples; j++) {
			model.cacheFactors(samples.load(model, j));
			drwnCumulativeUtils::copulaDensity(ct, samples, true);
		}
		t = clock() - t;
		times << ((double) numSamples) / (((double)t) / ((double) CLOCKS_PER_SEC)) << " ";

		t = clock();
		for (unsigned int j = 0; j < numSamples; j++) {
			model.cacheFactors(samples.load(model, j), true);
			drwnCumulativeUtils::copulaDensity(ct, samples, true, true);
		}
		t = clock() - t;
		times << ((double) numSamples) / (((double)t) / ((double) CLOCKS_PER_SEC)) << endl;
	}

	delete &model;

	if (printTime) {
		t = clock() - t;
		wcout << (((double)t) / ((double)CLOCKS_PER_SEC)) << " seconds" << endl;
	}

	if (offLineResults)
		times.close();
}

//! Run learning experiment
template <class G>
void drwnTestCases::learningExperiment(G& model, unsigned int numSamples, unsigned int numRepeats, bool offLineResults, bool printTime, bool logSpaceSampling, double eps)
{
	clock_t t, tExp;
	if (printTime)
		t = clock();

	// Create clique tree
	drwnCliqueTree<G> ct(model);
	//drwnCumulativeUtils::printCliqueTree(ct);

	// Set up HTTP object (I don't think it actually does any interweb stuff yet)
	//WinHttpClient client(L"http://127.0.0.1/learning.php");
	WinHttpClient client(L"http://www.stefanwebb.com/thesis/learning.php");
	if (client.GetLastError()) {
		wcout << "Couldn't open upload page" << endl;
		drwnCumulativeDebug::pressAnyKey();
		return;
	}

	// Open output file (try to give it a unique name)
	wofstream pars;
	if (offLineResults) {
		/*unsigned int uniqueNumber = 1;
		string fileName(string("learning") + string(G::factor_type::name()) + model.name() + std::to_string(model.size()) + string("Samples") + std::to_string(numSamples) + string("Repeats") + std::to_string(numRepeats));
		boost::filesystem::path p((fileName + string(".txt")).c_str());
		while (boost::filesystem::exists(p) || boost::filesystem::is_directory(p)) {
			p = (fileName + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
		}
		pars.open(p.c_str());
		pars.precision(std::numeric_limits<double>::digits10);*/
		//DRWN_TODO
	}

	// Print type of experiment
	/*if (offLineResults)
		wcout << "Offline";
	else
		wcout << "Online";*/
	wcout << "Learning with " << G::factor_type::name() << " " << model.name().c_str() << " of size n = " << model.size() << ", and " << numRepeats << " repeats of " << numSamples << " samples" << endl;

	// Loop over repeats
	string truePars;
	for (unsigned int i = 0; i < numRepeats; i++) {
		model.resetMarginals();

		if (numRepeats > 1)
			wcout << "Repeat " << i + 1 << ": ";

		if (offLineResults)
			wcout << endl;

		// Draw sample
		model.randomizeParameters();
		
		if (!offLineResults) {
			//truePars.assign("true=");
			model.printParameters(truePars);
			truePars.append("&learnt=");
		}

		// Save true parameters, so can calculate MSE later
		VectorXd trueX(model.numFactors());
		for (unsigned int i = 0; i < model.numFactors(); i++)
			trueX[i] = model.factor(i).getParameter();

		drwnSamples<G> samples(model, numSamples, true, false, logSpaceSampling);
		drwnCopulaSamples<G> copulaSamples(samples, model, true);

		// We are comparing gradient descent and L-BFGS methods
		drwnLearnModel<G, drwnGradientDescent> learnModel(ct, copulaSamples, false);
		wcout << "gradient descent";
		model.randomizeParameters();
		tExp = clock();
		learnModel.learn(eps, 0, 0, 0, false);	// change to "true" for output
		tExp = clock() - tExp;

		// Calculate MSE
		VectorXd learntX(model.numFactors());
		for (unsigned int i = 0; i < model.numFactors(); i++)
			learntX[i] = model.factor(i).getParameter();
		double mse = (trueX - learntX).squaredNorm() / (double) model.numFactors();

		if (!offLineResults) {
			string learntPars(truePars);
			model.printParameters(learntPars);
			uploadLearningResults(client, model, learnModel.isSuccess(), numSamples, learntPars, "graddescent", learnModel.iterations(), mse, ((double)tExp) / (double) CLOCKS_PER_SEC, eps);
			wcout << ", ";
		}
		else {
			wcout << ", mse " << mse << ", iters " << learnModel.iterations() << ", success " << (learnModel.isSuccess() ? "true" : "false") << endl;
		}

		// L-BFGS with restart
		drwnLearnRestartOnError<G> learnModel2(ct, copulaSamples, false);
		wcout << "L-BFGS restart";
		model.randomizeParameters();
		tExp = clock();
		learnModel2.learn(eps, 0, 0, 0, false);
		tExp = clock() - tExp;

		for (unsigned int i = 0; i < model.numFactors(); i++)
			learntX[i] = model.factor(i).getParameter();
		mse = (trueX - learntX).squaredNorm() / (double) model.numFactors();

		if (!offLineResults) {
			string learntPars(truePars);
			model.printParameters(learntPars);
			uploadLearningResults(client, model, learnModel2.isSuccess(), numSamples, learntPars, "lbfgsrestart", learnModel2.iterations(), mse, ((double)tExp) / (double)CLOCKS_PER_SEC, eps);
			wcout << ", ";
		}
		else {
			wcout << ", mse " << mse << ", iters " << learnModel2.iterations() << ", success " << (learnModel2.isSuccess() ? "true" : "false") << endl;
		}

		// L-BFGS barrier method
		drwnLearnBarrierMethod<G> learnModel3(ct, copulaSamples, false);
		wcout << "L-BFGS barrier method";
		model.randomizeParameters();
		tExp = clock();
		learnModel3.learn(eps, 0, 0, 0, false);
		tExp = clock() - tExp;

		for (unsigned int i = 0; i < model.numFactors(); i++)
			learntX[i] = model.factor(i).getParameter();
		mse = (trueX - learntX).squaredNorm() / (double) model.numFactors();

		if (!offLineResults) {
			string learntPars(truePars);
			model.printParameters(learntPars);
			uploadLearningResults(client, model, learnModel3.isSuccess(), numSamples, learntPars, "lbfgsbarrier", learnModel3.iterations(), mse, ((double)tExp) / (double)CLOCKS_PER_SEC, eps);
		}
		else {
			wcout << ", mse " << mse << ", iters " << learnModel3.iterations() << ", success " << (learnModel3.isSuccess() ? "true" : "false") << endl;
		}
		
		wcout << endl;
	}

	delete &model;

	if (printTime) {
		t = clock() - t;
		wcout << (((double)t) / (double)CLOCKS_PER_SEC) << " seconds" << endl;
	}

	//if (offLineResults)
	//	pars.close();
}

//! Upload learning results
template <class G>
bool drwnTestCases::uploadLearningResults(WinHttpClient& client, G& model, bool isSuccess, unsigned int numSamples, string& truePars, char* method, unsigned int numIterations, double mse, double time, double eps)
{
	// Set post data.
	unsigned int intSuccess = (isSuccess ? 1 : 0);
	stringstream ss;
	ss.precision(std::numeric_limits<double>::digits10);
	ss << "&mse=" << mse << "&time=" << time << "&eps=" << eps;
	string data(string("copula=") + G::factor_type::name() + string("&converged=") + std::to_string(intSuccess) + string("&model=") + model.name() + string("&size=") + std::to_string(model.size()) + string("&samples=") + std::to_string(numSamples) + string("&iters=") + std::to_string(numIterations) + string("&method=") + string(method) + string("&true=") + truePars);
	data.append(ss.str());

	// Debug
	//wcout << "upload string" << endl;
	//wcout << data.c_str() << endl;

	if (!client.SetAdditionalDataToSend((BYTE *)data.c_str(), data.size())) {
		wcout << "Couldn't set data to send" << endl;
		return false;
	}

	// Set request headers.
	wchar_t szSize[50] = L"";
	swprintf_s(szSize, L"%d", data.size());
	wstring headers = L"Content-Length: ";
	headers += szSize;
	headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
	if (!client.SetAdditionalRequestHeaders(headers)) {
		wcout << "Couldn't set additional request headers" << endl;
		return false;
	}

	// Send HTTP post request.
	if (!client.SendHttpRequest(L"POST")) {
		wcout << "Couldn't send HTTP request" << endl;
		return false;
	}

	if (client.GetResponseContent().compare(L"success")) {
		wcout << "Couldn't update database" << endl;\
		wcout << client.GetResponseContent() << endl;
		return false;
	}

	return true;
}

//! Upload learning results
template <class G>
bool drwnTestCases::uploadCmarResults(WinHttpClient& client, G& model, bool isSuccess, unsigned int numSamples, string& truePars, char* method, unsigned int numIterations, double mse, double missing, double time, double eps)
{
	// Set post data.
	unsigned int intSuccess = (isSuccess ? 1 : 0);
	stringstream ss;
	ss.precision(std::numeric_limits<double>::digits10);
	ss << "&mse=" << mse << "&time=" << time << "&eps=" << eps;
	string data(string("copula=") + G::factor_type::name() + string("&converged=") + std::to_string(intSuccess) + string("&model=") + model.name() + string("&size=") + std::to_string(model.size()) + string("&samples=") + std::to_string(numSamples) + string("&iters=") + std::to_string(numIterations) + string("&method=") + string(method) + string("&true=") + truePars + string("&missing=") + std::to_string(missing));
	data.append(ss.str());

	if (!client.SetAdditionalDataToSend((BYTE *)data.c_str(), data.size())) {
		wcout << "Couldn't set data to send" << endl;
		return false;
	}

	// Set request headers.
	wchar_t szSize[50] = L"";
	swprintf_s(szSize, L"%d", data.size());
	wstring headers = L"Content-Length: ";
	headers += szSize;
	headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
	if (!client.SetAdditionalRequestHeaders(headers)) {
		wcout << "Couldn't set additional request headers" << endl;
		return false;
	}

	// Send HTTP post request.
	if (!client.SendHttpRequest(L"POST")) {
		wcout << "Couldn't send HTTP request" << endl;
		return false;
	}

	if (client.GetResponseContent().compare(L"success")) {
		wcout << "Couldn't update database" << endl;
		wcout << client.GetResponseContent() << endl;
		return false;
	}

	return true;
}

//! Run CMAR learning experiment
template <class G>
void drwnTestCases::cmarExperiment(G& model, unsigned int numSamples, unsigned int numRepeats, bool offLineResults, double missing, bool printTime, bool logSpaceSampling, double eps)
{
	clock_t t, tExp;
	if (printTime)
		t = clock();

	// Create clique tree
	drwnCliqueTree<G> ct(model);
	//drwnCumulativeUtils::printCliqueTree(ct);

	// Set up HTTP object (I don't think it actually does any interweb stuff yet)
	//WinHttpClient client(L"http://127.0.0.1/cmar.php");
	WinHttpClient client(L"http://www.stefanwebb.com/thesis/cmar.php");
	if (client.GetLastError()) {
		wcout << "Couldn't open upload page" << endl;
		drwnCumulativeDebug::pressAnyKey();
		return;
	}

	// Open output file (try to give it a unique name)
	wofstream pars;
	if (offLineResults) {
		/*unsigned int uniqueNumber = 1;
		string fileName(string("learning") + string(G::factor_type::name()) + model.name() + std::to_string(model.size()) + string("Samples") + std::to_string(numSamples) + string("Repeats") + std::to_string(numRepeats));
		boost::filesystem::path p((fileName + string(".txt")).c_str());
		while (boost::filesystem::exists(p) || boost::filesystem::is_directory(p)) {
			p = (fileName + string("_") + std::to_string(uniqueNumber++) + string(".txt")).c_str();
		}
		pars.open(p.c_str());
		pars.precision(std::numeric_limits<double>::digits10);*/
		//DRWN_TODO
	}

	// Print type of experiment
	/*if (offLineResults)
		wcout << "Offline";
	else
		wcout << "Online";*/
	wcout << "CMAR learning with " << G::factor_type::name() << " " << model.name().c_str() << " of size n = " << model.size() << ", and " << numRepeats << " repeats of " << numSamples << " samples, missing " << 1.0 - missing << endl;

	// Randomize seed
	//drwnCumulativeUtils::rng.seed(std::time(0));

	// Loop over repeats
	string truePars;
	for (unsigned int i = 0; i < numRepeats; i++) {
		model.resetMarginals();

		if (numRepeats > 1)
			wcout << "Repeat " << i + 1 << ": ";

		if (offLineResults)
			wcout << endl;

		// Draw sample
		model.randomizeParameters();
		
		if (!offLineResults) {
			//truePars.assign("true=");
			model.printParameters(truePars);
			truePars.append("&learnt=");
		}

		// Save true parameters, so can calculate MSE later
		VectorXd trueX(model.numFactors());
		for (unsigned int i = 0; i < model.numFactors(); i++)
			trueX[i] = model.factor(i).getParameter();

		drwnSamples<G> samples(model, numSamples, true, false);
		if (missing != 1.0)
			samples.makeMissing(1.0 - missing);
		drwnCopulaSamples<G> copulaSamples(samples, model, true);

		/*// We are comparing gradient descent and L-BFGS methods
		drwnLearnModel<G, drwnGradientDescent> learnModel(ct, samples, false, true);
		wcout << "gradient descent";
		model.randomizeParameters();
		tExp = clock();
		learnModel.learn(eps, 0, 0, 0, false);
		tExp = clock() - tExp;

		// Calculate MSE
		VectorXd learntX(model.numFactors());
		for (unsigned int i = 0; i < model.numFactors(); i++)
			learntX[i] = model.factor(i).getParameter();
		double mse = (trueX - learntX).squaredNorm() / (double) model.numFactors();

		if (!offLineResults) {
			string learntPars(truePars);
			model.printParameters(learntPars);
			uploadCmarResults(client, model, learnModel.isSuccess(), numSamples, learntPars, "graddescent", learnModel.iterations(), mse, missing, ((double)tExp) / (double)CLOCKS_PER_SEC, eps);
			wcout << ", ";
		}
		else {
			wcout << ", mse " << mse << ", iters " << learnModel.iterations() << ", success " << (learnModel.isSuccess() ? "true" : "false") << endl;
		}*/

		// L-BFGS with restart
		VectorXd learntX(model.numFactors());
		drwnLearnRestartOnError<G> learnModel2(ct, samples, false, true);
		wcout << "L-BFGS restart";
		model.randomizeParameters();
		tExp = clock();
		learnModel2.learn(eps, 0, 0, 0, false);
		tExp = clock() - tExp;

		for (unsigned int i = 0; i < model.numFactors(); i++)
			learntX[i] = model.factor(i).getParameter();
		double mse = (trueX - learntX).squaredNorm() / (double) model.numFactors();

		if (!offLineResults) {
			string learntPars(truePars);
			model.printParameters(learntPars);
			uploadCmarResults(client, model, learnModel2.isSuccess(), numSamples, learntPars, "lbfgsrestart", learnModel2.iterations(), mse, missing, ((double)tExp) / (double)CLOCKS_PER_SEC, eps);
			wcout << ", ";
		}
		else {
			wcout << ", mse " << mse << ", iters " << learnModel2.iterations() << ", success " << (learnModel2.isSuccess() ? "true" : "false") << endl;
		}

		/*// L-BFGS barrier method
		drwnLearnBarrierMethod<G> learnModel3(ct, samples, false, true);
		wcout << "L-BFGS barrier method";
		model.randomizeParameters();
		tExp = clock();
		learnModel3.learn(eps, 0, 0, 0, false);
		tExp = clock() - tExp;

		for (unsigned int i = 0; i < model.numFactors(); i++)
			learntX[i] = model.factor(i).getParameter();
		mse = (trueX - learntX).squaredNorm() / (double) model.numFactors();

		if (!offLineResults) {
			string learntPars(truePars);
			model.printParameters(learntPars);
			uploadCmarResults(client, model, learnModel3.isSuccess(), numSamples, learntPars, "lbfgsbarrier", learnModel3.iterations(), mse, missing, ((double)tExp) / (double)CLOCKS_PER_SEC, eps);
			wcout << ", ";
		}
		else {
			wcout << ", mse " << mse << ", iters " << learnModel3.iterations() << ", success " << (learnModel3.isSuccess() ? "true" : "false") << endl;
		}*/
		
		wcout << endl;
	}

	delete &model;

	if (printTime) {
		t = clock() - t;
		wcout << (((double)t) / (double)CLOCKS_PER_SEC) << " seconds" << endl;
	}

	//if (offLineResults)
	//	pars.close();
}