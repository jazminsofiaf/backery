#ifndef BAKER_H_
#define BAKER_H_


#include "CookerMan.h"
using namespace std;

class Baker: public CookerMan{

    public:
        //constructor
        Baker(int id_num,
              FifoEscritura * dough_order_channel,
              FifoLectura * sourdough_channel,
              std::string orders_channel_name,
              std::string delivery_channel_name);
        ~Baker();
        std::string identify() const override;
        std::string getMeal() const override;

};

#endif