#include <iostream>
#include <unistd.h>

#include "Handlers/SIGINT_Handler.h"
#include "Handlers/SignalHandler.h"
#include "Logger.h"
#include "ArgsHelper.h"
#include "Sourdough.h"
#include "Baker.h"
#include "Pizzamaker.h"
#include "Recepcionist.h"

#include "Fifos/FifoLectura.h"


using namespace std;

int main(int argc, char** argv){
	//event handler para la senial SIGINT (-2)
	//A child created via fork(2) inherits a copy of its parent's signal dispositions
	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler, SA_RESTART);
	
	ArgsHelper::args * args = ArgsHelper::parse(argc, argv);
	Logger::init();

	
	std::ifstream file(args->pedido); 
	int bytes_by_delivery_recepcionist = ( args->file_size + args->delivery - 1 )/ args->delivery;
	for(int start =0, num = 1 ; start < args->file_size && num <= args->delivery ; start = start + bytes_by_delivery_recepcionist, num=num+1){
		Recepcionist recepcionist(num,"bread", "pizza", start, start + bytes_by_delivery_recepcionist, args->pedido);
		recepcionist.start();
	}
	
	//Sourdough sourdough("dough");
	//sourdough.start();
	//Delivery delivery("delivery");
	//delivery.start();
	
	Pizzamaker pizza_maker(1, "dough", "pizza");
	pizza_maker.start();
	
	Baker baker(1, "dough", "bread");
	baker.start();
	
	
	while( sigint_handler.getGracefulQuit() == 0 ) {
		//mientras no se reciba la senial SIGINT, el canal de log sigue abierto
	}
	// se recibio la senial SIGINT, el proceso termina
	
	std::cout << "[Main] SIGINT stoping all" << std::endl;

	
	/*
	//stop de los recepcionist
	sourdough.stop();
	pizza_maker.stop();
	baker.stop();

	*/
	
	Logger::destruir();
	SignalHandler::destruir();
	exit(0);
}

