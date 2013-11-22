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

/*! \brief A type of class with which you communicate, and need to determine if it is ready to do so. */
class drwnAgent
{
public:
	/*! \brief Default constructor. */
	drwnAgent();

	/*! \brief Determine if agent is ready to receive communication.
		\return True if agent is ready. False, otherwise. */
	bool isReady();

protected:
	/*! \brief Sets the state of the agent.
		\param state True if agent is ready. False, otherwise. */
	void setState(bool state);

	/*! \brief Flag holding state of agent. */
	bool isReady_;
};