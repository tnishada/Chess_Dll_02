#include "DLL_definition.h"
#include "Cell.h"
#include "Command.h"
#include <string> 
#include "States.h"


class ChessBoard
{

	public :
		static CHESS_DLL_API bool  invokeMove(std::string command, States *state);

	private :
		static void executeCommand(Command *command, States *state);
	
};