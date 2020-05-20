#include "Pizzamaker.h"

Pizzamaker::Pizzamaker(int id_num,
                                    FifoEscritura * dough_order_channel,
                                    FifoLectura * sourdough_channel,
                                    std::string orders_channel_name,
                                    std::string delivery_channel_name)
: CookerMan(id_num, dough_order_channel,sourdough_channel, orders_channel_name, delivery_channel_name){}

std::string Pizzamaker::identify() const {
    return "PizzaMaker " + std::to_string(this->id);
}
std::string Pizzamaker::getMeal() const {
    return PIZZA;
}

Pizzamaker::~Pizzamaker() {}


