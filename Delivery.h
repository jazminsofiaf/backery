#ifndef DELIVERY_H_
#define DELIVERY_H_

#include "Constant.h"
#include "Fifos/FifoLectura.h"
#include "Logger.h"
#include "CookerMan.h"
#include "Employee.h"


using namespace std;
class Delivery: public Employee{
    private:
        std::string channel_name;
        FifoLectura * read_channel;
        
    public:
        //constructor
        Delivery( std::string channel_name);
        //~Delivery();
        void run() override;
        void stop() override;
        std::string identify() const override; 
};

#endif