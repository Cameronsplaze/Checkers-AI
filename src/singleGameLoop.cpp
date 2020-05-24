
#include "../include/singleGameLoop.hpp"


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

Game::Game(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, const bool useGUI, std::bitset<96> start_board)
	: player1_(player1),
	  player2_(player2),
	  useGUI_(useGUI)
{
	// Create GUI, update with the starting checker positions:
	if (useGUI_){
		gameGUI_ = std::make_shared<GUI>();
		gameGUI_->setBoard(start_board);
	}
	// Check how many random moves you take, to see who's turn starts:
	std::bitset<96> current_board = start_board;
	bool isRedTurn = NUM_RANDO_TURNS%2 == 0;


	while(true)
	{
		std::shared_ptr<Player> current_plr = (isRedTurn) ? player1_ : player2_;
		current_board = current_plr->getMove(current_board, isRedTurn, gameGUI_);

		// If it's an empty board, that player had no moves:
		if(current_board == std::bitset<96>(0)){
			break;
		} 
		// If the window closed, quit:
		if(!gameGUI_->isWindowOpen()){
			break;
		}
		isRedTurn = !isRedTurn;
	}

	return;
}
