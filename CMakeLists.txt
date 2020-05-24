 #This is the version I happened to have while making this file
#Earlier versions may/maynot work, up to you to figure out if neeeded
cmake_minimum_required(VERSION "3.7.1")
project(CheckersAI VERSION 1.0
		   LANGUAGES CXX)

include_directories("${PROJECT_BINARY_DIR}")


# Enabling debug mode by default, pass "-DCMAKE_BUILD_TYPE=Release" to change

# Lets set some flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 --std=c++17 -pthread -lsfml-graphics -lsfml-window -lsfml-system") # -Wall -Wextra


# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD -MP -mavx -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused")

# Define where the files are
set(SOURSE_FILES
	src/checkerboard.cpp
	src/globalconsts.cpp
	src/managerSFML.cpp
	src/player.cpp
	src/negamax.cpp
	src/tests.cpp
	src/singleGameLoop.cpp
)
set(HEADER_FILES
	include/catch.hpp
	include/checkerboard.hpp
	include/globalconsts.hpp
	include/managerSFML.hpp
	include/player.hpp
	include/negamax.hpp
	include/cxxopts.hpp
	include/singleGameLoop.hpp
)

add_executable(${PROJECT_NAME} main.cpp ${SOURSE_FILES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()
