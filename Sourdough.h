#ifndef SOURDOUGH_H_
#define SOURDOUGH_H_

#include "Constant.h"
#include "SIGUSR_Handler.h"
#include "SignalHandler.h"
#include "Fifos/FifoEscritura.h"
#include "Logger.h"
#include "Employee.h"


using namespace std;
class Sourdough: public Employee{
    private:
        std::string channel_name;
        FifoEscritura * write_channel;
        int num;
        
    public:
        //constructor
        Sourdough( std::string channel_name);
        void run() override;
        void stop() override;
        std::string identify() const override; 

        struct Dough{
            int num;
            std::string to_string(){
                return " Dough{ num = " +std::to_string(num) +" }";
            }
        };

};

#endif