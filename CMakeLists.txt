cmake_minimum_required(VERSION 2.8)
project(Bakery)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DBAKERY")

include_directories(Locks)
add_executable(bakery main.cc  Bakery.cpp ArgsHelper.cpp Receptionist.cpp Sourdough.cpp CookerMan.cpp Employee.cpp Pizzamaker.cpp Baker.cpp Delivery.cpp Logger.cpp Locks/LockFile.cpp Locks/Lock.cpp  Locks/ExclusiveFile.cpp  Locks/SharedFile.cpp Handlers/SignalHandler.cpp Fifos/Fifo.cpp Fifos/FifoEscritura.cpp Fifos/FifoLectura.cpp Fifos/FifoShared.cpp Locks/LockFile.cpp Locks/LockFile.h)