#include "Fifo.h"

using namespace std;

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	const char *  file_name = static_cast<const char*>( this->nombre.c_str());
	mknod (file_name, S_IFIFO | 0666, 0 );
}

Fifo::~Fifo() {
}

int Fifo::close_fifo() {
	int rv = close( this->fd );
	this->fd = -1;
	return rv;
}

void Fifo::eliminar() const {
	unlink ( this->nombre.c_str() );
}
