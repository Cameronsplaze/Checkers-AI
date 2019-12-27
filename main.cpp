#define CATCH_CONFIG_RUNNER

#include <iostream>
#include <string>
#include <bitset>

#include "include/globalconsts.hpp"
#include "include/checkerboard.hpp"
#include "include/managerSFML.hpp"
#include "include/cxxopts.hpp"

#include "include/negamax.hpp"
#include "include/catch.hpp"
// #include "include/tests.hpp" // make tests.cpp too to reduce compile time.


// this auto'd to: int main(int argc, char const *argv[])
// BUT cxxopts parse doesn't take a "const". Look into if this is bad.
int main(int argc, char *argv[])
{    

    // Parser used for command line args: https://github.com/jarro2783/cxxopts
    cxxopts::Options options("Checkers AI", "One AI to rule them all");
    options.add_options()
        ("h,help", "Prints help... maybe")
        ("f,file", "File name", cxxopts::value<std::string>())
        ;
    auto result = options.parse(argc, argv);
    // std::cout << (std::string) result << std::endl;
    if(result.count("help") > 0)
    {
        std::cout << "IT WORKS!!!!" << std::endl;
        return 0;
    }
    // std::cout << boardToString(getStartBoard()) << std::endl;
    // return 0;

    				//	01234567890123456789012345678901
    // std::string tmp1 = "b___r____BR______Rbbr____Br_____";
    // std::string tmp2 = "_______________R________________";

    // std::bitset<96> tmpB1 = stringToBoard(tmp1);
    // std::bitset<96> tmpB2 = stringToBoard(tmp2);
    // std::cout << tmpB2 << " Start" << std::endl; 

    // ######################
    // CheckerboardGUI("random", "random");
    CheckerboardGUI("random", "piece_count");
    // ######################

    // std::vector<std::bitset<96>> possBoards = std::move( CheckerBoardMoves(tmpB1, redTurn, false).getAllMoves() );
    // for (int i=0; i<possBoards.size(); ++i)
    // {
    //    std::cout << boardToString(possBoards[i]) << " " << i << std::endl;
    // }


    if(true)
    {
        Catch::Session session; // There must be exactly one instance

        // writing to session.configData() here sets defaults
        // this is the preferred way to set them

        int returnCode = session.applyCommandLine( argc, argv );
        if( returnCode != 0 ) // Indicates a command line error
            return returnCode;

        // writing to session.configData() or session.Config() here 
        // overrides command line args
        // only do this if you know you need to

        int numFailed = session.run();
        // When adding cmd parcer: add --verify? would run tests, and only continue to AI if numFailed == 0?
        return numFailed;
    }
    return 0;
}


