#ifndef RECEPCIONIST_H_
#define RECEPCIONIST_H_

#define CHUNK_LEN 5 

#include <fstream> 
#include "Constant.h"
#include "Fifos/FifoEscritura.h"
#include "Fifos/FifoLectura.h"
#include "Logger.h"
#include "Employee.h"   
using namespace std;


class Recepcionist : public Employee {
    private:
        
        
        std::string bread_channel_name;
        std::string pizza_channel_name;
        int read_start;
        int read_end;
        std::string orders_file;
        FifoEscritura * bread_channel;
        FifoEscritura * pizza_channel;
        pid_t process_id;
        bool is_delimiter(char c);
        void validate_order(std::string order);
    

        
    public:
        //constructor
        Recepcionist(int id_num, 
                    std::string bread_name, std::string pizza_name,
                    int start, int end, std::string file_name);
        void run();
        void stop();
        std::string identify() const override; 
};

#endif