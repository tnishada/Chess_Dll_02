// Chess_DLL_2.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Main_DLL.h"
#include "CommandReader.h"
#include "Validator.h"
#include <iostream>

/*this functions is invoked by the application
	return true if valid move otherwise false
*/
bool ChessBoard::invokeMove(std::string command, States *state){
	
	Command *cm = Validator::getCommand(state, command);

	if(cm == 0)
	{
		return false;
	}
	else
	{
		executeCommand(cm,state);
		return true;
	}	
}

/*
	if the command is valid  then execute it
*/
void ChessBoard::executeCommand(Command *command, States *state)
{
	state->board[ command->newLocation.number ][ command->newLocation.letter ] = 
		state->board[ command->currentLocation.number ][ command->currentLocation.letter ];

	state->board[ command->currentLocation.number ][ command->currentLocation.letter ] = '0';
}