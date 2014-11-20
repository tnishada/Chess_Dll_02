#pragma once

#include "DLL_definition.h"
#include "Enumeration.h"

/*used to keep all the states of the game*/

class CHESS_DLL_API States{

	public:
		Player currentPlayer;
		char board[8][8];	//represent 64 cells

		/* for below 2 arrays: 0=> white 1=>black*/
		bool enpass[2][8]; 
		bool castle[2];  
		bool castleR[2][2]; //  [0 - white, 1 - black][0 - letter 0 , 1 - letter 7]

};