#ifndef COOKERMAN_H_
#define COOKERMAN_H_

#include "Constant.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Logger.h"
#include "Sourdough.h"
#include "Employee.h"   
using namespace std;


class CookerMan : public Employee{
    protected:
        std::string sourdough_channel_name;
        FifoLectura * sourdough_channel;
        pid_t process_id;

        
    public:
        //constructor
        CookerMan(int id_num, std::string sourdough_channel_name);
        void run();
        void stop();
        virtual ~CookerMan() = default;
};

#endif