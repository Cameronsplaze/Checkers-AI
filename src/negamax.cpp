

#include "../include/negamax.hpp"

float pieceCount(const std::bitset<96> &board, bool redTeam)
{
	int redScore = 0;
	int blackScore = 0;
	for(uint i=0; i<32; ++i)
	{
		// if no checker
		if(board[i+32] == 0 && board[i+64] == 0)
		{
			continue;
		}
		// if red checker
		else if(board[i+32] == 1 && board[i+64] == 0)
		{
			if(board[i] == 0)
			{
				redScore += 1;
			}
			else
			{
				redScore += START_KING_VAL;
			}
		}
		// if black checker
		else if(board[i+32] == 0 && board[i+64] == 1)
		{
			if(board[i] == 0)
			{
				blackScore += 1;
			}
			else
			{
				blackScore += START_KING_VAL;
			}
		}
	}
	if(redTeam)
	{
		return (float)(redScore - blackScore);
	}
	else
	{
		return (float)(blackScore - redScore);
	}
}

Negamax::Negamax(const std::bitset<96> &board, bool isRedTeam, const bool usingNeuralNet, const bool doIterativeDeepening) :
	isRedTeam_(isRedTeam),
	usingNeuralNet_(usingNeuralNet)
{
	std::vector<std::bitset<96>> boards = std::move( CheckerBoardManager(board, isRedTeam, false).getAllMoves() );
	possibleMoves_.resize(boards.size());
	for(uint i=0; i<boards.size(); ++i)
	{
		possibleMoves_[i] = std::make_pair(boards[i], NegamaxRecursive(board, START_DEPTH, isRedTeam));
	}
	std::sort(possibleMoves_.begin(), possibleMoves_.end(), [](auto &a, auto &b) {
		return a.second > b.second;
	});

	std::cout << "Board Scores for: " << isRedTeam << " scores: ";
	for(uint i=0; i<possibleMoves_.size(); ++i)
		std::cout << possibleMoves_[i].second << " ";
	std::cout << std::endl << std::endl;
	
	if(doIterativeDeepening == false){
		return;
	}

	uint depth = START_DEPTH;
	while(true)
	{
		depth += 2;

		//call recursive on all (deeper on first part?).
		// if time is out, return
		// sort everything
		// -> sort after return because if time IS out, you don't know how deep it got.
	}
}

float Negamax::NegamaxRecursive(const std::bitset<96> &board, const uint depthLeft, const bool color)
{
	if(depthLeft == 0 && isRedTeam_ != color)
		std::cout << "Flip team?" << std::endl;
	if( depthLeft == 0 && isRedTeam_ == color)// && (function call for horizen efect)
	{ 
		return pieceCount(board, isRedTeam_);
	}
	std::vector<std::bitset<96>> children = std::move(CheckerBoardManager(board, color, false).getAllMoves());
	if(children.size() == 0){
		// return positive / negative infinity? here, someone wins / looses
	}
	float value = -1 * NegamaxRecursive(children[0], depthLeft-1, !color);
	for(uint i=1; i<children.size(); ++i)
	{
		value = std::max(value, -1 * NegamaxRecursive(children[i], depthLeft-1, !color));
	}
	return value;
	// ADD TO CHECKERMANAGER: basic sort before returning. speeds up alpha-beta.
}

std::bitset<96> Negamax::getBestBoard()
{
	if(possibleMoves_.size() == 0)
	{
		std::bitset<96> tmp(0);
		return tmp;
	}
	else
	{
		return possibleMoves_[0].first;
	}
}
