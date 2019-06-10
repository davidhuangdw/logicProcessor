cmake_minimum_required(VERSION 3.14)
project(programming_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(programming_test
        logicfunction.cpp
        logicfunction.h
        testcases.h
        testlogicfunction.cpp)
