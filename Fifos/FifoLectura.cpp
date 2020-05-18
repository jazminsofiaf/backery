#include "FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
	//std::cout << "calling fifo read detructor ~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
}

void FifoLectura::abrir() {
	this->fd = open( nombre.c_str(),O_RDONLY );
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	return read ( this->fd,buffer,buffsize );
}
