cmake_minimum_required(VERSION 3.10)

project(lab3)

add_compile_options(-pedantic -Wall)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(main src/main.cpp)
add_executable(child1 src/child1.cpp)
add_executable(child2 src/child2.cpp)