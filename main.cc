#include <iostream>
#include <unistd.h>

#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "Logger.h"

void bloquearSigint ();

using namespace std;

int main () {
	Logger::init();



	// event handler para la senial SIGINT (-2)
	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	while( sigint_handler.getGracefulQuit() == 0 ) {
		//mientras no se reciba la senial SIGINT, el canal de log sigue abierto
	}
	// se recibio la senial SIGINT, el proceso termina
	std::cout << "[Main] se recibio la senial SIGINT cierro el fifo" << std::endl;
	SignalHandler::destruir();
	Logger::destruir();
	exit(0);
}

void bloquearSigint () {
	sigset_t sa;
	sigemptyset ( &sa );
	sigaddset ( &sa,SIGINT );
	sigprocmask ( SIG_BLOCK,&sa,NULL );
}
