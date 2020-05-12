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

int get_file_size(std::string filename){
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

int main(int argc, char** argv){
	//event handler para la senial SIGINT (-2)
	//A child created via fork(2) inherits a copy of its parent's signal dispositions
	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler, SA_RESTART);
	
	ArgsHelper::args * args = ArgsHelper::parse(argc, argv);
	Logger::init();

	std::ifstream file(args->pedido); 
  	int file_size = get_file_size(args->pedido);	
	int bytes_by_delivery_recepcionist = ( file_size + args->delivery - 1 )/ args->delivery;
	for(int start =0, num = 1 ; start < file_size && num <= args->delivery ; start = start + bytes_by_delivery_recepcionist, num=num+1){
		Recepcionist recepcionist(num,"bread", "pizza", start, start + bytes_by_delivery_recepcionist, args->pedido);
		recepcionist.run();
	}
	


	


	



	Sourdough sourdough("masa");
	sourdough.start();
	Pizzamaker pizza_maker(1, "masa");
	pizza_maker.start();

	Pizzamaker pizza_maker2(2, "masa");
	pizza_maker2.start();


	
	while( sigint_handler.getGracefulQuit() == 0 ) {
		//mientras no se reciba la senial SIGINT, el canal de log sigue abierto
	}
	// se recibio la senial SIGINT, el proceso termina
	
	std::cout << "[Main] SIGINT stoping all" << std::endl;

	
	sourdough.stop();
	pizza_maker.stop();
	pizza_maker2.stop();
	
	
	Logger::destruir();
	SignalHandler::destruir();
	exit(0);
}

