#ifndef SOURDOUGH_H_
#define SOURDOUGH_H_

#include "Constant.h"
#include "Handlers/SIGUSR_Handler.h"
#include "Handlers/SignalHandler.h"
#include "Fifos/FifoEscritura.h"
#include "Logger.h"
#include "Employee.h"


using namespace std;
class Sourdough: public Employee{
    private:
        std::string channel_name;
        FifoEscritura * write_channel;
        int num;
        Logger * logger;
        
    public:
        //constructor
        Sourdough(Logger * logger, std::string channel_name);
        ~Sourdough();
        void run() override;
        void stop() override;
        std::string identify() const override; 

        struct Dough{
            int num;
            std::string toString(){
                return " Dough { num = " +std::to_string(num) +" }";
            }
        };

};

#endif