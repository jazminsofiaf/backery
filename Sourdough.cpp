#include "Sourdough.h"
Sourdough::Sourdough(std::string channel_name): Employee(0), channel_name(channel_name), num(0) {
    this->write_channel = new FifoEscritura(this->channel_name);
    std::cout << "[Sourdough ]" << std::strerror(errno) << std::endl;
}

std::string Sourdough::identify() const {
    return "Sourdough";
}

void Sourdough::init() {
    std::cout << "[Sourdough] por abrir fifo" << std::endl;
    this->write_channel->abrir(); //other side already open
}

void Sourdough::run(){
    std::cout << "[Sourdough]" << std::strerror(errno) << std::endl;
    SIGUSR_Handler sigusr_handler;
	SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr_handler,0);

    std::cout << "[Sourdough] despues de abrir fifo" << sigusr_handler.getGracefulQuit() << std::endl;
	while( sigusr_handler.getGracefulQuit() == 0 ) {
			std::cout << "[Sourdough] looping " << sigusr_handler.getGracefulQuit() << std::endl;
            sleep(1);
            Sourdough::Dough dough;
            dough.num = 1 + num++;
            this->write_channel->escribir(&dough, sizeof(Sourdough::Dough));
            
	}
	std::cout << "[Sourdough] loop ends " << sigusr_handler.getGracefulQuit() << std::endl;
	this->write_channel->close_fifo();
	this->write_channel->eliminar();
	
}

void Sourdough::waitMe(){
    std::cout << "[Sourdough] stoping..." <<this->process_id << std::endl;
	kill(this->process_id, SIGUSR1);
	Employee::waitMe();
}

Sourdough :: ~Sourdough() {
    std::cout << "calling sourfough detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    delete this->write_channel;
}
