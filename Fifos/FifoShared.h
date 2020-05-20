#ifndef FIFOSHARED_H_
#define FIFOSHARED_H_

#include "FifoLectura.h"
#include "../Constant.h"
#include "../Locks/Lock.h"
#include <unistd.h>
#include <fcntl.h>

class FifoShared : public FifoLectura {
private:
    std::string lock_file_name;
    int lock_fd;

public:
	FifoShared(const std::string nombre);
	~FifoShared();
	ssize_t leer(void* buffer, const ssize_t buffsize, const std::string cooker) const ;
};

#endif
