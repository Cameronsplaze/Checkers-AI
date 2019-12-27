#ifndef CHECKERBOARD_HPP_INCLUDED
#define CHECKERBOARD_HPP_INCLUDED

#include <vector> // vector
#include <iostream> // cout, endl
#include <memory> // shared_ptr, unique_ptr
#include <string> // string
#include <random> // for randomness
#include <bitset> // bitset

#include "globalconsts.hpp"


class TheChecker
{
public:
	TheChecker(bool onRedTeam, bool isKing)
	{ 
		isOnRed_ = onRedTeam; 
		isKing_ = isKing; 
	}
	bool isTeamRed(){ 
		return isOnRed_; 
	}
	bool isKing(){ 
		return isKing_; 
	}
	void setKing(bool value){ 
		isKing_ = value; 
	}
private:
	bool isOnRed_;
	bool isKing_;
};


class CheckerBoardMoves
{
public:
	CheckerBoardMoves(const std::bitset<96> &startBoard, const bool redPlayerTurn, bool jumpOnlyOnce = false); 

	std::bitset<96> getRandoMove();
	std::vector<std::bitset<96>> getAllMoves();
	bool isValidBoard(std::bitset<96> &newBoard);

private:
	std::bitset<96> turnBoardToBit();
	void JumpingRecursion(uint i, uint j, bool currTeamDirection); 
	void moveJumpManager(uint i, uint j, bool goingRightWay);
	void updatePossibleMoves();

	// pointers to all the checkers
	std::vector<std::unique_ptr<TheChecker>> checkers_;

	// will have all possible moves, each vector is a different board
	std::vector<std::bitset<96>> possibleMoves_;
	// which turn
	bool redTeamTurn_;
	// should look for moves or not
	bool firstJumpFound_;

	// If true, possible moves will not contain recursive jumps (double jumps, triple, etc)
	//		false will only contain the ends of double jumps. True used by gui for
	//		intermediate turns.
	bool jumpOnlyOnce_;

	std::shared_ptr<LookupTable> currTeamMoveBoard_;
	std::shared_ptr<LookupTable> oppTeamMoveBoard_;
	std::shared_ptr<LookupTable> currTeamJumpBoard_;
	std::shared_ptr<LookupTable> oppTeamJumpBoard_;
};

std::bitset<96> getStartBoard();
std::string boardToString(const std::bitset<96> startBoard);
std::bitset<96> stringToBoard(const std::string &charBoard);


#endif // CHECKERBOARD_H_INLCUDED
