#ifndef PIZZA_MAKER_H_
#define PIZZA_MAKER_H_


#include "CookerMan.h"
using namespace std;


class Pizzamaker: public CookerMan{
        
    public:
        //constructor
        Pizzamaker(int id_num, std::string sourdough_channel_name);
        std::string identify() const override; 

};

#endif