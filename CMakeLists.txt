cmake_minimum_required(VERSION 3.10)
project(ConsoleChess CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(include)

add_executable(chess
    src/main.cpp
    src/Board.cpp
    src/AI.cpp
)
