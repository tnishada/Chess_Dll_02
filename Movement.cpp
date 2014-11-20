#include "StdAfx.h"
#include "Movement.h"
#include <math.h>

/*
get the Command object and determine whether a valid rooks move
*/
bool Movement::isValidRooksMove( Command *command ){ 
	//  letter(L) => column letter of the board
	// Number(N) => row number of the board

	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;

	if( currentL == newL && currentN == newN ){ // both cells are same
		return false;
	}

	if( currentL== newL || currentN == newN ){
		return true;
	}
	else
	{
		return false;
	}
}

/*
get the Command object and determine whether a valid bishops move
*/
bool Movement::isValidBishopsMove( Command *command ){

	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;

	if( currentL == newL && currentN == newN )// both cells are same
	{ 
		return false;
	}

	if( abs( currentL - newL ) == abs( currentN - newN ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
get the Command object and determine whether a valid Knights move
*/
bool Movement::isValidKnightsMove( Command *command ){

	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;

	if( currentL == newL && currentN == newN )// both cells are same
	{ 
		return false;
	}

	if(abs( currentN - newN ) == 1 && abs( currentL - newL ) == 2 )
	{
		return true;
	}
	else if( abs( currentL - newL ) == 1 && abs( currentN - newN ) == 2 )
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*
get the Command object and determine whether a valid Kings move
*/
bool Movement::isValidKingsMove( Command *command ){

	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;

	if( currentL == newL && currentN == newN )// both cells are same
	{ 
		return false;
	}

	if(abs( currentN - newN ) <= 1 && abs ( currentL - newL ) <= 1 )
	{ // king can move only 1 cell at any direction
		return true;
	}
	else
	{
		return false;
	}
}

/*
get the Command object and determine whether a valid queens move
*/
bool Movement::isValidQueensMove(Command *command)
{ // queen can move as a bishop or a rook

	if(Movement::isValidBishopsMove( command ) || Movement::isValidRooksMove( command) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*
get the Command objec and state object and determine whether a valid pawns move
*/
bool Movement::isValidPawnsMove(Command *command, States *state )// board is used to check the pawn's location
{ 

	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;

	

	if( currentL == newL && currentN == newN )// both cells are same
	{ 
		return false;
	}

	if( currentL != newL )
	{ // if pawn is moved horizontally (invalid move)
		return false;
	}

	if((state->board[currentN][currentL]=='P' && newN > currentN) ||
		( state->board[currentN][currentL]=='p' && currentN>newN))// capital p => white pawn, newN>currentN => moving direction is +
	{	// simple p => black pawn, newN<currentN => moving direction is -

		if(currentN==1 || currentN==6)// white pawn is not moved yet
		{ 
			if(abs( currentN-newN ) == 1 || abs( currentN - newN ) == 2 )
			{
				return true;
			}			
		}
		else
		{ 
			if( abs( currentN - newN ) == 1 )
			{
				return true;
			}
		}
	}

	return false;
}

/*
as pawns capture deffers from its movement a new functions is
implemented to check a pawns cut
*/
bool Movement::isValidPawnsCapture( Command *command , States *state )
{
	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;

	if( currentL == newL && currentN == newN )// both cells are same
	{ 
		return false;
	}

	if( (state->board[ currentN ][ currentL ] == 'P' && newN > currentN ) ||
		( state->board [currentN ] [currentL ] == 'p' && currentN>newN))// capital p => white pawn, newN>currentN => moving direction is +
		{	// simple p => black pawn, newN<currentN => moving direction is -

			if( abs( currentL - newL) == 1)
			{
				return true;
			}
		}

	return false;
}

bool Movement::isValidMovement( Command *command, States *state )
{


	/*only pawns cut differs from move*/
	if(( command->piece == 'p' || command->piece == 'P' ))
	{
		if(command->type == move)
		{
			return isValidPawnsMove(command , state);
		}
		else
		{
			return isValidPawnsCapture(command, state);
		}
	}
	else // move and captur same for other pieces
	{
		if( command->piece == 'K' || command->piece == 'k' )
		{
			return isValidKingsMove( command );
		}

		if( command->piece == 'Q' || command->piece == 'q' )
		{
			return isValidQueensMove( command );
		}
		if( command->piece == 'B' || command->piece == 'b' )
		{
			return isValidBishopsMove( command );
		}
		if( command->piece == 'N' || command->piece == 'n' )
		{
			return isValidKnightsMove( command );
		}
		if( command->piece == 'R' || command->piece == 'r' )
		{
			return isValidRooksMove( command );
		}
		
	}

	return true; // enpass or castling move	
	
}


bool Movement::isvalidEnpassMove(Command *command, States *state)
{
	int currentL = command->currentLocation.letter;
	int currentN = command->currentLocation.number;
	int newL = command->newLocation.letter;
	int newN = command->newLocation.number;


	if(abs(currentL - newL) == 1 && abs(currentN - newN) == 1)
	{
		if(state->currentPlayer == white && newN > currentN)
		{
			return true;
		}

		if(state->currentPlayer == black && newN < currentN)
		{
			return true;
		}

	}
		
	return false;
}