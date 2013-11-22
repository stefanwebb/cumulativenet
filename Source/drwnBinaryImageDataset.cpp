/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnBinaryImageDataset.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include <rapidxml/rapidxml.hpp>
#include "drwnBinaryImageDataset.h"

#include <string>

using namespace std;
using namespace rapidxml;

// Construct data from an XML file listing dataset files
// NOTE: File must be in ASCII format!! UTF-8 byte mark will stuff it up
drwnBinaryImageDataset::drwnBinaryImageDataset(wchar_t* file)
{
	// Open file and read contents into buffer
	std::ifstream t;
	int length;
	t.open(file, std::ifstream::in | std::ifstream::binary);      // open input file

	// TODO: Error handling!!

	t.seekg(0, std::ios::end);    // go to the end
	length = (int) t.tellg();           // report location (this is the length)
	t.seekg(0, std::ios::beg);    // go back to the beginning
	char* buffer = new char[length + 1];    // allocate memory for a buffer of appropriate dimension
	t.read(buffer, length);       // read the whole file into the buffer
	t.close();                    // close file handle

	// Make zero-terminated string
	buffer[length] = 0;

	// TODO: XML processing
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(buffer);    // 0 means default parse flags

	xml_node<> *node = doc.first_node();
	DRWN_ASSERT_MSG(!strcmp(node->name(), "dataset"), "First node was not 'dataset'");

	xml_attribute<> *pathAttr = node->first_attribute();

	// Create black and white palette
	ColorPalette* pal = (ColorPalette*)malloc(sizeof(ColorPalette) + sizeof(ARGB));
	pal->Count = 2;
	pal->Flags = 0;
  
	Bitmap::InitializePalette(pal, PaletteTypeFixedBW, 0, FALSE, NULL); 

	// Loop over nodes
	for (node = node->first_node(); node != 0; node = node->next_sibling()) {
		// Check that name is correct
		DRWN_ASSERT_MSG(!strcmp(node->name(), "item"), "Node inside 'dataset' was not called 'item'");

		// Check that has filename
		xml_attribute<> *attr = node->first_attribute();
		DRWN_ASSERT_MSG(!strcmp(attr->name(), "file"), "Attribute of 'item' was not called 'file'");
		DRWN_ASSERT_MSG(attr->value_size() > 0, "'file' attribute was empty");

		// Construct filename
		wstring path;
		if (pathAttr->value() != 0) {
			path.append(pathAttr->value(), pathAttr->value() + pathAttr->value_size());
			path.append(L"/");
			path.append(attr->value(), attr->value() + attr->value_size());
		}

		// Open image
		Bitmap* horse = new Bitmap(path.c_str(), false);
		DRWN_ASSERT_MSG(horse->GetLastStatus() == Ok, "Couldn't open image");

		// Convert to binary
		horse->ConvertFormat(
			PixelFormat8bppIndexed,
			DitherTypeSolid,
			PaletteTypeCustom,
			pal,
			0);
		DRWN_ASSERT_MSG(horse->GetLastStatus() == Ok, "Couldn't convert image format");

		_images.push_back(horse);
	}

	// Free palate
	free(pal);
	
	// Free file string buffer
	delete [] buffer;
}

// Default destructor
drwnBinaryImageDataset::~drwnBinaryImageDataset()
{
	while (_images.size()) {
		delete _images.back();
		_images.pop_back();
	}
}

// Print image as ASCII (works best when width is less than width of console
void drwnBinaryImageDataset::printImage(unsigned int i)
{
	// Copy the horse image to a memory buffer
	//unsigned char* horseData = new unsigned char[horse.GetHeight() * horse.GetWidth()];

	BitmapData* bitmapData = new BitmapData;

	_images[i]->LockBits(
		&Rect(0, 0, _images[i]->GetWidth(), _images[i]->GetHeight()),
		ImageLockModeRead,
		PixelFormat8bppIndexed,
		bitmapData);

	DRWN_ASSERT_MSG(_images[i]->GetLastStatus() == Ok, "Couldn't lock bitmap bits");

	// Try "printing" image
	for (unsigned int y = 0; y < _images[i]->GetHeight(); y++) {
		for (unsigned int x = 0; x < _images[i]->GetWidth(); x++) {
			if (((unsigned char*)bitmapData->Scan0)[x + y * bitmapData->Width] == 0)
				wcout << "0";
			else if (((unsigned char*)bitmapData->Scan0)[x + y * bitmapData->Width] == 1)
				wcout << "1";
			else
				wcout << "?";
		}
		wcout << endl;
	}

	_images[i]->UnlockBits(bitmapData);

	delete bitmapData;
}