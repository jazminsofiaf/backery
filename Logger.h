#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <string.h>
#include <iostream>	
#include <fstream>
#include <stdexcept>

#include "SIGINT_Handler.h"
#include "SignalHandler.h"

#include "Fifos/FifoLectura.h"
#include "Fifos/FifoEscritura.h"


using namespace std;
class Logger {

private:
    static const int BUFFSIZE;
	static const std::string ARCHIVO_FIFO;
    static FifoEscritura * write_channel;

    static const std::string FILENAME;
    static std::fstream * outputFile;

public:

    //constructor
    static void init( void );

    // as method is friend it can access private members of logger
    static void log(std::string message);
	
    //detructor
    static void destruir();


};
#endif
