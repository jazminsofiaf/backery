
#include "Lock.h"

Lock :: Lock (const int fd) {
	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
    this->fd = fd;
    fcntl ( this->fd,F_SETLKW, &(this->fl) );
    lseek (this->fd, 0 ,SEEK_END );
}


Lock :: ~Lock () {
    this->fl.l_type = F_UNLCK;
    fcntl ( this->fd,F_SETLK,&(this->fl) );
}
 