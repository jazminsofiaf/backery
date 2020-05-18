#ifndef EXCLUSIVEFILE_H_
#define EXCLUSIVEFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "LockFile.h"

class ExclusiveFile : public LockFile {

public:
    ExclusiveFile ( const std::string name);
    ssize_t escribir ( const void* buffer,const ssize_t buffsize ) const;
    ~ExclusiveFile();
};

#endif