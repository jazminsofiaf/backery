cmake_minimum_required(VERSION 2.8)
project(Bakery)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DBAKERY")
add_executable(bakery main.cc Logger.cpp)
