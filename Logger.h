#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <string.h>
#include <iostream>	
#include <fstream>
#include <stdexcept>

#include "Lock.h"


using namespace std;
class Logger {

private:

    static const std::string FILENAME;
    static std::mutex mutex;
    std::fstream * outputFile;
    void init();
    Logger( void );

public:

    //constructor
    static Logger * getInstance( void );


    void log(std::string message);


};
#endif
