#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "Lock.h"

class LockFile : public Lock {

private:
	FILE * file;

public:
	LockFile ( const std::string nombre, int start, int end);
	~LockFile();

	int getSharedLock();

	bool getChar(char & c);

};

#endif /* LOCKFILE_H_ */