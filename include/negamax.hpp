#ifndef NEGAMAX_HPP_INCLUDED
#define NEGAMAX_HPP_INCLUDED

#include <bitset>
#include <vector>
#include <algorithm>
#include <iostream>

#include "checkerboard.hpp"
#include "globalconsts.hpp"

float pieceCount(const std::bitset<96> &board, bool redTeam);

class Negamax
{
public:
	Negamax(const std::bitset<96> &board, bool isRedTeam, const bool usingNeuralNet, const bool doIterativeDeepening);

	float NegamaxRecursive(const std::bitset<96> &board, const uint depthLeft, const bool color);

	std::bitset<96> getBestBoard();
	
private:
	// clock
	std::vector<std::pair< std::bitset<96>,float >>possibleMoves_;

	const bool usingNeuralNet_;
	const bool isRedTeam_;
};

#endif