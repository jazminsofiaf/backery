#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {
    this->lock_file_name = this->nombre + std::to_string(this->fd);
    this->lock_fd = open ( this->lock_file_name.c_str(), O_CREAT|O_WRONLY, 0777 );
}

FifoShared::~FifoShared() {
    close( this->lock_fd );
    remove(this->lock_file_name.c_str());
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize) const {
    if(this->fd == -1){
        return -1;
    }
    Lock  lock(this->lock_fd);
    ssize_t  bytes_read = read ( this->fd, buffer, buffsize );
	return bytes_read;
}
