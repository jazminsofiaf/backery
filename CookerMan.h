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
        std::mutex mutex;
        FifoLectura * sourdough_channel;

        
    public:
        //constructor
        CookerMan(int id_num, std::string sourdough_channel_name);
        void run() override;
        void stop() override;
        virtual ~CookerMan() = default;
};

#endif