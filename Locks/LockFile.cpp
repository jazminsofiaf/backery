#include <iostream>
#include "LockFile.h"

LockFile :: LockFile (const std::string name, int type, int start, int end, std::string mode) {
    this->name = name;
    this->fl.l_type = type;
    this->fl.l_whence = SEEK_SET;
    this->fl.l_start = start;
    this->fl.l_len = end - start;
    this->file = fopen(this->name.c_str(), mode.c_str());
    this->fd = fileno(this->file);
}

int LockFile:: getLock () {
    this->fl.l_type = F_WRLCK;
    return fcntl ( this->fd,F_SETLKW, &(this->fl) );
}


int LockFile:: freeLock() {
    this->fl.l_type = F_UNLCK;
    return fcntl (this->fd,F_SETLK,  &(this->fl) );
}

LockFile :: ~LockFile () {
    fclose ( this->file );

}

