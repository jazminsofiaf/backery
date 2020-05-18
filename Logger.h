#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <string.h>
#include <iostream>	
#include <fstream>
#include <stdexcept>
#include <signal.h>
#include "Constant.h"
#include "Employee.h"
#include "Fifos/FifoLectura.h"
#include "Fifos/FifoEscritura.h"
#include "EndChildException.h"
#include "Locks/ExclusiveFile.h"

#define LOGGER_FIFO "logger_fifo" 


using namespace std;
class Logger {

private:
    static const int BUFFSIZE;

    size_t seek(std::string & str_message, size_t position);
    ExclusiveFile * exclusiveFile;

public:
    static const std::string FILENAME;

    Logger();

    void log(const Employee * employee, const std::string message);

    ~Logger();


};
#endif
