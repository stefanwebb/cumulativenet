/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnGdiPlus.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

using namespace Gdiplus;

//! Handles Gdi+ startup and shutdown
// TODO: Inherit from some singleton base class
class drwnGdiPlus
{
public:
	//! ??? 
	static ULONG_PTR m_gdiplusToken;
	static GdiplusStartupInput gdiplusStartupInput;

	//! Default constructor
	drwnGdiPlus()
	{
		// If running for the first time then initialize Gdi+
		if (!isCreated) {
			GdiplusStartup(&drwnGdiPlus::m_gdiplusToken, &drwnGdiPlus::gdiplusStartupInput, NULL);
			drwnGdiPlus::isCreated = true;
		}
	}

	//! Default constructor
	~drwnGdiPlus()
	{
		// Note: "if" not necessary when object is used as in this program
		if (isCreated) {
			GdiplusShutdown(drwnGdiPlus::m_gdiplusToken);
			drwnGdiPlus::isCreated = false;
		}
	}

private:
	//! Has been created before (i.e. implements singleton)
	static bool isCreated;
};


// Static members
ULONG_PTR drwnGdiPlus::m_gdiplusToken;

GdiplusStartupInput drwnGdiPlus::gdiplusStartupInput;

bool drwnGdiPlus::isCreated = false;

// Create global object, so we have Gdi+ initialized on startup and deinitialized on shutdown
drwnGdiPlus globalGdiPlus;