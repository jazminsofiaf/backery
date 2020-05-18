#include "Pizzamaker.h"

Pizzamaker::Pizzamaker(int id_num,
                                    std::string sourdough_channel_name, 
                                    std::string orders_channel_name,
                                    std::string delivery_channel_name)
: CookerMan(id_num, sourdough_channel_name, orders_channel_name, delivery_channel_name){}

std::string Pizzamaker::identify() const {
    return "PizzaMaker "+std::to_string(this->id);
}

Pizzamaker::~Pizzamaker() {}


