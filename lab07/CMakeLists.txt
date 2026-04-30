cmake_minimum_required(VERSION 3.10)
project(RPNExpressionParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(include)

file(GLOB SOURCES "src/*.cpp" "src/Main.cxx")

add_executable(RPNExpressionParser ${SOURCES})