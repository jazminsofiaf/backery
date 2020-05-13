#include "CookerMan.h"


CookerMan::CookerMan(int id_num, std::string sourdough_channel_name): 
Employee(id_num),
sourdough_channel_name(sourdough_channel_name){}

void CookerMan::run(){
    this->sourdough_channel = new FifoLectura(this->sourdough_channel_name);
	this->sourdough_channel->abrir();
	Sourdough::Dough dough_piece;
    this->mutex.lock();
	size_t read_bytes = this->sourdough_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
    this->mutex.unlock();
    while(read_bytes > FIFO_EOF ){
		std::cout << this->identify() << " took dough pice " << dough_piece.toString() <<std::endl;
        Logger::log(this, " took dough pice " + dough_piece.toString());

        this->mutex.lock();
        read_bytes = this->sourdough_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
        this->mutex.unlock();
    } 
    if(read_bytes == ERROR){
        std::cout << this->identify() << " error " <<  read_bytes  << std::endl;
        throw std::runtime_error("Error: fifo reading error");     
    } 
	std::cout << this->identify() << " loop ends " << std::endl;
    int rv =this->sourdough_channel->close_fifo();
    if(rv != OK){
		std::string msg = this->identify() + "  Error closing fifo ";	
		std::cout << msg << std::strerror(errno) << std::endl;
    }
		 
}

void CookerMan::stop(){
    Employee::stop();
}
 