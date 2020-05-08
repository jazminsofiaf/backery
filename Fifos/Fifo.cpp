#include "Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	const char *  file_name = static_cast<const char*>( nombre.c_str());
	mknod (file_name, S_IFIFO | 0666, 0 );
}

Fifo::~Fifo() {
}

void Fifo::cerrar() {
	close ( fd );
	fd = -1;
}

void Fifo::eliminar() const {
	unlink ( nombre.c_str() );
}
