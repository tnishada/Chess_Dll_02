#pragma once

#include "DLL_definition.h"
#include "Command.h"
#include "States.h"

class BoardState
{
	public:
		static bool CHESS_DLL_API isValidState(Command command, States state);

	private:
		static bool findThreatToKing(Cell *cell, States *state);
		static bool isChecked(States *state, char king);
		static bool isPathClear(Command *command, States *state);
		static bool pieceExists(Command *command, States *state);
		static bool isDestinationClear(Command *command, States *state);
		static bool isOppositePiece(Command *command, States *state);
		static bool isValidCastling(Command *command, States *state);
		static bool isValidEnpass(Command *command, States *state);
};

