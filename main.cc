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
	Pizzamaker pizza_maker(1, "masa");
	pizza_maker.run();

	Pizzamaker pizza_maker2(2, "masa");
	pizza_maker2.run();


	
	while( sigint_handler.getGracefulQuit() == 0 ) {
		//mientras no se reciba la senial SIGINT, el canal de log sigue abierto
	}
	// se recibio la senial SIGINT, el proceso termina
	
	std::cout << "[Main] SIGINT stoping all" << std::endl;

	
	sourdough.stop();
	pid_t pid = fork ();
	if ( pid == CHILD_PD ) {
		pizza_maker.stop();
		exit(OK);
	}

	pid = fork ();
	if ( pid == CHILD_PD ) {
		pizza_maker2.stop();
		exit(OK);
	}
	//pizza_maker.stop();
	//pizza_maker2.stop();
	
	Logger::destruir();
	SignalHandler::destruir();
	exit(0);
}

