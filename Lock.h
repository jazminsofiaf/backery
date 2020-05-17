#ifndef LOCK_H_
#define LOCK_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>

class Lock {

protected:
	struct flock fl;
	int fd;
	std::string nombre;

public:
	Lock ( const std::string nombre );
	Lock ( const std::string nombre, FILE ** file, int type, int start, int end);
	~Lock();

	int tomarLock ();
	
	int liberarLock ();

};

#endif /* LOCK_H_ */