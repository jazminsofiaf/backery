#include "Logger.h"

const string Logger::FILENAME = "../log.txt";

Logger::Logger(){
    this->outputFile.open(this->FILENAME, ios::out | ios::trunc );
}

void Logger::log(std::string message){
    this->outputFile << message << std::endl;
}

Logger::~Logger(){
    this->outputFile.flush();
    this->outputFile.close();
}

