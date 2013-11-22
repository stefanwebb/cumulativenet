/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnWindow.h
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#pragma once

//#pragma comment(lib, "Gdiplus.lib")

#include "drwnAgent.h"

using namespace std;

/*! \brief Enumeration of supported image output types. */
// TODO: Put in namespace
/*enum TypeImage
{
	JPG, PNG, BMP, TIFF
};*/

/*! \brief Simplifies construction of Win32 windows. */
class drwnWindow : public drwnAgent
{
public:
	/*! Constructor with width, height, and flag to display or not. */
	drwnWindow(unsigned int width, unsigned int height, bool display = true);

	/*! Default destructor. */
	~drwnWindow();

	/*! Count for objects. */
	//static int objectCount_;

private:
	static LRESULT APIENTRY WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LPTHREAD_START_ROUTINE ThreadProc(LPVOID lpParameter);

	//static CRITICAL_SECTION CriticalSection_;

	HANDLE threadId_;

	unsigned int width_;

	unsigned int height_;

	HDC hDC_;

	HGLRC hGLRC_;

	HWND hWnd_;

	wchar_t className_[16];
};