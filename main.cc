#include <iostream>
#include <unistd.h>
#include <list>

#include "Handlers/SIGINT_Handler.h"
#include "Handlers/SignalHandler.h"
#include "EndChildException.h"
#include "Logger.h"
#include "ArgsHelper.h"
#include "Bakery.h"



using namespace std;

int main(int argc, char** argv){
	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler ( SIGINT,&sigint_handler, 0);
	Bakery bakery;
	while ( sigint_handler.getGracefulQuit() == 0 ) { //loop for handle SIGINT
		ArgsHelper::args * args;
		try{
			args = ArgsHelper::parse(argc, argv);
		} catch (const runtime_error& arguments_error){
			return ERROR;
		}
		try{
			bakery.initWorkDay(args);
		} catch (const EndChildException& e){
	
			return OK;
		}
		break;
	
	}
	bakery.endWorkDay();
	SignalHandler::destruir();
	return OK;
}


