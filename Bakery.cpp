#include "Bakery.h"

Bakery::Bakery(ArgsHelper::args * args ){
    this->sourdough = new Sourdough(DOUGH_CHANNEL);
    this->delivery = new Delivery(DELIVERY_CHANNEL);

    for(int num = 1;  num <= args->pizzeros;  num = num + 1 ){
        Pizzamaker * a_pizza_maker = new Pizzamaker(num, DOUGH_CHANNEL, PIZZA_CHANNEL, DELIVERY_CHANNEL);
        this->allPizzaMaker.push_front(a_pizza_maker);
    }

    for(int num = 1;  num <= args->panaderos ; num = num + 1 ){
        Baker * a_baker = new Baker(num, DOUGH_CHANNEL, BREAD_CHANNEL, DELIVERY_CHANNEL);
        this->allBakers.push_front(a_baker);
    }

    this->bread_channel = new FifoEscritura(BREAD_CHANNEL);
    this->pizza_channel = new FifoEscritura(PIZZA_CHANNEL);

    int bytes_by_delivery_receptionist = ( args->file_size + args->delivery - 1 )/ args->delivery;
    for(int start =0, num = 1 ; start < args->file_size && num <= args->delivery ; start = start + bytes_by_delivery_receptionist, num=num+1){
        Receptionist * a_receptionist = new Receptionist(num, this->bread_channel , this->pizza_channel , start, start + bytes_by_delivery_receptionist, args->pedido);
        this->allReceptionists.push_front( a_receptionist);
    }

}


void Bakery::initWorkDay(){
	this->sourdough->start();
	this->delivery->start();

    for(Pizzamaker * pizza_maker : this->allPizzaMaker){
        pizza_maker->start();
    }

    for(Baker *  baker : this->allBakers){
        baker->start();
    }

    std::cout <<  " abriendo channels.." <<  std::strerror(errno) << endl;
    this->bread_channel->abrir();//block until at least one cooker man open read side
    std::cout << " bread channel abierto .." <<  std::strerror(errno) << endl;
    this->pizza_channel->abrir(); //block until at least one cooker man open read side
    std::cout << " pizza channel abierto .." <<  std::strerror(errno) << endl;

    for(Receptionist * receptionist : this->allReceptionists){
        receptionist->start();
    }

}

void Bakery::endWorkDay(){
    for(Receptionist * receptionist : this->allReceptionists){
        receptionist->stop();
    }
    this->bread_channel->close_fifo(); //signal to stop loop for baker man
    this->pizza_channel->close_fifo(); //signal to stop loop for pizza man
    //all receptionist already finished

    for(Pizzamaker * pizza_maker : this->allPizzaMaker){ //wait until all cookers finished
        pizza_maker->stop();
    }
    for(Baker * baker : this->allBakers){
        baker->stop();
    }

    //all cookers already finished

    this->delivery->stop(); //wait until all cookers finish
    this->sourdough->stop();


}

Bakery :: ~Bakery() {
    //std::cout << "calling Bakery detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    delete this->sourdough;
    delete this->delivery;
    for(Pizzamaker * pizza_maker : this->allPizzaMaker){ //wait until all cookers finished
        delete pizza_maker;
    }
    for(Baker * baker : this->allBakers){
        delete baker;
    }
    for(Receptionist * receptionist : this->allReceptionists){
        delete receptionist;
    }
    delete this->bread_channel;
    delete this->pizza_channel;
}

    