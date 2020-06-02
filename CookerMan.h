#ifndef COOKERMAN_H_
#define COOKERMAN_H_

#include "Constant.h"
#include "Handlers/SIGINT_Handler.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Fifos/FifoShared.h"
#include "Logger.h"
#include "Receptionist.h"
#include "Sourdough.h"
#include "Employee.h"

using namespace std;


class CookerMan : public Employee{
    protected:
        FifoEscritura * dough_order_channel;
        FifoLectura * sourdough_channel;
        FifoShared * orders_channel;
        FifoEscritura * delivery_channel;
        Logger * logger;

        
    public:
        //constructor
        CookerMan(int id_num,  FifoEscritura * dough_order_channel,
                                FifoLectura * sourdough_channel,
                                std::string orders_channel_name, 
                                std::string delivery_channel_name);
        void run() override;
        void waitMe() override;
        virtual std::string getMeal() const = 0;
        ~CookerMan();

        struct Product{
            Receptionist::Order order;
            Sourdough::Dough dough;
            int cook_id;
            std::string toString(){
                return  "Product { " +dough.toString()+", "+ order.toString()+" by = " + std::to_string(cook_id) +" }";
            }
            const char * serialize() {
                std::ostringstream out;
                out  << order.serialize() << '|' << dough.serialize();
                return out.str().c_str();
            }

            void deserialize(const char * msg){
                char  pipe1 = 0;
                std::istringstream in(msg);
                char order_msg[ORDER_SIZE];
                char dough_msg[DOUGH_SIZE];
                in >> order_msg >> pipe1 >> dough_msg;
                order.deserialize(order_msg);
                dough.deserialize(dough_msg);
            }
        };
};

#endif