#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <string.h>
#include <iostream>	
#include <fstream>
using namespace std;

class Logger {

private:
    static const string FILENAME;
	ofstream outputFile;

public:
    //constructor
	Logger ();

    // as method is friend it can access private members of logger
    void log(std::string message);

    //detructor
     ~Logger();

};
#endif
