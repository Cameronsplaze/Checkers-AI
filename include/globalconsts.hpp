#ifndef CONSTS_HPP_INCLUDED
#define CONSTS_HPP_INCLUDED

#include <vector> // vector
#include <memory> // shared_ptr
#include <cstdint> // int32_t
#include <string> // string

// All typedef's:
typedef std::uint32_t uint;
typedef const std::vector<std::vector<int>> LookupTable;

// for GUI configuration:
const std::string PATH_TO_IMAGES = "images/";

// for ALL games:
const std::string START_BOARD = "rrrrrrrrrrrr________bbbbbbbbbbbb";
const uint NUM_RANDO_TURNS = 0;
const float START_KING_VAL = 1.4f; // relative to normal checker = 1.

// search limits:
const uint START_DEPTH = 8;
// clock duration, 15s here


// const int8_t LOSS_VAL = -1;
// const int8_t DRAW_VAL = 0;
// const int8_t WIN_VAL = 2;
// const int8_t NUM_THREADS = 12;
// const int POPULATION_SIZE = 30;
// const int8_t MATCHES = 5;
// const uint TOTAL_MOVES = 200;


// defined in globals.cpp
extern std::shared_ptr<LookupTable> RED_MOVE_BOARD;
extern std::shared_ptr<LookupTable> RED_JUMP_BOARD;
extern std::shared_ptr<LookupTable> BLACK_MOVE_BOARD;
extern std::shared_ptr<LookupTable> BLACK_JUMP_BOARD;
extern std::shared_ptr<LookupTable> NN91_NODE_LOCATIONS;

#endif
