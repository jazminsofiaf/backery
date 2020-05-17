#ifndef COOKERMAN_H_
#define COOKERMAN_H_

#include "Constant.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Fifos/FifoShared.h"
#include "Logger.h"
#include "Recepcionist.h"
#include "Sourdough.h"
#include "Employee.h"   
using namespace std;


class CookerMan : public Employee{
    protected:
        Logger * logger;
        std::string sourdough_channel_name;
        std::string orders_channel_name;
        std::string delivery_channel_name;
        FifoShared * sourdough_channel;
        FifoShared * orders_channel;
        FifoEscritura * delivery_channel;

        
    public:
        //constructor
        CookerMan(int id_num, Logger * logger,  std::string sourdough_channel_name, 
                                std::string orders_channel_name, 
                                std::string delivery_channel_name);
        void run() override;
        void stop() override;
        virtual ~CookerMan() = default;

        struct Product{
            Recepcionist::Order order;
            Sourdough::Dough dough;
            std::string made_by;
            std::string toString(){
                return  "Product { " +dough.toString()+", "+ order.toString()+" by = " + made_by +" }";
            }
        };
};

#endif