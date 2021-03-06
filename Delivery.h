#ifndef DELIVERY_H_
#define DELIVERY_H_

#include "Constant.h"
#include "Fifos/FifoLectura.h"
#include "Handlers/SIGINT_Handler.h"
#include "Logger.h"
#include "CookerMan.h"
#include "Employee.h"


using namespace std;
class Delivery: public Employee{
    private:
        std::string channel_name;
        FifoLectura * read_channel;
        Logger * logger;
        
    public:
        //constructor
        Delivery( std::string channel_name);
        ~Delivery();
        void run() override;
        void waitMe() override;
        std::string identify() const override; 
};

#endif