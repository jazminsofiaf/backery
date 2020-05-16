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
#include "Recepcionist.h"

#define BREAD_CHANNEL "bread"
#define PIZZA_CHANNEL "pizza"
#define DELIVERY_CHANNEL "delivery"
#define DOUGH_CHANNEL "dough"


class Bakery{
    private:
        list<Recepcionist> allRecepcionists;
        list<Pizzamaker> allPizzaMaker;
        list<Baker> allBakers;
        Sourdough  * sourdough;
		Delivery * delivery;

    public:
        //constructor
        Bakery(Logger * logger);
        void initWorkDay(ArgsHelper::args * args, Logger * logger);
        void endWorkDay();

};

#endif