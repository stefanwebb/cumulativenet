/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnBinaryImageDataset.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace Gdiplus;

class drwnBinaryImageDataset
{
//private:
public:
	//! Construct data from an XML file listing dataset files
	drwnBinaryImageDataset(wchar_t* file);

	//! Default destructor
	~drwnBinaryImageDataset();

	//! Print image as ASCII (works best when width is less than width of console
	void printImage(unsigned int i);

	//! Get number of images
	unsigned int numImages() const { return _images.size(); }

	//! Image objects
	vector<Bitmap*> _images;
};