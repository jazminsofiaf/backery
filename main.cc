#include <list>
#include "Handlers/SIGINT_Handler.h"
#include "Handlers/SignalHandler.h"
#include "Logger.h"
#include "ArgsHelper.h"
#include "Bakery.h"



using namespace std;

int main(int argc, char** argv){
	ArgsHelper::args *args;
    try {
        args = ArgsHelper::parse(argc, argv);
    } catch (const runtime_error& arguments_error){
        return ERROR;
    }

    Bakery bakery(args);
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler ( SIGINT,&sigint_handler, 0);
    while ( sigint_handler.getGracefulQuit() == 0 ) { //loop for handle SIGINT
        bakery.initWorkDay();
		break;
	}
    bakery.endWorkDay();
	SignalHandler::destruir();
	return OK;
}


