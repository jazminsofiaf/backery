#include <iostream>
#include <unistd.h>

#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "Logger.h"
#include "ArgsHelper.h"
#include "Sourdough.h"
#include "Pizzamaker.h"

#include "Fifos/FifoLectura.h"


using namespace std;

int main(int argc, char** argv){
	//event handler para la senial SIGINT (-2)
	//A child created via fork(2) inherits a copy of its parent's signal dispositions
	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	
	
	ArgsHelper::args * args = ArgsHelper::parse(argc, argv);
	std::cout << "se parsea el archivo de pedidos: "<<args->pedido << std::endl;

	Logger::init();

	Sourdough sourdough("masa");
	sourdough.run();
	//Pizzamaker pizza_maker("masa");
	//pizza_maker.run();

	
	pid_t pid = fork ();
	if ( pid == 0 ) {
		FifoLectura * sourdough_channel = new FifoLectura("masa");
		sourdough_channel->abrir();
		Sourdough::Dough buffer;
		sourdough_channel->leer(&buffer,sizeof(Sourdough::Dough));
		std::cout << "[Main] first pice of bread dough" << buffer.to_string() <<std::endl;
		exit(0);
	}


	
	while( sigint_handler.getGracefulQuit() == 0 ) {
		//mientras no se reciba la senial SIGINT, el canal de log sigue abierto
	}
	// se recibio la senial SIGINT, el proceso termina
	
	std::cout << "[Main] SIGINT stoping all" << std::endl;

	sourdough.stop();
	//pizza_maker.stop();
	Logger::destruir();
	SignalHandler::destruir();
	exit(0);
}

