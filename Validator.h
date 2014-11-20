#pragma once

#include "States.h"
#include <string>
#include "Command.h"

class Validator
{
public:
	/*
	if the command is valid return the command object 
	otherwise return null

	check validity in all the aspects:
		move validity, board validity, command validity
	*/
	static Command * getCommand(States *state, std::string command);
};

