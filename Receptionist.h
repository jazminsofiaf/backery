#ifndef RECEPCIONIST_H_
#define RECEPCIONIST_H_

#define CHUNK_LEN 5 


#include <fstream> 
#include <locale>
#include <fcntl.h>
#include <map>
#include "Constant.h"
#include "ProductEnum.h"
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
            std::string product;
            std::string toString(){
                return " Order { id = " +std::to_string(id) +", product = "+ product +" }";
            }
        };
};

#endif