#ifndef COOKERMAN_H_
#define COOKERMAN_H_

#include "Constant.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Logger.h"
#include "Recepcionist.h"
#include "Sourdough.h"
#include "Employee.h"   
using namespace std;


class CookerMan : public Employee{
    protected:
        std::string sourdough_channel_name;
        std::string orders_channel_name;
        std::mutex mutex;
        FifoLectura * sourdough_channel;
        FifoLectura * orders_channel;

        
    public:
        //constructor
        CookerMan(int id_num, std::string sourdough_channel_name, std::string orders_channel_name);
        void run() override;
        void stop() override;
        virtual ~CookerMan() = default;

        struct Product{
            Recepcionist::Order order;
            Sourdough::Dough dough;
            std::string toString(){
                return  "Product{ order = " +order.toString() +"\n"
                       +"          dough ="+ dough.toString()+"\n}";
            }
        };
};

#endif