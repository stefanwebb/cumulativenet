/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnSamples.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//#include "drwnCumulativeNet.h"
//#include "drwnCumulativeUtils.h"

#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

// DEBUG: Forward declaration of files to output sampling algorithm values
namespace drwnCumulativeUtils {
	//extern wofstream firstVar, secondVar, kS, firstTerms, secondTerms, denom;
}

class drwnBinaryImageDataset;

enum TypeImage { JPG, PNG, BMP, TIFF };

//! This class is a wrapper for samples.
template <class G>
class drwnSamples
{
private:
	//! Memory for samples
	typename G::marginal_type::domain_type* samples_;

	//! Memory for transform of sample
	typename G::marginal_type::range_type* tempSamples_;

	//! Memory for gradient
	//V* gradient_;

	//! Number of variables in sample
	unsigned int numVars_;

	//! Number of samples in this object (note that a sample can be represented as a number of these objects if more convenient)
	unsigned int numSamples_;

	//! Value to set marginalized variables to
	// NOTE: Must have been constructed with clique tree for this to be valid. This value mainly used for debugging
	typename G::marginal_type::domain_type margValue_;

	boost::random::mt19937 rng; 

	boost::random::uniform_int_distribution<> randomSample;

	boost::random::uniform_int_distribution<>::param_type randomParams;

public:
	//! Construct sample object by assigning a new block of memory
	drwnSamples(unsigned int numVars, unsigned int numSamples);

	//! Draw samples from network and save to memory
	drwnSamples(const G& cn, unsigned int numSamples, bool useBrents = true, bool outputProgress = true, bool takeLog = false);

	//! Draw samples from model conditioning on some variables and save to memory
	drwnSamples(const G& cn, unsigned int numSamples, const set<int>& vars, typename G::marginal_type::domain_type* cond, bool outputProgress = true, bool takeLog = false);

	//! Construct sample object from an existing block of memory, and take ownership
	drwnSamples(unsigned int numVars, typename G::marginal_type::domain_type* samples, unsigned int numSamples);

	//! Construct sample object from binary image dataset
	drwnSamples(drwnBinaryImageDataset& images);

	//! Default destructor
	~drwnSamples();

	//! Initialize variables and memory
	// NOTE: Delegating constructors doesn't seem to work on VS2012, so I had to break the initialization into separate function
	void drwnSamples<G>::initialize(unsigned int numVars, unsigned int numSamples);

	//! Returns number of samples in object
	unsigned int numSamples() const { return numSamples_; }

	//! Returns number of variables in a sample
	unsigned int numVars() const { return numVars_; }

	//! Return reference to start of sample
	typename G::marginal_type::domain_type* operator[](int i) { return &samples_[numVars() * i]; }

	//! Get sample
	typename G::marginal_type::domain_type* const sample(unsigned int i) const { return &samples_[numVars() * i]; }

	//! Get memory to store temporary sample
	typename G::marginal_type::range_type* temp() const { return tempSamples_; }

	//! Get memory to store temporary transform of sample
	typename G::marginal_type::range_type* transform() const { return &tempSamples_[numVars()]; }

	//! Get memory to store temporary sample transformed for copula
	typename G::marginal_type::range_type* copula() const { return &tempSamples_[numVars() << 1]; }

	//! Get memory to store temporary sample for variables that are conditioned on
	typename G::marginal_type::range_type* cond() const { return &tempSamples_[numVars() * 3]; }

	//! Copy a block of memory into sample
	void copy(typename G::marginal_type::domain_type* samples);

	//! Print samples to csv file
	void print(const G& cn, const wchar_t* filename);

	//! Print sample to screen
	void print(unsigned int i, const G& cn);

	//! Print binary sample to screen (for debug purposes)
	void print(unsigned int i, unsigned int xres, unsigned int yres);

	//! Print binary temporary sample to screen
	void print(unsigned int xres, unsigned int yres);

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		ImageCodecInfo* pImageCodecInfo = NULL;

		GetImageEncodersSize(&num, &size);
		if(size == 0)
			return -1;  // Failure

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if(pImageCodecInfo == NULL)
			return -1;  // Failure

		GetImageEncoders(num, size, pImageCodecInfo);

		for(UINT j = 0; j < num; ++j)
		{
			if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}    
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}

	/* Save the pixel buffer to an image. */
	bool save(unsigned int i, unsigned int xres, unsigned int yres, wchar_t* filename, TypeImage type = BMP)
	{
		// Local variables
		CLSID encoderClsid;
		EncoderParameters encoderParameters;
		ULONG quality;
		Status stat;

		DRWN_ASSERT_MSG(xres * yres == numVars(), "Invalid dimension to save sample as image (doesn't equal number of variables)")

		// Create a bitmap from the pixel buffer.
		char* pixels = new char[xres * yres * 4];
		Bitmap* image = new Bitmap(xres, yres, 4 * yres, PixelFormat32bppRGB, (BYTE*) pixels);

		// Copy image to bitmap
		//unsigned int pixelIdx = 0;
		for (unsigned int j = 0, pixelIdx = 0; j < numVars(); j++) {
			char pixelColour = (sample(i)[j] > 0) ? 0: 255;
			pixels[pixelIdx++] = pixelColour;
			pixels[pixelIdx++] = pixelColour;
			pixels[pixelIdx++] = pixelColour;
			pixels[pixelIdx++] = 0;
		}

		// Get the CLSID of the desired encoder.
		if (type == JPG)
			GetEncoderClsid(L"image/jpeg", &encoderClsid);
		else if (type == PNG)
			GetEncoderClsid(L"image/png", &encoderClsid);
		else if (type == BMP)
			GetEncoderClsid(L"image/bmp", &encoderClsid);
		else if (type == TIFF)
			GetEncoderClsid(L"image/tiff", &encoderClsid);

		// If we encoding a JPEG image then set quality to 90 and save with parameters
		if (type == JPG) {
			encoderParameters.Count = 1;
			encoderParameters.Parameter[0].Guid = EncoderQuality;
			encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
			encoderParameters.Parameter[0].NumberOfValues = 1;
			quality = 90;
			encoderParameters.Parameter[0].Value = &quality;
			stat = image->Save(filename, &encoderClsid, &encoderParameters);
		}

		// Otherwise save without parameters
		else
			stat = image->Save(filename, &encoderClsid, NULL);

		// Free image memory
		delete image;

		// Generate error if couldn't save to file
		if (stat != Ok) {
			DRWN_LOG_ERROR("Couldn't save sample to image!")
			delete [] pixels;
			return false;
		}

		delete [] pixels;

		// Return true
		return true;   
	}

	//! Marginalize variable
	void marginalizeVariable(unsigned int i)
	{
		DRWN_ASSERT_MSG(i < numVars_, "Invalid variable number to marginalize.")	 

		// Marginalizing a variable means setting it to 1
		for (unsigned int j = 0; j < numSamples_; j++)
			sample(j)[i] = margValue_;
	}

	//! Make a given percentage of data missing
	void makeMissing(double fraction)
	{
		DRWN_ASSERT_MSG(fraction > 0 && fraction < 1, "Invalid fraction of data to make missing.")
		unsigned int numEntries = numVars_ * numSamples_, numToClear = ceil((double) numEntries * (double) fraction);

		// Continue until we have cleared correct number of samples
		// NOTE: This could go on forever if variables have been marginalized already, and if fraction is high it will take a long time
		// TODO: Better method for selecting fraction of data
		while(numToClear) {
			unsigned int randEntry = randomSample(rng, randomParams);
			if (samples_[randEntry] != margValue_) {
				samples_[randEntry] = margValue_;
				numToClear--;
			}
		}
	}

	//! Load sample into temporary structures, ready to be used by cacheDerivatives and density etc.
	drwnSamples& load(G& cn, const unsigned int i) {
		// Calculate U_i, V_i, and W_i
		for (unsigned j = 0; j < numVars(); j++) {
			transform()[j] = cn.marginal(j).cdf(((*this)[i])[j]);
			copula()[j] = pow(transform()[j], 1.0 / ((double) cn.howManyScopes(j)));
			if (G::factor_type::isTransformCopula())
				temp()[j] = G::factor_type::transformArgument(copula()[j]);
		}

		// Reason why I return reference to this is so you can put into cacheFactors method
		return *this;
	}
};

typedef drwnSamples<normalCumulativeNet> normalSamples;

typedef drwnSamples<bernoulliCumulativeNet> bernoulliSamples;

typedef drwnSamples<exponentialCumulativeNet> exponentialSamples;

typedef drwnSamples<gumbelCumulativeNet> gumbelSamples;

typedef drwnSamples<claytonCumulativeNet> claytonSamples;

typedef drwnSamples<frankCumulativeNet> frankSamples;

// Print binary sample to screen
template <class G>
void drwnSamples<G>::print(unsigned int i, unsigned int xres, unsigned int yres)
{
	DRWN_ASSERT_MSG(numVars() == xres * yres, "Invalid image resolution")

	// Try "printing" image
	for (unsigned int y = 0; y < yres; y++) {
		for (unsigned int x = 0; x < xres; x++) {
			typename G::marginal_type::domain_type value = sample(i)[x + y * xres];
			if (value < 0)
				cout << " ";
			else if (value >= 0)
				cout << "\xdb";
			else
				cout << "?";
		}
		cout << endl;
	}
}

// Print binary temporary sample to screen
template <class G>
void drwnSamples<G>::print(unsigned int xres, unsigned int yres)
{
	DRWN_ASSERT_MSG(numVars() == xres * yres, "Invalid image resolution")

	// Try "printing" image
	for (unsigned int y = 0; y < yres; y++) {
		for (unsigned int x = 0; x < xres; x++) {
			typename G::marginal_type::domain_type value = temp()[x + y * xres];
			if (value < 0)
				cout << " ";
			else if (value >= 0)
				cout << "\xdb";
			else
				cout << "?";
		}
		wcout << endl;
	}
}

//! Print samples to csv file
template <class G>
void drwnSamples<G>::print(const G& cn, const wchar_t* filename)
{
	// Open file
	wofstream myfile;
	myfile.open(filename);
	myfile.precision(std::numeric_limits<double>::digits10);

	// Check that we were able to open file
	DRWN_ASSERT_MSG(myfile.is_open(), "Unable to open file")
		
	// Write first column with variable names
	for (unsigned int i = 0; i < cn.numVariables(); i++) {
		myfile << cn.getUniverse()->varName(i).c_str();
		if (i < cn.numVariables() - 1)
			myfile << " ";
		else
			myfile << endl;
	}

	// Generate samples
	for (unsigned int i = 0; i < numSamples(); i++) {
		// Print sample to file
		for (unsigned int j = 0; j < cn.numVariables(); j++) {
			myfile << sample(i)[j];
			if (j < cn.numVariables() - 1)
				myfile << " ";
			else if (i != numSamples() - 1)
				myfile << endl;
		}
	}

	// Close file
	myfile.close();
}

//! Print sample to screen
template <class G>
void drwnSamples<G>::print(unsigned int i, const G& cn)
{
	// Write first column with variable names
	for (unsigned int j = 0; j < cn.numVariables(); j++) {
		wcout << cn.getUniverse()->varName(j).c_str();
		if (j < cn.numVariables() - 1)
			wcout << L" ";
		else
			wcout << endl;
	}

	// Print sample
	for (unsigned int j = 0; j < cn.numVariables(); j++) {
		wcout << (*this)[i][j];
		if (j < cn.numVariables() - 1)
			wcout << " ";
	}

	wcout << endl;
}

// Construct sample object by assigning a new block of memory
template <class G>
drwnSamples<G>::drwnSamples(unsigned int numVars, unsigned int numSamples)
{
	initialize(numVars, numSamples);
}

// Construct sample object from an existing block of memory, and take ownership
template <class G>
drwnSamples<G>::drwnSamples(unsigned int numVars, typename G::marginal_type::domain_type* samples, unsigned int numSamples)
{
	samples_ = samples;
	numVars_ = numVars;
	numSamples_ = numSamples;
	tempSamples_ = new typename G::marginal_type::range_type[numVars * 3];
}

// Construct sample object from binary image dataset
template <class G>
drwnSamples<G>::drwnSamples(drwnBinaryImageDataset& images)
{
	// Copy the horse image to a memory buffer
	//unsigned char* horseData = new unsigned char[horse.GetHeight() * horse.GetWidth()];

	DRWN_ASSERT_MSG(images.numImages(), "Image dataset is empty!")

	initialize(images._images[0]->GetWidth() * images._images[0]->GetHeight(), images.numImages());

	BitmapData* bitmapData = new BitmapData;

	unsigned int writePosition = 0;
	for (unsigned int i = 0; i < numSamples(); i++) {
		DRWN_ASSERT_MSG(images._images[i]->GetWidth() * images._images[i]->GetHeight() == numVars(), "All images must be of identical size!")

		images._images[i]->LockBits(
			&Rect(0, 0, images._images[i]->GetWidth(), images._images[i]->GetHeight()),
			ImageLockModeRead,
			PixelFormat8bppIndexed,
			bitmapData);

		DRWN_ASSERT_MSG(images._images[i]->GetLastStatus() == Ok, "Couldn't lock bitmap bits")

		// Copy image into sample
		for (unsigned int j = 0; j < numVars(); j++) {
			if (((unsigned char*) bitmapData->Scan0)[j])
				samples_[writePosition] = 1.0;
			else
				samples_[writePosition] = -1.0;
			writePosition++;
		}

		images._images[i]->UnlockBits(bitmapData);
	}

	delete bitmapData;
}

// Initialize variables and memory
// NOTE: Delegating constructors doesn't seem to work on VS2012, so I had to break the initialization into separate function
template <class G>
void drwnSamples<G>::initialize(unsigned int numVars, unsigned int numSamples)
{
	randomParams = boost::random::uniform_int_distribution<>::param_type(0, numVars * numSamples - 1);
	
	if (numSamples)
		samples_ = new typename G::marginal_type::domain_type[numVars * numSamples];
	else
		samples_ = 0;
	tempSamples_ = new typename G::marginal_type::range_type[numVars * 4];
	numVars_ = numVars;
	numSamples_ = numSamples;
}

// Draw samples from model conditioning on some variables and save to memory
template <class G>
drwnSamples<G>::drwnSamples( const G& cn, unsigned int numSamples, const set<int>& vars, typename G::marginal_type::domain_type* conditioned, bool outputProgress, bool takeLog)
{
	initialize(cn.numVariables(), numSamples);

	// Initialize conditioned variables
	//for (unsigned int i = 0; i < numVars(); i++)
	for (auto& v: vars)
		cond()[v] = cn.marginal(v).cdf(conditioned[v]);
	
	drwnSampleCliques<G> sc(cn, vars);

	margValue_ = sc.model().marginal(0).quantile(1.0);

	// Generate samples
	for (unsigned int i = 0; i < numSamples; i++) {
		if (outputProgress) {
			if (i % 200 == 0)
				wcout << "Sample #" << i << endl;
		}

		// Choose root finding method according to flag
		drwnCumulativeUtils::sample(sc, *this, i, takeLog);
	}
}

// Draw samples from network and save to memory
template <class G>
drwnSamples<G>::drwnSamples(const G& cn, unsigned int numSamples, bool useBrents, bool outputProgress, bool takeLog)
{
	initialize(cn.numVariables(), numSamples);
	
	// DEBUG: Reseed random number generator
	/*unsigned int seed = 1355985858;		//std::time(0);
	drwnCumulativeUtils::rng.seed(seed);
	wcout << "Seed: " << seed << endl;*/

	// Create array of clique trees for sampling and data structure to hold temporary sample
	// DEBUG: fix our random seed, so that we have same sample cliques every time
	//srand (10);
	drwnSampleCliques<G> sc(cn);
	//drwnCumulativeUtils::printSampleCliques(sc);

	margValue_ = sc.model().marginal(0).quantile(1.0);

	// Generate samples
	for (unsigned int i = 0; i < numSamples; i++) {
		// DEBUG: Output sample progress every so often
		// NOTE: We control this by a flag so mass sampling experiments aren't slowed down
		if (outputProgress) {
			if (i % 200 == 0)
				wcout << "Sample #" << i << endl;
		}

		// Choose root finding method according to flag
		if (useBrents)
			drwnCumulativeUtils::sample(sc, *this, i, takeLog);
		else
			drwnCumulativeUtils::sample2(sc, *this, i);
	}
}

// Default destructor
template <class G>
drwnSamples<G>::~drwnSamples()
{
	if (samples_)
		delete [] samples_;
	delete [] tempSamples_;
}

// Copy a block of memory into sample
template <class G>
void drwnSamples<G>::copy(typename G::marginal_type::domain_type* samples)
{
	// Loop over elements of samples
	for (unsigned int i = 0; i < numSamples() * numVars(); i++)
		samples_[i] = samples[i];
}