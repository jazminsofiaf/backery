#ifndef RECEPCIONIST_H_
#define RECEPCIONIST_H_
#include <iostream>
#include <sstream>



#include <fstream> 
#include <locale>
#include <fcntl.h>
#include <map>
#include "Constant.h"
#include "Locks/SharedFile.h"
#include "Handlers/SIGUSR_Handler.h"
#include "Handlers/SignalHandler.h"
#include "Logger.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Employee.h"
using namespace std;


class Receptionist : public Employee {
    private:
        int read_start;
        int read_end;
        std::string orders_file;
        FifoEscritura * bread_channel;
        FifoEscritura * pizza_channel;
        Logger * logger;
        std::map<string, FifoEscritura *> channel_map;
        bool isDelimiter(char c);
        void tryToSend(std::string order, int pos);
        std::string toUpper(std::string str);
    

        
    public:
        //constructor
        Receptionist(int id_num,
                     FifoEscritura * bread_channel,
                     FifoEscritura * pizza_channel,
                     int start, int end, std::string file_name);
        ~Receptionist();
        void run() override;
        void waitMe() override;
        std::string identify() const override; 

        struct Order {
            int id;
            char product[PRODUCT_LEN];
            std::string toString(){
                return " Order { id = " +std::to_string(id) +", product = "+ std::string(product) +" }";
            }
            const char * serialize() {
                std::ostringstream out;
                out  << id << '|' << product;
                return out.str().c_str();
            }

            void deserialize(const char * msg){
                char  pipe1 = 0;
                std::istringstream in(msg);
                in >> id >> pipe1 >> product;
            }
        };
};

#endif