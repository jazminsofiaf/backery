#include "FifoShared.h"

FifoShared::FifoShared(const std::string nombre) : FifoLectura(nombre) {
}

FifoShared::~FifoShared() {
}

ssize_t FifoShared::leer(void* buffer,const ssize_t buffsize) const {
    /*
    struct flock lock;
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	fcntl(this->fd, F_SETLKW, &lock);
    */
    //All the file locked exclusive
    ssize_t  bytes_read = read ( this->fd,buffer,buffsize );
    //release the lock
    /*
    lock.l_type = F_UNLCK;
 	fcntl (fd, F_SETLKW, &lock);
     */
	return bytes_read;
}
