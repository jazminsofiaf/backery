#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {
    this->lock =  new Lock(this->nombre + std::to_string(this->fd));
}

FifoShared::~FifoShared() {
    //std::cout << "calling fifo shared detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    //delete this->lock;
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize) const {
    if(this->fd == -1){
        return 0;
    }
   
    this->lock->tomarLock();
    ssize_t  bytes_read = read ( this->fd,buffer,buffsize );     
    this->lock->liberarLock();
     
	return bytes_read;
}
