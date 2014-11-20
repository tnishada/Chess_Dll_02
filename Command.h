#pragma once

#include "DLL_definition.h"
#include "Cell.h"
#include "Enumeration.h"


/*Command class stores the data related to a user given command(move)*/

class  Command
{
	public:

		Command();	//constructor

		char piece;			//eg : pawn, Knight	: not specified for castling
		CommandType type;	// capture, move, enpass, castle (king side, queen side)
		Cell currentLocation;
		Cell newLocation;
};
