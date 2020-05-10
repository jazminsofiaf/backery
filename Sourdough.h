#ifndef SOURDOUGH_H_
#define SOURDOUGH_H_

#include "SIGUSR_Handler.h"
#include "SignalHandler.h"
#include "Fifos/FifoLectura.h"
#include "Fifos/FifoEscritura.h"
#include "Logger.h"


using namespace std;
class Sourdough{
    private:
        std::string channel_name;
        FifoEscritura * write_channel;
        pid_t process_id;
        int num;
        
    public:
        //constructor
        Sourdough(std::string channel_name);
        void run();
        void stop();

        struct Dough{
            int num;
            std::string toString(){
                return " Dough{ num = " +std::to_string(num) +" }";
            }
        };

};

#endif