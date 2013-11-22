/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnWindow.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

#include "drwnWindow.h"

using namespace Gdiplus;

wchar_t *windowName = L"Output";

// Initialize critical section
//CRITICAL_SECTION drwnWindow::CriticalSection_;

// Default constructor
drwnWindow::drwnWindow(unsigned int width, unsigned int height, bool display)
{
	// State is good until otherwise proven
	setState(true);

	// Initialize variables and declare memory for pixels
	width_ = width;
	height_ = height;
	threadId_ = 0;
	drwnWindow* thisWindow = 0;

	// Create thread to display image (if requested)
	if (display) {
		threadId_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadProc, this, 0, NULL);
		//SetThreadPriority(threadId_, THREAD_PRIORITY_LOWEST);
	}

	// Increment object count
	//objectCount_++;
}

// Default destructor
drwnWindow::~drwnWindow()
{
	// Close thread handle if was created
	if (threadId_)
		CloseHandle(threadId_);
}

// TODO: Can replace this with functor?
LPTHREAD_START_ROUTINE drwnWindow::ThreadProc(LPVOID lpParameter)
{
	// Enter critical section
	//EnterCriticalSection(&CriticalSection_); 

	WNDCLASS wndClass;
    HWND hWnd;
    MSG msg;

	drwnWindow* thisWindow = ((drwnWindow*) lpParameter);

    // Register window class
    wndClass.style = CS_HREDRAW | CS_VREDRAW;	// CS_OWNDC |
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = 0; // NEEDS SOMETHING HERE?
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) COLOR_BACKGROUND; //0;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName =  thisWindow->className_;
	thisWindow->className_[0] = 0;

	//int randomNumber = rand();
	_itow_s((int)thisWindow,  thisWindow->className_, 16);
    RegisterClass(&wndClass);

    // Create window
    hWnd = CreateWindow(thisWindow->className_, windowName, (WS_OVERLAPPEDWINDOW | 
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS) & ~WS_THICKFRAME, (GetSystemMetrics(SM_CXSCREEN) - thisWindow->width_)/2, 
		(GetSystemMetrics(SM_CYSCREEN) - thisWindow->height_)/2, thisWindow->width_ + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
		thisWindow->height_ + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION), NULL, NULL, 0, &thisWindow);

	thisWindow->hWnd_ = hWnd;

	// Display window
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

	// Leave critical section
	//LeaveCriticalSection(&CriticalSection_);

    // Process messages
    while (GetMessage(&msg, NULL, 0, 0) == TRUE) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (LPTHREAD_START_ROUTINE) msg.wParam;
}

LRESULT APIENTRY drwnWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Get pointer to Window object 
	drwnWindow* thisWindow = (drwnWindow*) GetWindowLong(hWnd, GWL_USERDATA);
	//int debugValue;

	// Handle messages
    switch (message) {
    case WM_CREATE:
        // Set the userdata to the correct Window object
		thisWindow = *((drwnWindow**)(((CREATESTRUCT*)lParam)->lpCreateParams));
		SetWindowLong(hWnd, GWL_USERDATA, (LONG) thisWindow);

		// Get device context handle
        thisWindow->hDC_ = GetDC(hWnd);
		DRWN_ASSERT_MSG(thisWindow->hDC_, "Function GetDC failed.");
		
        return 0;
    case WM_DESTROY:
		ReleaseDC(hWnd, thisWindow->hDC_);
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
		// Track window size changes
        /*if (thisWindow->hGLRC_) {
            int winWidth = (int) LOWORD(lParam);
            int winHeight = (int) HIWORD(lParam);
            
            return 0;
        }*/
    case WM_PAINT:
        {
            /*PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            if (thisWindow->hGLRC_) {
				drawFrame(thisWindow);
            }
            EndPaint(hWnd, &ps);*/

			HDC          hdc;
			PAINTSTRUCT  ps;
			Pen*         myPen;
			Graphics*    myGraphics;
			hdc = BeginPaint(hWnd, &ps);
			myPen = new Pen(Color(255, 255, 0, 0), 3);
			myGraphics = new Graphics(hdc);
			myGraphics->DrawLine(myPen, 20, 10, 200, 100);
			delete myGraphics;
			delete myPen;
			EndPaint(hWnd, &ps);

            return 0;
        }
        break;
    case WM_CHAR:
        /* handle keyboard input */
        switch ((int)wParam) {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            return 0;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}