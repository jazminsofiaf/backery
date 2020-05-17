#include "Logger.h"

const int Logger::BUFFSIZE = 100;
const std::string Logger::FILENAME = "../log.txt";


Logger::Logger(){
    this->outputFile = new std::fstream();
    this->outputFile->open(Logger::FILENAME, ios::out | fstream::trunc); 
    
    
    this->writeToFile();
    
    this->write_channel = new FifoEscritura(LOGGER_FIFO);
	this->write_channel->abrir();
    this->lock =  new Lock("log.txt");
    
}

//writes are atomic on pipe if buffer size is not greater than PIPE_BUF
void Logger::log(const Employee * employee, const std::string message){
   this->lock->tomarLock();
        if(this->write_channel == NULL){
            std::string error_msg = "Error: logger not initialized";
            std::cerr << error_msg << std::endl;
            throw std::runtime_error(error_msg);
        }
        std::string formated_message = "[" + employee->identify() + "] " + message+"\n";

        char buffer[Logger::BUFFSIZE];
        size_t  start_position = this->seek(formated_message, 0);
        size_t  end_position = this->seek(formated_message, Logger::BUFFSIZE);
        while( end_position <= formated_message.length()){
            std::string::iterator msg_start  = formated_message.begin() + start_position;
            std::string::iterator msg_end = formated_message.begin() + end_position;
            memset(buffer, 'A', sizeof(buffer)); 
            std::copy(msg_start, msg_end, buffer);
            
            size_t sent = this->write_channel->escribir(buffer, Logger::BUFFSIZE );
            std::cout <<"{Logger} writing ---------------------------------> " << buffer << sent << std::endl;
            this->lock->escribir(buffer, Logger::BUFFSIZE);
            if(end_position == formated_message.length()){
                break;
            }
            start_position = this->seek(formated_message, end_position);
            end_position = this->seek(formated_message, end_position + Logger::BUFFSIZE);
        }
    this->lock->liberarLock();  
    }

 size_t Logger::seek(std::string & str_message, size_t position) {
    return str_message.length() <= position ? str_message.length() : position;
}



void Logger::destruir () {
    std::cout << "[Logger] stops" << std::endl;
    if(this->write_channel == NULL || outputFile == NULL ){
        std::string error_msg = "Error: logger not initialized";
        std::cerr << error_msg  << std::endl;
    } 
	int rv = this->write_channel->close_fifo(); //stops flush loop
    if(rv != OK){
        std::string error_msg = "[Logger] Error closing write channel fifo ";
        std::cerr << error_msg << std::strerror(errno) << std::endl;
	} 
    int status;
    waitpid(this->process_id, &status, 0);
    this->outputFile->close(); 
    //delete this->outputFile;
    //delete this->write_channel;
    //delete this->read_channel;    
}


void Logger::writeToFile(){
    pid_t pid = fork ();
	if ( pid == CHILD_PD ) {
        
        this->read_channel = new FifoLectura(LOGGER_FIFO);
        this->read_channel->abrir();
        char buffer[Logger::BUFFSIZE];
    
        std::cout  << "[logger] a punto de leer del fifo " << std::endl;
	
        ssize_t bytesLeidos = this->read_channel->leer(static_cast<void*>(buffer),Logger::BUFFSIZE);
        //if bytes is EOF = 0, then the write channel is closed.
        std::cout  << "[logger] reading fifo: " << static_cast<int>(bytesLeidos) << std::strerror(errno) << std::endl;
	
        while(bytesLeidos > FIFO_EOF){
                std::cout <<"{Logger} reading <---------------------------------  " << buffer << bytesLeidos << std::endl;
                std::size_t end_line = std::string(buffer).find("\n");
                end_line = ( end_line == std::string::npos ) ? bytesLeidos : 1 + end_line;
                std::string mensaje = buffer;
                
                mensaje.resize ( bytesLeidos );    
                (*this->outputFile) << mensaje;
                this->outputFile->flush();

                bytesLeidos = this->read_channel->leer(static_cast<void*>(buffer),BUFFSIZE);
                std::cout  << "[logger] reading fifo: " << static_cast<int>(bytesLeidos) << std::strerror(errno) << std::endl;
	
        }
        if(bytesLeidos == ERROR){
            std::string error_msg = "[Logger] Error reading fifo ";
            std::cout << error_msg <<  bytesLeidos << std::endl;
            throw std::runtime_error(error_msg);
                
        }    
        std::cout << "[Logger] quit flush ok " <<  bytesLeidos << std::endl;
        int rv = this->read_channel->close_fifo();
        if(rv != OK){
            std::string error_msg = "[Logger] Error closing read channel fifo ";
            std::cerr << error_msg << std::strerror(errno) << std::endl;
            throw std::runtime_error(error_msg);
	    } 
        this->read_channel->eliminar();
        throw EndChildException();
    }
    this->process_id = pid; 
    std::cout << "[Logger] pid: " <<  process_id << std::endl;

}

