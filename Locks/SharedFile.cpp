#include "SharedFile.h"


SharedFile :: SharedFile ( const std::string name, int start, int end )
: LockFile(name, start, end, F_RDLCK, std::string("rb")){
    fseek(this->file, start, SEEK_SET);
}


int SharedFile :: getSharedLock(){
    this->fl.l_type = F_UNLCK;
    return fcntl (this->fd,F_SETLK,&(this->fl) );

}

bool SharedFile :: getChar(char & c){
    int char_read;
    if( (char_read = fgetc(file)) == EOF ){
        return false;
    }
    putchar(char_read);
    c = char_read;
    return true;
}

SharedFile :: ~SharedFile () {
}
