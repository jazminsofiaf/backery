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
#include <iostream>
#include <sstream>



using namespace std;
class Sourdough: public Employee{
private:
    Logger * logger;
    FifoLectura * dough_order_channel;
    std::list<FifoEscritura *> write_channels_sorted;
    std::map<std::string, FifoEscritura *> all_write_channels;
    int dough_index;
    int dough_size;

    void feedDough();

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

            const char * serialize() {
                std::ostringstream out;
                out  << order_id << '|' << cook_id << '|' << product;
                return out.str().c_str();
            }

            void deserialize(const char * msg){
                char  pipe1 = 0, pipe2 = 0;
                std::istringstream in(msg);
                in >> order_id >> pipe1 >> cook_id >> pipe2 >> product;
            }
        };

        struct Dough{
            int num;
            int index;
            int dough_size;
            std::string toString(){
                return " Dough { num = " +std::to_string(num) +", index = " +std::to_string(index) +" size = "+std::to_string(dough_size)+"}";
            }

            const char * serialize() {
                std::ostringstream out;
                out  << num << '|' << index << '|' << dough_size;
                return out.str().c_str();
            }

            void deserialize(const char * msg){
                char  pipe1 = 0, pipe2 = 0;
                std::istringstream in(msg);
                in >> num >> pipe1 >> index >> pipe2 >> dough_size;
            }
        };

};

#endif