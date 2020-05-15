#include <iostream>
#include <unistd.h>
#include <list>

#include "Handlers/SIGINT_Handler.h"
#include "Handlers/SignalHandler.h"
#include "Logger.h"
#include "ArgsHelper.h"
#include "Bakery.h"



using namespace std;

int main(int argc, char** argv){
	SIGINT_Handler sigint_handler;
	SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler, 0);
	Bakery bakery;
	while ( sigint_handler.getGracefulQuit() == 0 ) { //loop for handle SIGINT
		ArgsHelper::args * args;
		try{
			args = ArgsHelper::parse(argc, argv);
			Logger::init();
		}catch(const runtime_error& arguments_error){
			return ERROR;
		}
		bakery.initWorkDay(args);
		break;
	}
	bakery.endWorkDay();
	Logger::destruir();
	SignalHandler::destruir();
	return OK;
}


