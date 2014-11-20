#pragma once

/*cell store the number and letter of a chess board cell*/

#include "DLL_definition.h"
class CHESS_DLL_API Cell
{
	public:
		int number ;
		int letter ;
		Cell(int, int);
		Cell();
		~Cell();
};



