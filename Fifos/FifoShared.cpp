#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {
    this->lock_file_name = this->nombre + std::to_string(this->fd);
    this->lock_fd = open ( this->lock_file_name.c_str(), O_CREAT|O_WRONLY, 0777 );
}

FifoShared::~FifoShared() {
    //std::cout << "calling fifo shared detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    close( this->lock_fd );
    remove(this->lock_file_name.c_str());
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize,  const std::string cooker ) const {
    if(this->fd == -1){
        std::cout << cooker << " por leer shared fifo -1  " <<  endl;
        return -1;
    }
    Lock * lock = new Lock(this->lock_fd,  this->nombre + std::to_string(this->fd), cooker);
    std::cout << cooker << " por leer shared fifo " <<  endl;
    ssize_t  bytes_read = read ( this->fd, buffer, buffsize );
    std::cout << cooker << " ya leyo shared fifo " <<  endl;
    delete lock; //unlock

	return bytes_read;
}
