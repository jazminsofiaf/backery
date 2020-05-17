cmake_minimum_required(VERSION 2.8)
project(Bakery)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DBAKERY")
add_executable(bakery main.cc  Bakery.cpp ArgsHelper.cpp Recepcionist.cpp Sourdough.cpp CookerMan.cpp Employee.cpp Pizzamaker.cpp Baker.cpp Delivery.cpp Logger.cpp Lock.cpp Handlers/SignalHandler.cpp Fifos/Fifo.cpp LockFile.cpp Fifos/FifoEscritura.cpp Fifos/FifoLectura.cpp Fifos/FifoShared.cpp)