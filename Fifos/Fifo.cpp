#include "Fifo.h"

using namespace std;

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	const char *  file_name = static_cast<const char*>( this->nombre.c_str());
	mknod (file_name, S_IFIFO | 0666, 0 );
}

Fifo::~Fifo() {}

int Fifo::close_fifo() {
	int rv = close( this->fd );
	if(rv != 0){
		std::cerr << "Error closing fifo " << this->nombre << ": "<<std::strerror(errno) << this->fd  <<std::endl;
	}
	this->fd = -1;
	return rv;
}

void Fifo::eliminar() const {
	unlink ( this->nombre.c_str() );
}
