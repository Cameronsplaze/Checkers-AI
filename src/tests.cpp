// Framework:
#include "include/catch.hpp"
// Testing includes:
#include "include/checkerboard.hpp"
// Needed libraries:
#include <string>
#include <bitset>
#include <vector>
#include <iostream>

TEST_CASE("Testing global constants - Correctness (globalconsts.hpp)")
{
	SECTION("Checking START_BOARD")
	{
		REQUIRE( START_BOARD.size() == 32 );
		for(uint i=0; i<START_BOARD.size(); ++i)
			if(START_BOARD[i] != 'r' && START_BOARD[i] != 'R' && START_BOARD[i] != '_' && START_BOARD[i] != 'b' && START_BOARD[i] != 'B')
				REQUIRE( "UNKNOWN CHAR IN START_BOARD" == "this will fail" );
	}
}

TEST_CASE("Testing Class - CheckerBoardMoves (checkerboard.hpp)")
{
	bool redTurn = true; // pass to test red
	bool blackTurn = false; // pass to test black

	// Boards as strings:
	const std::string blankBoard_str = "________________________________"; // blank case
	const std::string basicBoard1_str = "____________B_________R_________"; // red in middle, black on edge
	const std::string basicBoard2_str = "___B__rr_____r__b_______________"; // non king sould jump, but not back, king should
	// SAME Boards as Bits:
	// (first 32 = isKing, second 32 = isRed, third 32 = isBlack. Starts on top left square, and goes across first)
	const std::bitset<96> blankBoard_bit(std::string("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"));
	const std::bitset<96> basicBoard1_bit(std::string("000000000000000000010000000000000000000001000000000000000000000000000000010000000001000000000000"));
	const std::bitset<96> basicBoard2_bit(std::string("000000000000000100000000000010000000000000000000001000001100000000000000000000000000000000001000"));

	SECTION("Test boardToString method")
	{
		REQUIRE( boardToString(blankBoard_bit) == blankBoard_str );
		REQUIRE( boardToString(basicBoard1_bit) == basicBoard1_str );
		REQUIRE( boardToString(basicBoard2_bit) == basicBoard2_str );
	}
	SECTION("Test stringToBoard method")
	{
		REQUIRE( stringToBoard(blankBoard_str) == blankBoard_bit );
		REQUIRE( stringToBoard(basicBoard1_str) == basicBoard1_bit );
		REQUIRE( stringToBoard(basicBoard2_str) == basicBoard2_bit );
	}

	// Now you can use stringToBoard and BoardToString in other tests:

	SECTION("Test Blank board - Both players")
	{	
		CheckerBoardMoves board1r( blankBoard_bit, redTurn, false);
		CheckerBoardMoves board1b( blankBoard_bit, blackTurn, false);
		REQUIRE( board1r.getAllMoves().size() == 0 );
		REQUIRE( board1b.getAllMoves().size() == 0 );
	}
	SECTION("Test Two-piece board - Both players")
	{
		std::vector<std::bitset<96>> BoardMoves_red = CheckerBoardMoves(basicBoard1_bit, redTurn, false).getAllMoves();
		std::vector<std::bitset<96>> BoardMoves_black = CheckerBoardMoves(basicBoard1_bit, blackTurn, false).getAllMoves();

		std::vector<std::bitset<96>> redPossMoves;
		std::vector<std::bitset<96>> blackPossMoves;

		redPossMoves.push_back(stringToBoard("____________B____R______________")); // red on 17
		redPossMoves.push_back(stringToBoard("____________B_____R_____________")); // red on 18
		redPossMoves.push_back(stringToBoard("____________B____________R______")); // red on 25
		redPossMoves.push_back(stringToBoard("____________B_____________R_____")); // red on 26

		blackPossMoves.push_back(stringToBoard("________B_____________R_________ ")); // black on 8
		blackPossMoves.push_back(stringToBoard("________________B_____R_________")); // black on 16

		// Confirm the lists are the same size:
		REQUIRE(redPossMoves.size() == BoardMoves_red.size());
		REQUIRE(blackPossMoves.size() == BoardMoves_black.size());

		// Confirm that AFTER removing all possible from the other, sizes are 0:
		for(uint i=0; i<BoardMoves_red.size(); ++i)
		{
			for(uint j=0; j<redPossMoves.size(); ++j)
			{
				if(BoardMoves_red[i] == redPossMoves[j])
				{
					redPossMoves.erase(redPossMoves.begin() + j);
					break;
				}
			}
		}
		for(uint i=0; i<BoardMoves_black.size(); ++i)
		{
			for(uint j=0; j<blackPossMoves.size(); ++j)
			{
				if(BoardMoves_black[i] == blackPossMoves[j])
				{
					blackPossMoves.erase(blackPossMoves.begin() + j);
					break;
				}
			}
		}
		// Size = 0:
		REQUIRE(redPossMoves.size() == 0);
		REQUIRE(blackPossMoves.size() == 0);
	}

	SECTION("Testing Advanced board - Black can move king/pawn, Red forced Jump")
	{
		std::vector<std::bitset<96>> BoardMoves_red = CheckerBoardMoves(basicBoard2_bit, redTurn, false).getAllMoves();
		std::vector<std::bitset<96>> BoardMoves_black = CheckerBoardMoves(basicBoard2_bit, blackTurn, false).getAllMoves();

		std::vector<std::bitset<96>> redPossMoves;
		std::vector<std::bitset<96>> blackPossMoves;

		redPossMoves.push_back(stringToBoard("___B__rr____________r___________"));

		blackPossMoves.push_back(stringToBoard("_B___________r__b_______________ ")); // black on 8
		blackPossMoves.push_back(stringToBoard("__BB___r________________________")); // black on 16

		// Confirm the lists are the same size:
		REQUIRE(redPossMoves.size() == BoardMoves_red.size());
		REQUIRE(blackPossMoves.size() == BoardMoves_black.size());

		// Confirm that AFTER removing all possible from the other, sizes are 0:
		for(uint i=0; i<BoardMoves_red.size(); ++i)
		{
			for(uint j=0; j<redPossMoves.size(); ++j)
			{
				if(BoardMoves_red[i] == redPossMoves[j])
				{
					redPossMoves.erase(redPossMoves.begin() + j);
					break;
				}
			}
		}
		for(uint i=0; i<BoardMoves_black.size(); ++i)
		{
			for(uint j=0; j<blackPossMoves.size(); ++j)
			{
				if(BoardMoves_black[i] == blackPossMoves[j])
				{
					blackPossMoves.erase(blackPossMoves.begin() + j);
					break;
				}
			}
		}
		// Size = 0:
		REQUIRE(redPossMoves.size() == 0);
		REQUIRE(blackPossMoves.size() == 0);
	}

}