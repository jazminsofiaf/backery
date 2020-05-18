#ifndef BAKER_H_
#define BAKER_H_


#include "CookerMan.h"
using namespace std;

class Baker: public CookerMan{

    public:
        //constructor
        Baker(int id_num,std::string sourdough_channel_name, std::string orders_channel_name, std::string delivery_channel_name);
        std::string identify() const override; 

};

#endif