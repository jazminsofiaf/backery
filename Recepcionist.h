#ifndef RECEPCIONIST_H_
#define RECEPCIONIST_H_

#define CHUNK_LEN 5 


#include <fstream> 
#include <locale>
#include <fcntl.h>
#include <map>
#include "Constant.h"
#include "ProductEnum.h"
#include "LockFile.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Logger.h"
#include "Employee.h"   
using namespace std;


class Recepcionist : public Employee {
    private:
        Logger * logger;
        std::string bread_channel_name;
        std::string pizza_channel_name;
        int read_start;
        int read_end;
        std::string orders_file;
        FifoEscritura * bread_channel;
        FifoEscritura * pizza_channel;
        std::map<string, FifoEscritura *> channel_map;
        bool isDelimiter(char c);
        void tryToSend(std::string order, int pos);
        std::string toUpper(std::string str);
    

        
    public:
        //constructor
        Recepcionist(int id_num, 
                    Logger * logger, 
                    std::string bread_name, std::string pizza_name,
                    int start, int end, std::string file_name);
        void run() override;
        void stop() override;
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