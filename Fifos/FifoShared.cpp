#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {}

FifoShared::~FifoShared() {
    //std::cout << "calling fifo shared detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize,  const std::string cooker ) const {
    if(this->fd == -1){
        std::cout << cooker << " por leer shared fifo -1  " <<  endl;
        return -1;
    }
    Lock * lock = new Lock( this->nombre + std::to_string(this->fd), cooker);
    //std::cout << cooker << " por leer shared fifo " <<  endl;
    ssize_t  bytes_read = read ( this->fd, buffer, buffsize );
    //std::cout << cooker << " ya leyo shared fifo " <<  endl;
    delete lock; //unlock

	return bytes_read;
}
