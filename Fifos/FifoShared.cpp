#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {
}

FifoShared::~FifoShared() {
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize) const {
    if(this->fd == -1){
        return 0;
    }
    LockFile lock("prueba");
    lock.tomarLock();
    
    
    //All the file locked exclusive
    ssize_t  bytes_read = read ( this->fd,buffer,buffsize );
    //release the lock
     
    lock.liberarLock();
     
	return bytes_read;
}
