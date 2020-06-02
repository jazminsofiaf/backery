#include "Delivery.h"


Delivery::Delivery(std::string channel_name): Employee(0), channel_name(channel_name){
    this->read_channel = new FifoLectura(this->channel_name);
    this->logger = new Logger();
}

std::string Delivery::identify() const {
    return "Delivery";
}

void Delivery::run(){
    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler ( SIGINT,&sigint_handler, 0);

	this->read_channel->abrir(); //blocking until cooker opens for write

    CookerMan::Product product;
	size_t read_bytes = this->read_channel->leer(&product, sizeof(CookerMan::Product));
	while(read_bytes > FIFO_EOF && sigint_handler.getGracefulQuit() == 0 ){ // all cookers closed write channel

        this->logger->log(this, " ------> " + product.toString());
        read_bytes = this->read_channel->leer(&product, sizeof(CookerMan::Product));
    } 
    if(read_bytes == ERROR){
        std::cerr << this->identify() <<" delivery channel closed" << std::endl;
    }
	this->read_channel->close_fifo();
	this->read_channel->eliminar();
    SignalHandler::destruir();
}

void Delivery::waitMe(){
	Employee::waitMe();
	
}


Delivery:: ~Delivery() {
	delete this->read_channel;
    delete this->logger;
}
