#ifndef SHAREDFILE_H_
#define SHAREDFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "LockFile.h"

class SharedFile : public LockFile {

public:
    SharedFile ( const std::string name, int start, int end);
    ~SharedFile();

    int getSharedLock();

    bool getChar(char & c);

};

#endif