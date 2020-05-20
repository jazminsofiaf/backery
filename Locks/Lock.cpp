
#include "Lock.h"

Lock :: Lock (const int fd, const std::string nombre, const std::string cooker) {
    std::cout << cooker << " ask lock: " << nombre <<  endl;
    this->cooker = cooker;
	this->nombre = nombre;
	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
    this->fd = fd;
    int rv = fcntl ( this->fd,F_SETLKW, &(this->fl) );
    lseek (this->fd, 0 ,SEEK_END );
    std::string str = cooker + " get lock: " +nombre +"\n";
    std::cout << rv << str;
    char buffer[100];
    memset(buffer, '\0', sizeof(buffer));
    std::copy(str.begin(), str.end(), buffer);
    write( this->fd, buffer, sizeof(buffer) );
}


Lock :: ~Lock () {
    this->fl.l_type = F_UNLCK;
    std::string str = cooker + " free lock: " +nombre +"\n";
    char buffer[100];
    memset(buffer, '\0', sizeof(buffer));
    std::copy(str.begin(), str.end(), buffer);
    write( this->fd, buffer, sizeof(buffer) );

    fcntl ( this->fd,F_SETLK,&(this->fl) );
    //close ( this->fd );
    //remove(this->nombre.c_str());
    std::cout << this->cooker << " free lock: " << nombre <<  endl;
}
 