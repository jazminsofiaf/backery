#include "Bakery.h"

Bakery::Bakery(ArgsHelper::args * args ){
    this->dough_orders_channel = new FifoEscritura(DOUGH_CHANNEL); //send orders of dough

    this->sourdough = new Sourdough(DOUGH_CHANNEL, args->pizzeros, args->panaderos);
    this->delivery = new Delivery(DELIVERY_CHANNEL);

    for(int num = 1;  num <= args->pizzeros;  num = num + 1 ){
        std::string sourdough_channel_name = PIZZA + std::to_string(num);
        FifoLectura * sourdough_channel = new FifoLectura(sourdough_channel_name);
        this->sourdough_channels.push_back(sourdough_channel);
        Pizzamaker * a_pizza_maker = new Pizzamaker(num, dough_orders_channel, sourdough_channel, PIZZA_CHANNEL, DELIVERY_CHANNEL);
        this->allPizzaMaker.push_front(a_pizza_maker);
    }

    for(int num = 1;  num <= args->panaderos ; num = num + 1 ){
        std::string sourdough_channel_name = BREAD + std::to_string(num);
        FifoLectura * sourdough_channel = new FifoLectura(sourdough_channel_name);
        this->sourdough_channels.push_back(sourdough_channel);
        Baker * a_baker = new Baker(num, dough_orders_channel, sourdough_channel, BREAD_CHANNEL, DELIVERY_CHANNEL);
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
	this->dough_orders_channel->abrir(); //blocked until sourdough channel open other side;
    for(FifoLectura * sourdough_channel: this->sourdough_channels){
        //order is important
        sourdough_channel->abrir();//blocked until sourdough channel open other side;
    }
	this->delivery->start();


    for(Pizzamaker * pizza_maker : this->allPizzaMaker){
        pizza_maker->start();
    }

    for(Baker *  baker : this->allBakers){
        baker->start();
    }

    this->bread_channel->abrir();//block until at least one cooker man open read side
    this->pizza_channel->abrir(); //block until at least one cooker man open read side

    for(Receptionist * receptionist : this->allReceptionists){
        receptionist->start();
    }

}

void Bakery::endWorkDay(){

    for(Receptionist * receptionist : this->allReceptionists){
        receptionist->waitMe();
    }
    this->bread_channel->close_fifo(); //not used anymore
    this->pizza_channel->close_fifo(); //not used anymore
    this->bread_channel->eliminar();
    this->pizza_channel->eliminar();

    std::cout << "orders channels closed ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;

    //all receptionist already finished

    for(Pizzamaker * pizza_maker : this->allPizzaMaker){ //wait until all cookers finished
        pizza_maker->waitMe();
    }
    for(Baker * baker : this->allBakers){
        baker->waitMe();
    }
    //all cookers already finished
    this->dough_orders_channel->close_fifo(); //avoid sourdough block at read
    this->dough_orders_channel->eliminar();
    this->sourdough->waitMe();
    this->delivery->waitMe(); //wait until all cookers finish



    for(FifoLectura * sourdough_channel: this->sourdough_channels){
        //order is important
        sourdough_channel->close_fifo();//blocked until sourdough channel open other side;
        sourdough_channel->eliminar();
    }

    std::cout << "todo cerro bien"  << std::endl;

}

Bakery :: ~Bakery() {
    std::cout << "calling Bakery detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
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
    delete this->dough_orders_channel;
    for(FifoLectura * sourdough_channel: this->sourdough_channels){
        delete sourdough_channel;
    }
}

    