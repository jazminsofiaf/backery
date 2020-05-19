#ifndef BAKERY_H_
#define BAKERY_H_

#include <iostream>
#include <unistd.h>
#include <list>

#include "ArgsHelper.h"
#include "Sourdough.h"
#include "Baker.h"
#include "Delivery.h"
#include "Pizzamaker.h"
#include "Receptionist.h"

#define BREAD_CHANNEL "bread"
#define PIZZA_CHANNEL "pizza"
#define DELIVERY_CHANNEL "delivery"
#define DOUGH_CHANNEL "dough"


class Bakery{
    private:
        list<Receptionist*> allReceptionists;
        list<Pizzamaker*> allPizzaMaker;
        list<Baker*> allBakers;
        Sourdough  * sourdough;
		Delivery * delivery;

        FifoEscritura * bread_channel;
        FifoEscritura * pizza_channel;

    public:
        //constructor
        Bakery(ArgsHelper::args * args);
        ~Bakery();
        void initWorkDay();
        void endWorkDay();

};

#endif