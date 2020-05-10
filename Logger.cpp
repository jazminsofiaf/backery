#include "Logger.h"
const int ERROR = -1;
const int FIFO_EOF = 0;

const int Logger::BUFFSIZE = 100;
const std::string Logger::ARCHIVO_FIFO = "/tmp/archivo_fifo";
FifoEscritura * Logger::write_channel;

const std::string Logger::FILENAME = "../log.txt";
std::fstream * Logger::outputFile;
pid_t Logger::process_id;


 
void Logger::init(){
    if(outputFile != NULL){ //el logger se debe crear una sola vez en todo el programa.
        std::cerr << "Error: logger already initialized" << std::endl;
        throw std::runtime_error("Error: logger already initialized");
    }
    outputFile = new std::fstream();
    outputFile->open(Logger::FILENAME, ios::out | fstream::trunc); 
    
    Logger::writeToFile();
    
    write_channel = new FifoEscritura(Logger::ARCHIVO_FIFO);
	write_channel->abrir();
    
}

void Logger::log(std::string message){
    if(write_channel == NULL){
        std::cerr << "Error: logger not initialized" << std::endl;
        throw std::runtime_error("Error: logger not initialized");
    }
    message += '\n';
	write_channel->escribir (static_cast<const void*>(message.c_str()), message.length() );
}


void Logger::destruir () {
    std::cout << "[Logger] stops" << std::endl;
    if(write_channel == NULL || outputFile == NULL ){
        std::cerr << "Error: logger not initialized" << std::endl;
        throw std::runtime_error("Error: logger not initialized");
    }
    outputFile->close();   
	write_channel->cerrar(); //stops flush loop
    int status;
    waitpid(process_id, &status, 0);
}


void Logger::writeToFile(){
    pid_t pid = fork ();
	if ( pid == 0 ) {
        //SIGINT_Handler sigint_handler;
	    //SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

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
            std::cout << "[Logger] error " <<  bytesLeidos << std::endl;
            throw std::runtime_error("Error: fifo reading error");
                
        }    
        std::cout << "[Logger] quit flush loop ok " <<  bytesLeidos << std::endl;
        canal.cerrar();
        canal.eliminar();
        exit(0);
    }
    process_id = pid;
    std::cout << "[Logger] pid: " <<  process_id << std::endl;

}

