cmake_minimum_required(VERSION 3.10)
project(lab5-7)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from above to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS} )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR} PUBLIC ${ZeroMQ_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_DIR}/*.cpp)

add_library(lab5-7_lib ${SOURCES})
add_executable(client_exe ${SOURCE_DIR}/main.cpp)
add_executable(server_exe ${SOURCE_DIR}/server.cpp)

target_link_libraries(client_exe PUBLIC ${ZeroMQ_LIBRARY} lab5-7_lib)
target_link_libraries(server_exe PUBLIC ${ZeroMQ_LIBRARY} lab5-7_lib)

