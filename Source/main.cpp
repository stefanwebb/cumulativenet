// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// TODO: Move these into stdafx when finished debugging!
#include "drwnTestCases.h"

#include "drwnGradientDescent.h"
//#include "drwnML.h"
#include "drwnOptimizer.h"
#include "drwnLearnModel.h"
#include "drwnLearnBarrierMethod.h"
#include "drwnLearnRestartOnError.h"
#include "drwnLearnPiecewise.h"

using namespace std;

using namespace Gdiplus;

int commandLine(int argc, _TCHAR* argv[]);
int commandLine(std::vector<char *>& args);
int autoExperiments(bool doShutdown);
BOOL MySystemShutdown();

// learning online normal chain 2 samples 100 repeats 1 eps 1e-8 time
// learning online normal chain 3 samples 1000 repeats 1 time

int _tmain(int argc, _TCHAR* argv[])
{
	// Randomize seed
	drwnCumulativeUtils::rng.seed(std::time(0));
	srand(std::time(0));

	//drwnTestCases::printShapeModelDataset<drwnCumulativeNet<drwnClaytonCopula, drwnPixelMarginal> >();

	//drwnTestCases::learnShapeModel<drwnCumulativeNet<drwnClaytonCopula, drwnPixelMarginal> >();
	//drwnTestCases::learnShapeModel<drwnCumulativeNet<drwnNormCopula, drwnPixelMarginal> >();

	//drwnTestCases::learnSampleShapeModel<drwnCumulativeNet<drwnClaytonCopula, drwnPixelMarginal> >(FOUR, 16, 1e-6, 100, 0, true, true, 0);
	//drwnTestCases::learnSampleShapeModel<drwnCumulativeNet<drwnNormCopula, drwnPixelMarginal> >(FOUR, 16, 1e-6, 100, 0, true, true, 0);

	//G& cn = drwnCumulativeDebug::createBivariateChain<G>(3);

	//normalCumulativeNet& cn1 = drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(15);
	//drwnTestCases::testPiecewiseLearning<normalCumulativeNet>(cn1, 10000, 15);

	//normalCumulativeNet& cn2 = drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(6);
	//drwnTestCases::testPiecewiseLearning<normalCumulativeNet>(cn2, 1000, 15);

	claytonCumulativeNet* model;

	// Trees, 10 samples
	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(3);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(4);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(5);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(6);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	// Trees, 100 samples
	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(3);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(4);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(5);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(6);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);

	// Trees, 1000 samples
	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(3);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(4);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(5);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	model = &drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(6);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	// Grids, 10 samples
	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(2, 2);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(3, 3);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);
	
	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(4, 4);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(5, 5);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 10, 17);

	// Grids, 100 samples
	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(2, 2);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);

	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(3, 3);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);
	
	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(4, 4);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);

	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(5, 5);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 100, 17);
	
	// Grids, 1000 samples
	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(2, 2);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(3, 3);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);
	
	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(4, 4);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	model = &drwnCumulativeDebug::createGrid<claytonCumulativeNet>(5, 5);
	drwnTestCases::testPiecewiseLearning<claytonCumulativeNet>(*model, 1000, 17);

	//drwnTestCases::testModelLimitations<normalCumulativeNet>();
	

	//commandLine(argc, argv);

	//drwnCumulativeDebug::pressAnyKey();
	return 0;
}

int commandLine(int argc, _TCHAR* argv[])
{
	// Values we need to extract from arguments
	bool offlineResults = true, useNormalCopula = true;
	unsigned int sizeModel, numSamples, numRepeats;
	bool printTiming = false;
	double missingData, eps;
	enum TypeExperiment { INFERENCE, LEARNING, CMARLEARNING };
	enum TypeModel { CHAIN, LOOP, TREE, GRID };
	TypeExperiment typeExp;
	TypeModel typeModel;

	// No arguments
	if (argc == 1) {
		wcout << "No argument specified" << endl;
		return 0;
	}

	// Automatic experiment running
	if (argc == 2 || argc == 3) {
		if (!wcscmp(argv[1], L"learningEx2")) {
			if (argc == 3) {
				unsigned int i = _wtoi(argv[2]);
				drwnTestCases::learnSingleClique<normalCumulativeNet>(i);
				return 0;
			}

			else {
					wcout << "Invalid arguments" << endl;
					return 1;
			}
		}

		else if (!wcscmp(argv[1], L"auto")) {
			if (argc == 3) {
				if (!wcscmp(argv[2], L"shutdown")) {
					autoExperiments(true);
					return 0;
				}

				else {
					wcout << "Invalid arguments" << endl;
					return 1;
				}
			}

			autoExperiments(false);
			return 0;
		}

		else {
			wcout << "Invalid arguments" << endl;
			return 1;
		}
	}

	// Determine type of experiment to perform
	if (!wcscmp(argv[1], L"inference")) {
		typeExp = INFERENCE;
		if (argc != 10 && argc != 11) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}
	}
	else if (!wcscmp(argv[1], L"learning")) {
		typeExp = LEARNING;
		if (argc != 12 && argc != 13) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}
	}
	else if (!wcscmp(argv[1], L"cmar")) {
		typeExp = CMARLEARNING;
		if (argc != 13 && argc != 14) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}
	}

	// Shape experiment fits in here
	else if (!wcscmp(argv[1], L"shape")) {
		if (argc != 18 && argc != 20) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}

		unsigned int maxIters, fixedIters;
		bool stopOne, shuffleEachTime;
		ShapemodelType type;
		
		if (wcscmp(argv[2], L"type")) {
			wcout << "second argument must be 'type'" << endl;
			return 1;
		}

		if (!wcscmp(argv[3], L"mistake"))
			type = MISTAKE;
		else if (!wcscmp(argv[3], L"square"))
			type = SQUARE;
		else if (!wcscmp(argv[3], L"diagonal"))
			type = DIAGONAL;
		else {
			wcout << "third argument must be 'mistake' / 'square' / 'diagonal'" << endl;
			return 1;
		}

		if (wcscmp(argv[4], L"copula")) {
			wcout << "fourth argument must be 'copula'" << endl;
			return 1;
		}

		// Type of copula to use
		if (!wcscmp(argv[5], L"clayton"))
			useNormalCopula = false;
		else if (wcscmp(argv[5], L"normal")) {
			wcout << "fifth argument must be 'clayton' / 'normal'" << endl;
			return 1;
		}

		if (wcscmp(argv[6], L"samples")) {
			wcout << "sixth argument must be 'samples'" << endl;
			return 1;
		}

		numSamples = _wtoi(argv[7]);
		
		if (wcscmp(argv[8], L"eps")) {
			wcout << "eight argument must be 'eps'" << endl;
			return 1;
		}

		eps = _wtof(argv[9]);

		if (wcscmp(argv[10], L"maxIters")) {
			wcout << "tenth argument must be 'maxIters'" << endl;
			return 1;
		}

		maxIters = _wtoi(argv[11]);

		if (wcscmp(argv[12], L"fixedIters")) {
			wcout << "twelfth argument must be 'fixedIters'" << endl;
			return 1;
		}

		fixedIters = _wtoi(argv[13]);
				
		if (wcscmp(argv[14], L"stopOne")) {
			wcout << "fourteenth argument must be 'stopOne'" << endl;
			return 1;
		}

		if (!wcscmp(argv[15], L"true"))
			stopOne = true;
		else if (!wcscmp(argv[15], L"false"))
			stopOne = false;
		else {
			wcout << "fifteenth argument must be 'true' / 'false'" << endl;
			return 1;
		}

		if (wcscmp(argv[16], L"shuffleEachTime")) {
			wcout << "sixteenth argument must be 'shuffleEachTime'" << endl;
			return 1;
		}

		if (!wcscmp(argv[17], L"true"))
			shuffleEachTime = true;
		else if (!wcscmp(argv[17], L"false"))
			shuffleEachTime = false;
		else {
			wcout << "seventeenth argument must be 'true' / 'false'" << endl;
			return 1;
		}

		unsigned int seed = 0;
		if (argc == 20) {
			if (wcscmp(argv[18], L"seed")) {
				wcout << "eighteenth argument must be 'seed', if used" << endl;
				return 1;
			}

			seed = _wtoi(argv[19]);
		}

		if (useNormalCopula)
			drwnTestCases::learnSampleShapeModel<drwnCumulativeNet<drwnNormCopula, drwnPixelMarginal> >(type, numSamples, eps, maxIters, fixedIters, stopOne, shuffleEachTime, seed);
		else
			drwnTestCases::learnSampleShapeModel<drwnCumulativeNet<drwnClaytonCopula, drwnPixelMarginal> >(type, numSamples, eps, maxIters, fixedIters, stopOne, shuffleEachTime, seed);

		return 0;
	}
	
	else {
		wcout << "Invalid experiment type" << endl;
		return 1;
	}

	// Check whether offline or online saving of results
	if (!wcscmp(argv[2], L"online"))
		offlineResults = false;
	else if (wcscmp(argv[2], L"offline")) {
		wcout << "Invalid method for saving results" << endl;
		return 1;
	}

	// Type of copula to use
	if (!wcscmp(argv[3], L"clayton"))
		useNormalCopula = false;
	else if (wcscmp(argv[3], L"normal")) {
		wcout << "Invalid copula type" << endl;
		return 1;
	}

	// Type of model
	if (!wcscmp(argv[4], L"chain"))
		typeModel = CHAIN;
	else if (!wcscmp(argv[4], L"loop"))
		typeModel = LOOP;
	else if (!wcscmp(argv[4], L"tree"))
		typeModel = TREE;
	else if (!wcscmp(argv[4], L"grid"))
		typeModel = GRID;
	else {
		wcout << "Invalid model type" << endl;
		return 1;
	}

	// Size of model
	sizeModel = _wtoi(argv[5]);

	// Samples
	if (wcscmp(argv[6], L"samples")) {
		wcout << "Sixth argument must be 'samples'" << endl;
		return 1;
	}

	// Number of samples
	numSamples = _wtoi(argv[7]);

	// Repeats
	if (wcscmp(argv[8], L"repeats")) {
		wcout << "Eighth argument must be 'repeats'" << endl;
		return 1;
	}

	// Number of repeats
	numRepeats = _wtoi(argv[9]);

	// No epsilon for inference experiments
	if (typeExp == LEARNING || typeExp == CMARLEARNING) {
		// Epsilon
		if (wcscmp(argv[10], L"eps")) {
			wcout << "Tenth argument must be 'repeats'" << endl;
			return 1;
		}

		// Epsilon
		eps = _wtof(argv[11]);
	}

	if (typeExp == CMARLEARNING) {
		missingData = _wtof(argv[12]);
		if (argc == 14) {
			if (!wcscmp(argv[13], L"time")) {
				printTiming = true;
			}
			else {
				wcout << "Invalid timing argument" << endl;
				return 1;
			}
		}
	}

	else {
		if (argc == 13) {
			if (!wcscmp(argv[12], L"time")) {
				printTiming = true;
			}
			else {
				wcout << "Invalid timing argument" << endl;
				return 1;
			}
		}
	}

	// Run experiment
	if (typeExp == INFERENCE) {
		if (useNormalCopula) {
			if (typeModel == CHAIN)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateChain<normalCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming, false);
			else if (typeModel == LOOP)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, false);
			else if (typeModel == TREE)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, false);
			else if (typeModel == GRID)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createGrid<normalCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming, false);
		}
		else {
			if (typeModel == CHAIN)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateChain<claytonCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming, true);
			else if (typeModel == LOOP)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateLoop<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, true);
			else if (typeModel == TREE)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, true);
			else if (typeModel == GRID)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createGrid<claytonCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming, true);
		}
	}

	else if (typeExp == LEARNING) {
		if (useNormalCopula) {
			if (typeModel == CHAIN)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateChain<normalCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming, false, eps);
			else if (typeModel == LOOP)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, false, eps);
			else if (typeModel == TREE)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, false, eps);
			else if (typeModel == GRID)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createGrid<normalCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming, false, eps);
		}
		else {
			if (typeModel == CHAIN)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateChain<claytonCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming, true, eps);
			else if (typeModel == LOOP)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateLoop<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, true, eps);
			else if (typeModel == TREE)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, true, eps);
			else if (typeModel == GRID)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createGrid<claytonCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming, true, eps);
		}
	}

	else if (typeExp == CMARLEARNING) {
		if (useNormalCopula) {
			if (typeModel == CHAIN)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateChain<normalCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
			else if (typeModel == LOOP)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
			else if (typeModel == TREE)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
			else if (typeModel == GRID)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createGrid<normalCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
		}
		else {
			if (typeModel == CHAIN)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateChain<claytonCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
			else if (typeModel == LOOP)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateLoop<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
			else if (typeModel == TREE)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
			else if (typeModel == GRID)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createGrid<claytonCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
		}
	}
	
	return 0;
}

// TODO: Combine with previous function to reduce code
int commandLine(std::vector<char *>& args)
{
	// Values we need to extract from arguments
	bool offlineResults = true, useNormalCopula = true;
	unsigned int sizeModel, numSamples, numRepeats;
	bool printTiming = false;
	double missingData, eps;
	enum TypeExperiment { INFERENCE, LEARNING, CMARLEARNING };
	enum TypeModel { CHAIN, LOOP, TREE, GRID };
	TypeExperiment typeExp;
	TypeModel typeModel;

	// Determine type of experiment to perform
	if (!strcmp(args[0], "inference")) {
		typeExp = INFERENCE;
		if (args.size() != 9 && args.size() != 10) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}
	}
	else if (!strcmp(args[0], "learning")) {
		typeExp = LEARNING;
		if (args.size() != 11 && args.size() != 12) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}
	}
	else if (!strcmp(args[0], "cmar")) {
		typeExp = CMARLEARNING;
		if (args.size() != 12 && args.size() != 13) {
			wcout << "Invalid number of commands" << endl;
			return 1;
		}
	}
	else {
		wcout << "Invalid experiment type" << endl;
		return 1;
	}

	// Check whether offline or online saving of results
	if (!strcmp(args[1], "online"))
		offlineResults = false;
	else if (strcmp(args[1], "offline")) {
		wcout << "Invalid method for saving results" << endl;
		return 1;
	}

	// Type of copula to use
	if (!strcmp(args[2], "clayton"))
		useNormalCopula = false;
	else if (strcmp(args[2], "normal")) {
		wcout << "Invalid copula type" << endl;
		return 1;
	}

	// Type of model
	if (!strcmp(args[3], "chain"))
		typeModel = CHAIN;
	else if (!strcmp(args[3], "loop"))
		typeModel = LOOP;
	else if (!strcmp(args[3], "tree"))
		typeModel = TREE;
	else if (!strcmp(args[3], "grid"))
		typeModel = GRID;
	else {
		wcout << "Invalid model type" << endl;
		return 1;
	}

	// Size of model
	sizeModel = atoi(args[4]);

	// Samples
	if (strcmp(args[5], "samples")) {
		wcout << "Fifth argument must be 'samples'" << endl;
		return 1;
	}

	// Number of samples
	numSamples = atoi(args[6]);

	// Repeats
	if (strcmp(args[7], "repeats")) {
		wcout << "Seventh argument must be 'repeats'" << endl;
		return 1;
	}

	// Number of repeats
	numRepeats = atoi(args[8]);

	// No epsilon for inference experiments
	if (typeExp == LEARNING || typeExp == CMARLEARNING) {
		// Epsilon
		if (strcmp(args[9], "eps")) {
			wcout << "Ninth argument must be 'repeats'" << endl;
			return 1;
		}

		// Epsilon
		eps = atof(args[10]);
	}

	if (typeExp == CMARLEARNING) {
		missingData = atof(args[11]);
		if (args.size() == 13) {
			if (!strcmp(args[12], "time")) {
				printTiming = true;
			}
			else {
				wcout << "Invalid timing argument" << endl;
				return 1;
			}
		}
	}

	else {
		if (args.size() == 12) {
			if (!strcmp(args[11], "time")) {
				printTiming = true;
			}
			else {
				wcout << "Invalid timing argument" << endl;
				return 1;
			}
		}
	}

	// Run experiment
	if (typeExp == INFERENCE) {
		if (useNormalCopula) {
			if (typeModel == CHAIN)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateChain<normalCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming);
			else if (typeModel == LOOP)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming);
			else if (typeModel == TREE)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming);
			else if (typeModel == GRID)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createGrid<normalCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming);
		}
		else {
			if (typeModel == CHAIN)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateChain<claytonCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming);
			else if (typeModel == LOOP)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBivariateLoop<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming);
			else if (typeModel == TREE)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming);
			else if (typeModel == GRID)
				drwnTestCases::inferenceExperiment(drwnCumulativeDebug::createGrid<claytonCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming);
		}
	}

	else if (typeExp == LEARNING) {
		if (useNormalCopula) {
			if (typeModel == CHAIN)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateChain<normalCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming, false, eps);
			else if (typeModel == LOOP)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, false, eps);
			else if (typeModel == TREE)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, false, eps);
			else if (typeModel == GRID)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createGrid<normalCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming, false, eps);
		}
		else {
			if (typeModel == CHAIN)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateChain<claytonCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, printTiming, true, eps);
			else if (typeModel == LOOP)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBivariateLoop<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, true, eps);
			else if (typeModel == TREE)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, printTiming, true, eps);
			else if (typeModel == GRID)
				drwnTestCases::learningExperiment(drwnCumulativeDebug::createGrid<claytonCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, printTiming, true, eps);
		}
	}

	else if (typeExp == CMARLEARNING) {
		if (useNormalCopula) {
			if (typeModel == CHAIN)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateChain<normalCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
			else if (typeModel == LOOP)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateLoop<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
			else if (typeModel == TREE)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBinaryTree<normalCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
			else if (typeModel == GRID)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createGrid<normalCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, false, eps);
		}
		else {
			if (typeModel == CHAIN)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateChain<claytonCumulativeNet>(sizeModel - 1), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
			else if (typeModel == LOOP)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBivariateLoop<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
			else if (typeModel == TREE)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createBinaryTree<claytonCumulativeNet>(sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
			else if (typeModel == GRID)
				drwnTestCases::cmarExperiment(drwnCumulativeDebug::createGrid<claytonCumulativeNet>(sizeModel, sizeModel), numSamples, numRepeats, offlineResults, missingData, printTiming, true, eps);
		}
	}
	
	return 0;
}

// Automatically fetch an experiment to be run
int autoExperiments(bool doShutdown)
{
	// Set up HTTP object (I don't think it actually does any interweb stuff yet)
	//WinHttpClient client(L"http://127.0.0.1/schedule.php?command=fetch");
	WinHttpClient client(L"http://www.stefanwebb.com/thesis/schedule.php?command=fetch");
	if (client.GetLastError()) {
		wcout << "Couldn't open upload page" << endl;
		drwnCumulativeDebug::pressAnyKey();
		return 1;
	}

	// Set post data.
	/*string data("command=fetch");
	
	if (!client.SetAdditionalDataToSend((BYTE *)data.c_str(), data.size())) {
		wcout << "Couldn't set data to send" << endl;
		return 1;
	}

	// Set request headers.
	wchar_t szSize[50] = L"";
	swprintf_s(szSize, L"%d", data.size());
	wstring headers = L"Content-Length: ";
	headers += szSize;
	headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
	if (!client.SetAdditionalRequestHeaders(headers)) {
		wcout << "Couldn't set additional request headers" << endl;
		return 1;
	}*/

	while (true) {
		// Send HTTP post request.
		if (!client.SendHttpRequest(L"GET")) {
			wcout << "Couldn't send HTTP request" << endl;
			return 1;
		}

		wstring wCmd = client.GetResponseContent();
		string cmd(wCmd.begin(), wCmd.end());

		if (!cmd.compare("shutdown"))
			break;

		// Split command line string into arguments
		std::vector<char *> args;
		std::istringstream iss(cmd.c_str());

		std::string token;
		while(iss >> token) {
			char *arg = new char[token.size() + 1];
			copy(token.begin(), token.end(), arg);
			arg[token.size()] = '\0';
			args.push_back(arg);
		}

		// now exec with &args[0], and then:
		if (commandLine(args))
			break;

		for(size_t i = 0; i < args.size(); i++)
			delete[] args[i];
	}

	if (true) {
		//MySystemShutdown();
	}

	return 0;
}

BOOL MySystemShutdown()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return( FALSE ); 

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED)) 
		return FALSE; 

	//shutdown was successful
	return TRUE;
}