#include "StdAfx.h"
#include "BoardState.h"
#include "CommandReader.h"
#include <locale>

#define K_SIDE_ROOKS_LOCATION 7
#define Q_SIDE_ROOKS_LOCATION 0
#define WHITE_KINGS_INITIAL_PNUMBER 0
#define BLACK_KINGS_INITIAL_PNUMBER 7
#define KINGS_INIT_LETER 4

/*
find the threats from opposite side pieces to the given cell of king
except knights threat
*/
bool BoardState::findThreatToKing(Cell *cell, States *state)
{
	// return true if a threat found
	// factors are used to go through 8 directions

	int locationL = cell->letter;
	int locationN = cell->number;

	int factorN = -1 , factorL = -1;
	std::locale loc;

	for ( factorN = -1 ; factorN <= 1 ; factorN++ )
	{
		for ( factorL = -1 ; factorL <= 1 ; factorL++)
		{
			if( factorL == 0 && factorN == 0 ) continue;
			
			int tempL = locationL + factorL;
			int tempN = locationN + factorN;

			while ( tempL >= 0 && tempL <= 7 && tempN >= 0 && tempN <= 7 && state->board[tempN][tempL] == '0' )
			{  // go thorugh all the empty cells in that direction
				tempL += factorL;
				tempN += factorN;
			}

			if( tempL<0 || tempL>7 || tempN<0 || tempN>7) continue ;

			char foundPiece = toupper(state->board[tempN][tempL]);

			/*find a piece of opposite color*/
			if( islower ( state->board[locationN][locationL] , loc) ^ islower( state->board[tempN][tempL] , loc ) )
			{ 
				if( abs( factorN ) == abs( factorL ) )
				{ // bishop or queens move
					if( foundPiece == 'B' || foundPiece == 'b' || foundPiece == 'q' || foundPiece == 'Q')  return true;

					//pawn check
					if( ( foundPiece == 'p' || foundPiece == 'P') && abs( tempN - locationN )  ==  1)
					{
						if( foundPiece == 'P' ) // white pawn
						{
							if( locationN > tempN ) return true;

						}
						else // black pawn
						{
							if( locationN < tempN ) return true;
						}
					}
				}
				else
				{ // rook or queen

					if( foundPiece == 'R' || foundPiece == 'b' || foundPiece == 'q' || foundPiece == 'Q')  return true;
				}
			}
		}
	}	

	return false;
}

bool BoardState::isChecked( States *state , char King )
{
	int locationN , locationL; // kings location number and letter
	std::locale loc;

	// find the location of the king
	for ( locationN = 0 ; locationN < 8 ; locationN++ )
	{
		for ( locationL = 0 ; locationL < 8 ; locationL++ )
		{
			if( King == state->board[locationN][locationL] ) break;
											
		}
		if( King == state->board[locationN][locationL] ) // to stop outer loop
		{								
			break;
		}
	}

	int tempL = locationL;
	int tempN = locationN;

	int  i;
	int  j;

	/*Knights check is implemented below*/
	for( i = 2 ; i >= -2 ; i-- )
	{
		if( i == 0 ) continue;
				
		for( j = 2 ; j >= -2 ; j-- )
		{
			if( j == 0 || j == i ) continue;

			if( CommandReader::isValidCell( tempN+i , tempL+j ) )
			{
				if( ( state->board[tempN+i][tempL+j] == 'N' || state->board[tempN+i][tempL+j] == 'n' )  &&
					( islower( state->board[locationN][locationL] , loc ) ^ islower( state->board[tempN][tempL] , loc) ))
				{
					return true;
				}
			}
		}
	}

	return BoardState::findThreatToKing( &Cell(locationN, locationL) , state );
}

/*
check whether the given command is valid against the current
board state
*/
bool BoardState::isValidState( Command command , States state )
{
	int currentL = command.currentLocation.letter;
	int currentN = command.currentLocation.number;
	int newL = command.newLocation.letter;
	int newN = command.newLocation.number;

	/*check whether the destination location is empty (if move)*/
	if( command.type == move )
	{
		if( !isDestinationClear( &command , &state ))
		{
			return false;
		}
	}

	/*check whether the destination piece is opposite (if capture)*/
	if( command.type == capture)
	{
		if( !isOppositePiece( &command , &state ))
		{
			return false;
		}
	}

	/*check whethr the piece exists on the given location*/
	if( !pieceExists( &command , &state ))
	{
		return false;
	}

	if( isPathClear( &command , &state ) )
	{
		state.board[ command.newLocation.number ][ command.newLocation.letter ]  =  
			state.board[ command.currentLocation.number ][ command.currentLocation.letter ];

		state.board[ command.currentLocation.number ][ command.currentLocation.letter ] = '0';

		if( state.currentPlayer == white )
		{
			if( isChecked( &state,'K' ) )
			{
				return false;
			}
		}
		else
		{
			if( isChecked( &state , 'k') )
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}		
	return true;
}

bool BoardState::isPathClear( Command *command , States *state )
{

	/*Knight can jump over other pieces*/
	if( command->piece == 'N')
	{
		return true;
	}

	int currentN = command->currentLocation.number;
	int currentL = command->currentLocation.letter;
	int newN = command->newLocation.number;
	int newL = command->newLocation.letter;

	if( abs( currentN-newN ) == abs( currentL-newL ) )// bishops move
	{ 

		int NFactor = ( currentN < newN ) ? 1 : -1;
		int LFactor = ( currentL < newL ) ? 1 : -1;

		while(currentN!=(newN-NFactor))
		{		
			currentN += NFactor;
			currentL += LFactor;

			if(state->board[currentN][currentL]!='0')
			{
				return false;
			}
		}	

		return true;
	}

	if(currentN==newN ^ newL==currentL)// rooks move
	{ 

		int factor;
		if(currentN==newN)
		{				
			factor = (currentL<newL) ? 1 : -1;
			currentL+= factor;
		}else if(newL==currentL)
		{
			factor = (currentN<newN) ? 1 : -1;		
			currentN+= factor;
		}


		while( !(currentL == newL && currentN == newN))
		{
			if(state->board[currentN][currentL]!='0'){
				return false;
			}

			if(currentN == newN)
			{						
				currentL+= factor;
			}
			else if(newL== currentL)
			{						
				currentN+= factor;
			}

		}

		return true;
	}

	return false; // if neither of above conditions are satisfied
}

/*
check whether the piece in the command exist in the given cell
*/
bool BoardState::pieceExists( Command *command , States *state )
{
	char commandPiece = command->piece;
	char locationPiece = state->board[ command->currentLocation.number ][ command->currentLocation.letter ];
			

	if( command->type == move || command->type == capture )
	{
		std::locale loc;
		/*check whether a player tries to move opposite color piece*/
		if( isupper( locationPiece , loc ) ^ ( state->currentPlayer ==  white ) )
		{
			return false;
		}

		locationPiece = toupper( locationPiece );
		commandPiece = toupper( commandPiece) ;

		if( locationPiece == commandPiece )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;

}

bool BoardState::isDestinationClear( Command *command , States * state )
{
	if( state->board[ command->newLocation.number ][ command->newLocation.letter ] == '0' )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BoardState::isOppositePiece( Command *command, States *state )
{
	std::locale loc;
	
	
	if( isupper( state->board[ command->newLocation.number ][ command->newLocation.letter ] , loc )
		^ state->currentPlayer == white )
		{
			return true;
		}
		else
		{
			return false;
		}	
}

bool BoardState::isValidCastling( Command *command , States *state )
{
	int locationNumber; // 0 or 7 depends on the player color

	if( state->currentPlayer == white )
	{
		locationNumber = WHITE_KINGS_INITIAL_PNUMBER;

		 /* check whether white king is moved before*/
		if( ! state->castle[0] ) return false;
					
	}
	else
	{
		locationNumber = BLACK_KINGS_INITIAL_PNUMBER;

		/*check whether black king is moved before*/
		if (!state->castle[1] ) return false;
		 		
	}

	Command cm;

	if( command->type == kCastle ) // kings side castle
	{		
		cm.currentLocation.number = locationNumber;
		cm.currentLocation.letter = K_SIDE_ROOKS_LOCATION;

		cm.newLocation.number = locationNumber;
		cm.newLocation.letter = KINGS_INIT_LETER;

		if( isPathClear ( &cm , state ) )
		{
			return true;
		}

	}
	else // queens side castle
	{
		cm.currentLocation.number = locationNumber;
		cm.currentLocation.letter = Q_SIDE_ROOKS_LOCATION;

		cm.newLocation.number = locationNumber;
		cm.newLocation.letter = KINGS_INIT_LETER;

		if( isPathClear ( &cm , state) )
		{
			return true;
		}
	}
	return false; // if neither of above conditions met
}


bool BoardState::isValidEnpass( Command *command , States *state )
{
	int capturedPiecesL = command->newLocation.letter;
	int capturedPiecesN = command->currentLocation.number;

	if( toupper(state->board[ capturedPiecesN ][ capturedPiecesL ] ) != 'P' )
		return false;

	/*check whether is it an opposite type piece*/
	if( !(state->currentPlayer == black ^ isupper( state->board[ capturedPiecesN ][ capturedPiecesL ] )))
	{
		return false;
	}

	if( state->currentPlayer == white )
	{
		return ( state->enpass[ 0 ] [ command->currentLocation.letter ] && state->enpass[1][ command->newLocation.letter ]);
	}
	else
	{
		return ( state->enpass[ 1 ][ command->currentLocation.letter ] && state->enpass[ 0 ][ command->newLocation.letter ]);
	}
}  