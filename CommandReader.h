#pragma once

#include <string>
#include "Command.h"
#include "DLL_definition.h"

class CommandReader
{
	public:
		
		static CHESS_DLL_API bool isValid(std::string command);		
		static CHESS_DLL_API Command splitCommand(std::string command); // invoked only when isvalid() true

		static bool isValidCell(int number, int letter);

	private :
		
		static std::string removeSpaces(std::string str);		
		static char getPiece(std::string trimmedString );
		static bool isCastling(std::string trimmedString);
		static CommandType getMoveType(std::string trimmedString);
		static Cell getCurrentCell(std::string trimmedString);
		static Cell getNewCell(std::string trimmedString);
};

