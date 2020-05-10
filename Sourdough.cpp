#include "Sourdough.h"
Sourdough::Sourdough(std::string channel_name): channel_name(channel_name), num(0) {}

void Sourdough::run(){
    pid_t pid = fork ();
	if ( pid == 0 ) {

		SIGUSR_Handler sigusr_handler;
		SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr_handler);
	
		this->write_channel = new FifoEscritura(this->channel_name);
		//this->write_channel->abrir(); //blocking until open for read
		while( sigusr_handler.getGracefulQuit() == 0 ) {
            sleep(1);
            Sourdough::Dough * dough = new Dough();
            dough->num = num++;
            //this->write_channel->escribir(dough, sizeof(dough));
            Logger::log("[Sourdough] " + dough->toString() );
			std::cout << "[Sourdough] looping " << std::endl;
	    }
		std::cout << "[Sourdough] loop ends " << std::endl;
		exit(0);
	}
	std::cout << "[Sourdough] pid: "<<pid << std::endl;
	this->process_id = pid;
}

void Sourdough::stop(){
    std::cout << "[Sourdough] stoping..." << std::endl;
	kill(this->process_id, SIGUSR1);
	int status;
	Logger::log("[Sourdough] end");
	this->write_channel->cerrar();
	this->write_channel->eliminar();
	waitpid(this->process_id, &status,0);
	std::cout << "[Sourdough] stoped" << std::endl;
}