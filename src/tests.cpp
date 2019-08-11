// Framework:
#include "include/catch.hpp"
// Testing includes:
#include "include/checkerboard.hpp"
// Needed libraries:
#include <string>
#include <bitset>
#include <vector>
#include <iostream>


TEST_CASE("Testing Class - CheckerBoardManager (inside checkerboard.hpp")
{
	bool redTurn = true; // pass to test red
	bool blackTurn = false; // pass to test black
				//					00000000001111111111222222222233
    			//					01234567890123456789012345678901
	const std::string charBoard1 = "________________________________"; // blank case
	const std::string charBoard2 = "____________B_________R_________"; // red in middle, black on edge
	const std::string charBoard3 = "___B__rr_____r__b_______________"; // non king sould jump, but not back, king should

	SECTION("Make sure input is valid")
	{
		REQUIRE( charBoard1.size() == 32 ); 
		REQUIRE( charBoard2.size() == 32 ); 
		REQUIRE( charBoard3.size() == 32 );
	}
	/*

	const std::bitset<96> bitBoard1 = stringToBoard(charBoard1);
	const std::bitset<96> bitBoard2 = stringToBoard(charBoard2);
	const std::bitset<96> bitBoard3 = stringToBoard(charBoard3);

	CheckerBoardManager board1r( bitBoard1, redTurn, false);
	CheckerBoardManager board1b( bitBoard1, blackTurn, false);

	SECTION("Testing Blank board")
	{
		REQUIRE( board1r.getAllMoves().size() == 0 );
		REQUIRE( board1b.getAllMoves().size() == 0 );
	}

	CheckerBoardManager board2r( bitBoard2, redTurn, false);
	CheckerBoardManager board2b( bitBoard2, blackTurn, false);

	std::vector<std::bitset<96>> redPossMoves;
	std::vector<std::bitset<96>> blackPossMoves;

	redPossMoves.push_back(stringToBoard("____________B___R___________")); // red on 17
	redPossMoves.push_back(stringToBoard("____________B____R__________")); // red on 18
	redPossMoves.push_back(stringToBoard("____________B___________R___")); // red on 25
	redPossMoves.push_back(stringToBoard("____________B____________R__")); // red on 26

	blackPossMoves.push_back(stringToBoard("________B____________R__________")); // black on 8
	blackPossMoves.push_back(stringToBoard("________________B____R__________")); // black on 16

	for(uint i=0; i<board2r.getAllMoves().size(); ++i)
	{
		auto itr = std::find(redPossMoves.begin(), redPossMoves.end(), board2r.getAllMoves()[i]);
		redPossMoves.erase(itr);
	}

	

	SECTION("Testing Basic board - Red in middle, Black on edge")
	{
		REQUIRE( board2r.getAllMoves().size() == 4 );
		REQUIRE( board2b.getAllMoves().size() == 2 );
		REQUIRE( redPossMoves.size() == 0 );
		REQUIRE( blackPossMoves.size() == 0 );
	}

	CheckerBoardManager board3r( bitBoard3, redTurn, false);
	CheckerBoardManager board3b( bitBoard3, blackTurn, false);

	SECTION("Testing Advanced board - king goes both ways, non-king shouldn't (black)")
	{
		REQUIRE ( true );
	}
	
	*/
}