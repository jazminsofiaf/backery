#ifndef PIZZA_MAKER_H_
#define PIZZA_MAKER_H_


#include "CookerMan.h"
using namespace std;


class Pizzamaker: public CookerMan{
    public:
        Pizzamaker(int id_num,
                FifoEscritura * dough_order_channel,
                FifoLectura * sourdough_channel,
                std::string orders_channel_name,
                std::string delivery_channel_name);
        ~Pizzamaker();
        std::string identify() const override;
        std::string getMeal() const override;

};

#endif