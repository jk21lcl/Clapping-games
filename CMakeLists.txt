cmake_minimum_required(VERSION 3.9.1)

project(Game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/games")

add_library(game player.cpp game.cpp games/fortune.cpp games/accumulating_beans.cpp)

add_executable(fortune main/main_fortune.cpp)
target_link_libraries(fortune game)

add_executable(accumulating_beans main/main_accumulating_beans.cpp)
target_link_libraries(accumulating_beans game)