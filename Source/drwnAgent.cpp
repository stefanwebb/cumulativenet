/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Copyright (c) 2007-2012, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    drwnAgent.cpp
** AUTHOR(S):   Stefan Webb <stefan.webb@anu.edu.au>
**
*****************************************************************************/

#include "stdafx.h"

/* Default Constructor. */
drwnAgent::drwnAgent()
{
	// Set state to default of false.
	isReady_ = false;
}

/* Determine if agent is ready to receive communication. */
bool drwnAgent::isReady()
{
	// Return state
	return isReady_;
}

/* Sets the state of the agent. */
void drwnAgent::setState(bool state)
{
	// Set state to new value.
	isReady_ = state;
}