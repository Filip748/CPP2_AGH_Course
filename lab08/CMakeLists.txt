cmake_minimum_required(VERSION 3.10)
project(TensorProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)

add_executable(tensor src/Main.cxx)