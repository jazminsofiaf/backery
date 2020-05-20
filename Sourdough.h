#ifndef SOURDOUGH_H_
#define SOURDOUGH_H_
#include <map>
#include <list>
#include "Constant.h"
#include "Handlers/SIGUSR_Handler.h"
#include "Handlers/SignalHandler.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Employee.h"
#include "Logger.h"



using namespace std;
class Sourdough: public Employee{
private:
    Logger * logger;
    FifoLectura * dough_order_channel;
    std::list<FifoEscritura *> write_channels_sorted;
    std::map<std::string, FifoEscritura *> all_write_channels;
    int dough_index;

    public:
        //constructor
        Sourdough(std::string channel_name, int pizza, int bread);
        ~Sourdough();
        void run() override;
        void cookSourDough(FifoEscritura * write_channel, int num);
        void waitMe() override;
        std::string identify() const override;

        struct DoughOrder {
            int order_id;
            int cook_id;
            char product[PRODUCT_LEN];
            std::string toString(){
                return  "Dough Order { order = " +std::to_string(order_id)
                        +", by = { id = "+std::to_string(cook_id)+", product = "+ std::string(product) +" } ";
            }
        };

        struct Dough{
            int num;
            int index;
            std::string toString(){
                return " Dough { num = " +std::to_string(num) +", index = " +std::to_string(index) +" }";
            }
        };

};

#endif