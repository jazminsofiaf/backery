#include "ExclusiveFile.h"

ExclusiveFile :: ExclusiveFile ( const std::string name)
    : LockFile(name, 0, 0, F_WRLCK, std::string("w")){
}



ssize_t ExclusiveFile :: escribir( const void* buffer, const ssize_t buffsize ) const {
    lseek (this->fd, 0 ,SEEK_END );
    return write ( this->fd, buffer, buffsize );
}


ExclusiveFile :: ~ExclusiveFile() {
}