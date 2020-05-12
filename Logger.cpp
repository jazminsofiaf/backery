#include "Logger.h"

const int Logger::BUFFSIZE = 100;
const std::string Logger::ARCHIVO_FIFO = "/tmp/archivo_fifo";
FifoEscritura * Logger::write_channel;

const std::string Logger::FILENAME = "../log.txt";
std::fstream * Logger::outputFile;
pid_t Logger::process_id;


 
void Logger::init(){
    if(outputFile != NULL){ //Logger should be initialized only once.
        std::string error_msg = "Error: logger already initialized";
        std::cerr << error_msg << std::endl;
        throw std::runtime_error(error_msg );
    }
    outputFile = new std::fstream();
    outputFile->open(Logger::FILENAME, ios::out | fstream::trunc); 
    
    Logger::writeToFile();
    
    write_channel = new FifoEscritura(Logger::ARCHIVO_FIFO);
	write_channel->abrir();
    
}

//writes are atomic on pipe if buffer size is not greater than PIPE_BUF
void Logger::log(const Employee * employee, const std::string message){
        if(write_channel == NULL){
            std::string error_msg = "Error: logger not initialized";
            std::cerr << error_msg << std::endl;
            throw std::runtime_error(error_msg);
        }
        std::string formated_message = "[" + employee->identify() + "] " + message +"\n";
        write_channel->escribir(static_cast<const void*>(formated_message.c_str()), formated_message.length() );
    }


void Logger::destruir () {
    std::cout << "[Logger] stops" << std::endl;
    if(write_channel == NULL || outputFile == NULL ){
        std::string error_msg = "Error: logger not initialized";
        std::cerr << error_msg  << std::endl;
        throw std::runtime_error(error_msg);
    }
    outputFile->close();   
	int rv = write_channel->close_fifo(); //stops flush loop
    if(rv != OK){
        std::string error_msg = "[Logger] Error closing write channel fifo ";
        std::cerr << error_msg << std::strerror(errno) << std::endl;
        throw std::runtime_error(error_msg);
	} 
    int status;
    waitpid(process_id, &status, 0);
}


void Logger::writeToFile(){
    pid_t pid = fork ();
	if ( pid == CHILD_PD ) {

        FifoLectura canal(Logger::ARCHIVO_FIFO);
        char buffer[Logger::BUFFSIZE];
        canal.abrir();
        ssize_t bytesLeidos = canal.leer(static_cast<void*>(buffer),BUFFSIZE);
        //if bytes is EOF = 0, then the write channel is closed.
        while(bytesLeidos > FIFO_EOF){
                std::string mensaje = buffer;
                mensaje.resize ( bytesLeidos );
                (*outputFile) << mensaje;
                outputFile->flush();

                bytesLeidos = canal.leer(static_cast<void*>(buffer),BUFFSIZE);
        }
        if(bytesLeidos == ERROR){
            std::string error_msg = "[Logger] Error reading fifo ";
            std::cout << error_msg <<  bytesLeidos << std::endl;
            throw std::runtime_error(error_msg);
                
        }    
        std::cout << "[Logger] quit flush ok " <<  bytesLeidos << std::endl;
        int rv = canal.close_fifo();
        if(rv != OK){
            std::string error_msg = "[Logger] Error closing read channel fifo ";
            std::cerr << error_msg << std::strerror(errno) << std::endl;
            throw std::runtime_error(error_msg);
	    } 
        canal.eliminar();
        exit(OK);
    }
    process_id = pid;
    std::cout << "[Logger] pid: " <<  process_id << std::endl;

}

