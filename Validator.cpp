#include "StdAfx.h"
#include "Validator.h"
#include "Movement.h"
#include "CommandReader.h"
#include "BoardState.h"

Command *Validator::getCommand(States *state, std::string commandString)
{
	Command *c =0;
	/*
	check whether the given comman is valid
	check only the command string(cells , move types)
	*/
	if( !CommandReader::isValid( commandString ))
	{		
		return 0;
	}

	c = &CommandReader::splitCommand( commandString );

	if(Movement::isValidMovement( c , state ) )
	{
		
		if(BoardState::isValidState( *c , *state ))
		{
			return c;
		}		
	}
	
	return 0;		
}
