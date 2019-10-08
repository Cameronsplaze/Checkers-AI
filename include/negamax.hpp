#ifndef NEGAMAX_HPP_INCLUDED
#define NEGAMAX_HPP_INCLUDED

#include <bitset>		// std::bitset
#include <vector>		// std::vector
#include <algorithm>	// std::max
#include <iostream>		// std::cout, std::endl
#include <limits>       // std::numeric_limits

#include "checkerboard.hpp"
#include "globalconsts.hpp"

float pieceCountScore(const std::bitset<96> &board, const bool redTeam);

class Negamax
{
public:
	Negamax(const std::bitset<96> &board, bool isRedTeam);

	float NegamaxRecursive(const std::bitset<96> &board, const uint depth_left, const bool maximizing_player=false);

	std::bitset<96> getBestBoard();
	
private:
	std::vector<std::pair< std::bitset<96>,float >>possibleMoves_;
	const bool isRedTeam_;
};

#endif