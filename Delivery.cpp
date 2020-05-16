#include "Delivery.h"
Delivery::Delivery(Logger * logger, std::string channel_name): Employee(0), channel_name(channel_name){}

std::string Delivery::identify() const {
    return "Delivery";
}

void Delivery::run(){
	this->read_channel = new FifoLectura(this->channel_name);
	this->read_channel->abrir(); //blocking until open for write

    CookerMan::Product product;
	size_t read_bytes = read_channel->leer(&product,sizeof(CookerMan::Product));
	while(read_bytes > FIFO_EOF ){
		this->logger->log(this, product.toString());
		std::cout << "[Delivery] " << product.toString() << std::endl;
        read_bytes = read_channel->leer(&product,sizeof(CookerMan::Product));
    } 
    if(read_bytes == ERROR){
        throw std::runtime_error("Error reading orders fifo");     
    } 
	
	this->read_channel->close_fifo();
	this->read_channel->eliminar();
	std::cout << "[Delivery] loop ends " << std::endl;
}

void Delivery::stop(){
	Employee::stop();
}