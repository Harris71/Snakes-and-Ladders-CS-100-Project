cmake_minimum_required(VERSION 3.16)
project(SnakesAndLaddersSFML LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(snakes-and-ladders src/main.cpp)
target_link_libraries(snakes-and-ladders PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
