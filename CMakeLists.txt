cmake_minimum_required(VERSION 2.8)
project(Bakery)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DBAKERY")
add_executable(bakery main.cc  ArgsHelper.cpp Sourdough.cpp Logger.cpp SignalHandler.cpp Fifos/Fifo.cpp Fifos/FifoEscritura.cpp Fifos/FifoLectura.cpp)
