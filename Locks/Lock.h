

#ifndef LOCK_H_
#define LOCK_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdio>
#include <iostream>
using namespace std;

class Lock {

protected:
	struct flock fl{};
	int fd;
	std::string nombre;
    std::string cooker;

public:
	Lock ( const std::string nombre, const std::string cooker);

    ~Lock();

};

#endif /* LOCK_H_ */