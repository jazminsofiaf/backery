#include "Sourdough.h"
Sourdough::Sourdough(std::string channel_name): Employee(0), channel_name(channel_name), num(0) {}

std::string Sourdough::identify() const {
    return "Sourdough";
}

void Sourdough::run(){
    pid_t pid = fork ();
	if ( pid == CHILD_PD ) {

		SIGUSR_Handler sigusr_handler;
		SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr_handler,0);
	
		this->write_channel = new FifoEscritura(this->channel_name);
		this->write_channel->abrir(); //blocking until open for read
		while( sigusr_handler.getGracefulQuit() == 0 ) {
            sleep(1);
            Sourdough::Dough dough;
            dough.num = 1 + num++;
            this->write_channel->escribir(&dough, sizeof(Sourdough::Dough));
            Logger::log(this, dough.to_string() );
			std::cout << "[Sourdough] looping " << std::endl;
		}
		int rv = this->write_channel->close_fifo();
		if(rv != OK){
			std::string msg = "[Sourdough]  Error closing fifo ";	
			std::cout << msg << std::strerror(errno) << std::endl;
		} 
		this->write_channel->eliminar();
		std::cout << "[Sourdough] loop ends " << std::endl;
		exit(OK);
	}
	std::cout << "[Sourdough] pid: "<<pid << std::endl;
	this->process_id = pid;
}

void Sourdough::stop(){
    std::cout << "[Sourdough] stoping..." << this->process_id << std::endl;
	kill(this->process_id, SIGUSR1);
	int status;
	Logger::log(this, "end");
	waitpid(this->process_id, &status,0);
	std::cout << "[Sourdough] stoped " << status << std::endl;
}