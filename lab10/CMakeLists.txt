cmake_minimum_required(VERSION 3.10)

project(Channel CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

include_directories(include)

add_executable(Channel 
    src/Main.cxx
)