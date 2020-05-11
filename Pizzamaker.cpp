#include "Pizzamaker.h"

Pizzamaker::Pizzamaker(int id_num, std::string sourdough_channel_name): CookerMan(id_num, sourdough_channel_name) {}

std::string Pizzamaker::identify() const {
    return "PizzaMaker "+std::to_string(this->id);
}


