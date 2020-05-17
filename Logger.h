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

#define LOGGER_FIFO "logger_fifo" 


using namespace std;
class Logger {

private:
    static const int BUFFSIZE;
	static const std::string ARCHIVO_FIFO;
    FifoEscritura * write_channel;
    FifoLectura * read_channel;

    static const std::string FILENAME;
    std::fstream * outputFile;
    pid_t process_id;
    void writeToFile();
    size_t seek(std::string & str_message, size_t position);

public:

    Logger();

    void log(const Employee * employee, const std::string message);

    void destruir();


};
#endif
