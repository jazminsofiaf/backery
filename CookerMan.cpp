#include "CookerMan.h"


CookerMan::CookerMan(int id_num, std::string sourdough_channel_name): 
Employee(id_num),
sourdough_channel_name(sourdough_channel_name){}

void CookerMan::run(){
    pid_t pid = fork ();
	if ( pid == CHILD_PD ) {
        this->sourdough_channel = new FifoLectura(this->sourdough_channel_name);
	    this->sourdough_channel->abrir();
		Sourdough::Dough dough_piece;
		size_t read_bytes = this->sourdough_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
        while(read_bytes > FIFO_EOF ){
		    std::cout << this->identify() << " took dough pice " << dough_piece.to_string() <<std::endl;
            Logger::log(this, " took dough pice " + dough_piece.to_string());

            read_bytes = this->sourdough_channel->leer(&dough_piece,sizeof(Sourdough::Dough));
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
		exit(OK);
	}
	std::cout << this->identify() << " pid: "<< pid << std::endl;
	this->process_id = pid;
}

void CookerMan::stop(){
    std::cout << this->identify() << " stoping..." << this->process_id << std::endl;
	int status;
    waitpid(this->process_id, &status, WEXITED);   
	Logger::log(this, "end");
	std::cout << this->identify() << " stoped " << status << std::endl;
}
 