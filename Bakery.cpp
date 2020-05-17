#include "Bakery.h"

Bakery::Bakery(){} 


void Bakery::initWorkDay( ArgsHelper::args * args, Logger * logger){
	this->sourdough = new Sourdough(logger, DOUGH_CHANNEL);
	this->sourdough->start();
	this->delivery = new Delivery(logger, DELIVERY_CHANNEL);
	this->delivery->start();

	
	for(int num = 1;  num <= args->pizzeros ; num=num+1){
			Pizzamaker pizza_maker(num, logger, DOUGH_CHANNEL, PIZZA_CHANNEL, DELIVERY_CHANNEL);
			pizza_maker.start();
			this->allPizzaMaker.push_front(pizza_maker);
	}

	for(int num = 1;  num <= args->panaderos ; num=num+1){
			Baker baker(num, logger,DOUGH_CHANNEL, BREAD_CHANNEL, DELIVERY_CHANNEL);
			baker.start();
			this->allBakers.push_front(baker);
	}

	
	int bytes_by_delivery_recepcionist = ( args->file_size + args->delivery - 1 )/ args->delivery;
	for(int start =0, num = 1 ; start < args->file_size && num <= args->delivery ; start = start + bytes_by_delivery_recepcionist, num=num+1){
			Recepcionist recepcionist(num, logger, BREAD_CHANNEL ,PIZZA_CHANNEL , start, start + bytes_by_delivery_recepcionist, args->pedido);
			recepcionist.start();
			this->allRecepcionists.push_front(recepcionist);
	}
	

}

void Bakery::endWorkDay(){
    // espero a que los cocineros terminen de cocinar todos los pedidos
	while(!this->allPizzaMaker.empty()){
       Pizzamaker pizza_maker = allPizzaMaker.back();
	   allPizzaMaker.pop_back();
	   pizza_maker.stop();
	}

	while(!this->allBakers.empty()){
       Baker baker = allBakers.back();
	   allBakers.pop_back();
	   baker.stop();
   	}	 
	this->sourdough->stop();	

	this->delivery->stop();
	
	while(!this->allRecepcionists.empty()){
       Recepcionist recepcionist = allRecepcionists.back();
	   allRecepcionists.pop_back();
	   recepcionist.stop();
   	}	

}
/*
Bakery :: ~Bakery() {
	std::cout << "calling Bakery detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
	//delete this->sourdough;
	//delete this->delivery;
}
*/
    