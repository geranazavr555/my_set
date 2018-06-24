cmake_minimum_required(VERSION 3.5)
project(my_set)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -pedantic -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

add_executable(my_set
        main.cpp
        my_set.h
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)


target_link_libraries(my_set -lpthread)
