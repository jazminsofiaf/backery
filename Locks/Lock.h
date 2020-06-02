

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

public:
	Lock (const int fd);

    ~Lock();

};

#endif /* LOCK_H_ */