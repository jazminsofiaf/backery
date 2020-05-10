#ifndef PIZZA_MAKER_H_
#define PIZZA_MAKER_H_

#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Logger.h"
#include "Sourdough.h"
using namespace std;


class Pizzamaker{
    private:
        std::string channel_name;
        FifoLectura * read_channel;
        pid_t process_id;
        
    public:
        //constructor
        Pizzamaker(std::string channel_name);
        void run();
        void stop();
};

#endif