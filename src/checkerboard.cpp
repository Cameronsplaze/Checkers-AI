

#include "../include/checkerboard.hpp"



//***************************************************
// 			PUBLIC METHODS
//***************************************************
CheckerBoardMoves::CheckerBoardMoves(const std::bitset<96> &startBoard, const bool redPlayerTurn){
	CheckerBoardMoves(startBoard, redPlayerTurn, false);
}

CheckerBoardMoves::CheckerBoardMoves(const std::bitset<96> &startBoard, const bool redPlayerTurn, bool jumpOnlyOnce):
	redTeamTurn_(redPlayerTurn), firstJumpFound_(false), jumpOnlyOnce_(jumpOnlyOnce)
{
	if(redTeamTurn_)
	{
		currTeamMoveBoard_ = RED_MOVE_BOARD;
		currTeamJumpBoard_ = RED_JUMP_BOARD;
		oppTeamMoveBoard_ = BLACK_MOVE_BOARD;
		oppTeamJumpBoard_ = BLACK_JUMP_BOARD;
	}
	else
	{
		currTeamMoveBoard_ = BLACK_MOVE_BOARD;
		currTeamJumpBoard_ = BLACK_JUMP_BOARD;
		oppTeamMoveBoard_ = RED_MOVE_BOARD;
		oppTeamJumpBoard_ = RED_JUMP_BOARD;
	}

	checkers_.resize(32);
	for(uint i=0; i<32; ++i)
	{
		// if no checker
		if(startBoard[i+32] == 0 && startBoard[i+64] == 0)
		{
			checkers_[i] = nullptr;
		}
		// if red checker
		else if(startBoard[i+32] == 1 && startBoard[i+64] == 0)
		{
			if(startBoard[i] == 0)
			{
				checkers_[i] = std::make_unique<TheChecker>(TheChecker(true, false));
			}
			else
			{
				checkers_[i] = std::make_unique<TheChecker>(TheChecker(true, true));
			}
		}
		// if black checker
		else if(startBoard[i+32] == 0 && startBoard[i+64] == 1)
		{
			if(startBoard[i] == 0)
			{
				checkers_[i] = std::make_unique<TheChecker>(TheChecker(false, false));
			}
			else
			{
				checkers_[i] = std::make_unique<TheChecker>(TheChecker(false, true));
			}
		}
	}
	updatePossibleMoves();
}

std::bitset<96> CheckerBoardMoves::getRandoMove()
{
    if(possibleMoves_.size() == 0)
    {
    	// if no moves, still return something so you wont crash (maybe say theres red and black on every square?)
    	std::cout << "NO MOVES FOUND. RETURNING BLANK BOARD. FIX LATER!!!!" << std::endl;
        std::bitset<96> tmp(0);
        return tmp;
    }
    else if(possibleMoves_.size() == 1)
    {
    	// if only option, just return it
    	return possibleMoves_[0];
    }
    else
    {
    	std::random_device rd;
    	std::mt19937 rando(rd());
        std::uniform_int_distribution<uint> movePicked(0, (possibleMoves_.size()-1));
        return possibleMoves_[movePicked(rando)];
    }
}

std::vector<std::bitset<96>> CheckerBoardMoves::getAllMoves()
{
	return possibleMoves_;
}

bool CheckerBoardMoves::isValidBoard(std::bitset<96> &newBoard)
{
	for(uint i=0; i<possibleMoves_.size(); ++i)
	{
		if(possibleMoves_[i] == newBoard)
		{
			return true;
		}
	}
	return false;
}

std::bitset<96> CheckerBoardMoves::turnBoardToBit()
{
	std::bitset<96> returnMe(0);
	for(uint i=0; i<32; ++i){
		// If null, do nothing. Update king list reguardless of color
		if(checkers_[i] == nullptr)
			continue;
		
		if(checkers_[i]->isKing() == true)
			returnMe[i] = 1;

		if(checkers_[i]->isTeamRed() == true){
			returnMe[i+32] = 1;
		}
		else{ // if(checkers_[i]->isTeamRed() == false) { // black team
			returnMe[i+64] = 1;
		}

	}
	return returnMe;
}

//***************************************************
// 			REST OF THE METHODS ARE PRIVATE
//***************************************************

void CheckerBoardMoves::updatePossibleMoves()
{

	for(uint i=0; i<checkers_.size(); ++i)
	{
		// If no checker on that square, or wrong teams checker, do nothing....
		if(checkers_[i] == nullptr || checkers_[i]->isTeamRed() != redTeamTurn_)
		{
			continue;
		}

		//for loop because 0 deals with moving left, and 1 deals with right
		//	check both sides independently
		for(uint j=0; j<2; j++)
		{
			moveJumpManager(i, j, true);

			if(checkers_[i]->isKing())
			{
				moveJumpManager(i, j, false);
			}
		}
	}
	// sort poss bords here <---?
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
void CheckerBoardMoves::moveJumpManager(uint i, uint j, bool goingRightWay)
{

	// Use the correct boards:
	std::shared_ptr<LookupTable> teamMoveBoard;
	std::shared_ptr<LookupTable> teamJumpBoard;

	if(goingRightWay)
	{
		teamMoveBoard = currTeamMoveBoard_;
		teamJumpBoard = currTeamJumpBoard_;
	}
	else // A king using the other board:
	{
		teamMoveBoard = oppTeamMoveBoard_;
		teamJumpBoard = oppTeamJumpBoard_;
	}

	// if you can't even move that direction, don't check for other options
	if((*teamMoveBoard)[i][j] == -1)
	{
		return;
	}
	// if there is no checker there (you can move if no jump has happened yet)
	if(checkers_[(*teamMoveBoard)[i][j]] == nullptr)
	{
		if(firstJumpFound_ == false)
		{
			//move the checker to the new spot
			checkers_[(*teamMoveBoard)[i][j]] = std::move(checkers_[i]);

			//If already a king, dont change it. If not, change it and change it back
			bool isAlreadyKing = checkers_[(*teamMoveBoard)[i][j]]->isKing();
			bool changedToKing = false;
			if(!isAlreadyKing && ((redTeamTurn_ && (*teamMoveBoard)[i][j]>27) || (!redTeamTurn_ && (*teamMoveBoard)[i][j]<4)))
			{
				checkers_[(*teamMoveBoard)[i][j]]->setKing(true);
				changedToKing = true;
			}

			possibleMoves_.push_back(turnBoardToBit());

			if(changedToKing)
			{
				checkers_[(*teamMoveBoard)[i][j]]->setKing(false);
			}

			//put the checker back
			checkers_[i] = std::move(checkers_[(*teamMoveBoard)[i][j]]);

		}
	}
	// if there is a checker there, and not on your team. check for jumpable
	else if(checkers_[(*teamMoveBoard)[i][j]]->isTeamRed() != redTeamTurn_)
	{
		// if it wont jump off the board, and the spot it can jump to is empty
		if((*teamJumpBoard)[i][j] != -1 && checkers_[(*teamJumpBoard)[i][j]] == nullptr)
		{
			// Jump found
			if(!firstJumpFound_)
			{
				//everything in possibleMoves are not jumps, so wipe it
				possibleMoves_.clear();
				firstJumpFound_ = true;
			}

			JumpingRecursion(i, j, goingRightWay);
		}
	}
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
void CheckerBoardMoves::JumpingRecursion(uint i, uint j, bool currTeamDirection) // currTeamDirection gets passed going right way
{
	std::unique_ptr<TheChecker> pieceJumped;
	uint k;
	bool jumpNeverFound = true;

	if(currTeamDirection)
	{
		pieceJumped = std::move(checkers_[(*currTeamMoveBoard_)[i][j]]);
		checkers_[(*currTeamJumpBoard_)[i][j]] = std::move(checkers_[i]);
		k = (*currTeamJumpBoard_)[i][j];
		
	}
	else // currTeamDirection == false
	{
		pieceJumped = std::move(checkers_[(*oppTeamMoveBoard_)[i][j]]);
		checkers_[(*oppTeamJumpBoard_)[i][j]] = std::move(checkers_[i]);
		k = (*oppTeamJumpBoard_)[i][j];
	}

	if(!jumpOnlyOnce_)
	{
		for(uint l=0; l<2; ++l)
		{
			// if "Can move that way" && "Theres a checker that way" && "That checker is not yours" && "The space to jump it is empty"
			if((*currTeamMoveBoard_)[k][l] != -1 && checkers_[(*currTeamMoveBoard_)[k][l]] != nullptr && checkers_[(*currTeamMoveBoard_)[k][l]]->isTeamRed() != redTeamTurn_ && (*currTeamJumpBoard_)[k][l] != -1 && checkers_[(*currTeamJumpBoard_)[k][l]] == nullptr)
			{
				jumpNeverFound = false;
				JumpingRecursion(k, l, true);
			}
			// if king, check other direction too
			if(checkers_[k]->isKing())
			{
				if((*oppTeamMoveBoard_)[k][l] != -1 && checkers_[(*oppTeamMoveBoard_)[k][l]] != nullptr && checkers_[(*oppTeamMoveBoard_)[k][l]]->isTeamRed() != redTeamTurn_ && (*oppTeamJumpBoard_)[k][l] != -1 && checkers_[(*oppTeamJumpBoard_)[k][l]] == nullptr)
				{
					jumpNeverFound = false;
					JumpingRecursion(k, l, false);
				}
			}
		}
	}

	// if at tail of jump sequence, board is a possible answer
	if(jumpNeverFound)
	{
		//isAlreadyKing default to true so that it bypasses checks if it doesn't get changed
		bool isAlreadyKing = true;

		if(currTeamDirection)
		{
			isAlreadyKing = checkers_[(*currTeamJumpBoard_)[i][j]]->isKing();
			if(!isAlreadyKing && ((redTeamTurn_ && (*currTeamJumpBoard_)[i][j]>27) || (!redTeamTurn_ && (*currTeamJumpBoard_)[i][j]<4)))
			{
				checkers_[(*currTeamJumpBoard_)[i][j]]->setKing(true);

			}
		}

		possibleMoves_.push_back(turnBoardToBit());

		if(currTeamDirection && !isAlreadyKing && ((redTeamTurn_ && (*currTeamJumpBoard_)[i][j]>27) || (!redTeamTurn_ && (*currTeamJumpBoard_)[i][j]<4)))
		{
			checkers_[(*currTeamJumpBoard_)[i][j]]->setKing(false);

		}
	}


	// undo the beginning for previous recurse call
	if(currTeamDirection)
	{
		checkers_[i] = std::move(checkers_[(*currTeamJumpBoard_)[i][j]]);
		checkers_[(*currTeamMoveBoard_)[i][j]] = std::move(pieceJumped);
	}
	else
	{
		checkers_[i] = std::move(checkers_[(*oppTeamJumpBoard_)[i][j]]);
		checkers_[(*oppTeamMoveBoard_)[i][j]] = std::move(pieceJumped);
	}
}
#pragma GCC diagnostic pop



//////////////////////////////////////////////////////////////
////////////    ----- "Helper" Functions -----    ////////////
//////////////////////////////////////////////////////////////

std::bitset<96> getStartBoard()
{
	std::bitset<96> theBoard = stringToBoard(START_BOARD);
	bool redTurn = true;
	for(uint i=0; i<NUM_RANDO_TURNS; ++i)
	{
		theBoard = CheckerBoardMoves(theBoard, redTurn).getRandoMove();
		redTurn = !redTurn;
	}

	return theBoard;
}



std::string boardToString(const std::bitset<96> startBoard)
{
    std::string returnMe;
    for(uint i=0; i<32; ++i)
    {
        // if no checker
        if(startBoard[i+32] == 0 && startBoard[i+64] == 0)
        {
            returnMe += '_';
        }
        // if red checker
        else if(startBoard[i+32] == 1 && startBoard[i+64] == 0)
        {
            if(startBoard[i] == 0)
            {
                returnMe += 'r';
            }
            else
            {
                returnMe += 'R';
            }
        }
        // if black checker
        else if(startBoard[i+32] == 0 && startBoard[i+64] == 1)
        {
            if(startBoard[i] == 0)
            {
                returnMe += 'b';
            }
            else
            {
                returnMe += 'B';
            }
        }
    }
    return returnMe;
}

std::bitset<96> stringToBoard(const std::string &charBoard)
{
	// first 32 bits hold if king, second 32 hold if red, third 32 hold if black.
	// king & red => red king
	// ~(red | black) => empty square
	// red & black => nothing, all 0's
	std::bitset<96> bitBoard(0);
	for(uint i=0; i<32; ++i)
	{
		switch(charBoard.at(i)) 
		{
			case '_':
				break;
			case 'r':
				bitBoard[i+32] = 1; // red
			break;
			case 'b':
				bitBoard[i+64] = 1; // black
				break;
			case 'R':{
				bitBoard[i+32] = 1; // red
				bitBoard[i] = 1; //king
				}break;
			case 'B':{
				bitBoard[i+64] = 1; // black
				bitBoard[i] = 1; //king
				}break;
		}
	}
	return bitBoard;
}

