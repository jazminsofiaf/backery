#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {}

FifoShared::~FifoShared() {
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize) const {
    if(this->fd == -1){
        return 0;
    }
    Lock * lock = new Lock( this->nombre + std::to_string(this->fd) );
    ssize_t  bytes_read = read ( this->fd,buffer,buffsize );
    delete lock; //unlock

	return bytes_read;
}
