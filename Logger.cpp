#include "Logger.h"

const int Logger::BUFFSIZE = 100;
const std::string Logger::FILENAME = "../log.txt";


Logger::Logger(){
    this->exclusiveFile = new ExclusiveFile(Logger::FILENAME);
}

//writes are atomic on pipe if buffer size is not greater than PIPE_BUF
void Logger::log(const Employee * employee, const std::string message){
    std::string formated_message = "[" + employee->identify() + "] " + message+"\n";
    char buffer[Logger::BUFFSIZE];


    this->exclusiveFile->getLock();

    size_t  start_position = this->seek(formated_message, 0);
    size_t  end_position = this->seek(formated_message, Logger::BUFFSIZE);
    while( end_position <= formated_message.length()){
            std::string::iterator msg_start  = formated_message.begin() + start_position;
            std::string::iterator msg_end = formated_message.begin() + end_position;
            memset(buffer, '\0', sizeof(buffer)); 
            std::copy(msg_start, msg_end, buffer);

            //std::cout <<"{Logger} writing ---------------------------------> " << buffer << std::endl;
            
            this->exclusiveFile->escribir(static_cast<const void *>(&buffer), sizeof(buffer));
            
            if(end_position == formated_message.length()){
                break;
            }
            start_position = this->seek(formated_message, end_position);
            end_position = this->seek(formated_message, end_position + Logger::BUFFSIZE);
    }
    this->exclusiveFile->freeLock();
}


 size_t Logger::seek(std::string & str_message, size_t position) {
    return str_message.length() <= position ? str_message.length() : position;
}
Logger::~Logger() {
    //std::cout << "calling logger detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    delete this->exclusiveFile;
}



