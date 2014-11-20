#pragma once

#include "DLL_definition.h"
#include "Command.h"
#include "States.h"
class Movement
{
	public:
		static bool isValidMovement(Command *command, States *state);
		//static bool CHESS_DLL_API isValidMove(Command command, States state);
		//static bool CHESS_DLL_API isValidCapture(Command command , States state);

	private:

		//static  void Move(char board[][8], int currentN, int currentL, int newN, int newL);
		static bool isValidRooksMove(Command *command);
		static bool isValidBishopsMove(Command *command);
		static bool isValidKnightsMove(Command *command);
		static bool isValidKingsMove(Command *command);
		static bool isValidQueensMove(Command *command);
		static bool isValidPawnsMove(Command *command, States *state );
		static bool isValidPawnsCapture(Command *command, States *state);
		static bool isvalidEnpassMove(Command *command, States *state);
		
		
};

