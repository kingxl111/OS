cmake_minimum_required(VERSION 3.10)

add_compile_options(-pedantic -Wall)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lab4)

add_library(PrimeCountLib SHARED PrimeCountLib/primeCount.cpp)
add_library(PrimeCountLib::lib1 ALIAS PrimeCountLib)
target_include_directories(PrimeCountLib PUBLIC ${PROJECT_SOURCE_DIR}/PrimeCountLib)

add_library(GCDLib SHARED GCDLib/GCD.cpp)
add_library(GCDLib::lib2 ALIAS GCDLib)
target_include_directories(GCDLib PUBLIC ${PROJECT_SOURCE_DIR}/GCDLib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE PrimeCountLib::lib1 GCDLib::lib2)
