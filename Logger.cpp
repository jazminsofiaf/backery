#include "Logger.h"

const std::string Logger::FILENAME = "../log.txt";
static std::mutex Logger::mutex;

Logger::Logger(){
    this->outputFile.open(this->FILENAME, ios::out | ios::trunc );
}
 
void Logger::getInstance(){
    
}

void log(std::string message){
    new Lock(Logger::mutex);
    this->outputFile << message << std::endl;
}

void stop(std::string message){
    if(){
        this->outputFile.close();
    }
}





