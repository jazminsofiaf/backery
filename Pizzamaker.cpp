#include "Pizzamaker.h"
Pizzamaker::Pizzamaker(std::string channel_name): channel_name(channel_name){}

void Pizzamaker::run(){
    pid_t pid = fork ();
	if ( pid == 0 ) {

        this->read_channel = new FifoLectura("masa");
		this->read_channel->abrir();
		Sourdough::Dough dough_piece;
		size_t read_bytes = this->read_channel->leer(&dough_piece, sizeof(Sourdough::Dough) );
        std::cout << "[Pizza maker] took dough pice " << dough_piece.to_string() <<std::endl;
       /* while(read_bytes > 0 ){
		    std::cout << "[Pizza maker] took dough pice " << dough_piece.to_string() <<std::endl;
            Logger::log("[Pizza maker] took dough pice " + dough_piece.to_string() );

            read_bytes = this->read_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
        } 
  
        if(read_bytes == -1 ){
            std::cout << "[Pizza maker] error " <<  read_bytes  << std::endl;
            throw std::runtime_error("Error: fifo reading error");
                
        } */
		std::cout << "[Pizza maker] loop ends " << std::endl;
		exit(0);
	}
	std::cout << "[Pizza maker] pid: "<<pid << std::endl;
	this->process_id = pid;
}

void Pizzamaker::stop(){
    std::cout << "[Pizza maker] stoping..." << std::endl;
    this->read_channel->cerrar();
	int status;
    waitpid(this->process_id, &status,0);
	Logger::log("[Pizza maker] end");
	std::cout << "[Pizza maker] stoped" << std::endl;
}