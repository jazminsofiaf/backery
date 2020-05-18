#include "Bakery.h"

Bakery::Bakery(ArgsHelper::args * args ){
    this->sourdough = new Sourdough(DOUGH_CHANNEL);
    this->delivery = new Delivery(DELIVERY_CHANNEL);

    for(int num = 1;  num <= args->pizzeros;  num = num + 1 ){
        std::cerr << "[MAIN] creating baker man" << num << std::endl;
        Pizzamaker pizza_maker(num, DOUGH_CHANNEL, PIZZA_CHANNEL, DELIVERY_CHANNEL);
        this->allPizzaMaker.push_front(pizza_maker);
    }

    for(int num = 1;  num <= args->panaderos ; num = num + 1 ){
        Baker baker(num, DOUGH_CHANNEL, BREAD_CHANNEL, DELIVERY_CHANNEL);
        this->allBakers.push_front(baker);
    }

    int bytes_by_delivery_recepcionist = ( args->file_size + args->delivery - 1 )/ args->delivery;
    for(int start =0, num = 1 ; start < args->file_size && num <= args->delivery ; start = start + bytes_by_delivery_recepcionist, num=num+1){
        Receptionist recepcionist(num, BREAD_CHANNEL , PIZZA_CHANNEL , start, start + bytes_by_delivery_recepcionist, args->pedido);
        this->allReceptionists.push_front(recepcionist);
    }

}


void Bakery::initWorkDay(){
	this->sourdough->start();
	this->delivery->start();

	int num = 1;
    for(Pizzamaker pizza_maker : this->allPizzaMaker){
        std::cerr << "[MAIN] starting baker man" << num << std::endl;
        pizza_maker.start();
        num++;
    }


    for(Baker baker : this->allBakers){
        baker.start();
    }

    for(Receptionist receptionist : this->allReceptionists){
        receptionist.start();
    }

}

void Bakery::endWorkDay(){
    for(Pizzamaker pizza_maker : this->allPizzaMaker){ //wait until all cookers finished
        pizza_maker.stop();
    }
    for(Baker baker : this->allBakers){
        baker.stop();
    }

    //all cookers already finished

    for(Receptionist receptionist : this->allReceptionists){
        receptionist.stop();
    }

    //all receptionist already finished

    this->delivery->stop(); //wait until all cookers finish
    this->sourdough->stop();


}

Bakery :: ~Bakery() {
    //std::cout << "calling Bakery detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    //delete this->sourdough;
    //delete this->delivery;
}

    