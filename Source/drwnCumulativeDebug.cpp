/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnCumulativeDebug.cpp
** AUTHOR(S):   Stephen Gould <stephen.gould@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"
#include "drwnCumulativeDebug.h"

// Function that waits until user has pressed a key. Stops program from exiting before we have examined output
void drwnCumulativeDebug::pressAnyKey()
{
	char ch;
	std::cin.get(ch);
}