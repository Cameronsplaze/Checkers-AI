

#include "../include/negamax.hpp"

float pieceCountScore(const std::bitset<96> &board, const bool redTeam)
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

Negamax::Negamax(const std::bitset<96> &board, bool isRedTeam):
	isRedTeam_(isRedTeam)
{
	// NegamaxRecursive(board, START_DEPTH, isRedTeam);
	std::vector<std::bitset<96>> possible_moves = std::move( CheckerBoardMoves(board, isRedTeam_).getAllMoves() );
	// zip the board, with it's known score, together into possibleMoves_
	possibleMoves_.resize(possible_moves.size());
	for(uint i=0; i<possible_moves.size(); ++i)
	{
		possibleMoves_[i] = std::make_pair(possible_moves[i], NegamaxRecursive(board, START_DEPTH, false));
	}
	// Sort the list with best score at [0]:
	std::sort(possibleMoves_.begin(), possibleMoves_.end(), [](auto &a, auto &b) {
		return a.second > b.second;
	});
	std::cout << "Boards:\n";
	for(uint i=0; i<possibleMoves_.size(); ++i){
		std::cout << "    Board: " << possibleMoves_[i].first << " Score: " << possibleMoves_[i].second << std::endl;
	}
}

// float Negamax::NegamaxRecursive(const std::bitset<96> &board, const uint depth_left, const bool isRedMove)
// {
// 	bool isRedBoard = !(isRedTeam_^isRedMove);
// 	int negativeMod = (isRedBoard) ? 1 : -1; 
// 	std::vector<std::bitset<96>> possible_moves = std::move( CheckerBoardMoves(board, isRedBoard).getAllMoves() );
// 	// If no possible moves to make, someone lost. return 'infinity':
// 	if (possible_moves.size() == 0){
// 		return negativeMod * std::numeric_limits<float>::max();
// 	}
// 	// If at the end of the search, score the board:
// 	if (depth_left == 0){
// 		return negativeMod * pieceCountScore(board, isRedTeam_);
// 	}

// 	return 0.0;
// }

// This is called AFTER you get the first set of boards. It's saying if you move here, what will opponent do.
// Default: optimizingPlayer=False:
float Negamax::NegamaxRecursive(const std::bitset<96> &board, const uint depth_left, const bool maximizing_player)
{	
	// You can find who's color moves next, based on who started, and are they moving now:
	bool isRedsTurn = !(isRedTeam_^maximizing_player);

	std::vector<std::bitset<96>> possible_moves = std::move( CheckerBoardMoves(board, isRedsTurn).getAllMoves() );
	// Split apart \/depth_left\/ to above ^possible_moves^ for possible speedup. Keeping it here for now for readability
	int value = (maximizing_player) ? 1 : -1; 
	if( depth_left == 0 || possible_moves.size() == 0 ){
		return value * pieceCountScore(board, isRedTeam_);
	}
	// closest to inf you'll get. (TODO: see how much speedup you'll get if you fencepost this; check if optimizer already doing it?)
	float best_score = -1 * std::numeric_limits<float>::max();
	for(uint i=0; i<possible_moves.size(); ++i)
	{
		best_score = std::max(best_score, -1 * NegamaxRecursive(possible_moves[i], depth_left-1, !maximizing_player));
	}
	return best_score;

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
