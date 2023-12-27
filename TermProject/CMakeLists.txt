cmake_minimum_required(VERSION 3.10)
project(planner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false yaml-cpp/src/*.cpp ${SOURCE_DIR}/*.cpp)

add_library(planner_lib ${SOURCES})
add_executable(planner_exe ${SOURCE_DIR}/main.cpp)

target_link_libraries(planner_exe PRIVATE planner_lib)
