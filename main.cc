#include <iostream>
#include <unistd.h>

#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "Logger.h"
#include "ArgsHelper.h"
#include "Sourdough.h"
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

	pid_t pid = fork ();
	if ( pid == CHILD_PD ) {
		FifoLectura * pizza_channel = new FifoLectura("pizza");
		//pizza_channel->abrir();

		/*size_t  read_bytes = pizza_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
		while(read_bytes > FIFO_EOF ){
        	read_bytes = pizza_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
    	} 
    	if(read_bytes == ERROR){
        	throw std::runtime_error("Error: fifo reading error");     
    	} */
		exit(OK);
	}

	pid = fork ();
	if ( pid == CHILD_PD ) {
		FifoLectura * bread_channel = new FifoLectura("bread");
		//bread_channel->abrir();

		/*size_t  read_bytes = pizza_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
		while(read_bytes > FIFO_EOF ){
        	read_bytes = pizza_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
    	} 
    	if(read_bytes == ERROR){
        	throw std::runtime_error("Error: fifo reading error");     
    	} */
		exit(OK);
	}


	

	
	/*
	Sourdough sourdough("masa");
	sourdough.start();
	Pizzamaker pizza_maker(1, "masa");
	pizza_maker.start();
	Pizzamaker pizza_maker2(2, "masa");
	pizza_maker2.start();
	*/

	
	while( sigint_handler.getGracefulQuit() == 0 ) {
		//mientras no se reciba la senial SIGINT, el canal de log sigue abierto
	}
	// se recibio la senial SIGINT, el proceso termina
	
	std::cout << "[Main] SIGINT stoping all" << std::endl;

	
	/*
	sourdough.stop();
	pizza_maker.stop();
	pizza_maker2.stop();
	*/
	
	
	Logger::destruir();
	SignalHandler::destruir();
	exit(0);
}

