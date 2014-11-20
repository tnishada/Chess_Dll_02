#include "StdAfx.h"
#include "CommandReader.h"
#include "Enumeration.h"

/*
get the user input and split it into data and 
return command object
this mehtod is invoked only when isValid() returns true;
*/
Command CommandReader::splitCommand( std::string commandString )
{
	Command commandObject;	

	/////////////////////////////////////////
	commandString = removeSpaces(commandString);
	//////////////////////////////////////
	
	commandObject.piece  =  getPiece( commandString );
	commandObject.type  =  getMoveType( commandString );
	commandObject.currentLocation  =  getCurrentCell( commandString );
	commandObject.newLocation  =  getNewCell( commandString );

	return commandObject;
}


bool CommandReader::isValid( std::string command )
{
	command = removeSpaces( command );

	/*if length < 5 only kings side castling is available*/
	if( command.length() < 5 )
	{
		if( isCastling ( command ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*queens side castling*/
	if( command.length() == 5 && isCastling( command ) )
	{
		return true;
	}

	/*
		other types of notations not detected by above 
		conditions eg: o-o+, o-o++ etc.
	*/
	if( isCastling( command ) )
	{
		return true;
	}

	char piece =getPiece( command ); // get peice related to the command
		
	if (piece == '0' ) return false;

	CommandType type = getMoveType( command );

	if( type == invalid )
	{
		return false;
	}

	/* 
	as piece letter is not used for a pawn its cell locations can be taken
	from the begining of the command (position 0) eg: a2-a3

	as piece letter used for other pieces their cell locations can be obtained
	from the second position of the command (position 1) eg: Rg5-g8
	*/
	int position = ( piece=='P') ? 0 : 1;	
	
		if( !( isValidCell(command.at(1 + position) - '1', command.at(0 + position)-'a')  &&
			isValidCell(command.at(4 + position) - '1', command.at(3 + position)-'a')))
		{
			return false;
		}
	
	return true;

}

/*
remove the white spaces of the sent string 
then return it
*/
std::string CommandReader::removeSpaces(std::string str)
{
	unsigned int i;
	for( i=0; i<str.length(); i++)
	{
		if(str[i] == ' ') str.erase(i,1);
	}

	return str;
}

/*
check whether the given cell number is valid (within the scope)
*/
bool CommandReader::isValidCell(int number, int letter)
{
	if( number>=0 && number<=7 && letter>=0 && letter<=7)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
get the trimmed string(no white spaces) and return the piece
involved in the command. return '0' if no piece detected(invalid move)

NOTE : assume every piece is named capital otherwise invalid
*/
char CommandReader::getPiece(std::string trimmedString)
{
	char piece = trimmedString.at(0);

	/*
	as no letter involved with pawns piece should comtains the
	letter of the current cell
	*/
	if(piece>='a' && piece<='h')
	{
		return 'P';
	}


	if(piece == 'R' || piece == 'N' || piece == 'B' || 
		piece == 'K' || piece == 'Q')
	{
		return piece;
	}

	//castling move
	if(piece == 'o')
	{
		return 'O';
	}

	/*
	if none of above conditions met then it is 
	not a valid move
	*/
	return '0';
}


/*
get the trimmed string and return true if it is a castling
move
*/
bool CommandReader::isCastling(std::string trimmedString)
{
	/* kings side castle*/
	if(trimmedString.at(0) == 'o' && trimmedString.at(2) == 'o' && trimmedString.length() == 3)
	{
		return true;
	}	
	/*queens side castle*/
	else if(trimmedString.at(0) == 'o' && trimmedString.at(2) == 'o' && trimmedString.at(4) == 'o'&& trimmedString.length() == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
get the trimmed string(without spaces) and return the type
of the movement related to the command
types : capture, move, enpass, kCastle, qCastle, invalid
*/
CommandType CommandReader::getMoveType(std::string trimmedString){

	char piece = getPiece(trimmedString);
	int movePosition =0;

	if(piece=='O') // castling
	{
		if(trimmedString.length()==3) // kings side castle
		{	
			if(trimmedString.at(1) == '-')
			{
				return kCastle;
			}
			else
			{
				return invalid;
			}
		}

		if(trimmedString.length()==5) // queens side castle
		{
			if(trimmedString.at(1) == '-'&& trimmedString.at(3) == '-')
			{
				return qCastle;
			}
			else
			{
				return invalid;
			}
		}
	}

	if(piece=='P') // pawn
	{
		movePosition = 0;
	}

	if(piece == 'R' || piece == 'N' || piece == 'B' ||   // other types
		piece == 'K' || piece == 'Q')
	{
		movePosition = 1;
	
	}
	
	if(trimmedString.at(2 + movePosition) == '-')
		{
			return move;
		}
		else if(trimmedString.at(2 + movePosition) == 'x')
		{
			return capture;
		}
		else
		{
			return invalid;
		}

	return invalid;

}

/*
get the trimmed string as the parameter and return the 
current cell location of the piece
*/
Cell CommandReader::getCurrentCell(std::string trimmedString)
{
	char piece = getPiece(trimmedString);

	int movePosition =0;

	if(piece == 'O' || piece == '0')
	{
		return  Cell(-1,-1);
	}

	if(piece == 'P')
	{
		movePosition = 0;
	}
	if(piece == 'R' || piece == 'N' || piece == 'B' ||   // other types
		piece == 'K' || piece == 'Q')
	{
		movePosition = 1;
	
	}

	return Cell(trimmedString.at(1+movePosition) -'1', trimmedString.at(0+movePosition) - 'a');

}

/*
get the trimmed string as the parameter and return the 
 cell location of the piece after move is executed
*/

Cell CommandReader::getNewCell(std::string trimmedString)
{
	char piece = getPiece(trimmedString);

	int movePosition;

	//castle or invalid : no cell locations are used
	if(piece == 'O' || piece == '0')
	{
		return  Cell(-1,-1);
	}

	if(piece == 'P')
	{
		movePosition = 0;
	}
	if(piece == 'R' || piece == 'N' || piece == 'B' ||   // other types
		piece == 'K' || piece == 'Q')
	{
		movePosition = 1;
	
	}

	return Cell(trimmedString.at(4+movePosition) -'1', trimmedString.at(3+movePosition) - 'a');

}